/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
* @file  ccl.c
* @brief main file for ccl Library, contains all external entry points
*******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>

#if defined(LINUX_EMB) || defined(LINUX_PC) || defined(GHS)
    #include "errno.h"
#endif

#include "ccl.h"
#include "ccl_frame_queue.h"
#include "ccl_cca.h"
#include "ccl_internal.h"

#if CCL_FEATURE_CAPTURE_FILE
#include "ccl_cap_file.h"
#endif
#if CCL_FEATURE_CAPTURE_V4L
#include "ccl_cap_v4l.h"
#endif
#if CCL_FEATURE_CAPTURE_CGD
#include "ccl_cap_cgd.h"
#endif
#if CCL_FEATURE_CAPTURE_FASTDDS
#include "ccl_cap_dds.h"
#endif
#if CCL_FEATURE_CAPTURE_MEMCPY
#include "ccl_cap_memcpy.h"
#endif
#include "ccl_isp.h"

#include "mshr.h"
#include "ispdrv_usr.h"


#ifdef GUI_DEMO
#include "gui_control.h"
#endif

#ifdef ISIM
    #define MIN_FRAME_TASKS_PER_STREAM (1)
#else
    #define MIN_FRAME_TASKS_PER_STREAM (3)
#endif

/** @brief CCL state variable
 * @details We only do static initialization of single flag so we can recognize when to dynamic variable init */
DDR_NOINIT ccl_t gCclState;

uint32_t gCclInitialized = 0;

/***********************************************************************
 *
 * @brief Bits per pixel translation (enum to value)
 *
 * @details This variable is a translation of IMG_CONF_CAM0.IBPP0 field's "bits per pixel enum"
 *          into "bits per pixel value".
 *
 *              NEO Camera 0 Image Configuration Register (IMG_CONF_CAM0)
 *              3-0 IBPP0  -> Input image 0 bpp: Indicates the size of pixel components of image 0 in input buffer 0.
 *                            0000b - 12 bpp (packed into 16 bits)
 *                            0001b - 14 bpp (packed into 16 bits)
 *                            0010b - 16 bpp (packed into 16 bits)
 *                            0011b - 20 bpp (packed into 32 bits)
 *                            0100b - 10 bpp (packed into 16 bits)
 *                            0101b - 10 bpp packed (3 pixels packed into 32 bits)
 *                            0110b - 8 bpp (packed into 8 bits)
 * *********************************************************************/
const uint8_t gcBppToBits[] = {12, 14, 16, 20, 10, 10, 8};

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/**
 * @brief Local variable and system object initialization
 *
 * @return 0 on success or negative error otherwise.
 */
static ccl_Err_t VarInit()
{
    memset(&gCclState, 0, sizeof(gCclState));

    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t *Stream = &gCclState.Streams[HwId][CamId];

            if (0 != ccl_sem_Init(&Stream->mFrameReadyEvent, 0))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Semaphore init error\n");
                return CCL_ERR_GENERIC;
            }

            Stream->mState = CCL_STREAM_STATE_IDLE;
            Stream->mCurrentFrameIdSchedule = 0;
            Stream->mScheduledCnt = 0;

            if (0 != ccl_sem_Init(&Stream->mSchedulePending, 0))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to init semaphore \n");
                return CCL_ERR_GENERIC;
            }

            if (0 != ccl_mutex_Create(&Stream->mLock, true))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mutex init error\n");
                return CCL_ERR_GENERIC;
            }

            Stream->mFTLogFd = -1;
            if (0 != ccl_mutex_Create(&Stream->mFTLogLock, true))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mutex init error\n");
                return CCL_ERR_GENERIC;
            }

            Stream->mSensLogFd = -1;
            if (0 != ccl_mutex_Create(&Stream->mSensLogLock, true))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mutex init error\n");
                return CCL_ERR_GENERIC;
            }

            for (uint32_t LatencyType = 0; LatencyType < CCL_TIME_STAT_CNT; LatencyType++)
            {
                gCclState.mStats.mTimeStats[HwId][CamId][LatencyType].mMin = UINT64_MAX;
            }
            gCclState.mStats.mGlobalCcaTimes.mMin = UINT64_MAX;

        }
    }

    return CCL_ERR_SUCCESS;
}

/**
 * @brief local variable and system object cleanup
 *
 */
static void VarDeInit()
{
    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t *Stream = &gCclState.Streams[HwId][CamId];

            if (0 != ccl_sem_Destroy(&Stream->mFrameReadyEvent))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Semaphore deinit error\n");
            }
            if (0 != ccl_mutex_Destroy(&Stream->mLock))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mutex deinit error\n");
            }
            if (0 != ccl_sem_Destroy(&Stream->mSchedulePending))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy schedule semaphore\n");
            }
            if (0 != ccl_mutex_Destroy(&Stream->mFTLogLock))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mutex deinit error\n");
            }
            if (0 != ccl_mutex_Destroy(&Stream->mSensLogLock))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mutex deinit error\n");
            }
        }
    }
}

#if CCL_FEATURE_LOG_FRAME_TIMES
/**
 * @brief Logs times recorded in memory to a file
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - Frame to record
 */
static void Logtimes(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    char Line[2048];
    int offset = 0;
    offset += snprintf(Line+offset, sizeof(Line) - offset, "%ld,", apFrame->FrameData.mMetaData.mFrameNo);

    for (int i = CCL_FRAME_STATE_PRE_ISP_PREP; i < CCL_FRAME_STATE_COUNT; i++)
    {
        offset += snprintf(Line+offset, sizeof(Line) - offset, "%ld,", apFrame->mTimeStamps[i]);
    }
    offset += snprintf(Line+offset, sizeof(Line) - offset, "%ld\n", apFrame->mTimeStamps[CCL_FRAME_STATE_EMPTY]);

    if (0 != ccl_mutex_Lock(&Stream->mFTLogLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
    }

    if (-1 == write(Stream->mFTLogFd, Line, offset))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to write frame times - %s. hwid: %d, camid: %d\n", strerror(errno), aHwId, aCamId);
    }

    if (0 != ccl_mutex_Unlock(&Stream->mFTLogLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
    }
}
#endif

#if CCL_FEATURE_RUNTIME_CONTROL
/**
 * @brief Validation of external control context
 *
 * @param apExtCtrlCtx - pointer to external control context to validate
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t ValidateExtCtrlCtx(utils_ExtControlCtx_t *apExtCtrlCtx)
{
    if (apExtCtrlCtx == NULL)
    {
        return -1;
    }
    if (!apExtCtrlCtx->mInitialized)
    {
        return -1;
    }

    // Validate pointers to functions
    if (apExtCtrlCtx->mReadI2cFunc == NULL || apExtCtrlCtx->mWriteI2cFunc == NULL || apExtCtrlCtx->mChangeFpsFunc == NULL) {
        return -1;
    }

    // Validate mutexes
    if (pthread_mutex_trylock(&apExtCtrlCtx->mMutexSaveImg) != 0) {
        return -1;
    }
    pthread_mutex_unlock(&apExtCtrlCtx->mMutexSaveImg);

    if (pthread_mutex_trylock(&apExtCtrlCtx->mMutexI2c) != 0) {
        return -1;
    }
    pthread_mutex_unlock(&apExtCtrlCtx->mMutexI2c);

    return 0;
}
#endif

/**
 * @brief Set capture functions for given stream based on capture method selection
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aCapDev - capture method selection
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t SetCapOps(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapDev_t aCapDev)
{
    int32_t lRet;
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    switch (aCapDev)
    {
#if CCL_FEATURE_CAPTURE_FILE
        case CCL_CAP_VIA_FILE:
            ccl_CapOps_File(&Stream->mCapOps);
            lRet = 0;;
            break;
#endif
#if CCL_FEATURE_CAPTURE_V4L
        case CCL_CAP_VIA_V4L:
            lRet = ccl_CapOps_V4l(&Stream->mCapOps);
            break;
#endif
#if CCL_FEATURE_CAPTURE_CGD
        case CCL_CAP_VIA_CGD:
            lRet = ccl_CapOps_Cgd(&Stream->mCapOps, aHwId, aCamId);
            break;
#endif
#if CCL_FEATURE_CAPTURE_FASTDDS
        case CCL_CAP_VIA_FASTDDS:
            lRet = ccl_CapOps_DDS(&Stream->mCapOps);
            break;
#endif
#if CCL_FEATURE_CAPTURE_MEMCPY
        case CCL_CAP_VIA_MEMCPY:
            lRet = ccl_CapOps_Memcpy(&Stream->mCapOps);
            break;
#endif

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error - unknown Capture method (API) - %d.\n", aCapDev);
            lRet = -1;
    }

    if ( (CCL_CAP_MODE_STREAM == Stream->mCapCfg.mCapMode) && !Stream->mCapOps.StreamingModeSupported)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Streaming mode not supported for selected capture device - %d.\n", aCapDev);
        lRet = -1;
    }

    return lRet;
}

void ccl_ComputeInImgSizes(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_ImgBuf_t *apInBuf)
{
    assert(apInBuf != NULL);
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);

    cca_CamInit_t *StreamCfg = &gCclState.mInitCfg.mpCams[aHwId][aCamId];
    cca_ImgBuf_t* lpIoBuf;

    for (uint8_t lPlaneId = 0; lPlaneId < CCL_INBUF_PLANE_CNT; lPlaneId++)
    {
        // NOTE: actual allocation is done in capture-method-specific code, where is also information if this plane is actually used (allocated)
        lpIoBuf = &apInBuf[lPlaneId];
        lpIoBuf->mWidth = StreamCfg->mcpExtDef->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mWIDTH;
        lpIoBuf->mHeight = StreamCfg->mcpExtDef->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mHEIGHT;
        lpIoBuf->mChan = 1;
        lpIoBuf->mBpp = ccl_GetIspIbufBitsPP(aHwId, aCamId, lPlaneId);
        lpIoBuf->mSizeBytes = lpIoBuf->mWidth * lpIoBuf->mHeight * ccl_GetBitsPP2BytesPP(lpIoBuf->mBpp);
    }
} // ccl_ComputeInImgSizes()


/**
 * @brief Allocate all needed memory for given stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param ImgBufCnt - number of frame tasks to allocate
 *
 * @return 0 on success or negative error otherwise.
 */
static ccl_Err_t AllocateBufs(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint32_t ImgBufCnt)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (0 >= ImgBufCnt || ImgBufCnt > CCL_CFG_MAX_BUF_PER_STREAM )
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Bad buffer count - %d hwid: %d, camid: %d\n", ImgBufCnt, aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }

    for (uint32_t i=0; i < ImgBufCnt; i++)
    {
        ccl_FrameTask_t *FrTsk = &Stream->mFramePool[i];
        memset(FrTsk, 0, sizeof(ccl_FrameTask_t));

        // pre-compute input sizes on input buffers, those values serve only for computations bellow,
        // all values will be overwritten by any buffer generated by capture subsystem prior schedule() or ready() events
        ccl_ComputeInImgSizes(aHwId, aCamId, FrTsk->FrameData.mImg);

        if (0 != ccl_AllocImgBufsOut(aHwId, aCamId, FrTsk))
        {
            return CCL_ERR_ALLOC;
        }
        if (0 != ccl_AllocIspTask(aHwId, aCamId, FrTsk))
        {
            return CCL_ERR_ALLOC;
        }
    }

    return CCL_ERR_SUCCESS;
}

