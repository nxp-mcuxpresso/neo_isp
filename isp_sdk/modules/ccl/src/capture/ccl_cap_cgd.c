/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ccl_cap_cgd.c
 * @brief Implements raw image capture system by reading data from CGD driver
 * @details Internal module of ccl. Each streams gets its own thread that reads
 * data from CGD and signals new frame event to ccl.
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>

#include "ilog.h"
#include "ccl_cap_cgd.h"
#include "ccl.h"
#include "ccl_cca.h" // for frame dropped signal

#include "utils_api.h"
#include "mshr.h"

#if defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))
// CGD
#include "cgd.h"
#include "cgd_types.h"
#include "cgd_config.h"

#if defined(SDK_OS_FREE_RTOS)
#include "utils_edma.h"
#include "fsl_debug_console.h"
#endif

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

#define MAX_FRAME_WAIT_MS 1000U
#define CGD_INITIAL_BUFFER_COUNT 3 // BUF1, BUF2 and Free positions

/** @brief Enum about buffer states */
typedef enum
{
    CCL_CGD_BUF_STATE_IDLE, /** @brief buffer free */
    CCL_CGD_BUF_STATE_CGD, /** @brief buffer assigned to ISI channel */
    CCL_CGD_BUF_STATE_CCL, /** @brief buffer returned from ISI with frame data */
    CCL_CGD_BUF_STATE_CNT
} ccl_cgd_BufState_t;

/** @brief structure holding information about single plane memory allocation */
typedef struct
{
    /** @brief Absolute location of the memory block in RAM */
    uint64_t mPhyLocation;
    /** @brief Mapped work pointer to plane's memory. All work ptrs are pointing into this region. */
    void* mMmapPtr;
    /** @brief Size of plane memory. All work ptrs are pointing into this region. */
    size_t mMmapSize;
} PlaneInfo_t;

/** @brief structure holding information about single capture buffer (contains multiple planes) */
typedef struct
{
    /** @brief Capture parameters for this stream as supplied during init()*/
    ccl_CapBuf_t mExternal;

    /** @brief Extended information about memory allocations */
    PlaneInfo_t PlaneInfo[CCL_INBUF_PLANE_CNT];

    /** @brief Indication where is the buffer currently assigned. */
    ccl_cgd_BufState_t mState;
} BufInfo_t;

typedef struct
{
    /** @brief Frame Id since the start of capture */
    uint32_t mCurrentFrameId;
    /** @brief Semaphore signaling right moment to start sensor configuration update */
    ccl_sem_t mSensUpdateTrigger;
    /** @brief lock protecting sensor update variables */
    ccl_mutex_t mSensCfgLock;
    /** @brief Information for sensor update thread if there are new values to send to sensor */
    bool mSensorUpdateWaiting;
    /** @brief Id of frame this cfg is based on */
    int32_t mBasedOnId;
    /** @brief New configuration to be send to sensor */
    cca_SensPrm_t mNewSensCfg;
    /** @brief Id of thread that is reading frames from V4L during capture */
    ccl_thread_t mCaptureThreadId;
    /** @brief Id of thread that is updating sensor during control */
    ccl_thread_t mSensorThreadId;
    /** @brief Frame done event counter for single stream */
    ccl_sem_t mFrameDoneTrigger;
} RuntimeVars_t;

/** @brief State variables for single stream */
typedef struct
{
    /** @brief Hw ID associated with this stream*/
    ispdrv_HwId_t mHwId;

    /** @brief Cam ID associated with this stream*/
    ispdrv_CamPipeId_t mCamId;

    /** @brief Counter of frames in CCL pipe */
    ccl_sem_t mFreeBufferCnt;

    /** @brief Capture parameters for this stream as supplied during init()*/
    ccl_CapCfgCgd_t mCfg;

    /** @brief Number of planes in input image
     * @details In CGD unused for now, set to 1 */
    uint32_t mNoOfPlanes;

    /** @brief Local list of allocated buffers
     * @details This info is needed because:
     * - each ccl capture subsystem allocates its buffers
     * - we need to have buffer id (index into this array) to use as a reference when interacting with CCL */
    BufInfo_t mBufs[BUF_COUNT_MAX_CGD];

    /** @brief Number of buffers really used in mBufs array, Enforced max is BUF_COUNT_MAX_CGD*/
    uint32_t mBufCnt;

    RuntimeVars_t mRuntime;
} CapState_t;

/** @brief structure helping with race conditions for CGD system */
typedef struct
{
    /** @brief Actual CGD state, used for starting
     * @details cgd_Open method is called only once, for whole system, not for each stream
     */
    cgd_State_t mState;

    /** @brief System configuration for CGD */
    cgd_SystemExtCfg_t mSystemConfig;
} CgdCclState_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/** @brief state variables for all possible streams */
DDR_NOINIT static CapState_t gCapState[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];
/** @brief state variable for whole CGD system - to prevent thread race */
static CgdCclState_t gCgdState =
    {   .mState = CGD_STATE_OFF};

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Get free buffer for new image data, MUST be called only from the owning stream's CaptureThread
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param DropReadyIfNone Action if no free buffer is locally available. If TRUE attempt to force CCL to
 * drop old image a thus free old buffer. If FALSE wait until CCL gives-up old buffer on its own (potentially infinite wait)
 * @return Pointer to free buffer or NULL if none can be forced at this time.
 */
