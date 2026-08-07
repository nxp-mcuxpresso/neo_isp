/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "ilog.h"
#include "ccl_isp.h"
#include "ccl_cca.h"
#include "ccl_internal.h"

#include "mshr.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/**
 * @brief Invalidates cache over image data so it is loaded from RAM
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - frame task to prepare
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t LoadSharedMem(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk)
{
    int32_t ret = 0;
    // reload all output image buffers
    for (int i = CCA_IOBUFT_OUT_FIRST; i < CCA_IOBUFT_CNT; i++)
    {
        if (apFrTsk->mpAllocImgBufs[i] != NULL)
        {
            // NOTE stream 0 have MU 1 associated with it (e.g. there is not stream for MU0)
            if (0 != MShr_Transfer2local(aHwId, (Mu_ChannelId_t)(aCamId + 1), MSHR_DIR_TO_PC, apFrTsk->mpAllocImgBufs[i]))
            {
                ret = -1;
            }
        }
    }

    // reload task structure
    if (0 != MShr_Transfer2local(aHwId, (Mu_ChannelId_t)(aCamId + 1), MSHR_DIR_BOTH, apFrTsk->mIspCfg))
    {
        ret = -1;
    }

    return ret;
} // LoadSharedMem()

/**
 * @brief Handler for ISP stream error (signal from FW that something went wrong...)
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apParam - detail ifo about the error
 */
static void HandleStreamError(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_ParamError_t* apParam)
{
    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Error message received for HWID# %u, CID# %u, err: 0x%x, error_param: %d\n", aHwId, aCamId, apParam->mErrorId, apParam->mParam);
    // TODO nxf90122 IMISP-1724 - forward to ccl callback
}

/**
 * @brief Finds frame task based on ISP task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param TaskAddr - ISP task id
 * @param aTargetState - state list to search
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details This function is not writing any data and is not affected by old ones, so there is no
 * mutex locking of stream variables.
 */
static ccl_FrameTask_t *FindFrameTaskByPhy(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint64_t TaskAddr)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];

    for (uint i = 0; i < CCL_CFG_MAX_BUF_PER_STREAM; i++)
    {
        if (Stream->mFramePool[i].mIspPhy == TaskAddr)
        {
            return &Stream->mFramePool[i];
        }
    }

    return NULL;
}

/**
 * @brief Handler for ISP frame done event
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apParam - detail information for the event
 */
static void HandleFrameDone(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_ParamFrameDone_t* apParam)
{
    ILOG(ILOG_MOD_UTILS, ILOG_LEV_NFO, "Done message received for HWID# %"PRIu32", CID# %"PRIu32", Task: 0x%"PRIx64"\n", aHwId, aCamId, apParam->mTaskAddr);
    ccl_FrameTask_t *Frame = FindFrameTaskByPhy(aHwId, aCamId, apParam->mTaskAddr);

    if (Frame == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Received ISP-Done message for task not on wait list HWID# %"PRIu32", CID# %"PRIu32", IspCfg: 0x%"PRIx64"\n", aHwId, aCamId, apParam->mTaskAddr);
        return;
    }

    Frame->mIspError = apParam->mErrorId;

    if (0 != LoadSharedMem(aHwId, aCamId, Frame))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Fail to re-load task buffers 0x%"PRIx64"\n", Frame->mIspPhy);
    }

    if (0 != ispdrv_LoadFwMeta(aHwId, aCamId, Frame->mIspCfg, apParam->mProfilingIdx))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Fail to load fw metadata 0x%"PRIx64"\n", Frame->mIspPhy);
    }

    // signal event to CCL
    ccl_DoneIspFrame(aHwId, aCamId, Frame);
}

