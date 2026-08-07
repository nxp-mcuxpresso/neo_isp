/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ccl_cap_memcpy.c
 * @brief Implements simulated capture system by copying data from memory
 * @details Internal module of ccl. Each streams gets its own thread that reads
 * data in periodic intervals (simulated configurable fps) signals new
 * frame event to ccl.
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "ilog.h"
#include "ccl_cap_memcpy.h"
#include "ccl.h"
#include "ccl_cca.h"

#include "mshr.h"

#include "cca_ovx3c.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

#define CCL_CAP_IMAGE_SIZE_BYTES  (2*1920*1282)


typedef enum
{
    CCL_MEMCPY_BUF_STATE_IDLE,
    CCL_MEMCPY_BUF_STATE_ON_CAPTURE,
    CCL_MEMCPY_BUF_STATE_CCL,
    CCL_MEMCPY_BUF_STATE_CNT
}ccl_memcpy_BufState_t;

/** @brief structure holding information about single capture buffer (contains multiple planes) */
typedef struct
{
    ccl_CapBuf_t mExternal;

    /** @brief Indication where is the buffer currently assigned. */
    ccl_memcpy_BufState_t mState;
} BufInfo_t;

/** @brief State variables for single stream */
typedef struct
{
    /** @brief Thread id while the capture is running */
    ccl_thread_t mThreadId;

    /** @brief Hw ID associated with this stream*/
    ispdrv_HwId_t mHwId;
    /** @brief Cam ID associated with this stream*/
    ispdrv_CamPipeId_t mCamId;

    /** @brief Frame Id since the start of capture */
    uint32_t mCurrentFrameId;
    /** @brief Time of next frame (to maintain simulated FPS)*/
    double mNextFrameTime;
    /** @brief Counter of frames in CCL pipe - all frames*/
    ccl_sem_t mFreeBufferCnt;
    /** @brief Counter of frames in CCL pipe - between this code and FRAME_DONE event */
    ccl_sem_t mIspFreeCnt;

    /** @brief Capture parameters for this stream as supplied during init()*/
    ccl_CapCfgMemcpy_t mCfg;

    /** @brief Number of planes in input image
     * @details This number is internally generated, based on ccl_CapCfgMemcpy_t external inputs. */
    uint32_t mNoOfPlanes;
    /** @brief Number of frames to generate before stop
     * @details This number is internally generated, based on ccl_CapCfgMemcpy_t external inputs. */
    uint32_t mNoFramesToGenerate;

    /** @brief Local list of allocated buffers
     * @details This info is needed because:
     * - each ccl capture subsystem allocates its buffers
     * - we need to have buffer id (index into this array) to use as a reference when interacting with CCL */
    BufInfo_t mBufs[BUF_COUNT_MAX_MEMCPY];
} CapState_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/** @brief state variables for all possible streams */
DDR_NOINIT static CapState_t gCapState[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];
DDR_NOINIT static uint8_t gInImg1[CCL_CAP_IMAGE_SIZE_BYTES];
DDR_NOINIT static uint8_t gInImg2[CCL_CAP_IMAGE_SIZE_BYTES];

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/**
 * @brief Get current time in seconds
 *
 * @return Time in seconds
 */
static inline double GetNow(void)
{
    return ((double)ccl_GetTime()) / 1.0e6;
}


/**
 * @brief Flush in img data buffers for given task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Task to prepare
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t FlushImgBufs(CapState_t *apDev, BufInfo_t *apBufNfo)
{

    if (apBufNfo == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "apBufNfo is NULL!\n");
        return -1;
    }

    if (0 == apDev->mNoOfPlanes || apDev->mNoOfPlanes > CCL_INBUF_PLANE_CNT)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Number of planes out of range!\n");
        return -1;
    }

    for (int8_t PlaneIdx = apDev->mNoOfPlanes-1; PlaneIdx >= 0; PlaneIdx--)
    {
        if (apBufNfo->mExternal.mImg[PlaneIdx].mpData != NULL)
        {
            if (0 != MShr_Transfer2remote(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), MSHR_DIR_FROM_PC, apBufNfo->mExternal.mImg[PlaneIdx].mpData, NULL))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to flush In img buffer %d\n", PlaneIdx);
                return -1;
            }
        }
    }

    return 0;
}

#pragma GCC push_options
#pragma GCC optimize ("O0")
__attribute__((__used__)) DDR_INIT static uint32_t PreloadComplete = 123;
static void CheckDdrPrload(void)
{
    while (PreloadComplete != 0xA5AA550F)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "USE LAUTERBACH to load images.\n");
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "addr img 0: %p\n", gInImg1);
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "addr img 1: %p\n", gInImg2);
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "once done set %p to 0xA5AA550F\n", &PreloadComplete);
        ccl_Sleep(10);
    }
}
#pragma GCC pop_options

/**
 * @brief Loads next file(s) into frame task structure.
 *
 * @param apState Capture state of current stream
 * @param apFrame Frame task to fill with new data
 * @return true
 * @return false
 */