static BufInfo_t* CgdGetBufForCapture(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool DropReadyIfNone);

/******************************************************************************/
/**
 * @brief Handle fatal errors from CGD, registered to gpCgdErrorEvtHandler
 */
static void CgdFatalErrIrqHandle(cgd_RegisterErrorTable_t* lastError);

/******************************************************************************/
/**
 * @brief Prepare configuration for the CGD, all sensor at once!
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/******************************************************************************/
/**
 * @brief        picture stored event handler
 *
 * @param        aIsiChn - channel ID
 *
 * @return       none
 */
static void CgdFrameDoneHandler(cgd_IsiChannel_t aIsiChn);

/******************************************************************************/
/**
 * @brief        first line stored event handler
 *
 * @param        aIsiChn - channel ID
 *
 * @return       none
 */
static void CgdFirstLineHandler(cgd_IsiChannel_t aIsiChn);

/******************************************************************************/
/**
 * @brief Clean up for given channel
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/
/**
 * @brief Return buffer to the free buffer pool
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aBuf - Buffer to return to the pool
 *
 * @return none
 */
static inline void ReturnBuffer(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, BufInfo_t* aBuf)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    if (aBuf != NULL && aBuf->mState != CCL_CGD_BUF_STATE_IDLE)
    {
        CapState_t* Dev = &gCapState[aHwId][aCamId];

        aBuf->mState                   = CCL_CGD_BUF_STATE_IDLE;
        aBuf->mExternal.mFrameNo       = 0;
        aBuf->mExternal.mCaptureTimeNs = 0;

        if (CCL_ERR_SUCCESS != ccl_sem_Give(&Dev->mFreeBufferCnt))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post mFreeBufferCnt semaphore. hwid: %d, camid: %d\n", aHwId, aCamId);
        }
    }
}

/******************************************************************************/
/**
 * @brief Flush in img data buffers for given task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Task to prepare
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t FlushImgBufs(CapState_t* apDev, BufInfo_t* apBufNfo)
{
    if (apBufNfo == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "apBufNfo is NULL!\n");
        return -1;
    }

    for (uint32_t PlaneIdx = 0; PlaneIdx < CCL_INBUF_PLANE_CNT; PlaneIdx++)
    {
        if (apBufNfo->PlaneInfo[PlaneIdx].mMmapPtr != NULL)
        {
            if (0 != MShr_Transfer2remote(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), MSHR_DIR_FROM_PC, apBufNfo->PlaneInfo[PlaneIdx].mMmapPtr, NULL))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to flush In img buffer %d\n", PlaneIdx);
                return -1;
            }
        }
    }

    return 0;
}

/******************************************************************************/
/**
 * @brief Releases all allocated buffers
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void FreeBuffers(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    for (uint32_t BufIdx = 0; BufIdx < Dev->mBufCnt; BufIdx++)
    {
        BufInfo_t* Nfo = &Dev->mBufs[BufIdx];

        for (uint8_t PlaneIdx = 0; PlaneIdx < CCL_INBUF_PLANE_CNT; PlaneIdx++)
        {
            cca_ImgBuf_t* lpIoBuf = &Nfo->mExternal.mImg[PlaneIdx];

            MShr_free(Dev->mHwId, (Mu_ChannelId_t)(Dev->mCamId + 1), Nfo->PlaneInfo[PlaneIdx].mMmapPtr);

            Nfo->mExternal.mEmbed.mpEmbd0 = NULL;
            lpIoBuf->mpData               = NULL;
            lpIoBuf->mHwAddr              = 0;
        }
    }

    // init variables
    if (CCL_ERR_SUCCESS != ccl_sem_Destroy(&Dev->mFreeBufferCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy semaphore!\n");
    }
}

/******************************************************************************/
/**
 * @brief Updates Buffer data pointers so they will point into correctly into freshly allocated(mapped) region
 *
 * @param apDev - Capture state variable (needed for configuration)
 * @param apBufNfo - target buffer
 * @param aPlaneId - plane within the buffer to initialize
 * @return 0 on success or negative error otherwise.
 */
static int InitPlaneBuf(CapState_t* apDev, BufInfo_t* apBufNfo)
{
    if (apDev == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "apDev NULL\n");
        return -1;
    }
    if (apBufNfo == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "buffer NULL\n");
        return -1;
    }

    for (uint8_t planeIdx = 0; planeIdx < apDev->mNoOfPlanes; planeIdx++)
    {
        cca_ImgBuf_t* ImgBuf = &apBufNfo->mExternal.mImg[planeIdx];
        cca_EmbdData_t* Emb  = &apBufNfo->mExternal.mEmbed;
        uint64_t PhyBase     = apBufNfo->PlaneInfo[planeIdx].mPhyLocation;
        void* Base           = apBufNfo->PlaneInfo[planeIdx].mMmapPtr;

        // Sensor will capture embedded data into the same buffer as picture data => we need to setup ptrs appropriately
        uint32_t EmbdSz0 = apDev->mCfg.mOutPictureWidth * ccl_GetBitsPP2BytesPP(ImgBuf->mBpp) * apDev->mCfg.mEmbdLinesBefore;
        uint32_t ImgSz   = apDev->mCfg.mOutPictureWidth * ccl_GetBitsPP2BytesPP(ImgBuf->mBpp) * apDev->mCfg.mOutPictureHeight;
        uint32_t EmbdSz1 = apDev->mCfg.mOutPictureWidth * ccl_GetBitsPP2BytesPP(ImgBuf->mBpp) * apDev->mCfg.mEmbdLinesAfter;

        if (planeIdx == 0) // embedded data only in first plane
        {
            Emb->mpEmbd0       = Base;
            Emb->mEmbd0Sz      = EmbdSz0;
            ImgBuf->mpData     = (uint32_t*)(((uint8_t*)Base) + EmbdSz0);
            ImgBuf->mSizeBytes = ImgSz;
            ImgBuf->mHwAddr    = PhyBase + EmbdSz0;
            Emb->mpEmbd1       = ((uint8_t*)Base) + EmbdSz0 + ImgSz;
            Emb->mEmbd1Sz      = EmbdSz1;
        }
        else
        {
            ImgBuf->mpData     = (uint32_t*)(((uint8_t*)Base));
            ImgBuf->mSizeBytes = ImgSz;
            ImgBuf->mHwAddr    = PhyBase;
        }
    }

    return 0;
}