/**
 * @brief Handler for external closure of ISP pipe
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static void HandleStreamClosed(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    printf("Stream closed hwid: %d, camid: %d\n", aHwId, aCamId);
    // TODO nxf90122 IMISP-1724 - forward to ccl callback
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

void ccl_IspCallback(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_EventId_t aEventId, ispdrv_EventParam_t* apParam, void* apClientPtr)
{
    (void)apClientPtr;
    switch (aEventId)
    {
        case ISPDRV_EVENT_ID_ERROR:
            HandleStreamError(aHwId, aCamId, &apParam->mError);
            break;

        case ISPDRV_EVENT_ID_FRAME_DONE:
            HandleFrameDone(aHwId, aCamId, &apParam->mFrameDone);
            break;

        case ISPDRV_EVENT_ID_PIPE_CLOSED:
            HandleStreamClosed(aHwId, aCamId);
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "unknown event (HwId: %d, CamId: %d)\n", aHwId, aCamId);
            break;
    }
}

/*******************************************************************************/

int32_t ccl_AllocImgBufsOut(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrTsk)
{
    ccl_Stream_t *Stream = &gCclState.Streams[aHwId][aCamId];
    uint32_t IspProcessingHeight = 0;
    uint32_t EmbdDataBefore      = 0;

    if (CCL_CAP_MODE_STREAM == Stream->mCapCfg.mCapMode)
    {
        if (Stream->mCapOps.mpGetFullHeight == NULL)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "mpGetFullHeight() is NULL in straming mode! hwid: %d, camid: %d\n",  aHwId, aCamId);
            return -1;
        }
        IspProcessingHeight = Stream->mCapOps.mpGetFullHeight(&Stream->mCapCfg, &EmbdDataBefore, NULL);
    }
    else
    {
        IspProcessingHeight = apFrTsk->FrameData.mImg[CCA_IOBUFT_IN_FIRST].mHeight;
    }

    // Setup buffers to be allocated
    if( 0!= ccl_SetupImgBufsOut(aHwId, aCamId, apFrTsk, IspProcessingHeight) )
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Setup img buffers failed, hwid: %d, camid: %d\n", aHwId, aCamId);
        return -1;
    }


    for (int BufId = CCA_IOBUFT_OUT_FIRST; BufId < CCA_IOBUFT_CNT; BufId++)
    {
        cca_ImgBuf_t* lpIoBuf = &apFrTsk->FrameData.mImg[BufId];
        if (lpIoBuf->mSizeBytes > 0)
        {
            apFrTsk->mpAllocImgBufs[BufId]  = MShr_malloc(aHwId, (Mu_ChannelId_t)(aCamId + 1), lpIoBuf->mSizeBytes, -1);
            if (NULL == apFrTsk->mpAllocImgBufs[BufId])
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Out buf alloc failed size: 0x%x, hwid: %d, camid: %d\n",  lpIoBuf->mSizeBytes, aHwId, aCamId);
                return -1;
            }

            if (!MShr_GetGlobalMemHandle(aHwId, (Mu_ChannelId_t)(aCamId + 1), apFrTsk->mpAllocImgBufs[BufId], &apFrTsk->mImgBufsBasePhy[BufId]))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Out buf PHY location not found. hwid: %d, camid: %d\n",  aHwId, aCamId);
                return -1;
            }

            lpIoBuf->mpData = (uint32_t*)(((uint8_t*)apFrTsk->mpAllocImgBufs[BufId]) + (EmbdDataBefore * lpIoBuf->mBpp / 8));
            lpIoBuf->mHwAddr = apFrTsk->mImgBufsBasePhy[BufId] + (EmbdDataBefore * lpIoBuf->mBpp / 8);
        }
        else
        {
            apFrTsk->mpAllocImgBufs[BufId] = NULL;
            lpIoBuf->mpData = NULL;
        }
    }

    return 0;
} // ccl_AllocImgBufsOut()

/*******************************************************************************/

int32_t ccl_FreeImgBufsOut(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrTsk)
{
    cca_ImgBuf_t* lpIoBuf;
    for (int BufId = CCA_IOBUFT_OUT_FIRST; BufId < CCA_IOBUFT_CNT; BufId++)
    {
        if (apFrTsk->mpAllocImgBufs[BufId] != NULL)
        {
            lpIoBuf = &apFrTsk->FrameData.mImg[BufId];
            MShr_free(aHwId, (Mu_ChannelId_t)(aCamId + 1), apFrTsk->mpAllocImgBufs[BufId]);
            apFrTsk->mpAllocImgBufs[BufId] = NULL;
            lpIoBuf->mpData = NULL;
            apFrTsk->FrameData.mImg[BufId].mHwAddr = 0;
            apFrTsk->mImgBufsBasePhy[BufId] = 0;
        }
    }

    return 0;
} // InitImgBufsOut()