static bool LoadFrame(CapState_t* apDev, BufInfo_t* apBufNfo)
{
    static uint8_t ImgIdx = 0;
    uint8_t *gInImg[2] = {gInImg1, gInImg2};
    if (apBufNfo == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "apFrame is NULL!\n");
        return false;
    }
    if (0 == apDev->mNoOfPlanes || apDev->mNoOfPlanes > CCL_INBUF_PLANE_CNT)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Number of planes out of range!\n");
        return false;
    }

    CheckDdrPrload();

    for (int8_t PlaneIdx = apDev->mNoOfPlanes-1; PlaneIdx >= 0; PlaneIdx--)
    {
        if (apBufNfo->mExternal.mImg[PlaneIdx].mpData == NULL)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "In buffer have no mem allocated!\n");
            return false;
        }

        // update metadata from generator state
        apBufNfo->mExternal.mFrameNo = apDev->mCurrentFrameId;

        ImgIdx = (ImgIdx+1) % 2;
        uint32_t InOffset = 0;
        if (apBufNfo->mExternal.mEmbed.mpEmbd0 != NULL)
        {
            memcpy(apBufNfo->mExternal.mEmbed.mpEmbd0, gInImg[ImgIdx] + InOffset, apBufNfo->mExternal.mEmbed.mEmbd0Sz);
            InOffset += apBufNfo->mExternal.mEmbed.mEmbd0Sz;
        }
        memcpy(apBufNfo->mExternal.mImg[PlaneIdx].mpData, gInImg[ImgIdx] + InOffset, apBufNfo->mExternal.mImg[PlaneIdx].mSizeBytes);
        if (apBufNfo->mExternal.mEmbed.mpEmbd1 != NULL)
        {
            memcpy(apBufNfo->mExternal.mEmbed.mpEmbd1, gInImg[ImgIdx] + InOffset, apBufNfo->mExternal.mEmbed.mEmbd1Sz);
        }
    }

    if (0 != FlushImgBufs(apDev, apBufNfo))
    {
        return -1;
    }

    return true;
}

/**
 * @brief Releases all allocated buffers
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void FreeBuffers(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];

    for (uint32_t BufIdx = 0; BufIdx < BUF_COUNT_MAX_MEMCPY; BufIdx++)
    {
        BufInfo_t *Nfo = &Dev->mBufs[BufIdx];

        if (Nfo->mExternal.mEmbed.mpEmbd0 != NULL)
        {
            MShr_free(Dev->mHwId, (Mu_ChannelId_t)(Dev->mCamId + 1), Nfo->mExternal.mEmbed.mpEmbd0);
            Nfo->mExternal.mEmbed.mpEmbd0 = NULL;
        }
        if (Nfo->mExternal.mEmbed.mpEmbd1 != NULL)
        {
            MShr_free(Dev->mHwId, (Mu_ChannelId_t)(Dev->mCamId + 1), Nfo->mExternal.mEmbed.mpEmbd1);
            Nfo->mExternal.mEmbed.mpEmbd1 = NULL;
        }

        for (uint8_t PlaneIdx = 0; PlaneIdx < CCL_INBUF_PLANE_CNT; PlaneIdx++)
        {
            cca_ImgBuf_t *lpIoBuf = &Nfo->mExternal.mImg[PlaneIdx];

            if (lpIoBuf->mpData != NULL)
            {
                MShr_free(Dev->mHwId, (Mu_ChannelId_t)(Dev->mCamId + 1), lpIoBuf->mpData);
                lpIoBuf->mpData = NULL;
                lpIoBuf->mHwAddr = 0;
            }
        }
    }

    if (0 != ccl_sem_Destroy(&Dev->mFreeBufferCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy semaphore!\n");
    }
    if (0 != ccl_sem_Destroy(&Dev->mIspFreeCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy semaphore!\n");
    }
}

/**
 * @brief Allocates embedded buffers from OS memory (non-sharable memory)
 *
 * @param apDev - capture state structure
 * @param apBufNfo - target buffer to allocate
 * @return 0 on success, -1 otherwise.
 */