/******************************************************************************/
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
        cca_ImgBuf_t* lpIoBuf = &apBufNfo->mExternal.mImg[PlaneIdx];

        size_t size = (size_t)apDev->mCfg.mOutPictureWidth * ccl_GetBitsPP2BytesPP(lpIoBuf->mBpp) * (apDev->mCfg.mEmbdLinesBefore + apDev->mCfg.mOutPictureHeight + apDev->mCfg.mEmbdLinesAfter);

        // allocate memory with embedded data
        apBufNfo->PlaneInfo[PlaneIdx].mMmapSize = size;
        apBufNfo->PlaneInfo[PlaneIdx].mMmapPtr = MShr_malloc(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), size, -1);
        if (NULL == apBufNfo->PlaneInfo[PlaneIdx].mMmapPtr)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to allocate In img buffer\n");
            return -1;
        }

        // update physical location
        if (!MShr_GetGlobalMemHandle(apDev->mHwId, (Mu_ChannelId_t)(apDev->mCamId + 1), apBufNfo->PlaneInfo[PlaneIdx].mMmapPtr, &apBufNfo->PlaneInfo[PlaneIdx].mPhyLocation))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to get In img buffer PHY location\n");
            return -1;
        }
    }

    // make sure that the memory is not considered allocated
    for (uint8_t PlaneIdx = apDev->mNoOfPlanes; PlaneIdx < CCL_INBUF_PLANE_CNT; PlaneIdx++)
    {
        cca_ImgBuf_t* lpIoBuf                  = &apBufNfo->mExternal.mImg[PlaneIdx];
        lpIoBuf->mSizeBytes                    = 0;
        apBufNfo->PlaneInfo[PlaneIdx].mMmapPtr = NULL;
    }

    return 0;
}

/******************************************************************************/
/**
 * @brief Allocates all capture buffers
 *
 * @return 0 on success, -1 otherwise.
 */
static int32_t AllocBuffers(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    if (gCclState.Streams[aHwId][aCamId].mCapCfg.mBufCnt > BUF_COUNT_MAX_CGD)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCL code supports max of %d buffers. hwid: %d, camid: %d\n", BUF_COUNT_MAX_CGD, Dev->mHwId, Dev->mCamId);
        return -1;
    }

    if (CCL_ERR_SUCCESS != ccl_sem_Init(&Dev->mFreeBufferCnt, 0))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to init semaphore!\n");
        return -1;
    }

    // allocate requested number of input buffers
    for (uint32_t BufIdx = 0; BufIdx < gCclState.Streams[aHwId][aCamId].mCapCfg.mBufCnt; BufIdx++)
    {
        BufInfo_t* Nfo = &Dev->mBufs[BufIdx];

        ccl_ComputeInImgSizes(aHwId, aCamId, Nfo->mExternal.mImg);

        if (0 != AllocPlanes(Dev, Nfo)) // alloc plane memory
        {
            return -1;
        }

        if (0 != InitPlaneBuf(Dev, Nfo)) // setup correct pointers
        {
            return -1;
        }

        Nfo->mState           = CCL_CGD_BUF_STATE_IDLE;
        Nfo->mExternal.mBufId = BufIdx;
        Dev->mBufCnt++;

        if (CCL_ERR_SUCCESS != ccl_sem_Give(&Dev->mFreeBufferCnt))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post mFreeBufferCnt semaphore. hwid: %d, camid: %d\n", aHwId, aCamId);
        }
    }

    return 0;
}

/******************************************************************************/
/**
 * @brief Assign free buffers to CGD for capture, method check if CGD has capacity and if we can get a free buffer
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aAmount - Number of buffers to assign
 * @return 0 on success, -1 otherwise.
 */