/*******************************************************************************/

int32_t ccl_UpdateIspBufLocations(ccl_FrameTask_t *apFrTsk)
{
    for(uint8_t lBufId = 0; lBufId < CCA_IOBUFT_CNT; lBufId++)
    {
        if(apFrTsk->FrameData.mImg[lBufId].mSizeBytes > 0)
        {
            if (0 != ccl_WriteIspBufConfig(apFrTsk, (cca_IoBufTypes_t)lBufId))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to write ISP buffer configuration to HW ctx\n");
                return -1;
            }
        }
    }

    return 0;

} // ccl_UpdateIspBufLocations()


/*******************************************************************************/

int32_t ccl_AllocIspTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk)
{

    apFrTsk->mIspCfg = MShr_malloc(aHwId, (Mu_ChannelId_t)(aCamId + 1), sizeof(isp_Task_t), -1);
    if (apFrTsk->mIspCfg == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to allocate ISP task. hwid: %d, camid: %d\n",  aHwId, aCamId);
        return -1;
    }
    if (!MShr_GetGlobalMemHandle(aHwId, (Mu_ChannelId_t)(aCamId + 1), apFrTsk->mIspCfg, &apFrTsk->mIspPhy))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Task PHY location not found. hwid: %d, camid: %d\n",  aHwId, aCamId);
        return -1;
    }
    apFrTsk->FrameData.mMetaData.mIspcfg = &apFrTsk->mIspCfg->mIspCtx.mCtxHw;

    // preload ISP task with default configuration as CCA is not controlling all registers
    memcpy(&apFrTsk->mIspCfg->mIspCtx.mCtxHw, gCclState.mInitCfg.mpCams[aHwId][aCamId].mcpExtDef, sizeof(isp_CtxHw_t));

    // by default disable ISP debug registers, with default config ISP would stop for debug at first line - this preload needs to be here in case CCA is off
    apFrTsk->mIspCfg->mIspCtx.mCtxHw.mRegs.mIDBG1.mLINE_NUM.mLINE_NUM = 0x1FFFFU;
    apFrTsk->mIspCfg->mIspCtx.mCtxHw.mRegs.mIDBG2.mLINE_NUM.mLINE_NUM = 0x1FFFFU;

    return 0;
}

/*******************************************************************************/

int32_t ccl_FreeIspTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk)
{
    MShr_free(aHwId, (Mu_ChannelId_t)(aCamId + 1), apFrTsk->mIspCfg);
    return 0;
}

/*******************************************************************************/


int32_t ccl_FlushIspBufs(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk)
{
    int ret = 0;

    // flush task structure
    if (0 != MShr_Transfer2remote(aHwId, (Mu_ChannelId_t)(aCamId + 1), MSHR_DIR_BOTH, apFrTsk->mIspCfg, NULL))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to flush ISP task. hwid: %d, camid: %d\n",  aHwId, aCamId);
        ret = -1;
    }

    // flush all output image buffers
    for (int i = CCA_IOBUFT_OUT_FIRST; i < CCA_IOBUFT_CNT; i++)
    {
        if (apFrTsk->mpAllocImgBufs[i] != NULL)
        {
            // NOTE stream 0 have MU 1 associated with it (e.g. there is no stream for MU0)
            if (0 != MShr_Transfer2remote(ISP_HWID_0, (Mu_ChannelId_t)(aCamId + 1), MSHR_DIR_TO_PC, apFrTsk->mpAllocImgBufs[i], NULL))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to flush img buffer %d. hwid: %d, camid: %d\n", i, aHwId, aCamId);
                ret = -1;
            }
        }
    }

    return ret;
}