/**
 * @brief Transfer frame task from one state list to another
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - target frame task to move
 * @param aSrc - list to remove the task from
 * @param aDst - list to add the task to
 *
 * @return true on success, false otherwise
 */
static void SetFrameState(ccl_FrameTask_t *apFrame, ccl_FrameState_t aFinishedStage)
{
    if (apFrame == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Move of a NULL FrameTask !!!!!\n");
        return;
    }
    if (aFinishedStage >= CCL_FRAME_STATE_COUNT)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Move to Frame to bad state %d!!!!!\n", aFinishedStage);
        return;
    }

    apFrame->mStateFlags |= ((uint32_t)1U) << aFinishedStage;

#if CCL_FEATURE_LOG_FRAME_TIMES
    apFrame->mTimeStamps[aFinishedStage] = ccl_GetTime();
#endif
}


static void RecycleFrameTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame, bool aPostEvent)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    apFrame->mStateFlags = CCL_FRAME_STATE_EMPTY;
#if CCL_FEATURE_LOG_FRAME_TIMES
    if (Stream->mCapCfg.mLogFrameTimes)
    {
        apFrame->mTimeStamps[CCL_FRAME_STATE_EMPTY] = ccl_GetTime();
        Logtimes(aHwId, aCamId, apFrame);
        memset(apFrame->mTimeStamps, 0, sizeof(apFrame->mTimeStamps));
    }
#endif
    if (0 != ccl_FqDropCheck(&Stream->mAppReadyQueue, apFrame))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to remove task from ready queue.\n");
    }

    if (aPostEvent && apFrame->mInBuf != NULL)
    {
        Stream->mCapOps.mpCapBufReturn(aHwId, aCamId, apFrame->mInBuf);
    }
    apFrame->mInBuf = NULL;
}

/**
 * @brief Request from capture method for a frame task
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param[out] apFrame - variable that receives the task (can be NULL)
 */
static ccl_FrameTask_t *GetFreeBuf(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    ccl_FrameTask_t *Frame = NULL;

    for (uint i = 0; i < CCL_CFG_MAX_BUF_PER_STREAM; i++)
    {
        if (Stream->mFramePool[i].mStateFlags == CCL_FRAME_STATE_EMPTY)
        {
            Frame = &Stream->mFramePool[i];
            break;
        }
    }

    if (NULL == Frame)
    {
        // check APP queue for old frames to drop
        if (1 >= ccl_FqGetCnt(&Stream->mAppReadyQueue))
        {
            // there is not enough frames waiting for APP (we want to have at least one ready)
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "No buffer to drop (no frame ready for app)! hwid: %d, camid: %d\n", aHwId, aCamId);
            return NULL;
        }

        // Get pointer to oldest frame
        Frame = Stream->mAppReadyQueue.mItems[Stream->mAppReadyQueue.mRp];

        // check if CCF is complete
        if (IS_IN_STATE(Frame, CCL_FRAME_STATE_CCA_PROCESSED))
        {
            // we can't recycle tasks that could be running on CCF
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Oldest frame not finished with CCA! hwid: %d, camid: %d\n", aHwId, aCamId);
            return NULL;
        }

        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Ccl adds new task and needs to drop buffer, dropping no:%d hwid: %d, camid: %d\n", Frame->FrameData.mMetaData.mFrameNo , aHwId, aCamId);

        RecycleFrameTask(aHwId, aCamId, Frame, true);
    }

    SetFrameState(Frame, CCL_FRAME_STATE_PRE_ISP_PREP);

    return Frame;
}

/**
 * @brief Drops frame during processing (after it was captured, but before it added to ready list)
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - target task
 * @param aDropStage - state the task is currently in
 *
 * @details signals to CCA that frame was dropped and CCA can go without it if library supports it
 */
static void DropFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_CcaFrameDropped(aHwId, aCamId);

    RecycleFrameTask(aHwId, aCamId, apFrame, true);
}

/**
 * @brief Schedules frame task for ISP processing
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - target frame
 *
 * @return 0 on success or negative error otherwise.
 */
static ccl_Err_t Schedule(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (0 != ccl_UpdateIspBufLocations(apFrame))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to update isp buffer locations prior to schedule() -> dropping frame. hwid: %d, camid: %d\n", aHwId, aCamId);
        DropFrame(aHwId, aCamId, apFrame);
        return CCL_ERR_GENERIC;
    }

    if (0 != ccl_FlushIspBufs(aHwId, aCamId, apFrame))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Fail to flush ISP buffers. hwid: %d, camid: %d\n", aHwId, aCamId);
    }

    if (((unsigned)Stream->mScheduledCnt) >= MAX_FW_TASK_CNT_PER_STREAM)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Too many task already scheduled on fw -> dropping frame %d. hwid: %d, camid: %d\n", apFrame->FrameData.mMetaData.mFrameNo, aHwId, aCamId);
        DropFrame(aHwId, aCamId, apFrame);
        return CCL_ERR_BAD_STATE;
    }
    Stream->mScheduledCnt++;
    SetFrameState(apFrame, CCL_FRAME_STATE_ISP_PROCESSING);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
    }

    ILOG(ILOG_MOD_CCL, ILOG_LEV_NFO, "Scheduling #%"PRIu64" (0x%"PRIx64") hwid: %"PRId32", camid: %"PRId32"\n", apFrame->FrameData.mMetaData.mFrameNo, apFrame->mIspPhy, aHwId, aCamId);
    ispdrv_Result_t res = ispdrv_TaskSchedule(aHwId, aCamId, apFrame->mIspPhy);

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
    }

    if (ISPDRV_RES_SUCCESS != res)
    {
        Stream->mScheduledCnt--;
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Schedule #%u failed -> dropping frame. hwid: %d, camid: %d\n", apFrame->FrameData.mMetaData.mFrameNo, aHwId, aCamId);
        DropFrame(aHwId, aCamId, apFrame);
        return CCL_ERR_GENERIC;
    }

    return CCL_ERR_SUCCESS;
}