static int32_t AssignFreeBuffersToCGD(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint8_t aAmount, bool DropReadyIfNone)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    utils_ImgBuf_t lBuff = {0};

    for (int i = 0; i < aAmount; i++) // send buffers, to fill BUF1, BUF2 and Free positions in ISI driver
    {
        if (cgd_IsEmptyBufferPrepared((cgd_IsiChannel_t)aCamId)) // CGD capacity is full, don't submit more buffers
        {
            break;
        }

        BufInfo_t* Buf = CgdGetBufForCapture(aHwId, aCamId, DropReadyIfNone);

        if (NULL == Buf)
        {
            break;
        }

        lBuff.mHwAddr = Buf->PlaneInfo[0].mPhyLocation;
        lBuff.mpData  = Buf->PlaneInfo[0].mMmapPtr;
        lBuff.mIdx    = Buf->mExternal.mBufId;

        if (cgd_SubmitEmptyBuffer((cgd_IsiChannel_t)aCamId, &lBuff) == CGD_LIB_FAILURE)
        {
            ReturnBuffer(aHwId, aCamId, Buf);
            break;
        }
    }

    return 0;
} // AssignFreeBuffersToCGD

/******************************************************************************/
static BufInfo_t* CgdGetBufForCapture(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool DropReadyIfNone)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    // try to get available buffer
    if (CCL_ERR_SUCCESS != ccl_sem_Take(&Dev->mFreeBufferCnt, 0))
    {
        if (DropReadyIfNone)
        {
            // no local idle buffer available => we need to force drop of ready frame
            ccl_CapBuf_t* Buf = ccl_DropReady(aHwId, aCamId);
            if (Buf == NULL)
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Unable to recycle a READY frame for file capture!\n");
                return NULL;
            }

            // NOTE: we do not update Dev->mFreeBufferCnt because we immediately reuse the buffer
            BufInfo_t* Nfo          = &Dev->mBufs[Buf->mBufId];
            Nfo->mState             = CCL_CGD_BUF_STATE_CGD;
            Nfo->mExternal.mFrameNo = 0;
            return Nfo;
        }
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Unable to get a free capture buffer!\n");
        return NULL;
    }

    // now at least one buffer should be free
    for (uint32_t BufIdx = 0; BufIdx < Dev->mBufCnt; BufIdx++)
    {
        if (Dev->mBufs[BufIdx].mState == CCL_CGD_BUF_STATE_IDLE)
        {
            Dev->mBufs[BufIdx].mState = CCL_CGD_BUF_STATE_CGD;
            Dev->mBufs[BufIdx].mExternal.mFrameNo = 0;
            return &Dev->mBufs[BufIdx];
        }
    }

    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "No buffer in idle while at least one should be!\n");
    return NULL;
} // CgdGetBufForCapture

/******************************************************************************/
/**
 * @brief        picture stored event handler
 *
 * @details      example of event handler
 *
 * @param        aIsiChn - channel ID
 *
 * @return       none
 *******************************************************************************/
static void CgdFrameDoneHandler(cgd_IsiChannel_t aIsiChn)
{
    cgd_IsiExtCfg_t* isiCfg = cgd_GetIsiDeviceConfig(aIsiChn);

    if (isiCfg == NULL)
    {
        return;
    }

    CapState_t* Dev = &gCapState[isiCfg->mHwId][(ispdrv_CamPipeId_t)aIsiChn];

    /* send signal to CaptureThread */
    if (CCL_ERR_SUCCESS != ccl_sem_GiveFromISR(&Dev->mRuntime.mFrameDoneTrigger))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "EVT: failed to post sem., buff. id. %d.\n", aIsiChn);
    }

} /* CgdFrameDoneHandler() */

/******************************************************************************/
static void CgdFirstLineHandler(cgd_IsiChannel_t aIsiChn)
{
    cgd_IsiExtCfg_t* isiCfg = cgd_GetIsiDeviceConfig(aIsiChn);

    if (isiCfg == NULL)
    {
        return;
    }

    CapState_t* lpDev = &gCapState[isiCfg->mHwId][(ispdrv_CamPipeId_t)aIsiChn];

    /* send signal to SensorThread */
    if (CCL_ERR_SUCCESS != ccl_sem_GiveFromISR(&lpDev->mRuntime.mSensUpdateTrigger))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post sensor update event\n");
    }

} /* CgdFirstLineHandler() */

/******************************************************************************/
/**
 * @brief Thread that wait for frame done event from CGD
 *
 * @param apArg - pointer to stream associated with this thread (CapState_t *)
 * @return NULL
 */