static int32_t AllocEmbed(CapState_t* apDev, BufInfo_t* apBufNfo)
{
    apBufNfo->mExternal.mEmbed.mEmbd0Sz = apDev->mCfg.mEmbd0Sz;
    if (apBufNfo->mExternal.mEmbed.mEmbd0Sz > 0)
    {
        apBufNfo->mExternal.mEmbed.mpEmbd0 = MShr_malloc(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), apBufNfo->mExternal.mEmbed.mEmbd0Sz, -1);
        if (apBufNfo->mExternal.mEmbed.mpEmbd0 == NULL)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to allocate In img embedded data buffer\n");
            return -1;
        }
    }
    else
    {
        apBufNfo->mExternal.mEmbed.mpEmbd0 = NULL;
    }

    apBufNfo->mExternal.mEmbed.mEmbd1Sz = apDev->mCfg.mEmbd1Sz;
    if (apBufNfo->mExternal.mEmbed.mEmbd1Sz > 0)
    {
        apBufNfo->mExternal.mEmbed.mpEmbd1 = MShr_malloc(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), apBufNfo->mExternal.mEmbed.mEmbd1Sz, -1);
        if (apBufNfo->mExternal.mEmbed.mpEmbd1 == NULL)
        {
            return -1;
        }
    }
    else
    {
        apBufNfo->mExternal.mEmbed.mpEmbd1 = NULL;
    }

    return 0;
}

/**
 * @brief Allocates image buffers from sharable memory
 *
 * @param apDev - capture state structure
 * @param apBufNfo - target buffer to allocate
 * @return 0 on success, -1 otherwise.
 */
static int32_t AllocPlanes(CapState_t* apDev, BufInfo_t* apBufNfo)
{
    for (uint8_t PlaneIdx = 0; PlaneIdx < apDev->mNoOfPlanes; PlaneIdx++)
    {
        cca_ImgBuf_t *lpIoBuf = &apBufNfo->mExternal.mImg[PlaneIdx];

        // allocate memory
        lpIoBuf->mpData = MShr_malloc(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), lpIoBuf->mSizeBytes, -1);
        if (NULL == lpIoBuf->mpData)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to allocate In img buffer\n");
            return -1;
        }

        // update physical location
        if (!MShr_GetGlobalMemHandle(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), lpIoBuf->mpData, &lpIoBuf->mHwAddr))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to get In img buffer PHY location\n");
            return -1;
        }
    }

    // make sure that the memory is not considered allocated
    for (uint8_t PlaneIdx = apDev->mNoOfPlanes; PlaneIdx < CCL_INBUF_PLANE_CNT; PlaneIdx++)
    {
        cca_ImgBuf_t *lpIoBuf = &apBufNfo->mExternal.mImg[PlaneIdx];
        lpIoBuf->mSizeBytes = 0;
        lpIoBuf->mpData = NULL;
    }

    return 0;
}

/**
 * @brief Allocates all capture buffers
 *
 * @return 0 on success, -1 otherwise.
 */