/**
 * @brief Schedules next frame configuration on FW in streaming mode
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Should run on dedicated thread.
 */
static int32_t ScheduleNextFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    ccl_FrameTask_t *FrameTask;

    FrameTask = GetFreeBuf(aHwId, aCamId);
    if (FrameTask == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to schedule next frame on ISP !!! (no free frame task) HWID# %u, CID# %u\n", aHwId, aCamId);
        return -1;
    }

    // update frame task before schedule
    FrameTask->FrameData.mMetaData.mFrameNo = Stream->mCurrentFrameIdSchedule++;

    if (0 != ccl_CcaFindIspCfgStr(aHwId, aCamId, FrameTask, Stream->mCapCfg.mHdrDelay))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error filling frame ISP cfg before capturing -> dropping frame. hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }

    // contrary to M2M mode, in streaming mode, embedded data are also processed by ISP - we need to update ISP height here
    if (Stream->mCapOps.mpGetFullHeight == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Stream->mCapOps.mpGetFullHeight is NULL => streming mode not implemented properly in selected capture subsytem. hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }
    uint32_t FullHeight = Stream->mCapOps.mpGetFullHeight(&Stream->mCapCfg, NULL, NULL);

    FrameTask->mIspCfg->mIspCtx.mCtxHw.mRegs.mAUTOFOCUS.mROI0_SIZE_CAM0.mHEIGHT = FullHeight;
    FrameTask->mIspCfg->mIspCtx.mCtxHw.mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mHEIGHT = FullHeight;
    FrameTask->mIspCfg->mIspCtx.mCtxHw.mRegs.mRGBIR.mROI0_SIZE_CAM0.mHEIGHT = FullHeight;
    FrameTask->mIspCfg->mIspCtx.mCtxHw.mRegs.mSTAT.mROI0_SIZE_CAM0.mHEIGHT = FullHeight;

    if (0 != Schedule(aHwId, aCamId, FrameTask))
    {
        // error printing and frame dropping inside due to state change
        return -1;
    }

    return 0;
}

/**
 * @brief Awaits some time for all already scheduled ISP tasks to be finished.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void AwaitIspTasks(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    int32_t max_wait_ms = 100;
#ifdef ISIM
    max_wait_ms = 20000;
    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ONLY INFO: Waiting for isim frames during stream stop() - May take a long time (%d ms). hwid: %d, camid: %d\n", max_wait_ms, aHwId, aCamId);
#endif
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    switch (Stream->mCapCfg.mCapMode)
    {
        case CCL_CAP_MODE_M2M:
            // NOTE: as capture is now stopped, number of ISP frames on ISP can only decrease with time
            // => no need to lock stream for check
            while (0 != Stream->mScheduledCnt)
            {
                ccl_SleepUs(1000);
                max_wait_ms--;
                if (max_wait_ms <= 0)
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Timeout while waiting for ISP frames during stop. hwid: %d, camid: %d\n", aHwId, aCamId);
                    return;
                }
            }
            break;

        case CCL_CAP_MODE_STREAM:
            // NOTE: as capture is now stopped all tasks in ISP are frozen, and will never get to us.
            // ISM module upon mode change to M2M resets ISP and deletes all tasks that were scheduled on that stream.
            if (ISPDRV_RES_SUCCESS != ispdrv_ChangeMode(aHwId, aCamId, ISPDRV_MODE_M2M))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to switch to M2M mode. hwid: %d, camid: %d\n", aHwId, aCamId);
            }
            Stream->mScheduledCnt = 0;
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Can't await ISP tasks -> bad capture mode -> HWID# %u, CID# %u\n", aHwId, aCamId);
    }
}

/**
 * @brief Awaits CCF to finish started tasks
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void AwaitCcfTasks(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    // we can't stop CCA when already in progress, we must await its finish
    int32_t max_wait_ms = 10;
    while (Stream->mCcfStats.mpCcaFrame != NULL)
    {
        ccl_SleepUs(1000);
        max_wait_ms--;
        if (max_wait_ms <= 0)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Timeout while waiting for CCA to finish. hwid: %d, camid: %d\n", aHwId, aCamId);
            return;
        }
    }
}

/**
 * @brief Performs stop operation on the stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stream's lock must already by acquired before this call
 */
static ccl_Err_t Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (CCL_STREAM_STATE_RUNNING != Stream->mState && CCL_STREAM_STATE_STARTING != Stream->mState )
    {
        return CCL_ERR_BAD_STATE;
    }

    Stream->mState = CCL_STREAM_STATE_STOPPING;
    ILOG(ILOG_MOD_CCL, ILOG_LEV_NFO, "Stopping hwid: %d, camid: %d\n", aHwId, aCamId);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
    }

    if (0 != Stream->mCapOps.mpCapStop(aHwId, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error stopping stream.\n");
    }

    if (0 != ccl_thread_Cancel(&Stream->mScheduleThread))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error starting stream.\n");
    }

    // we stopped capture, so no new frames will come, but we want to wait for those already scheduled on ISP, if we do not
    // -> FW's tx queue will be blocked by the waiting done msgs
    // -> waiting done messages will be delivered later once stream resumes
    AwaitIspTasks(aHwId, aCamId);

    // await CCF
    AwaitCcfTasks(aHwId, aCamId);

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
    }

    // after stop all capture buffers are considered returned to capture subsystem
    Stream->mArmedInImg = NULL;
    // all ISP task now complete, clean local state
    Stream->mArmedOutImage = NULL;

    // *** Buffer reset ***
    // Capture is now stopped, ISP should hold no tasks, CCA should no longer use this camid and application requested stop - any buffers should be considered emtpy now
    Stream->mCcfStats.mpLastFrame = NULL;
    Stream->mCcfStats.mpCcaFrame = NULL;
    Stream->mCcfStats.mStatsState = CCL_STATS_STATE_NONE;
    ccl_FqClean(&Stream->mAppReadyQueue);
    for (uint i = 0; i < CCL_CFG_MAX_BUF_PER_STREAM; i++)
    {
        Stream->mFramePool[i].mStateFlags = CCL_FRAME_STATE_EMPTY;
    }


    Stream->mState = CCL_STREAM_STATE_READY;

    return CCL_ERR_SUCCESS;
}


/**
 * @brief Thread waiting impulse to schedule next fw task
 *
 * @param apArg - pointer to local capture device
 * @return NULL
 */
static void* ScheduleThread(void* apArg)
{
    ccl_Stream_t *Stream = apArg;

    // set thread termination style to "anytime" instead of "just in cancellation point"
    if (0 != ccl_thread_ManualStopControl())
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation type. hwid: %d, camid: %d\n", Stream->mHwId, Stream->mCamId);
    }

    while (true)
    {
        // wait for frame to be ready (blocking dequeue)
        {
            // NOTE: ioctl() should be cancellation point, but is not working (probably because of file was opened as blocking?)
            // To get by this we allow cancellation outside of cancel points (anytime). This might however lead to problems so we disable
            // the cancellation if not ready for it
            if (0 != ccl_thread_StopAllow())
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation state. hwid: %d, camid: %d\n", Stream->mHwId, Stream->mCamId);
            }

            if (0 != ccl_sem_Take(&Stream->mSchedulePending, CCL_SEM_WAIT_ININITE))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to wait on schedule event - exiting schedule thread.\n");
                return NULL;
            }

            if (0 != ccl_thread_StopBlocked())
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation state. hwid: %d, camid: %d\n", Stream->mHwId, Stream->mCamId);
            }

            if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
            }

            if (0 != ScheduleNextFrame(Stream->mHwId, Stream->mCamId))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to schedule next frame\n");
            }


            if (0 != ccl_mutex_Unlock(&Stream->mLock))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
            }
        }
    }

    /*NOTREACHED*/
    return NULL;
}