static void* CaptureThread(void* apArg)
{
    CapState_t* lpDev        = apArg;
    cgd_IsiChannel_t lIsiIdx = (cgd_IsiChannel_t)lpDev->mCamId;

    if (0 != AssignFreeBuffersToCGD(lpDev->mHwId, lpDev->mCamId, CGD_INITIAL_BUFFER_COUNT, false)) // send first buffers to CGD
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to assign first CGD buffers.\n");
        return NULL;
    }

    if (CGD_LIB_FAILURE == cgd_StreamOn(lIsiIdx)) // after that start streaming to assigned buffers
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed to cgd_StreamOn call.\n");
        return NULL;
    }

    if (lpDev->mCfg.mSensorStart != NULL)
    {
        // initialize the actual device - this code is camera specific
        if (0 != lpDev->mCfg.mSensorStart(lpDev->mHwId, lpDev->mCamId))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "User init of mSensorStart interface failed. hwid: %d, camid: %d\n", lpDev->mHwId, lpDev->mCamId);
        }
    }

    ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CaptureThread runing for CamId: %d\n", lpDev->mCamId);

    /* wait for image done */
    while (true)
    {
        if (CCL_ERR_SUCCESS != ccl_thread_StopAllow())
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation state. \n");
        }

        if (cgd_GetIsiState(lIsiIdx) != CGD_STATE_STRM)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ISI channel isn't streaming. hwid: %d, camid: %d\n", lpDev->mHwId, lpDev->mCamId);
            break;
        }

        /* Try to get a full buffer for CGD and process it
         * Not necessary here, because another way is later in CapBufFree() method
         */
        AssignFreeBuffersToCGD(lpDev->mHwId, lpDev->mCamId, 1, true);

        if (CCL_ERR_SUCCESS != ccl_sem_Take(&lpDev->mRuntime.mFrameDoneTrigger, MAX_FRAME_WAIT_MS)) // Waiting for Frame Done event
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to await frame done trigger - exiting sensor update thread %d.\n", lpDev->mCamId);
            break;
        }

        uint64_t CaptureTime = ccl_GetTime() * 1000U;

        if (cgd_GetIsiState(lIsiIdx) != CGD_STATE_STRM)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ISI channel isn't streaming. hwid: %d, camid: %d\n", lpDev->mHwId, lpDev->mCamId);
            break;
        }

        if (CCL_ERR_SUCCESS != ccl_thread_StopBlocked())
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation state. \n");
        }

        lpDev->mRuntime.mCurrentFrameId++;

        utils_ImgBuf_t fullBuf;

        if (cgd_GetFullBuffer(lIsiIdx, &fullBuf) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Get full buffer returned NULL! Frame %d dropped! hwid: %d, camid: %d\n", lpDev->mRuntime.mCurrentFrameId, lpDev->mHwId, lpDev->mCamId);
            continue;
        }

        if (fullBuf.mIdx >= lpDev->mBufCnt)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Get full buffer IDX invalid! Frame %d dropped! hwid: %d, camid: %d\n", lpDev->mRuntime.mCurrentFrameId, lpDev->mHwId, lpDev->mCamId);
            continue;
        }

        BufInfo_t* Buf = &lpDev->mBufs[fullBuf.mIdx];

        for (uint8_t PlaneIdx = 0; PlaneIdx < lpDev->mNoOfPlanes; PlaneIdx++)
        {
            Buf->mExternal.mImg[PlaneIdx].mTimeStamp = CaptureTime;
        }
        Buf->mExternal.mCaptureTimeNs = CaptureTime;

        if (0 != FlushImgBufs(lpDev, Buf))
        {
            break;
        }

        Buf->mState             = CCL_CGD_BUF_STATE_CCL;
        Buf->mExternal.mFrameNo = lpDev->mRuntime.mCurrentFrameId;

        if (cgd_GetIsiState(lIsiIdx) == CGD_STATE_STRM)
        {
            ccl_NewInImg(lpDev->mHwId, lpDev->mCamId, &Buf->mExternal);
        }
    }

    ILOG(ILOG_MOD_CCL, ILOG_LEV_DBG, "CaptureThread exited, hwid: %d, camid: %d\n", lpDev->mHwId, lpDev->mCamId);
    return NULL;
} // CaptureThread

/******************************************************************************/
/**
 * @brief Sensor update thread
 *
 * @param apArg - pointer to local capture device
 * @return NULL
 */
static void* SensorThread(void* apArg)
{
    // set thread termination style to "anytime" instead of "just in cancellation point"
    if (CCL_ERR_SUCCESS != ccl_thread_ManualStopControl())
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set thread cancellation type for SensorThread.\n");
    }

    CapState_t* Dev = apArg;

    while (true)
    {
        if (CCL_ERR_SUCCESS != ccl_thread_StopAllow())
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation state. \n");
        }

        // await window of opportunity for update
        if (CCL_ERR_SUCCESS != ccl_sem_Take(&Dev->mRuntime.mSensUpdateTrigger, CCL_SEM_WAIT_ININITE))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to await update window - exiting sensor update thread.\n");
            return NULL;
        }

        if (CCL_ERR_SUCCESS != ccl_thread_StopBlocked())
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to set cancellation state. \n");
        }

        if (CCL_ERR_SUCCESS != ccl_mutex_Lock(&Dev->mRuntime.mSensCfgLock, CCL_MUTEX_WAIT_ININITE))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to lock sens update mutex. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
            return NULL;
        }

        // check if there is update waiting
        if (!Dev->mRuntime.mSensorUpdateWaiting)
        {
            // nothing to do
        }
        else
        {
            Dev->mRuntime.mSensorUpdateWaiting = false;
            LogSensCfg(Dev->mHwId, Dev->mCamId, "SENDING", SENS_LOG_FRAME_ID_NOT_APPLICABLE, Dev->mRuntime.mBasedOnId, &Dev->mRuntime.mNewSensCfg);
            // Do the update
            if (0 != Dev->mCfg.mSensorUpdate(Dev->mHwId, Dev->mCamId, &Dev->mRuntime.mNewSensCfg))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Failed to update sensor to latest configuration. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
            }
            else
            {
                // log
                LogSensCfg(Dev->mHwId, Dev->mCamId, "SENT", SENS_LOG_FRAME_ID_NOT_APPLICABLE, Dev->mRuntime.mBasedOnId, &Dev->mRuntime.mNewSensCfg);
            }
        }

        if (CCL_ERR_SUCCESS != ccl_mutex_Unlock(&Dev->mRuntime.mSensCfgLock))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to unlock sens update mutex. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
            return NULL;
        }
    }

    /*NOTREACHED*/
    return NULL;
} // SensorThread