static int32_t AllocBuffers(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];

    uint32_t lBufCnt = gCclState.Streams[aHwId][aCamId].mCapCfg.mBufCnt;

    if ( lBufCnt > BUF_COUNT_MAX_MEMCPY)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCL code supports max of %d buffers on file capture. hwid: %d, camid: %d\n", BUF_COUNT_MAX_MEMCPY, Dev->mHwId, Dev->mCamId);
        return -1;
    }

    // allocate requested number of input buffers
    for (uint32_t BufIdx = 0; BufIdx < lBufCnt; BufIdx++)
    {
        BufInfo_t *Nfo = &Dev->mBufs[BufIdx];

        memset(Nfo, 0, sizeof(BufInfo_t));
        Nfo->mState = CCL_MEMCPY_BUF_STATE_IDLE;
        Nfo->mExternal.mBufId = BufIdx;
        ccl_ComputeInImgSizes(aHwId,aCamId, Nfo->mExternal.mImg);

        if (0 != AllocEmbed(Dev, Nfo))
        {
            return -1;
        }

        if (0 != AllocPlanes(Dev, Nfo))
        {
            return -1;
        }

        // prepare img data as well
        if (!LoadFrame(Dev, Nfo))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to preload frame %d for hwid: %d, camid: %d\n", BufIdx, aHwId, aCamId);
            return -1;
        }
    }

    // init variables
    if (0 != ccl_sem_Init(&Dev->mFreeBufferCnt, (int)lBufCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to init semaphore!\n");
        return -1;
    }
    if (0 != ccl_sem_Init(&Dev->mIspFreeCnt, MAX_FW_TASK_CNT_PER_STREAM))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to init semaphore!\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Get free buffer for new image data
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param DropReadyIfNone Acton if no free buffer is locally available. If TRUE attempt to force CCL to
 * drop old image a thus free old buffer. If FALSE wait until CCL gives-up old buffer on its own (potentially infinite wait)
 * @return Pointer to free buffer or NULL if none can be forced at this time.
 */
BufInfo_t *GetBufForCapture(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool DropReadyIfNone)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];

    // await free buffer
    if (DropReadyIfNone)
    {
        int NoFree;
        if (0 != ccl_sem_GetCnt(&Dev->mFreeBufferCnt, &NoFree))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to get free capture buffer amount!\n");
            return NULL;
        }
        if (NoFree <= 0)
        {
            // no local idle buffer available => we need to force drop of ready frame
            ccl_CapBuf_t *Buf = ccl_DropReady(aHwId,aCamId);
            if (Buf == NULL)
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Fail to recycle READY frame for file capture!\n");
                return NULL;
            }

            // NOTE: we do not update Dev->mFreeBufferCnt because we immediately reuse the buffer
            BufInfo_t *Nfo = &Dev->mBufs[Buf->mBufId];
            Nfo->mState = CCL_MEMCPY_BUF_STATE_ON_CAPTURE;
            return Nfo;
        }
        else
        {
            // local buffer is available, proceed normally
            // NOTE: there is a small window for race condition before sem_getvalue() above and
            // sem_wait() below. But there is only one thread that decreases semaphore counter (e.i. calls this function)
            // other threads only increase the counter
        }
    }

    // wait for available buffer
    if (0 != ccl_sem_Take(&Dev->mFreeBufferCnt,  CCL_SEM_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to wait for free capture buffer!\n");
        return NULL;
    }

    // now at least one buffer should be free
    for (uint32_t BufIdx = 0; BufIdx < gCclState.Streams[aHwId][aCamId].mCapCfg.mBufCnt; BufIdx++)
    {
        if (Dev->mBufs[BufIdx].mState == CCL_MEMCPY_BUF_STATE_IDLE)
        {
            Dev->mBufs[BufIdx].mState = CCL_MEMCPY_BUF_STATE_ON_CAPTURE;
            return &Dev->mBufs[BufIdx];
        }
    }

    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "No buffer in idle while at least one should be!\n");
    return NULL;
}

/**
 * @brief Thread that reads data from files and generates frame events
 *
 * @param apArg - pointer to stream associated with this thread (CapState_t *)
 * @return NULL
 */