/**
 * @brief Start stream procedure streaming mode
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 */
static int StartStream(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (ISPDRV_RES_SUCCESS != ispdrv_ChangeMode(aHwId, aCamId, ISPDRV_MODE_STREAM))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to switch to streaming mode. hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }

    // schedule initial tasks on ISP
    for (uint i = 0; i < MAX_FW_TASK_CNT_PER_STREAM; i++)
    {
        if (0 != ScheduleNextFrame(aHwId, aCamId))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to schedule initial ISP tasks hwid: %d, camid: %d\n", aHwId, aCamId);
            return -1;
        }
    }

    char ThreadName[20];

    if (snprintf(ThreadName, 20, "sched_%02d", aCamId) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }
    if (0 != ccl_thread_Create(&(Stream->mScheduleThread), ScheduleThread, Stream, ThreadName, CCL_TASK_STACK_SIZE_SMALL, CCL_THREAD_PRIORITY_NORMAL))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to start schedule thread. \n");
        return -1;
    }

    return 0;
}

/**
 * @brief Performs start operation on the stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stream's lock must already by acquired before this call
 */
static ccl_Err_t Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (CCL_STREAM_STATE_READY != Stream->mState)
    {
        return CCL_ERR_BAD_STATE;
    }

    Stream->mState = CCL_STREAM_STATE_STARTING;

    switch (Stream->mCapCfg.mCapMode)
    {
        case CCL_CAP_MODE_M2M:
            // no specialized start procedure
            break;

        case CCL_CAP_MODE_STREAM:
            if (0 != StartStream(aHwId, aCamId))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Can't start stream HWID# %u, CID# %u\n", aHwId, aCamId);
                if (0 != Stop(aHwId, aCamId))
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to cleanup stream after failed start. HWID# %u, CID# %u\n", aHwId, aCamId);
                }
                return CCL_ERR_GENERIC;
            }
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Can't start stream -> bad capture mode -> HWID# %u, CID# %u\n", aHwId, aCamId);
            return CCL_ERR_BAD_PARAM;
    }

    if (0 != Stream->mCapOps.mpCapStart(aHwId, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error starting stream.\n");
        if (0 != Stop(aHwId, aCamId))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to cleanup stream after failed start. HWID# %u, CID# %u\n", aHwId, aCamId);
        }
        return CCL_ERR_GENERIC;
    }

    Stream->mState = CCL_STREAM_STATE_RUNNING;

    return CCL_ERR_SUCCESS;
}

/**
 * @brief Performs close operation on the stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @details Stream's lock must already by acquired before this call
 */
static void Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    ccl_FrameTask_t *FrTsk = NULL;

    switch (Stream->mState)
    {
        case CCL_STREAM_STATE_IDLE:
            return;
        case CCL_STREAM_STATE_READY:
            break;
        case CCL_STREAM_STATE_STARTING:
        case CCL_STREAM_STATE_STOPPING:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "ccl_Close() called while in starting/stopping state. Bad sequence, continue as if already stopped. hwid: %d, camid: %d\n", aHwId, aCamId);
            break;
        case CCL_STREAM_STATE_RUNNING:
            (void)Stop(aHwId, aCamId);
            break;
        default:
            return;
    }

    Stream->mCapOps.mpCapClose(aHwId, aCamId);

    // we hold streams lock and we will be freeing all buffers below, so we do not brother about state of anything
    Stream->mCcfStats.mpLastFrame = NULL;
    Stream->mCcfStats.mpCcaFrame = NULL;
    Stream->mCcfStats.mStatsState = CCL_STATS_STATE_NONE;
    ccl_FqClean(&Stream->mAppReadyQueue);

    // free image buffers
    for (uint i = 0; i < CCL_CFG_MAX_BUF_PER_STREAM; i++)
    {
        FrTsk = &Stream->mFramePool[i];
        ccl_FreeImgBufsOut(aHwId, aCamId, FrTsk);
        ccl_FreeIspTask(aHwId, aCamId, FrTsk);
    }
#if CCL_FEATURE_CONTEXT_MATCH
    ccl_CcaCleanCfgHistory(aHwId, aCamId);
#endif

    if (0 != ispdrv_Close(aHwId, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to close ISP pipe. hwid: %d, camid: %d\n", aHwId, aCamId);
    }

    if (Stream->mFTLogFd != -1)
    {
        (void)close(Stream->mFTLogFd);
        Stream->mFTLogFd = -1;
    }
    if (Stream->mSensLogFd != -1)
    {
        (void)close(Stream->mSensLogFd);
        Stream->mSensLogFd = -1;
    }

    Stream->mState = CCL_STREAM_STATE_IDLE;
}


#if CCL_FEATURE_LOG_SENS_CFG
/**
 * @brief Opens time log file, so frame processing times can be recorded
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 */
static int OpenTimeLog(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    char Path[PATH_MAX];

    if (snprintf(Path, sizeof(Path), "FrameTimes_%d_%d.csv", aHwId, aCamId) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }

    Stream->mFTLogFd = open(Path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (Stream->mFTLogFd == -1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to open log for frame times. %s -> %s hwid: %d, camid: %d\n", Path, strerror(errno), aHwId, aCamId);
        return -1;
    }

    char Header[] = "Frame Id, PRE_ISP_PREP, ISP_PROCESSING, ISP_PROCESSED, READY, CCA_PROCESSING, CCA_PROCESSED, APP_PROCESSING, APP_PROCESSED, RECYCLED\n";
    if (-1 == write(Stream->mFTLogFd, Header, sizeof(Header) - 1))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to write header for frame times. %s hwid: %d, camid: %d\n", strerror(errno), aHwId, aCamId);
        return -1;
    }

    return 0;
}
#endif

static inline void UpdateTimeStats(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_TimeStat_t aLatencyId, uint64_t aNewTimeItem)
{
    ccl_TimeStatItem_t *Stats = &gCclState.mStats.mTimeStats[aHwId][aCamId][aLatencyId];

    Stats->mTime[Stats->mWp++] = aNewTimeItem;
    Stats->mWp %= STATS_AVG_PERIOD_FRAMES;

    if (aNewTimeItem > Stats->mMax)
    {
        Stats->mMax = aNewTimeItem;
    }
    if (aNewTimeItem < Stats->mMin)
    {
        Stats->mMin = aNewTimeItem;
    }

    Stats->mAverage = 0;
    for (uint32_t Period = 0U; Period < STATS_AVG_PERIOD_FRAMES; Period++)
    {
        Stats->mAverage += Stats->mTime[Period];
    }
    Stats->mAverage /= STATS_AVG_PERIOD_FRAMES;
}

static inline void UpdateFpsStats(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FpsStatId_t aStatId)
{
    ccl_FpsStat_t *Stats = &gCclState.mStats.mPipes[aHwId][aCamId][aStatId];

    // store new frame time
    Stats->mFrameCnt[Stats->mWp] += 1;
}