/******************************************************************************/
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
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* lDev = &gCapState[aHwId][aCamId];

    /* create semaphore variables for event handling - here together with other stuff related to event handling */
    if (CCL_ERR_SUCCESS != ccl_sem_Init(&lDev->mRuntime.mFrameDoneTrigger, 0))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "APP: failed to init sem.\n");
    }

    char lThreadName[14];
    if (snprintf(lThreadName, sizeof(lThreadName), "cap_cgd-%02d-%02d", aHwId, aCamId) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }

    if (CCL_ERR_SUCCESS != ccl_thread_Create(&(lDev->mRuntime.mCaptureThreadId), CaptureThread, &gCapState[aHwId][aCamId], lThreadName, CCL_TASK_STACK_SIZE_NORMAL, CCL_THREAD_PRIORITY_NORMAL))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to start capture thread. %s\n", strerror(errno));
        return -1;
    }

    if (snprintf(lThreadName, sizeof(lThreadName), "sen_cgd-%02d-%02d", aHwId, aCamId) < 0)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
        return -1;
    }

    if (CCL_ERR_SUCCESS != ccl_thread_Create(&(lDev->mRuntime.mSensorThreadId), SensorThread, &gCapState[aHwId][aCamId], lThreadName, CCL_TASK_STACK_SIZE_LARGE, CCL_THREAD_PRIORITY_HIGH))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to start sensor thread. %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

/******************************************************************************/
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
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    if (CGD_LIB_FAILURE == cgd_StreamOff((cgd_IsiChannel_t)aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed to cgd_StreamOff call.\n");
    }

    if (CCL_ERR_SUCCESS != ccl_thread_Cancel(&Dev->mRuntime.mCaptureThreadId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to stop capture thread.\n");
    }
    if (CCL_ERR_SUCCESS != ccl_thread_Cancel(&Dev->mRuntime.mSensorThreadId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to stop sensor thread.\n");
    }

    if (Dev->mCfg.mSensorStop != NULL)
    {
        // initialize the actual device - this code is camera specific
        if (0 != Dev->mCfg.mSensorStop(Dev->mHwId, Dev->mCamId))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "User init of mSensorStop interface failed. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
        }
    }

    if (CCL_ERR_SUCCESS != ccl_sem_Destroy(&Dev->mRuntime.mFrameDoneTrigger))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "APP: failed to destroy sem.\n");
    }

    for (uint32_t BufIdx = 0; BufIdx < Dev->mBufCnt; BufIdx++)
    {
        if (Dev->mBufs[BufIdx].mState != CCL_CGD_BUF_STATE_IDLE)
        {
            ReturnBuffer(aHwId, aCamId, &Dev->mBufs[BufIdx]);
        }
    }

    return 0;
}

/******************************************************************************/
/**
 * @brief Initialization of file capture for given stream, isn't thread-safe we need to ensure serialization of calls
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apCfg - capture parameters
 * @param apCamInitParams - sensor specific parameters
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapParams_t* apCfg, cca_CamInit_t* apCamInitParams)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    if (apCamInitParams->mpParams == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Camera init parameters are NULL.\n");
        return -1;
    }

    // store channel setting
    Dev->mHwId  = aHwId;
    Dev->mCamId = aCamId;

#if defined(SDK_OS_FREE_RTOS)
    utils_EdmaTransfer(&Dev->mCfg, &apCfg->mCgd, sizeof(ccl_CapCfgCgd_t));
#else
    memcpy(&Dev->mCfg, &apCfg->mCgd, sizeof(ccl_CapCfgCgd_t));
#endif

    if (CCL_ERR_SUCCESS != ccl_sem_Init(&Dev->mRuntime.mSensUpdateTrigger, 0))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to init semaphore %s\n", strerror(errno));
        return -1;
    }

    if (CCL_ERR_SUCCESS != ccl_mutex_Create(&Dev->mRuntime.mSensCfgLock, false))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to init mutex %s\n", strerror(errno));
        return -1;
    }

    // no of planes in cgd
    Dev->mNoOfPlanes = 1;
    bool lbRunCgdInit = false;

    if (gCgdState.mState == CGD_STATE_OFF)
    {
        lbRunCgdInit            = true;
        gCgdState.mState = CGD_STATE_INIT;
        gCgdState.mSystemConfig = sgCgdExtCfg; // copy the default system config
    }

    if (lbRunCgdInit)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CCL starting CGD initialization.\n");
        /* Open CGD system with provided configuration */
        if (CGD_LIB_FAILURE == cgd_Open(&sgCgdSystem))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed open!\n");
            Close(aHwId, aCamId);
            return -1;
        }

        cgd_RegisterFrameDoneEvtHandler(CgdFrameDoneHandler); // register own event handler to cgd
        cgd_RegisterFirstLineStoredEvtHandler(CgdFirstLineHandler); // register own event handler to cgd
        cgd_RegisterErrorEvtHandler(CgdFatalErrIrqHandle);
    }

    // Call to sensor api to initialize and construct/read sensor parameters for all channels (calibration, etc.).
    if (0 != ConfigureCgd(aHwId, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD sensor configuration failed. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
        Close(aHwId, aCamId);
        return -1;
    }

    // initialize the actual device - this code is camera specific
    if (0 != Dev->mCfg.mSensorInit(aHwId, aCamId, apCamInitParams->mpParams, apCamInitParams->mParamSize))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "User init of mSensorInit interface failed. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
        Close(aHwId, aCamId);
        return -1;
    }

    if (0 != AllocBuffers(aHwId, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Buffer allocation failed. hwid: %d, camid: %d\n", aHwId, aCamId);
        Close(aHwId, aCamId);
        return -1;
    }

    return 0;
}