static void* GenerationThread(void* apArg)
{
    CapState_t* Dev = apArg;
    BufInfo_t *NextBuf;

    double FrameInterval = 0;
    if (Dev->mCfg.mFps > 0)
    {
        // compute interval between frames from FPS
        FrameInterval = 1.0 / Dev->mCfg.mFps;
    }

    // Get first frame buffers
    BufInfo_t *Buf = GetBufForCapture(Dev->mHwId, Dev->mCamId, false);
    if (Buf == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Unable to get initial buffer for file capture! Thread exiting... hwid: %d, camid: %d\n", Dev->mHwId,  Dev->mCamId);
        return NULL;
    }

    // initialize frame counters
    Dev->mNextFrameTime = GetNow();
    // update current frame id
    Buf->mExternal.mFrameNo = Dev->mCurrentFrameId;

    // enter loop (single pass start->stop or infinite if Repeat active)
    while (Dev->mNoFramesToGenerate > 0 || Dev->mCfg.mRepeat)
    {
        // wait for the frame time
        double Now = GetNow();
        if (Dev->mCfg.mFps != 0.0)
        {
            // follow fix FPS
            while (Dev->mNextFrameTime > Now)
            {
                uint64_t wait = (Dev->mNextFrameTime - Now) * 1e6;
                //ILOG(ILOG_MOD_CCL, ILOG_LEV_NFO, "Gen thread waiting for %uus\n", wait);
                ccl_SleepUs(wait);
                Now = GetNow();
            }
            NextBuf = GetBufForCapture(Dev->mHwId, Dev->mCamId, true);
        }
        else
        {
            // wait for space on ISP
            if (0 != ccl_sem_Take(&Dev->mIspFreeCnt, CCL_SEM_WAIT_ININITE))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to wait for ISP space!\n");
                return NULL;
            }

            // wait for available buffer
            NextBuf = GetBufForCapture(Dev->mHwId, Dev->mCamId, false);
            if (NextBuf == NULL)
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Unable to get buffer for file capture! Thread exiting... hwid: %d, camid: %d\n", Dev->mHwId,  Dev->mCamId);
                return NULL;
            }
        }


        {
            if (NextBuf == NULL)
            {
                // there is no free buffer for next frame => reuse current for frame load (drop loaded data)

                // Drop current frame
                Buf->mState = CCL_MEMCPY_BUF_STATE_ON_CAPTURE;

                // signal frame drop on this stream to ccl
                ccl_CcaFrameDropped(Dev->mHwId, Dev->mCamId);
            }
            else
            {
                for (uint8_t PlaneIdx = 0; PlaneIdx < Dev->mNoOfPlanes; PlaneIdx++)
                {
                    Buf->mExternal.mImg[PlaneIdx].mTimeStamp = GetNow()*1e6;
                }
                // frame successfully loaded - signal new image
                ccl_NewInImg(Dev->mHwId, Dev->mCamId, &Buf->mExternal);

                Buf = NextBuf;
                NextBuf = NULL;
            }
        }

        // prep next frame data in advance
        Dev->mCurrentFrameId++;
        Dev->mNextFrameTime += FrameInterval;
        Buf->mExternal.mFrameNo   = Dev->mCurrentFrameId;
    }
    return NULL;
}