static void* StatsRecompute(void* apArg)
{
    (void)apArg;

    while (true)
    {
        ccl_Sleep(1);

        for (uint32_t StatsId = 0U; StatsId < CCL_FPS_STAT_CNT; StatsId++)
        {
            gCclState.mStats.mFpsAll[StatsId] = 0;
            gCclState.mStats.mPixelRateAll[StatsId] = 0;
        }
        for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
        {
            gCclState.mStats.mIspTimePercentage[HwId] = gCclState.mStats.mIspTime[HwId] / 1e9 * 100;
            gCclState.mStats.mIspTime[HwId] = 0;
            for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
            {
                for (uint32_t StatsId = 0U; StatsId < CCL_FPS_STAT_CNT; StatsId++)
                {
                    ccl_FpsStat_t *Stats = &gCclState.mStats.mPipes[HwId][CamId][StatsId];

                    Stats->mFps = 0;
                    for (uint32_t Period = 0U; Period < STATS_AVG_PERIOD_S; Period++)
                    {
                        Stats->mFps += Stats->mFrameCnt[Period];
                    }
                    Stats->mFps /= STATS_AVG_PERIOD_S;
                    Stats->mPixelRate = Stats->mFps * gCclState.mInitialIspCfg[HwId][CamId].mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mWIDTH * gCclState.mInitialIspCfg[HwId][CamId].mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mHEIGHT;
                    gCclState.mStats.mFpsAll[StatsId] += Stats->mFps;
                    gCclState.mStats.mPixelRateAll[StatsId] += Stats->mPixelRate;

                    Stats->mWp++;
                    Stats->mWp %= STATS_AVG_PERIOD_S;
                    Stats->mFrameCnt[Stats->mWp] = 0;
                }
            }
        }


        gCclState.mStats.mCcaFpsAll = 0;
        for (uint32_t CcaGroupIdx = 0; CcaGroupIdx < MAX_CCF_GROUP_CNT; CcaGroupIdx++)
        {
            ccl_CcaStat_t *Stats = &gCclState.mStats.mCca[CcaGroupIdx];

            Stats->mFps = 0;
            for (uint32_t Period = 0U; Period < STATS_AVG_PERIOD_S; Period++)
            {
                Stats->mFps += Stats->mPassCnt[Period];
            }
            Stats->mFps /= STATS_AVG_PERIOD_S;
            gCclState.mStats.mCcaFpsAll += Stats->mFps;

            Stats->mWp++;
            Stats->mWp %= STATS_AVG_PERIOD_S;
            Stats->mPassCnt[Stats->mWp] = 0;
        }
    }

    /*NOTREACHED*/
    return NULL;
}


#if CCL_FEATURE_LOG_SENS_CFG
/**
 * @brief Opens sensor log file, so sensor cfg changes can be recorded
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 */
static int OpenSensLog(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (0 != ccl_mutex_Lock(&Stream->mSensLogLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
    }

    char Path[PATH_MAX];

    if (snprintf(Path, sizeof(Path), "sens_prm_log_%u_%u.csv", aHwId, aCamId) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }

    Stream->mSensLogFd = open(Path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (Stream->mSensLogFd == -1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to open log for sensor cfg history. %s -> %s hwid: %d, camid: %d\n", Path, strerror(errno), aHwId, aCamId);
        return -1;
    }

    char Line[2048];
    int size = 0;
    size += SerializeSensPrmHeader(Line, sizeof(Line));
    if (size <= 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to write header for sensor cfg history. hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }

    if (-1 == write(Stream->mSensLogFd, Line, size))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to write frame times - %s. hwid: %d, camid: %d\n", strerror(errno), aHwId, aCamId);
    }

    if (0 != ccl_mutex_Unlock(&Stream->mSensLogLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
    }

    return 0;
}
#endif

/**
 * @brief Performs close operation on the stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apCapCfg - capture parameters as given to us by client app
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stream's lock must already by acquired before this call
 */
static ccl_Err_t Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapCfg_t *apCapCfg)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (apCapCfg == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Capture config must not be NULL. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }
    if (apCapCfg->mMaxCfgHistCnt < 1 || apCapCfg->mMaxCfgHistCnt > SENS_CFG_HISTORY_BUF_SIZE)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Stream cfg history size must be ate least 1 and max %d. hwid: %d, camid: %d\n", aHwId, aCamId, SENS_CFG_HISTORY_BUF_SIZE);
        return CCL_ERR_BAD_PARAM;
    }
    if (apCapCfg->mBufCnt < MIN_FRAME_TASKS_PER_STREAM)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "minimum number of buffers per stream is %d. hwid: %d, camid: %d\n", MIN_FRAME_TASKS_PER_STREAM, aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }
    if (apCapCfg->mCapMode >= CCL_CAP_MODE_COUNT)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Bad capture mode - %d. hwid: %d, camid: %d\n", apCapCfg->mCapMode, aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }
    if (aHwId >= ISP_HWID_CNT || aCamId >= ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Stream Is out of range. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }

    if (gCclState.mInitCfg.mpCams[aHwId][aCamId].mEnabled != 1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Stream was not enabled during init. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }
    if (Stream->mState != CCL_STREAM_STATE_IDLE)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Stream already active. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_BAD_STATE;
    }


    Stream->mHwId = aHwId;
    Stream->mCamId = aCamId;
    memcpy(&Stream->mCapCfg, apCapCfg, sizeof(ccl_CapCfg_t));
    if (0 != SetCapOps(aHwId, aCamId, apCapCfg->mCaptureMethod))
    {
        // Note: error message already set
        Close(aHwId, aCamId);
        return CCL_ERR_GENERIC;
    }


#if CCL_FEATURE_LOG_SENS_CFG
    if (Stream->mCapCfg.mLogSensCfg)
    {
        if (0 != OpenSensLog(aHwId, aCamId))
        {
            return CCL_ERR_GENERIC;
        }
    }
#endif

    if (0 != AllocateBufs(aHwId, aCamId, apCapCfg->mBufCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to initialize sensor. hwid: %d, camid: %d\n", aHwId, aCamId);
        Close(aHwId, aCamId);
        return CCL_ERR_GENERIC;
    }

    // This should
    // - initialize capture API (includes input buffer allocation)
    // - via capture API it will power up the sensor
    // - prepare sensor for operation
    // - read its calibration or in other means construct sensor parameters
    if (0 != Stream->mCapOps.mpCapOpen(aHwId, aCamId, &apCapCfg->mCapParams, &gCclState.mInitCfg.mpCams[aHwId][aCamId]))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to initialize sensor. hwid: %d, camid: %d\n", aHwId, aCamId);
        Close(aHwId, aCamId);
        return CCL_ERR_SENSOR;
    }

    if (0 != ccl_CcaSetDefaultCfg(aHwId, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to compute and set initial sensor values. hwid: %d, camid: %d\n", aHwId, aCamId);
        Close(aHwId, aCamId);
        return CCL_ERR_GENERIC;
    }

    ispdrv_OpenCfg_t IspDrvCfg;
    IspDrvCfg.mpClientPtr = Stream;
    IspDrvCfg.mEventCback = ccl_IspCallback;

    if (0 != ispdrv_Open(aHwId, aCamId, &IspDrvCfg))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to initialize ISP pipe. hwid: %d, camid: %d\n", aHwId, aCamId);
        Close(aHwId, aCamId);
        return CCL_ERR_ISP;
    }

#if CCL_FEATURE_LOG_FRAME_TIMES
    if (Stream->mCapCfg.mLogFrameTimes)
    {
        if (0 != OpenTimeLog(aHwId, aCamId))
        {
            return CCL_ERR_GENERIC;
        }
    }
#endif

    Stream->mState = CCL_STREAM_STATE_READY;

    return CCL_ERR_SUCCESS;
}

/**
 * @brief Call from APP requesting new image
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - variable that receive new image data
 * @param aTimeout - timeout on frame wait
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stream's lock must already by acquired before this call
 */
static ccl_Err_t PushFrameTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrameTask)
{
    if (!IS_IN_STATE(apFrameTask, CCL_FRAME_STATE_APP_PROCESSING)
        || IS_IN_STATE(apFrameTask, CCL_FRAME_STATE_APP_PROCESSED))
    {
        // Frame has bad state
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Invalid frame state for PutFrame. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_BAD_STATE;
    }

    // signal app processed state
    SetFrameState(apFrameTask, CCL_FRAME_STATE_APP_PROCESSED);

    // recycle if all done
    if (IS_IN_STATE(apFrameTask, CCL_FRAME_STATE_CCA_PROCESSED))
    {

        RecycleFrameTask(aHwId, aCamId, apFrameTask, true);
    }

    return CCL_ERR_SUCCESS;
}

static ccl_Err_t PushFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    ccl_FrameTask_t *FrameTask = NULL;
    for (uint i = 0; i < CCL_CFG_MAX_BUF_PER_STREAM; i++)
    {
        if (&Stream->mFramePool[i].FrameData == apFrame)
        {
            FrameTask = &Stream->mFramePool[i];
            break;
        }
    }

    if (FrameTask == NULL)
    {
        // FrameTask with given public part (ccl_Frame_t) is not on a list
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Invalid frame for PutFrame. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_BAD_PARAM;
    }

    return PushFrameTask(aHwId, aCamId, FrameTask);
}