/******************************************************************************/
static void Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    if (gCgdState.mState != CGD_STATE_OFF)
    {
        gCgdState.mState = CGD_STATE_OFF;

        if (CGD_LIB_FAILURE == cgd_Close()) // CGD close only whole system at once
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed to cgd_Close call.\n");
        }
    }

    (void)Stop(aHwId, aCamId);

    FreeBuffers(aHwId, aCamId);

    if (CCL_ERR_SUCCESS != ccl_sem_Destroy(&Dev->mRuntime.mSensUpdateTrigger))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy sensor update semaphore\n");
    }
    if (CCL_ERR_SUCCESS != ccl_mutex_Destroy(&Dev->mRuntime.mSensCfgLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy sensor cfg mutex\n");
    }
}

/******************************************************************************/
/**
 * @brief Update of sensor configuration
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param AsyncOk - if false can only return after sensor was reconfigured
 * @param apSensCfg - new sensor configuration
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool AsyncOk, cca_SensPrm_t* apSensCfg, cca_CamInit_t* apCamInitParams, uint32_t aBasedOnId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    (void)apCamInitParams;

    CapState_t* Dev = &gCapState[aHwId][aCamId];

    if (AsyncOk)
    {
        ccl_Err_t res = ccl_mutex_TryLock(&Dev->mRuntime.mSensCfgLock);
        if (CCL_ERR_TIMEOUT == res)
        {
            // update already in progress, skip this cfg
            LogSensCfg(aHwId, aCamId, "SKIP", SENS_LOG_FRAME_ID_NOT_APPLICABLE - 1, aBasedOnId, apSensCfg);
            return 0;
        }
        if (0 != res)
        {
            ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "ccl_mutex_TryLock fail %d\n", res);
            return -1;
        }

        if (Dev->mRuntime.mSensorUpdateWaiting)
        {
            // previous configuration still awaits its sending to sensor (sending didn't start at all)
            // we log it here as skipped and replace it with the new one we just received on lines below
            LogSensCfg(aHwId, aCamId, "SKIP", SENS_LOG_FRAME_ID_NOT_APPLICABLE - 2, Dev->mRuntime.mBasedOnId, &Dev->mRuntime.mNewSensCfg);
        }

        // we just set flag here, waking of sensor update thread is done just after we receive frame from V4L -> to use inter-frame gap
        Dev->mRuntime.mSensorUpdateWaiting = true;
        Dev->mRuntime.mBasedOnId           = aBasedOnId;

#if defined(SDK_OS_FREE_RTOS)
        utils_EdmaTransfer(&Dev->mRuntime.mNewSensCfg, apSensCfg, sizeof(cca_SensPrm_t));
#else
        memcpy(&Dev->mRuntime.mNewSensCfg, apSensCfg, sizeof(cca_SensPrm_t));
#endif

        if (CCL_ERR_SUCCESS != ccl_mutex_Unlock(&Dev->mRuntime.mSensCfgLock))
        {
            ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "ccl_mutex_Unlock fail %s\n", strerror(errno));
            return -1;
        }
    }
    else
    {
        // requested is synchronous update, e.g. upon exit of this fcn, sensor must be set to new values - we call the update directly
        if (0 != Dev->mCfg.mSensorUpdate(aHwId, aCamId, apSensCfg))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "SensorUpdate interface failed. hwid: %d, camid: %d\n", Dev->mHwId, Dev->mCamId);
        }
    }

    return 0;
}

/******************************************************************************/
/**
 * @brief Indication that capture buffer is no longer used.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apCapBuf - freed buffer
 */
static void CapBufFree(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapBuf_t* apCapBuf)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev = &gCapState[aHwId][aCamId];

    if (apCapBuf == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CapBufFree() -> apCapBuf == NULL. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    if (apCapBuf->mBufId >= Dev->mBufCnt)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CapBufFree() -> buffer index out of range. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    BufInfo_t* Buf = &Dev->mBufs[apCapBuf->mBufId];

    if (Buf->mState == CCL_CGD_BUF_STATE_IDLE)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CapBufFree() -> buffer %d already IDLE!. hwid: %d, camid: %d\n", apCapBuf->mBufId, aHwId, aCamId);
        return;
    }

    /* If CGD has available buffer slot, submit the buffer; otherwise return it to buffers pool */
    if (!cgd_IsEmptyBufferPrepared((cgd_IsiChannel_t)aCamId))
    {
        Buf->mState             = CCL_CGD_BUF_STATE_CGD;
        Buf->mExternal.mFrameNo = 0;

        utils_ImgBuf_t lBuff = {0};
        assert(Buf->mExternal.mBufId < UINT32_MAX);
        lBuff.mHwAddr = Buf->PlaneInfo[0].mPhyLocation;
        lBuff.mpData  = Buf->PlaneInfo[0].mMmapPtr;
        lBuff.mIdx    = Buf->mExternal.mBufId;

        if (cgd_SubmitEmptyBuffer((cgd_IsiChannel_t)aCamId, &lBuff) == CGD_LIB_FAILURE)
        {
            ReturnBuffer(aHwId, aCamId, Buf);
        }
    }
    else
    {
        ReturnBuffer(aHwId, aCamId, Buf);
    }
} // CapBufFree