/**
 * @brief Starts image capturing for given channel
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];

    Dev->mNoFramesToGenerate = Dev->mCfg.mStopIndex - Dev->mCfg.mStartIdx + 1;
    char ThreadName[10];

    if (snprintf(ThreadName, 10, "gen-%d-%d", aHwId, aCamId) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }

    if (0 != ccl_thread_Create(&(Dev->mThreadId), GenerationThread, &gCapState[aHwId][aCamId], ThreadName, CCL_TASK_STACK_SIZE_NORMAL, CCL_THREAD_PRIORITY_NORMAL))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to start generation thread.\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Stops image capturing for given channel
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];
    if (0 != ccl_thread_Cancel(&Dev->mThreadId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to stop generation thread.\n");
    }

    for (uint32_t BufIdx = 0; BufIdx <  gCclState.Streams[aHwId][aCamId].mCapCfg.mBufCnt; BufIdx++)
    {
        if (Dev->mBufs[BufIdx].mState != CCL_MEMCPY_BUF_STATE_IDLE)
        {
            Dev->mBufs[BufIdx].mState = CCL_MEMCPY_BUF_STATE_IDLE;

            if (0 != ccl_sem_Give(&Dev->mFreeBufferCnt))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post semaphore. hwid: %d, camid: %d\n", aHwId, aCamId);
            }
        }
    }
    return 0;
}

/**
 * @brief Initialization of file capture for given stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apCfg - capture parameters
 * @param apCamInitParams - sensor specific parameters
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapParams_t* apCfg, cca_CamInit_t *apCamInitParams)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];

    // store channel setting
    Dev->mHwId  = aHwId;
    Dev->mCamId = aCamId;
    memcpy(&Dev->mCfg, &apCfg->mMemcpy, sizeof(ccl_CapCfgMemcpy_t));

    // initialize generated cfg
    Dev->mCurrentFrameId       = Dev->mCfg.mStartIdx;
    // TODO nxf90122 for now we have only one plane
    // if ()
    // {
    //     Dev->mNoOfPlanes = 2;
    // }
    // else
    {
        Dev->mNoOfPlanes = 1;
    }

    // Normally there would be a call to sensor api to initialize and construct/read sensor parameters (calibration, etc.).
    // In file based we do not have this possibility, se we skip init() and just put in some sensor params our selfs
    switch (apCamInitParams->mSensorId)
    {
        case SENSOR_OV_X3C:
            {
                cca_ovx3c_Params_t *params = apCamInitParams->mpParams;
                if (params == NULL)
                {
                    // this memory must at this point be allocated
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Sensor parameters are NULL.\n");
                    return -1;
                }
                // for ovx3c if one param is zero, sensor specific code uses default value
                params->mOtpSensHCG = 0;
                params->mOtpSensLCG = 0;
            }
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Sensor type is unknown to file based, virtual(simulated) read of sensor parameters will not happen.\n");
            break;
    }

    if (0 != AllocBuffers(aHwId, aCamId))
    {
        FreeBuffers(aHwId, aCamId);
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Buffer allocation failed. hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }

    return 0;
}

/**
 * @brief Clean up for given channel
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    //CapState_t* lpDev = &gCapState[aHwId][aCamId];

    (void)Stop(aHwId, aCamId);

    FreeBuffers(aHwId, aCamId);
}

/**
 * @brief Update of sensor configuration - ignored for file based
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param AsyncOk - if false can only return after sensor was reconfigured
 * @param apSensCfg - new sensor configuration
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool AsyncOk, cca_SensPrm_t *apSensCfg, cca_CamInit_t *apCamInitParams, uint32_t aBasedOnId)
{
    (void)aHwId;
    (void)aCamId;
    (void)AsyncOk;
    (void)apCamInitParams;
    (void)aBasedOnId;

    // data already captured, give chance to select next file index to load based on sensor settings
    //CapState_t *lpDevState = &gCapState[aHwId][aCamId];

    return 0;
}

/**
 * @brief Indication that capture buffer is no longer used.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apCapBuf - freed buffer
 */
static void CapBufFree(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t *apCapBuf)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];

    if (apCapBuf == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CapBufFree() -> apCapBuf == NULL. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    if (apCapBuf->mBufId >= BUF_COUNT_MAX_MEMCPY)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CapBufFree() -> buffer index out of range. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    BufInfo_t *Buf = &Dev->mBufs[apCapBuf->mBufId];

    if (Buf->mState == CCL_MEMCPY_BUF_STATE_IDLE)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CapBufFree() -> buffer already IDLE!. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    Buf->mState = CCL_MEMCPY_BUF_STATE_IDLE;

    if (0 != ccl_sem_Give(&Dev->mFreeBufferCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post semaphore. hwid: %d, camid: %d\n", aHwId, aCamId);
    }
}

static void FrameDoneEvt(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    CapState_t *Dev = &gCapState[aHwId][aCamId];
    if (0 != ccl_sem_Give(&Dev->mIspFreeCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post semaphore. hwid: %d, camid: %d\n", aHwId, aCamId);
    }
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

int32_t ccl_CapOps_Memcpy(ccl_CapOps_t* apOps)
{
    if (apOps == NULL)
    {
        return -1;
    }

    apOps->mpCapOpen   = Open;
    apOps->mpCapClose   = Close;
    apOps->mpCapStart  = Start;
    apOps->mpCapStop   = Stop;
    apOps->mpCapSensorUpdate = Update;
    apOps->mpCapBufReturn = CapBufFree;
    apOps->StreamingModeSupported = false;
    apOps->mpFrameDoneEvt = FrameDoneEvt;

    return 0;
}

/***********************************************************************************/

// // For af tests
// void cca_register_af_getter(cca_af_index_getter_t getter)
// {
//     registered_getter = getter;
// }

/***********************************************************************************/