/**
 * @brief Call from APP requesting new image
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - variable that receive new image data
 * @param aTimeout - timeout on frame wait
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stream's lock must already by acquired before this call
 */
static ccl_Err_t PopFrameTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t **apFrameTask, uint32_t aTimeout)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    *apFrameTask = NULL;

    while (0 == ccl_FqGetCnt(&Stream->mAppReadyQueue))
    {
        if (0 != ccl_mutex_Unlock(&Stream->mLock))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
            return CCL_ERR_GENERIC;
        }

        // NOTE: repeated calls can in sum exceed original timeout
        ccl_Err_t res = ccl_sem_Take(&Stream->mFrameReadyEvent, aTimeout);

        if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
            return CCL_ERR_GENERIC;
        }

        if (0 != res)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Error waiting for frame. hwid: %d, camid: %d. %s\n", aHwId, aCamId, strerror(res));
            return res;
        }
    }

    *apFrameTask = ccl_FqPop(&Stream->mAppReadyQueue);
    if (NULL == *apFrameTask)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Error reading ready frame. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CCL_ERR_GENERIC;
    }

    SetFrameState(*apFrameTask, CCL_FRAME_STATE_APP_PROCESSING);

    return CCL_ERR_SUCCESS;
}

/**
 * @brief Call from APP requesting new image
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - variable that receive new image data
 * @param aTimeout - timeout on frame wait
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stream's lock must already by acquired before this call
 */
static ccl_Err_t GetFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t **apFrame, uint32_t aTimeout)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    ccl_FrameTask_t *ReturnedFrame = Stream->mAppAssignedFrame;
    if (ReturnedFrame != NULL)
    {
        PushFrameTask(aHwId, aCamId, ReturnedFrame);
    }

    ccl_FrameTask_t *FrameTask;
    ccl_Err_t ret = PopFrameTask(aHwId, aCamId, &FrameTask, aTimeout);
    if (FrameTask != NULL)
    {
        *apFrame = &FrameTask->FrameData;
    }
    else
    {
        *apFrame = NULL;
    }

    // store given frame task so we can recycle it during next GetFrame call
    Stream->mAppAssignedFrame = FrameTask;

    return ret;
}

/**
 * @brief Request from capture method for a frame task
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param[out] apFrame - variable that receives the task (can be NULL)
 */
static ccl_CapBuf_t *DropReady(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    ccl_FrameTask_t *Frame;

    // check APP queue for old frames to drop
    if (1 >= ccl_FqGetCnt(&Stream->mAppReadyQueue))
    {
        // there is not enough frames waiting for APP (we want to have at least one ready)
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "No buffer to drop (no frame ready for app)! hwid: %d, camid: %d\n", aHwId, aCamId);
        return NULL;
    }

    Frame = Stream->mAppReadyQueue.mItems[Stream->mAppReadyQueue.mRp];
    if (!IS_IN_STATE(Frame, CCL_FRAME_STATE_CCA_PROCESSED))
    {
        // we can't recycle tasks that could be running on CCF
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Oldest frame not finished with CCA! hwid: %d, camid: %d\n", aHwId, aCamId);
        return NULL;
    }
    if (Frame->mInBuf == NULL)
    {
        // recycling frame without input buffer will not help us.
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Oldest frame without input buffer. hwid: %d, camid: %d\n", aHwId, aCamId);
        return NULL;
    }

    ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Capture system needs buffer, dropping no:%d hwid: %d, camid: %d\n", Frame->FrameData.mMetaData.mFrameNo , aHwId, aCamId);

    ccl_CapBuf_t *ret = Frame->mInBuf;
    RecycleFrameTask(aHwId, aCamId, Frame, false);

    return ret;
}

/**
 * @brief Frame ready event - all image data (both in and out) and other info are now ready
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param[out] apFrame - Ready frame
 *
 * @details Signals to client APP that frame is available, and also initiates CCF processing for sensor and ISP adaptation to new scene
 */
static void FrameReady(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (Stream->mState != CCL_STREAM_STATE_RUNNING)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ISP_DONE received -> Stream not in running state -> dropping HWID# %u, CID# %u\n", aHwId, aCamId);
        apFrame->mStateFlags = CCL_FRAME_STATE_EMPTY;
        return;
    }

    if (apFrame->mIspError != 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Frame finished ISP processing with error -> dropping HWID# %"PRIu32", CID# %"PRIu32", IspTask:0x%"PRIx64", No:%"PRIu64", ERR# 0x%"PRIx32"\n", aHwId, aCamId, apFrame->mIspPhy, apFrame->FrameData.mMetaData.mFrameNo, apFrame->mIspError);
        DropFrame(aHwId, aCamId, apFrame);
        return;
    }

    apFrame->mIspCfg->mIspCtx.mCtxMeta.mReadyTimestampNs = ccl_GetTime() * 1000U;
    UpdateTimeStats(aHwId, aCamId, CCL_TIME_CAPTURE_TO_APP, apFrame->mIspCfg->mIspCtx.mCtxMeta.mReadyTimestampNs - apFrame->mIspCfg->mIspCtx.mCtxMeta.mCaptureTimestampNs);

    // ** ADD frame to APP-processing **
    SetFrameState(apFrame, CCL_FRAME_STATE_READY);

    if (0 != ccl_FqPush(&Stream->mAppReadyQueue, apFrame))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to push frame to ready list -> dropping HWID# %"PRIu32", CID# %"PRIu32", IspTask:0x%"PRIx64", No:%"PRIu64", ERR# 0x%"PRIu32"\n", aHwId, aCamId, apFrame->mIspPhy, apFrame->FrameData.mMetaData.mFrameNo, apFrame->mIspError);
        DropFrame(aHwId, aCamId, apFrame);
        return;
    }

    // notify APP processing threads (or just increase frame waiting counter)
    if (0 != ccl_sem_Give(&Stream->mFrameReadyEvent))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to send Frame ready event to APP.\n");
    }
#if CCL_FEATURE_RUNTIME_CONTROL
#ifdef GUI_DEMO
        if (gCclState.mpExtCtrlCtx->mSubscriber != NULL)
        {
            guiCtrl_ProcessParameters(gCclState.mpExtCtrlCtx);
        }
#endif
#endif
    // ** ADD frame to CCA-processing **
    ccl_CcaUpdate(aHwId, aCamId, apFrame);
}

/**
 * @brief Assigns input image data (and other like embedded data) with given CCL frame task (output data, etc.)
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param apInImg - Input image collection
 * @param apFrTask - output image (ISP processing) collection
 */
static void AssignInImg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t *apInImg, ccl_FrameTask_t *apFrTask)
{
    // associate new input image with frame task
    apFrTask->mInBuf = apInImg;
    apFrTask->FrameData.mMetaData.mFrameNo = apInImg->mFrameNo;
    if (apInImg->mCaptureTimeNs == 0)
    {
        // If capture was not set by capture system, use current time to get at least some meaningful number
        apFrTask->mIspCfg->mIspCtx.mCtxMeta.mCaptureTimestampNs = ccl_GetTime() * 1000U;
    }
    else
    {
        // use capture time from capture system
        apFrTask->mIspCfg->mIspCtx.mCtxMeta.mCaptureTimestampNs = apInImg->mCaptureTimeNs;
    }

    // shallow copy of embed data
    apFrTask->FrameData.mEmbed = apInImg->mEmbed;

    for (int i = CCA_IOBUFT_IN_FIRST; i <= CCA_IOBUFT_IN_LAST; i++)
    {
        // shallow copy of plane buffer
        apFrTask->FrameData.mImg[i] = apInImg->mImg[i - CCA_IOBUFT_IN_FIRST];
    }

    // compute sensor setting based on embedded data from input
    if (gCclState.mInitCfg.mCcfGroups[aHwId][aCamId] > 0)
    {
        memset(&apFrTask->mActSensCfg, 0, sizeof(cca_SensPrm_t));
        if (apFrTask->FrameData.mEmbed.mEmbd0Sz > 0 || apFrTask->FrameData.mEmbed.mEmbd1Sz > 0)
        {
            if (0 != cca_Embd2Prm(aCamId, &apFrTask->FrameData.mEmbed, &apFrTask->mActSensCfg))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to translate embedded data into cca sensor config. hwid: %d, camid: %d\n", aHwId, aCamId);
            }
        }
#if CCL_FEATURE_CAPTURE_FILE
        // in case of file based input use frame index from capture method
        if(gCclState.Streams[aHwId][aCamId].mCapCfg.mCaptureMethod == CCL_CAP_VIA_FILE)
        {
            apFrTask->mActSensCfg.mFrameCount = apInImg->mFrameNo;
        }
#endif

        // get last applied lens position
        apFrTask->mActSensCfg.mLensPos = apInImg->mLensPosition;
    }
}