/******************************************************************************/
static void CgdFatalErrIrqHandle(cgd_RegisterErrorTable_t* lastError)
{
    for (uint8_t aHwId = 0; aHwId < ISP_HWID_CNT; aHwId++)
    {
        for (uint8_t aCamId = 0; aCamId < ISPDRV_CAM_PIPE_ID_CNT; aCamId++)
        {
            CapState_t* Dev = &gCapState[aHwId][aCamId];

            if (ccl_thread_IsRunning(&Dev->mRuntime.mCaptureThreadId))
            {
                if (lastError != NULL)
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "\nCGD reported fatal error (%s) - stopping hwid: %d, camid: %d.\n", lastError->mErrorMsg, aHwId, aCamId);
                }

                if (CGD_LIB_FAILURE == cgd_StreamOff((cgd_IsiChannel_t)aCamId))
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed to cgd_StreamOff call.\n");
                }
            }
        }
    }
} // CgdFatalErrIrqHandle

/******************************************************************************/
static int32_t ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    CapState_t* Dev          = &gCapState[aHwId][aCamId];
    cca_CamInit_t* StreamCfg = &gCclState.mInitCfg.mpCams[aHwId][aCamId];

    if (Dev->mCfg.mConfigureCgd == NULL || gCgdState.mSystemConfig.mpIsi[aCamId] == NULL
        || StreamCfg->mcpExtDef == NULL)
    {
        return -1;
    }

    // prepare specific CCL configuration
    gCgdState.mSystemConfig.mpIsi[aCamId]->mImageWidth      = Dev->mCfg.mOutPictureWidth;
    gCgdState.mSystemConfig.mpIsi[aCamId]->mImageHeight     = Dev->mCfg.mOutPictureHeight;
    gCgdState.mSystemConfig.mpIsi[aCamId]->mEmbdLinesBefore = Dev->mCfg.mEmbdLinesBefore;
    gCgdState.mSystemConfig.mpIsi[aCamId]->mEmbdLinesAfter  = Dev->mCfg.mEmbdLinesAfter;
    gCgdState.mSystemConfig.mpIsi[aCamId]->mBPP             = ccl_GetBitsPP2BytesPP(ccl_GetIspIbufBitsPPCode2Val(StreamCfg->mcpExtDef->mRegs.mPIPE_CONF.mIMG_CONF_CAM0.mIBPP0));

    // initialize the actual device - this code is camera specific
    if (0 != Dev->mCfg.mConfigureCgd(aHwId, aCamId, &gCgdState.mSystemConfig))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "User init of mConfigureCgd interface failed. hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }

    if (CGD_LIB_FAILURE == cgd_Config(&gCgdState.mSystemConfig, (cgd_IsiChannel_t)aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed to config.\n");
        return -1;
    }

    return 0;
} // ConfigureCgd

/******************************************************************************/
/**
 * @brief Returns full height of picture as it comes into ISP (only called in streaming mode)
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static uint32_t  GetFullHeight(ccl_CapCfg_t *apCfg, uint32_t *apEmbSizeBefore, uint32_t *apEmbSizeAfter)
{
    if (apCfg == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "apCfg is NULL!\n");
        return 0;
    }

    if (apEmbSizeBefore != NULL)
    {
        *apEmbSizeBefore = apCfg->mCapParams.mCgd.mEmbdLinesBefore * apCfg->mCapParams.mCgd.mOutPictureWidth;
    }
    if (apEmbSizeAfter != NULL)
    {
        *apEmbSizeAfter = apCfg->mCapParams.mCgd.mEmbdLinesAfter * apCfg->mCapParams.mCgd.mOutPictureWidth;
    }

    return apCfg->mCapParams.mCgd.mEmbdLinesBefore + apCfg->mCapParams.mCgd.mOutPictureHeight + apCfg->mCapParams.mCgd.mEmbdLinesAfter;
} // GetFullHeight

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

int32_t ccl_CapOps_Cgd(ccl_CapOps_t* apOps, ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    if (apOps != NULL)
    {
        memset(&gCapState[aHwId][aCamId], 0, sizeof(CapState_t)); // DDR_NOINIT, need to prepare before init

        apOps->mpCapOpen              = Open;
        apOps->mpCapClose             = Close;
        apOps->mpCapStart             = Start;
        apOps->mpCapStop              = Stop;
        apOps->mpCapSensorUpdate      = Update;
        apOps->mpCapBufReturn         = CapBufFree;
        apOps->StreamingModeSupported = true;
        apOps->mpGetFullHeight        = GetFullHeight;
        apOps->mpFrameDoneEvt         = NULL;
    }

    return 0;
} // ccl_CapOps_Cgd

#else // defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))
// make sure the V4l variant is defined
// but signals error on use
int32_t ccl_CapOps_Cgd(ccl_CapOps_t* apOps, ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    (void)apOps;
    (void)aHwId;
    (void)aCamId;
    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error - CGD Capture method (API) supported only on IMX95 SoC\n");
    return -1;
}
#endif // else from defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))

/***********************************************************************************/