/**
 * @brief Handler of new image when in M2M mode
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param apNewImg - new Image
 *
 * @details Initiates embedded data reconstruction and ISP cfg search, then finally schedules image on ISP
 */
static void NewImageM2m(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t *apNewImg)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    ccl_FrameTask_t *FrTask;

    FrTask = GetFreeBuf(aHwId, aCamId);
    if (FrTask == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "New Frame received -> Unable to get free frame task -> dropping HWID# %u, CID# %u\n", aHwId, aCamId);
        Stream->mCapOps.mpCapBufReturn(aHwId, aCamId, apNewImg);
        return;
    }

    AssignInImg(aHwId, aCamId, apNewImg, FrTask);

    if (0 != ccl_CcaFindIspCfgM2M(aHwId, aCamId, FrTask))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Error during context matching -> dropping frame. hwid: %d, camid: %d\n", aHwId, aCamId);
        DropFrame(aHwId, aCamId, FrTask);
        return;
    }

    if (0 != Schedule(aHwId, aCamId, FrTask))
    {
        // error printing and frame dropping inside due to state change
        return;
    }
}

/**
 * @brief Handler of new image when in Streaming mode
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param apNewImg - new Image
 *
 * @details Looks if output data are already ready, if so initiates FrameReady event, if not stores new image and waits for output data
 */
static void NewImageStreaming(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t *apNewImg)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (Stream->mArmedOutImage != NULL)
    {
        ccl_FrameTask_t *FullyDoneTask = Stream->mArmedOutImage;
        Stream->mArmedOutImage = NULL;
        AssignInImg(aHwId, aCamId, apNewImg, FullyDoneTask);

        FrameReady(aHwId, aCamId, FullyDoneTask);
    }
    else
    {
        if (Stream->mArmedInImg != NULL)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Received Input image while old is still waiting its output match -> dropping old IN frame no: %d. hwid: %d, camid: %d\n", Stream->mArmedInImg->mFrameNo, aHwId, aCamId);
            Stream->mCapOps.mpCapBufReturn(aHwId, aCamId, Stream->mArmedInImg);
        }
        Stream->mArmedInImg = apNewImg;
    }

    return;
}

/**
 * @brief Signal from capture method that frame was generated
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param[in] apNewImg - newly generated image
 */
static void NewInImg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t *apNewImg)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (Stream->mState != CCL_STREAM_STATE_RUNNING)
    {
        // Dropping a late in-flight frame during stop is normal, not an error
        if (Stream->mState != CCL_STREAM_STATE_STOPPING)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "New Frame received -> Stream not in running state -> dropping HWID# %u, CID# %u\n", aHwId, aCamId);
        }
        Stream->mCapOps.mpCapBufReturn(aHwId, aCamId, apNewImg);
        return;
    }

#ifdef STORE_EMB_DATA
    char Path[4096];
    if (snprintf(Path, sizeof(Path), "emb_%06lu.bin", apNewImg->mFrameNo) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }

    int Fd = open(Path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (Fd != -1)
    {
        write(Fd, apNewImg->mEmbed.mpEmbd0, apNewImg->mEmbed.mEmbd0Sz);
        close(Fd);
    }
#endif

    UpdateFpsStats(aHwId, aCamId, CCL_FPS_STAT_CAP);

    switch (Stream->mCapCfg.mCapMode)
    {
        case CCL_CAP_MODE_M2M:
            NewImageM2m(aHwId, aCamId, apNewImg);
            break;

        case CCL_CAP_MODE_STREAM:
            NewImageStreaming(aHwId, aCamId, apNewImg);
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "New Frame received -> bad capture mode -> dropping HWID# %u, CID# %u\n", aHwId, aCamId);
            Stream->mCapOps.mpCapBufReturn(aHwId, aCamId, apNewImg);
            return;
    }
}

/**
 * @brief Processing of frame DONE event for sterming mode
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - newly processed frame
 */
static void DoneIspFrameStream(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (Stream->mArmedInImg != NULL)
    {
        AssignInImg(aHwId, aCamId, Stream->mArmedInImg, apFrame);
        Stream->mArmedInImg = NULL;

        FrameReady(aHwId, aCamId, apFrame);
    }
    else
    {
        if (Stream->mArmedOutImage != NULL)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Received new output image while old is still waiting its input match -> dropping old OUT frame. hwid: %d, camid: %d\n", aHwId, aCamId);

            DropFrame(aHwId, aCamId, Stream->mArmedOutImage);
        }
        Stream->mArmedOutImage = apFrame;
    }

    if (0 != ccl_sem_Give(&Stream->mSchedulePending))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post schedule event\n");
    }
}

static inline void DoneIspFrameStats(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    apFrame->mIspCfg->mIspCtx.mCtxMeta.mIspDoneTimestampNs = ccl_GetTime() * 1000U;
    uint64_t ContextSwap = apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_DDR_TO_REGS_DONE] - apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_TRIGGER_DDR_TO_REGS];
    ContextSwap += apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_DDR_TO_MEMS_DONE] - apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_TRIGGER_DDR_TO_MEMS];
    ContextSwap += apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_REGS_TO_DDR_DONE] - apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_TRIGGER_REGS_TO_DDR];
    ContextSwap +=  apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_MEMS_TO_DDR_DONE] - apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_TRIGGER_MEMS_TO_DDR];
    uint64_t isp_time = apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_FRAME_DONE] - apFrame->mIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[FW_TIME_TRIGGER_ISP];
    gCclState.mStats.mIspTime[aHwId] += isp_time*1000;
    UpdateTimeStats(aHwId, aCamId, CCL_TIME_ISP_CONTEXT_SWAP, ContextSwap*1000);
    UpdateTimeStats(aHwId, aCamId, CCL_TIME_ISP_PROC_TIME, isp_time*1000);
    UpdateFpsStats(aHwId, aCamId, CCL_FPS_STAT_FW);
}

/**
 * @brief Entry point of FRAME_DONE event generated by ISP
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - newly processed frame
 */
static void DoneIspFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!IS_IN_STATE(apFrame, CCL_FRAME_STATE_ISP_PROCESSING) || IS_IN_STATE(apFrame, CCL_FRAME_STATE_ISP_PROCESSED))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "ISP Done received for frame that is not scheduled on ISP HWID# %u, CID# %u\n", aHwId, aCamId);
        apFrame->mStateFlags = CCL_FRAME_STATE_EMPTY;
        return;
    }

    Stream->mScheduledCnt--;
    if (Stream->mScheduledCnt < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Scheduled count negative! HWID# %u, CID# %u\n", aHwId, aCamId);
    }

    if (Stream->mState != CCL_STREAM_STATE_RUNNING)
    {
        // do not display error for normal behavir
        if (Stream->mState != CCL_STREAM_STATE_STOPPING)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ISP Done received -> Stream not in running state -> dropping HWID# %u, CID# %u\n", aHwId, aCamId);
        }
        apFrame->mStateFlags = CCL_FRAME_STATE_EMPTY;
        return;
    }

    DoneIspFrameStats(aHwId, aCamId, apFrame);

    SetFrameState(apFrame, CCL_FRAME_STATE_ISP_PROCESSED);

    if (Stream->mCapOps.mpFrameDoneEvt != NULL)
    {
        Stream->mCapOps.mpFrameDoneEvt(aHwId, aCamId);
    }

    switch (Stream->mCapCfg.mCapMode)
    {
        case CCL_CAP_MODE_M2M:
            FrameReady(aHwId, aCamId, apFrame);
            break;

        case CCL_CAP_MODE_STREAM:
            DoneIspFrameStream(aHwId, aCamId, apFrame);
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ISP Done received -> bad capture mode -> dropping HWID# %u, CID# %u\n", aHwId, aCamId);
            RecycleFrameTask(aHwId, aCamId, apFrame, true);
            return;
    }
}


/**
 * @brief Signals CCA done on the frame, recycles it if APP is also finished
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - frame task to release
 *
 */
static void FreeFrameFromCca(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrame)
{
    if (apFrame == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "FreeFrameFromCca apFrame == NULL\n");
        return;
    }

    ccl_SetFrameState(aHwId, aCamId, apFrame, CCL_FRAME_STATE_CCA_PROCESSED);
    if (IS_IN_STATE(apFrame, CCL_FRAME_STATE_APP_PROCESSED))
    {
        // app already processed it we can reuse the buffer now
        ccl_RecycleFrameTask(aHwId, aCamId, apFrame, true);
    }
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

ccl_Err_t ccl_Init(ccl_Cfg_t *apCclCfg)
{
    if (gCclInitialized != 0)
    {
        return CCL_ERR_BAD_STATE;
    }
    if (apCclCfg == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }
    gCclInitialized = 2;

    if ( 0 != VarInit())
    {
        gCclInitialized = 0;
        return CCL_ERR_GENERIC;
    }

    // initialized shared memory
    if (!MShr_Init(MSHR_MODE_OWNER))
    {
        gCclInitialized = 0;
        return CCL_ERR_GENERIC;
    }

#if CCL_FEATURE_RUNTIME_CONTROL
    // check external control context validity
    if (0 != ValidateExtCtrlCtx(apCclCfg->mpExtCtrlCtx))
    {
        gCclState.mpExtCtrlCtx = NULL;
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Validation of external control context was not successfull. \n");
    }
    else
    {
        gCclState.mpExtCtrlCtx = apCclCfg->mpExtCtrlCtx;
    }
#endif

    // store configuration from client
    memcpy(&gCclState.mInitCfg, apCclCfg, sizeof(ccl_Cfg_t));
    // cca_CamInit_t defines .mcpExtDef as pointers, we need to make a
    // copy of the values and re-point to new mem in locally stored initial cfg
    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            // update mInitCfg (now identical copy of apCclCfg) to point into ccl's internal memory
            gCclState.mInitCfg.mpCams[HwId][CamId].mcpExtDef = &gCclState.mInitialIspCfg[HwId][CamId];

            // ... and then copy apCclCfg values into that internal memory
            if (apCclCfg->mpCams[HwId][CamId].mcpExtDef == NULL)
            {
                memset(&gCclState.mInitialIspCfg[HwId][CamId], 0, sizeof(isp_CtxHw_t));
            }
            else
            {
                memcpy(&gCclState.mInitialIspCfg[HwId][CamId], apCclCfg->mpCams[HwId][CamId].mcpExtDef, sizeof(isp_CtxHw_t));
            }
        }
    }

    if (0 != ccl_CcaInit())
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error initializing CCA subsystem\n");
        gCclInitialized = 0;
        return CCL_ERR_CCA;
    }

    char ThreadName[] = "ccl_stats";
    if (0 != ccl_thread_Create(&gCclState.mStats.mStatsPeriodThread, StatsRecompute, NULL, ThreadName, CCL_TASK_STACK_SIZE_SMALL, CCL_THREAD_PRIORITY_NORMAL))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to start schedule thread. \n");
        return CCL_ERR_BAD_STATE;
    }

    gCclInitialized = 1;
    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_Deinit(void)
{
    if (gCclInitialized != 1)
    {
        return CCL_ERR_BAD_STATE;
    }
    gCclInitialized = 2;

    if (0 != ccl_thread_Cancel(&gCclState.mStats.mStatsPeriodThread))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to stop ccl stats thread.\n");
    }

    for (ispdrv_HwId_t HwId = ISP_HWID_0; HwId < ISP_HWID_CNT; HwId++)
    {
        for (ispdrv_CamPipeId_t CamId = ISPDRV_CAM_PIPE_ID_0; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Close(HwId, CamId);
        }
    }

    ccl_CcaDeinit();

    MShr_DeInit();

    VarDeInit();

    gCclInitialized = 0;

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapCfg_t *apCapCfg)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_Err_t ret = Open(aHwId, aCamId, apCapCfg);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_Err_t ccl_Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    Close(aHwId, aCamId);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_Err_t ret = Start(aHwId, aCamId);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_Err_t ccl_Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_Err_t ret = Stop(aHwId, aCamId);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_Err_t ccl_GetFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t **apFrame, uint32_t aTimeout)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_Err_t ret = GetFrame(aHwId, aCamId, apFrame, aTimeout);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_Err_t ccl_PopFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t **apFrame, uint32_t aTimeout)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_FrameTask_t *FrameTask;
    ccl_Err_t ret = PopFrameTask(aHwId, aCamId, &FrameTask, aTimeout);
    if (FrameTask != NULL)
    {
        *apFrame = &FrameTask->FrameData;
    }
    else
    {
        *apFrame = NULL;
    }

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_Err_t ccl_PushFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_Err_t ret = PushFrame(aHwId, aCamId, apFrame);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_CapBuf_t *ccl_DropReady(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_DropReady() call while not initialized!\n");
        return NULL;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return NULL;
    }

    ccl_CapBuf_t *ret = DropReady(aHwId, aCamId);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return NULL;
    }

    return ret;
}

/******************************************************************************/

void ccl_NewInImg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t *apNewImg)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }

    NewInImg(aHwId, aCamId, apNewImg);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return;
    }
}

/******************************************************************************/

void ccl_DoneIspFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }

    DoneIspFrame(aHwId, aCamId, apFrame);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return;
    }
}

/******************************************************************************/

void ccl_RecycleFrameTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame, bool aPostEvent)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }

    RecycleFrameTask(aHwId, aCamId, apFrame, aPostEvent);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return;
    }
}

/******************************************************************************/

float ccl_GetFps(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FpsStatId_t aStatId, bool aGlobal)
{
    if (aGlobal)
    {
        return gCclState.mStats.mFpsAll[aStatId];
    }
    else
    {
        return gCclState.mStats.mPipes[aHwId][aCamId][aStatId].mFps;
    }
}

/******************************************************************************/

float ccl_GetPixelRate(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FpsStatId_t aStatId, bool aGlobal)
{
    if (aGlobal)
    {
        return gCclState.mStats.mPixelRateAll[aStatId];
    }
    else
    {
        return gCclState.mStats.mPipes[aHwId][aCamId][aStatId].mPixelRate;
    }
}

/******************************************************************************/

void ccl_GetTiming(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_TimeStat_t aStatId, float apLatency[3])
{
    apLatency[0] = gCclState.mStats.mTimeStats[aHwId][aCamId][aStatId].mMin;
    apLatency[1] = gCclState.mStats.mTimeStats[aHwId][aCamId][aStatId].mAverage;
    apLatency[2] = gCclState.mStats.mTimeStats[aHwId][aCamId][aStatId].mMax;
}

/******************************************************************************/

void ccl_GetCcaTiming(float apCcaTimes[3])
{
    apCcaTimes[0] = gCclState.mStats.mGlobalCcaTimes.mMin;
    apCcaTimes[1] = gCclState.mStats.mGlobalCcaTimes.mAverage;
    apCcaTimes[2] = gCclState.mStats.mGlobalCcaTimes.mMax;
}

/******************************************************************************/

float ccl_GetIspUse(ispdrv_HwId_t aHwId)
{
    return gCclState.mStats.mIspTimePercentage[aHwId];
}

/******************************************************************************/

void ccl_SetFrameState(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame, ccl_FrameState_t aFinishedStage)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }

    SetFrameState(apFrame, aFinishedStage);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return;
    }
}

/******************************************************************************/

void ccl_FreeFrameFromCca(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrame)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclInitialized)
    {
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }

    FreeFrameFromCca(aHwId, aCamId, apFrame);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return;
    }
}

/******************************************************************************/

uint8_t ccl_GetIspIbufBitsPPCode2Val(uint8_t aImgSz)
{
    if (aImgSz >= (sizeof(gcBppToBits) / sizeof(gcBppToBits[0])))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_GetIspIbufBitsPPCode2Val bad image pixel size enum value!\n");
        return 0;
    }

    return gcBppToBits[aImgSz];
}

/******************************************************************************/
uint32_t ccl_GetBitsPP2BytesPP(uint32_t aBpp)
{
    return (aBpp + 7) / 8;
}

/******************************************************************************/
