/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ccl_cca.c
 * @brief Internal CCL header implementing camera control organisation
 *
 * @details CCA organisation and interface. Takes care of camera runtime parameter adjustment for CCL lib.
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#if defined(LINUX_EMB) || defined(LINUX_PC) || defined(GHS)
    #include "errno.h"
#endif

#include "ilog.h"

#include "ccl_internal.h"
#include "ccl_cca.h"
#include "cca.h"
#include "ccl_isp.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

#define MAX_CCF_GROUP_CNT (ISP_HWID_CNT * ISPDRV_CAM_PIPE_ID_CNT)



/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/** @brief Static allocation of memory for CCF groups
 * @details not all must be used, filled in from idx 0 up (end is unused)*/
DDR_NOINIT CcfGroup_t gCcfGroups[MAX_CCF_GROUP_CNT];

/** @brief Lock to ensure that CCA is called sequentially not in parallel
 * @details Only used if CCF library have mCcfIsParallelCapable==false during ccl_init() */
CCL_DEFINE_MUTEX_STATIC_INIT(gCcaRunLock);


/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

#if CCL_FEATURE_LOG_SENS_CFG

#define SER(...)                                            \
    res = snprintf(apBufOut + offset, RemSpc, __VA_ARGS__); \
    if (res < 0)                                            \
    {                                                       \
        return -1;                                          \
    }                                                       \
    offset += res;                                          \
    RemSpc -= res;                                          \
    if (RemSpc <= 0)                                        \
    {                                                       \
        return -1;                                          \
    }

int32_t SerializeSensPrmHeader(char* apBufOut, ssize_t aBufSize)
{
    int32_t offset = -1;
    if ((apBufOut != NULL))
    {
        ssize_t RemSpc = aBufSize;
        int res;
        offset = 0;

        SER("Event Time,");
        SER("Event,");
        SER("Frame Id,");
        SER("Based On,");

        // EXP
        SER("ExpType,");
        for (int32_t lIter = 0; lIter < (int32_t)CCA_HDR_EXP_CNT; lIter++)
        {
            SER("Exp[%d],AGain[%d],DGain[%d],", lIter, lIter, lIter);
        }
        SER("L2S ratio,L2VS ratio,");
        SER("SensorTemp,");
        SER("WB gain Red,WB gain Green,WB gain Blue,");
        SER("WB offset Red,WB offset Green,WB offset Blue,");
        SER("WB ColorTemp,WB Loc,");
        // GENERIC
        SER("FrameCount\n");
    }

    return offset;
}

/**
 * @brief Serialize sensor configuration event into given buffer
 *
 * @param apBufOut - buffer to fill
 * @param aBufSize - max line size
 * @param apEvtStr - event id (string)
 * @param acpcSensPrm - sensor configuration
 * @param aFrameId - id of frame
 * @param aBasedOnFrameId - id of frame cfg is based on
 * @return actual size or -1 on error
 */
static int32_t SerializeSensPrm(char* apBufOut, ssize_t aBufSize, const char* apEvtStr, const cca_SensPrm_t* const acpcSensPrm, uint32_t aFrameId, uint32_t aBasedOnFrameId)
{
    int32_t offset = -1;
    if (apBufOut != NULL)
    {
        ssize_t RemSpc = aBufSize;
        int res;
        offset = 0;

        SER("%lu,", ccl_GetTime());
        SER("%s,", apEvtStr);
        SER("%u,", aFrameId);
        SER("%u,", aBasedOnFrameId);

        if (acpcSensPrm != NULL)
        {
            // EXP
            SER("%s,", (acpcSensPrm->mExpType == CCA_EXP_NRM) ? "NRM" : "HDR");
            for (int32_t lIter = 0; lIter < (int32_t)CCA_HDR_EXP_CNT; lIter++)
            {
                const cca_Exp_t* lpcExp = &(acpcSensPrm->mpExp[lIter]);
                SER("%u,%u,%u,", lpcExp->mExp, lpcExp->mAgain, lpcExp->mDgain);
            }
            SER("%u,%u,", acpcSensPrm->mL2sRatio, acpcSensPrm->mL2vsRatio);
            SER("%d,", acpcSensPrm->mSensorTemp);
            // WB
            const cca_WbGains_t* lpcGains = &(acpcSensPrm->mWb.mGains);
            SER("%u,%u,%u,", lpcGains->mRed, lpcGains->mGreen, lpcGains->mBlue);
            const cca_WbOffsets_t* lpcOff = &(acpcSensPrm->mWb.mOffsets);
            SER("%u,%u,%u,", lpcOff->mRed, lpcOff->mGreen, lpcOff->mBlue);
            const cca_Wb_t* lpcWb = &(acpcSensPrm->mWb);
            SER("%u,%s,", lpcWb->mColorTemp, (lpcWb->mWbLoc == CCA_WB_ISP) ? "ISP" : "SEN");
            // GENERIC
            SER("%u", acpcSensPrm->mFrameCount);
        }

        SER("\n");
    } // if not NULL

    return offset;
}

/**
 * @brief Logs sensor event with cfg
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apEvtStr - event string
 * @param FrameId - frame id if configuration is from frame embeded data
 * @param BasedOnId - frame id this configuration is based on (matched to for EMB event)
 * @param apSensCfg - sensor configuration
 */
void LogSensCfg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, const char* apEvtStr, uint32_t FrameId, uint32_t BasedOnId, cca_SensPrm_t* apSensCfg)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];
    char Line[4096];

    if (!Stream->mCapCfg.mLogSensCfg)
    {
        return;
    }

    int32_t size = SerializeSensPrm(Line, sizeof(Line), apEvtStr, apSensCfg, FrameId, BasedOnId);

    if (size <= 0 || Stream->mSensLogFd == -1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to log sens cfg.\n");
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mSensLogLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
    }

    if (-1 == write(Stream->mSensLogFd, Line, size))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to write frame times - %s. hwid: %d, camid: %d\n", strerror(errno), aHwId, aCamId);
    }

    if (0 != ccl_mutex_Unlock(&Stream->mSensLogLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
    }
}
#endif

void UpdateCcaTimeStats(uint64_t aNewTimeItem)
{
    ccl_TimeStatItem_t *Stats = &gCclState.mStats.mGlobalCcaTimes;

    const uint8_t AvgHistSize = 120;

    if (aNewTimeItem > Stats->mMax)
    {
        Stats->mMax = aNewTimeItem;
    }
    if (aNewTimeItem < Stats->mMin)
    {
        Stats->mMin = aNewTimeItem;
    }

    Stats->mAverage -= Stats->mAverage / AvgHistSize;
    Stats->mAverage += aNewTimeItem / AvgHistSize;
}

/**
 * @brief Check if given stream should be considered for CCF
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aCcfGroupId - id of CCF group
 *
 * @return true if CCF should be running over this stream, false if sensor should not be adjusted by CCF at this time
 */
static bool IsStreamActiveOnCcf(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint32_t aCcfGroupId)
{

    if (gCclState.mInitCfg.mpCams[aHwId][aCamId].mEnabled != 1)
    {
        // stream is not enabled at all
        return false;
    }

    if (gCclState.Streams[aHwId][aCamId].mState != CCL_STREAM_STATE_RUNNING)
    {
        // stream is currently not running
        return false;
    }

    if (0 == gCclState.mInitCfg.mCcfGroups[aHwId][aCamId])
    {
        // no CCF over this stream
        return false;
    }
    if (aCcfGroupId != gCclState.mInitCfg.mCcfGroups[aHwId][aCamId])
    {
        // stream belongs to different CCF group
        return false;
    }

    return true;
}

/**
 * @brief Checks if CCF group is ready to run CCF algos
 *
 * @param aCcfGroupId - CCF group to check
 *
 * @return true if ready, false if not
 *
 * @details Note that all streams that are capable of running remain locked
 */
static bool CheckRunPossible(CcfGroup_t* apCcfGroup)
{
    bool ret = true;

    for (ispdrv_HwId_t HwId = ISP_HWID_0; HwId < ISP_HWID_CNT; HwId++)
    {
        for (ispdrv_CamPipeId_t CamId = ISPDRV_CAM_PIPE_ID_0; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];

            // check that this stream belong to this CCF group before locking mutex
            if (apCcfGroup->mCcfGroupId != gCclState.mInitCfg.mCcfGroups[HwId][CamId])
            {
                // stream belongs to different CCF group
                continue;
            }

            if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
            }

            if (!IsStreamActiveOnCcf(HwId, CamId, apCcfGroup->mCcfGroupId))
            {
                apCcfGroup->mActive[HwId][CamId] = false;
            }
            else if (Stream->mCcfStats.mStatsState == CCL_STATS_STATE_NONE)
            {
                // this stream is not ready for CCF run -> we need to abort this run and wait for a frame to arrive from ISP
                ret                           = false;
                apCcfGroup->mActive[HwId][CamId] = false;
            }
            else
            {
                // this stream is ready, keep it locked
                apCcfGroup->mActive[HwId][CamId] = true;
            }

            if (!apCcfGroup->mActive[HwId][CamId])
            {
                if (0 != ccl_mutex_Unlock(&Stream->mLock))
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
                }
            }

            // do not continue with stream locking if we already know we will not run
            if (!ret)
            {
                return ret;
            }
        }
    }

    return ret;
}

#if CCL_FEATURE_CONTEXT_MATCH
/**
 * @brief Returns next item after most recent record. Makes sure that the item is no longer considered valid.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
static ccl_HwCfg_t* GetNewHistoryItem(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];

    // clean up the memory (in case of last prepared item was not used)
    memset(&Stream->mHwCfgHistory.mItems[Stream->mHwCfgHistory.mNewIdx], 0, sizeof(ccl_HwCfg_t));

    return &Stream->mHwCfgHistory.mItems[Stream->mHwCfgHistory.mNewIdx];
}
#endif

/**
 * @brief Prepares IO array structure for a call to cca_ProcCcf()
 *
 * @param apCcfIoArr - structure to prepare
 * @param apCcfIo - memories to hold input data for CCA
 * @param apNewCfg - memories to hold output data from CCA
 *
 * @return Number of streams that will be processed.
 */
static uint32_t PrepCcfIo(cca_CcfIoArr_t* apCcfIoArr, cca_CcfIo_t (*apCcfIo)[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT], CcfGroup_t* apCcfGroup)
{
    uint32_t ReadyCnt = 0;
    for (uint8_t HwId = 0; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint8_t CamId = 0; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];
            if (!apCcfGroup->mActive[HwId][CamId])
            {
                // this stream is not active or not on this CCF group
                apCcfIoArr->mppCcfIo[(ISPDRV_CAM_PIPE_ID_CNT * HwId) + CamId] = NULL;
                continue;
            }

            if (Stream->mCcfStats.mStatsState == CCL_STATS_STATE_DROPPED)
            {
                // there was a frame drop on this stream do not process is as we have no data
                apCcfIoArr->mppCcfIo[(ISPDRV_CAM_PIPE_ID_CNT * HwId) + CamId] = NULL;

                // reset stream stats
                Stream->mCcfStats.mStatsState = CCL_STATS_STATE_NONE;
                Stream->mCcfStats.mpCcaFrame  = NULL;

                apCcfGroup->mActive[HwId][CamId] = false;
                if (0 != ccl_mutex_Unlock(&Stream->mLock))
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
                }
                continue;
            }

            // *** Setup camera control functions (CCF) ***

            // move the frame to CCA buffer so new frames are stored for future CCA runs
            ccl_FrameTask_t* LastFrame    = Stream->mCcfStats.mpLastFrame;
            Stream->mCcfStats.mStatsState = CCL_STATS_STATE_NONE;
            Stream->mCcfStats.mpLastFrame = NULL;
            Stream->mCcfStats.mpCcaFrame  = LastFrame;
            ccl_SetFrameState((ispdrv_HwId_t)HwId, (ispdrv_CamPipeId_t)CamId, LastFrame, CCL_FRAME_STATE_CCA_PROCESSING);

            memset(&((*apCcfIo)[HwId][CamId]), 0, sizeof(cca_CcfIo_t));
            apCcfIoArr->mppCcfIo[ISPDRV_CAM_PIPE_ID_CNT * HwId + CamId] = &((*apCcfIo)[HwId][CamId]);

            // * History Input *
            // do not set embedded params if empty or invalid
            if(((LastFrame->FrameData.mEmbed.mEmbd0Sz + LastFrame->FrameData.mEmbed.mEmbd1Sz) > 0U)
                && (LastFrame->mActSensCfg.mDataInvalid == false))
            {
                (*apCcfIo)[HwId][CamId].mpEmbdPrm   = &LastFrame->mActSensCfg; // was parsed from embd data before
                (*apCcfIo)[HwId][CamId].mpcEmbdData = &LastFrame->FrameData.mEmbed; // only for tuning purpose
            }
            else
            {
                (*apCcfIo)[HwId][CamId].mpEmbdPrm    = NULL;
                (*apCcfIo)[HwId][CamId].mpcEmbdData  = NULL;
            }
            (*apCcfIo)[HwId][CamId].mpIspCtxIn = &LastFrame->mIspCfg->mIspCtx;

            // * Provide all image buffers to CCA *
            for (uint32_t BufType = 0U; BufType < CCA_IOBUFT_CNT; BufType++)
            {
                (*apCcfIo)[HwId][CamId].mppcIoBufs[BufType] = &LastFrame->FrameData.mImg[BufType];
            } // for all buffer types

            // * New Cfg Output *
            // allocate mem for new item
            (*apCcfIo)[HwId][CamId].mpIspCtxOut  = &Stream->mIspCfg[Stream->mIspCfgIdxCcf];
#if CCL_FEATURE_CONTEXT_MATCH
            ccl_HwCfg_t* NewCfgHistItem = GetNewHistoryItem(HwId, CamId);
            (*apCcfIo)[HwId][CamId].mpSensPrmNew = &NewCfgHistItem->mSensCfg;
#else
            (*apCcfIo)[HwId][CamId].mpSensPrmNew = &Stream->mHwCfg[Stream->mIspCfgIdxCcf].mSensCfg;
#endif


            ReadyCnt++;
        }
    }

    return ReadyCnt;
}

/**
 * @brief Releases mutexes for streams that were locked during ccf pass preparations
 *
 * @param apCcfGroup - CCF group that no longer needs stream lock
 */
static void UnlockStreams(CcfGroup_t* apCcfGroup, bool aNoRun)
{
    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];
            if (apCcfGroup->mActive[HwId][CamId])
            {
                if (0 != ccl_mutex_Unlock(&Stream->mLock))
                {
                    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
                }
                if (aNoRun)
                {
                    apCcfGroup->mActive[HwId][CamId] = false;
                }
            }
        }
    }
}

/**
 * @brief Fix hw issues on rev A by setting some registers
 *
 * @param apCtxHw - hw registers to fix
 */
#if defined(IMX95) && defined(A0)
static void FixIspRegs_RevA(isp_CtxHw_t* apCtxHw)
{
    /* Unfortunately the IRQ enable interrupts are in the middle of the config
    registers of ISP. Thus we need to set these values already in the configuration
    to not disable the IRQs by writing the configuration into the ISP. */
    apCtxHw->mRegs.mPIPE_CONF.mINT_EN0.mEN_FS1   = 1U;
    apCtxHw->mRegs.mPIPE_CONF.mINT_EN0.mEN_STATD = 1U;
    apCtxHw->mRegs.mPIPE_CONF.mINT_EN0.mEN_FD2   = 1U;
} // InitIspRegs()
#endif


#if CCL_FEATURE_CONTEXT_MATCH
static uint8_t GetCfgHistorySize(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];

    int16_t ret = Stream->mHwCfgHistory.mNewIdx - Stream->mHwCfgHistory.mOldIdx;

    if (ret < 0)
    {
        ret += SENS_CFG_HISTORY_BUF_SIZE;
    }

    return ret;
}
#endif
/**
 * @brief Option to apply changes to newly generated context
 *
 * @param apUpdCtx - context to be updated
 * @details invoked on init and right after CCA generates new configuration.
 * Note that only CCA relevant registers are copied to ccl_FrameTask_t.mIspCfg
 */
static void UpdCtx(ccl_UpdCtx_t* apUpdCtx)
{
    // *** hadrcoded updates forced by CCL - if any ***

    // by default disable ISP debug registers, with default config ISP would stop for debug at first line
    apUpdCtx->mpNewCtx->mCtxHw.mRegs.mIDBG1.mLINE_NUM.mLINE_NUM = 0x1FFFFU;
    apUpdCtx->mpNewCtx->mCtxHw.mRegs.mIDBG2.mLINE_NUM.mLINE_NUM = 0x1FFFFU;
#if 0
    // set alignment
    isp_PIPE_CONF_t* lpPipeConf          = &apUpdCtx->mpNewCtx->mCtxHw.mRegs.mPIPE_CONF;
    lpPipeConf->mIMG_CONF_CAM0.mINALIGN0 = 0;
    lpPipeConf->mIMG_CONF_CAM0.mINALIGN1 = 0;
    lpPipeConf->mIMG_CONF_CAM0.mLPALIGN0 = 0;
    lpPipeConf->mIMG_CONF_CAM0.mLPALIGN1 = 0;

    // Switch to RGB
    isp_GCM_t* lpGcm                   = &apUpdCtx->mpNewCtx->mCtxHw.mRegs.mGCM;
    lpGcm->mOMAT0_CAM0.mR0C0           = 0;
    lpGcm->mOMAT0_CAM0.mR0C1           = 0;
    lpGcm->mOMAT1_CAM0.mR0C2           = 284;
    lpGcm->mOMAT2_CAM0.mR1C0           = 0;
    lpGcm->mOMAT2_CAM0.mR1C1           = 284;
    lpGcm->mOMAT3_CAM0.mR1C2           = 0;
    lpGcm->mOMAT4_CAM0.mR2C0           = 284;
    lpGcm->mOMAT4_CAM0.mR2C1           = 0;
    lpGcm->mOMAT5_CAM0.mR2C2           = 0;
    lpGcm->mOOFFSET0_CAM0.mOFFSET0     = 0;
    lpGcm->mOOFFSET1_CAM0.mOFFSET1     = 0;
    lpGcm->mOOFFSET2_CAM0.mOFFSET2     = 0;
    lpGcm->mMAT_CONFG_CAM0.mSIGN_CONFG = 1;
#endif

// no change intentionally left under DTP control
#if 0
    isp_Packetizer_t *lpPacketizerRegs = &apUpdCtx->mpNewCtx->mCtxHw.mRegs.mPacketizer;
    // force packetizer output format
    lpPacketizerRegs->mPACK_CTRL_CAM0.mTYPE = 1U; // 0 == semiplanar, 1 == interleaved
    lpPacketizerRegs->mCH0_CTRL_CAM0.mOBPP = 6; // 0: 12bpp, 2: 16bpp (CH0 only), 4: 10bpp, 6: 8bpp
    lpPacketizerRegs->mCH12_CTRL_CAM0.mOBPP = 6;
    lpPacketizerRegs->mPACK_CTRL_CAM0.mA0S = 0; // appends 0s
    lpPacketizerRegs->mCH12_CTRL_CAM0.mSUBSAMPLE = 0; // C12 subsumpling: 0: 444, 1: 422 h, 2: 420 h&v.
#endif

    // *** user application updates ***
    if( NULL != gCclState.mInitCfg.mEventHandlers.mpUpdateContext)
    {
        gCclState.mInitCfg.mEventHandlers.mpUpdateContext(apUpdCtx, gCclState.mInitCfg.mEventHandlers.mpUsrPrm);
    }
    // *** gui demo control - highest priority ***
#ifdef GUI_DEMO
    // handle DRC alpha blending value
    if (!gCclState.mpExtCtrlCtx->mAutoDrcOn)
    {
        apUpdCtx->mpNewCtx->mCtxHw.mRegs.mDRC.mALPHA_CAM0.mALPHA = gCclState.mpExtCtrlCtx->mDrc;
    }
#endif
} // UpdCtx()

/**
 * @brief Event handling -> CCF generated new cfg for this stream
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apNewCtx - new ISP configuration
 * @param apNewSensPrm - new sensor configuration
 *
 * @details Performs ISP parameter adjustments (based on revision, mood, test, etc.).
 * Allocates new history item. Remove oldest cfg item if history is full.
 * Enforces new configuration on sensor. Stores the new cfg in streams history.
 */
static void ProcessNewCfg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint32_t aFrameId)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];
#if CCL_FEATURE_CONTEXT_MATCH
    ccl_HwCfg_t * NewHwConfig = &Stream->mHwCfgHistory.mItems[Stream->mHwCfgHistory.mNewIdx];
#else
    ccl_HwCfg_t * NewHwConfig = &Stream->mHwCfg[Stream->mIspCfgIdxCcf];
#endif

    if (Stream->mState != CCL_STREAM_STATE_RUNNING)
    {
        // stream is no longer running we do not update its configuration
        return;
    }

    // modify generated params if needed
    ccl_UpdCtx_t lUpdCtx;
    lUpdCtx.mHwId        = aHwId;
    lUpdCtx.mCamId       = aCamId;
    lUpdCtx.mpNewCtx     = &Stream->mIspCfg[Stream->mIspCfgIdxCcf];
    lUpdCtx.mpNewSensPrm = &NewHwConfig->mSensCfg;
    lUpdCtx.mFrameId     = aFrameId;
    UpdCtx(&lUpdCtx);

#if CCL_FEATURE_CONTEXT_MATCH
    // distribute newly generated configuration
    // NOTE: only partial overlay is extracted from CCA, ActiveIspCfg is preloaded with APP defaults then this partial CCA overlay is applied
    memcpy(&NewHwConfig->mHdrCfg, &Stream->mIspCfg[Stream->mIspCfgIdxCcf].mCtxHw.mRegs.mHDR_MERGE, sizeof(isp_HDR_MERGE_t));
#endif
    NewHwConfig->mBasedOnId                 = aFrameId;
    Stream->mIspCfg[Stream->mIspCfgIdxCcf].mCtxMeta.mFrameIdx = aFrameId;
    // activate new ISP cfg (note that "sensor cfg history item" is activated only after it was enforced on sensor)
    Stream->mIspCfgIdxActive = Stream->mIspCfgIdxCcf;
    Stream->mIspCfgIdxCcf = (Stream->mIspCfgIdxCcf + 1) % ISP_CFG_CNT_PER_STREAM;

    LogSensCfg(aHwId, aCamId, "GEN", SENS_LOG_FRAME_ID_NOT_APPLICABLE, NewHwConfig->mBasedOnId, &NewHwConfig->mSensCfg);

    Stream->mCfgCntr++;
    if ((Stream->mCfgCntr % Stream->mCapCfg.mSensUpdPeriod) == 0)
    {
        // enforce configuration on sensor
        if (0 != Stream->mCapOps.mpCapSensorUpdate(aHwId, aCamId, true, &NewHwConfig->mSensCfg, &gCclState.mInitCfg.mpCams[aHwId][aCamId], NewHwConfig->mBasedOnId))
        {
            printf("Failed to update sensor configuration #%u.\n", aCamId);
            return;
        }
    }
    else
    {
        // only send every n-th configuration
        LogSensCfg(aHwId, aCamId, "SKIP", SENS_LOG_FRAME_ID_NOT_APPLICABLE, NewHwConfig->mBasedOnId, &NewHwConfig->mSensCfg);
    }

#if CCL_FEATURE_CONTEXT_MATCH
    // configuration activated, mark it ready
    Stream->mHwCfgHistory.mNewIdx = (Stream->mHwCfgHistory.mNewIdx + 1) % SENS_CFG_HISTORY_BUF_SIZE;
    // make sure we limit history size and that there is always at least one item gap in buffer
    if ((Stream->mCapCfg.mMaxCfgHistCnt < GetCfgHistorySize(aHwId, aCamId)) || (Stream->mHwCfgHistory.mOldIdx == Stream->mHwCfgHistory.mNewIdx))
    {
        // drop oldest cfg
        Stream->mHwCfgHistory.mOldIdx++;
        Stream->mHwCfgHistory.mOldIdx %= SENS_CFG_HISTORY_BUF_SIZE;
    }
#endif
}

/**
 * @brief iterates through all the new configurations and pushes it to each stream
 *
 * @param aCcaResult result of CCA operation as a whole
 * @param apNewCfg list of new configurations
 */
static void PushNewCfg(uint32_t aCcaResult, CcfGroup_t* apCcfGroup)
{
    for (ispdrv_HwId_t HwId = ISP_HWID_0; HwId < ISP_HWID_CNT; HwId++)
    {
        for (ispdrv_CamPipeId_t CamId = ISPDRV_CAM_PIPE_ID_0; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];

            if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
            }

            if (apCcfGroup->mActive[HwId][CamId])
            {
                uint32_t FrameId = 0;
                if (Stream->mCcfStats.mpCcaFrame != NULL)
                {
                    FrameId = Stream->mCcfStats.mpCcaFrame->FrameData.mMetaData.mFrameNo;
                }

                // now we are done with last frames, we can reset Last-Frame-state and release the location for updates (lock was activated during PrepCcfIo())
                ccl_FreeFrameFromCca(HwId, CamId, Stream->mCcfStats.mpCcaFrame);
                Stream->mCcfStats.mpCcaFrame = NULL;

                if (aCcaResult == 0)
                {
                    ProcessNewCfg(HwId, CamId, FrameId);
                }
                apCcfGroup->mActive[HwId][CamId] = false;
            }

            if (0 != ccl_mutex_Unlock(&Stream->mLock))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
                return;
            }
        }
    }
}

/**
 * @brief CCF processing thread (one runs for each CCF group)
 *
 * @param Arg - target CCF group that should be serviced by this thread (CcfGroup_t *)
 * @return NULL
 */
static void* CcaProcessingThread(void* Arg)
{
    CcfGroup_t* CcfGroup = Arg;

    while (true)
    {
        // await event that ISP processed new frame for this CCF group
        if (0 != ccl_sem_Take(&CcfGroup->mNewFrameEvent, CCL_SEM_WAIT_ININITE))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to wait for \"check if CCF ready\" event.\n");
            return NULL;
        }

        // checks if conditions for running CCA are met
        // NOTE: if the stream is ready for CCA it will remain locked upon fcn exit
        if (!CheckRunPossible(CcfGroup))
        {
            // not all frames are in, wait for next event
            UnlockStreams(CcfGroup, true);
            continue;
        }

        // prepare CCA io structure while we still have the streams locked
        cca_CcfIoArr_t lCcfIoArr;
        memset(&lCcfIoArr, 0, sizeof(cca_CcfIoArr_t));
        cca_CcfIo_t lpCcfIo[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];
        uint32_t RdyCnt = PrepCcfIo(&lCcfIoArr, &lpCcfIo, CcfGroup);

        // release the streams as everything is now beeing held in IO array
        UnlockStreams(CcfGroup, false);

        if (RdyCnt == 0)
        {
            // all streams have frame drops, do not run
            continue;
        }

        // if CCF library isn't thread-safe we need to ensure serialization of requests
        if (!gCclState.mInitCfg.mCcfIsParallelCapable)
        {
            if (0 != ccl_mutex_Lock(&gCcaRunLock, CCL_MUTEX_WAIT_ININITE))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail - ignoring\n");
            }
        }

        uint64_t Start = ccl_GetTime();

        // Run CCF
        int CcaResult = cca_ProcCcf(&lCcfIoArr);

        uint64_t End = ccl_GetTime();

        UpdateCcaTimeStats((End-Start)*1000);

        gCclState.mStats.mCca[CcfGroup->mCcfGroupIdx].mPassCnt[gCclState.mStats.mCca[CcfGroup->mCcfGroupIdx].mWp] ++;

        // if CCF library isn't thread-safe - release lock so other CCF groups can run
        if (!gCclState.mInitCfg.mCcfIsParallelCapable)
        {
            if (0 != ccl_mutex_Unlock(&gCcaRunLock))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail - ignoring\n");
            }
        }

        if (CcaResult != 0)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCA processing failed for group: %d\n", CcfGroup->mCcfGroupId);
        }
        else
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_NFO, "CCA processing Done for group: %d\n", CcfGroup->mCcfGroupId);
        }

        // now we signal new configuration for each stream that have one
        PushNewCfg(CcaResult, CcfGroup);
    }
}

/**
 * @brief Initializes local variables and system objects
 *
 * @return 0 on success or negative error otherwise.
 */
static int32_t InitVars()
{
    memset(gCcfGroups, 0, sizeof(gCcfGroups));

    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];
            memset(Stream->mIspCfg, 0, sizeof(Stream->mIspCfg));
            Stream->mIspCfgIdxActive = 0;
            Stream->mIspCfgIdxCcf = (Stream->mIspCfgIdxActive+1) % ISP_CFG_CNT_PER_STREAM;
        }
    }

    return 0;
}

/**
 * @brief Clean up of local system objects
 *
 */
static void DeinitVars()
{
    // nothing to deinit now, keeping below cycle for future use
#if 0
    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];
        }
    }
#endif // if 0
}

/**
 * @brief Searches CCF groups if one with given ID already exists
 *
 * @param aGroupId - id to check
 *
 * @return pointer to existing group or NULL
 */
static CcfGroup_t* GetCcfGroup(uint32_t aGroupId)
{
    uint32_t GroupIdx = 0;
    while (GroupIdx < MAX_CCF_GROUP_CNT && gCcfGroups[GroupIdx].mCcfGroupId != aGroupId)
    {
        GroupIdx++;
    }

    if (GroupIdx >= MAX_CCF_GROUP_CNT)
    {
        return NULL;
    }

    gCcfGroups[GroupIdx].mCcfGroupIdx = GroupIdx;
    return &gCcfGroups[GroupIdx];
}

/**
 * @brief Generates CCF groups based on cfg given to us in ccl_init()
 * @param [in] apCcaCfg pointer to CCA config file
 * @param [out] apCcfEnabledCameras pointer to counter of CCF enabled cameras
 *
 * @return 0 on success or negative error otherwise.
 */
static ccl_Err_t GenerateCcfGroups(cca_Cfg_t* apCcaCfg, uint32_t* apCcfEnabledCameras)
{
    uint32_t lCcfEnabledCameras = 0U;
    for (uint32_t HwId = 0U; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint32_t CamId = 0U; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            ccl_Stream_t* Stream = &gCclState.Streams[HwId][CamId];
            // get ccf group id for current stream
            uint32_t CcfGid = gCclState.mInitCfg.mCcfGroups[HwId][CamId];
            // make sure the stream doesn't have group associated until we find one
            Stream->mCcfStats.mCcfgGroup = NULL;

            // check if ccf should be run over this stream
            if (CcfGid == 0)
            {
                // this stream should not be under CCF control
                // tell CCA to ignore this camear; CCL still needs to know this camera is active
                apCcaCfg->mpCam[CamId].mEnabled = 0U;
                continue;
            }

            lCcfEnabledCameras++;
            // check if this group id is already activated
            CcfGroup_t* Group = GetCcfGroup(CcfGid);
            if (Group != NULL)
            {
                // group already exists, add this stream to it
                Stream->mCcfStats.mCcfgGroup = Group;
                continue;
            }

            // activate new group
            Group = GetCcfGroup(0);
            if (Group == NULL)
            {
                // exceed maximum number of ccf groups
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Too many CCF groups\n");
                return CCL_ERR_TOO_MANY_CCF_GROUPS;
            }
            Group->mCcfGroupId = CcfGid;
            if (0 != ccl_sem_Init(&Group->mNewFrameEvent, 0))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to init semaphore\n");
                return CCL_ERR_GENERIC;
            }

            char ThreadName[20];
            if (snprintf(ThreadName, 20, "ccf_grp_%02" PRIu32, Group->mCcfGroupId) < 0)
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to format thread name.\n");
                return CCL_ERR_GENERIC;
            }

            if (0 != ccl_thread_Create(&Group->mProcessingThread, CcaProcessingThread, Group, ThreadName, CCL_TASK_STACK_SIZE_LARGE, CCL_THREAD_PRIORITY_LOW))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to start CCA processing thread.\n");
                return CCL_ERR_GENERIC;
            }

            Stream->mCcfStats.mCcfgGroup = Group;
        }
    }

    *apCcfEnabledCameras = lCcfEnabledCameras;

    return CCL_ERR_SUCCESS;
}


#if CCL_FEATURE_CONTEXT_MATCH
/**
 * @brief Finds ISP configuration for frame by searching stream's cfg history
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param[in] apHistBuf - streams cfg history
 * @param[in] apFrame - frame task with sensor configuration as reconstructed from data embedded in image
 * @param[out] apMatchedHistItem - matching cfg for this frame
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details if no match is found oldest cfg is used
 */
static ccl_Err_t ContextMatchM2M(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrame, ccl_HwCfg_t** apMatchedHistItem)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];
    *apMatchedHistItem = NULL;

    if (Stream->mHwCfgHistory.mNewIdx == Stream->mHwCfgHistory.mOldIdx)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "There is no active history item!!!\n");
        return CCL_ERR_BAD_STATE;
    }

    if (apFrame->mActSensCfg.mDataInvalid == false)
    {
        int8_t idx = Stream->mHwCfgHistory.mNewIdx - 1;
        if (idx < 0)
        {
            idx += SENS_CFG_HISTORY_BUF_SIZE;
        }
        int8_t StopIdx = Stream->mHwCfgHistory.mOldIdx - 1;
        if (StopIdx < 0)
        {
            StopIdx += SENS_CFG_HISTORY_BUF_SIZE;
        }
        while (idx != StopIdx)
        {
            if (0 != cca_MatchSensPrm(aCamId, &apFrame->mActSensCfg, &Stream->mHwCfgHistory.mItems[idx].mSensCfg))
            {
                *apMatchedHistItem = &Stream->mHwCfgHistory.mItems[idx];
                break;
            }

            idx--;
            if (idx < 0)
            {
                idx += SENS_CFG_HISTORY_BUF_SIZE;
            }
        }

        // if no item matched -> take oldest item
        if (*apMatchedHistItem == NULL)
        {
            // no match found
            *apMatchedHistItem = &Stream->mHwCfgHistory.mItems[Stream->mHwCfgHistory.mOldIdx];
            LogSensCfg(aHwId, aCamId, "EMB-", apFrame->FrameData.mMetaData.mFrameNo, (*apMatchedHistItem)->mBasedOnId, &apFrame->mActSensCfg);
        }
        else
        {
            // much found
            LogSensCfg(aHwId, aCamId, "EMB+", apFrame->FrameData.mMetaData.mFrameNo, (*apMatchedHistItem)->mBasedOnId, &apFrame->mActSensCfg);
        }
    }
    else
    {
        // no (valid) embedded data provided along with new image ->  take oldest item
        *apMatchedHistItem = &Stream->mHwCfgHistory.mItems[Stream->mHwCfgHistory.mOldIdx];
        LogSensCfg(aHwId, aCamId, "EMB!", apFrame->FrameData.mMetaData.mFrameNo, (*apMatchedHistItem)->mBasedOnId, &apFrame->mActSensCfg);
    }

    return 0;
}

/**
 * @brief Finds ISP configuration for frame by searching stream's cfg history
 *
 * @param[in] aHwId - ISP block the stream is running on
 * @param[in] aCamId - ISP pipe in the block stream is running on
 * @param[in] apHistBuf - streams cfg history
 * @param[in] aHdrDelay - delay in activation of new HDR cfg
 * @param[out] apMatchedHistItem - matching cfg for this frame
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details if no match is found oldest cfg is used
 */
static ccl_Err_t ContextMatchStr(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint8_t aHdrDelay, ccl_HwCfg_t** apMatchedHistItem)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];
    *apMatchedHistItem = NULL;

    // check we have history deep enough for the delay
    if (aHdrDelay > GetCfgHistorySize(aHwId, aCamId))
    {
        // not enough items in history
        *apMatchedHistItem = &Stream->mHwCfgHistory.mItems[Stream->mHwCfgHistory.mOldIdx];
        LogSensCfg(aHwId, aCamId, "HDR-", 0, (*apMatchedHistItem)->mBasedOnId, NULL);
    }
    else
    {
        // compute index of the delayed item
        int16_t idx = Stream->mHwCfgHistory.mNewIdx - aHdrDelay;
        if (idx < 0)
        {
            idx += SENS_CFG_HISTORY_BUF_SIZE;
        }
        *apMatchedHistItem = &Stream->mHwCfgHistory.mItems[idx];
        LogSensCfg(aHwId, aCamId, "HDR+", 0, (*apMatchedHistItem)->mBasedOnId, NULL);
    }

    return 0;
}

#endif

/**
 * @brief Event handling -> a frame was dropped during capture or processing.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @details Updates Last frame information for the stream allowing in some situations to run CCF without this stream.
 * Note stream must be locked before calling this function
 */
static void FrameDropped(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t* Stream    = &gCclState.Streams[aHwId][aCamId];
    ccl_FrameStats_t* Stats = &Stream->mCcfStats;

    if (Stats->mCcfgGroup == NULL || Stats->mCcfgGroup->mCcfGroupId == 0)
    {
        // there is no CCF processing over this stream, no need to store last frame info for it...
        return;
    }

    switch (Stats->mStatsState)
    {
        case CCL_STATS_STATE_NONE:
            // if there is no frame available for CCA, signal that its not coming soon (CCA decides if we process without or wait)
            Stats->mStatsState = CCL_STATS_STATE_DROPPED;
            // signal to CCA to check if processing can be done
            if (0 != ccl_sem_Give(&Stats->mCcfgGroup->mNewFrameEvent))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post CCA check event.\n");
            }
            break;

        case CCL_STATS_STATE_DROPPED:
            // we already signalled drop happen - no need to repeat
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "CCA -> previous drop still unprocessed. Possible skip of CCA pass on hwid: %d, camid: %d\n", aHwId, aCamId);
            break;

        case CCL_STATS_STATE_READY:
            // we have some frame for CCA already waiting...
            // we rather use older (but sill CCA-unprocessed) frame than none - so we do nothing here
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "CCA -> previous frame still unprocessed. Possible skip of CCA pass on hwid: %d, camid: %d\n", aHwId, aCamId);
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Unexpected state for last frame %d. hwid: %d, camid: %d\n", Stats->mStatsState, aHwId, aCamId);
            break;
    }
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

int32_t ccl_CcaInit()
{
    cca_Cfg_t lCcaCfg;
    uint32_t lCcfEnabledCameraCnt = 0U;
    (void)memset(&lCcaCfg, 0, sizeof(lCcaCfg));

    if (ISP_HWID_CNT > 1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCA library currently supports only single instance of ISP.\n");
        return -1;
    }

    if (0 != InitVars())
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to initialize CCL's CCA variables.\n");
        return -1;
    }

    // copy camera parameters as supplied to us via ccl_Init()
    // NOTE: Sensor specific parameters not available at this point, they will be
    // read during ccl_open(), and call to cca will be made with real sensor values
    // Now during ccl_Init() is sensor not powered and capture method (API) is not selected yet
    memcpy(&lCcaCfg.mpCam, &gCclState.mInitCfg.mpCams[ISP_HWID_0], sizeof(lCcaCfg.mpCam));
    // copy CCF parameters as supplied to us via ccl_Init()
    memcpy(&lCcaCfg.mCcfCfg, &gCclState.mInitCfg.mCcfCfg, sizeof(cca_CcfCfg_t));

    if (0 != GenerateCcfGroups(&lCcaCfg, &lCcfEnabledCameraCnt))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCF group creation fail.\n");
        return -1;
    }

    // at least one CCF enabled camera required to open CCA
    if (lCcfEnabledCameraCnt > 0U)
    {
        // *** open camera abstraction
        if (cca_Open(&lCcaCfg) != CCA_SUCCESS)
        {
            printf("Failed to open camera abstraction.\n");
            return -1;
        }
    }

    return 0;
} // InitCca()

/***********************************************************************************/

void ccl_CcaDeinit()
{
    int GroupIdx = 0;

    while ((GroupIdx < MAX_CCF_GROUP_CNT) && (gCcfGroups[GroupIdx].mCcfGroupId != 0))
    {
        // kill CCF thread
        if (0 != ccl_thread_Cancel(&gCcfGroups[GroupIdx].mProcessingThread))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to stop CCF thread for %d\n", gCcfGroups[GroupIdx].mCcfGroupId);
        }

        // variable cleanup
        if (0 != ccl_sem_Destroy(&gCcfGroups[GroupIdx].mNewFrameEvent))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to destroy CCF group's event semaphore\n");
        }
        gCcfGroups[GroupIdx].mCcfGroupId = 0;

        GroupIdx++;
    }

    // *** close camera abstraction
    if (cca_Close() != CCA_SUCCESS)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to close CCA\n");
    }

    // variable and primitives cleanup
    DeinitVars();
}

/***********************************************************************************/

#if CCL_FEATURE_CONTEXT_MATCH
void ccl_CcaCleanCfgHistory(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];
    memset(Stream->mHwCfgHistory.mItems, 0, sizeof(Stream->mHwCfgHistory.mItems));
    Stream->mHwCfgHistory.mOldIdx = 0;
    Stream->mHwCfgHistory.mNewIdx = 0;
}
#endif

/***********************************************************************************/

static int32_t SetDefaultCfg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    (void)aHwId;
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];

    if (ISP_HWID_CNT > 1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCA library currently supports only single instance of ISP.\n");
        return -1;
    }

#if CCL_FEATURE_CONTEXT_MATCH
    ccl_CcaCleanCfgHistory(aHwId, aCamId);

    // allocate new item for history of HW cfg
    ccl_HwCfg_t* DefHwConfig = GetNewHistoryItem(aHwId, aCamId);
#else
    ccl_HwCfg_t* DefHwConfig = &Stream->mHwCfg[Stream->mIspCfgIdxActive];
#endif
    DefHwConfig->mBasedOnId                 = SENS_LOG_FRAME_ID_INITIAL_CFG;
    Stream->mIspCfg[Stream->mIspCfgIdxActive].mCtxMeta.mFrameIdx = SENS_LOG_FRAME_ID_INITIAL_CFG;

    for (uint8_t i = 0; i < ISP_CFG_CNT_PER_STREAM; i++)
    {
        memcpy(&Stream->mIspCfg[i].mCtxHw, gCclState.mInitCfg.mpCams[aHwId][aCamId].mcpExtDef, sizeof(isp_CtxHw_t));
    }

    if (Stream->mCcfStats.mCcfgGroup == NULL || Stream->mCcfStats.mCcfgGroup->mCcfGroupId == 0)
    {
#if CCL_FEATURE_CONTEXT_MATCH
        // this stream have CCF disabled, use only default settings supplied by client APP
        memcpy(&DefHwConfig->mHdrCfg, &gCclState.mInitCfg.mpCams[aHwId][aCamId].mcpExtDef->mRegs.mHDR_MERGE, sizeof(isp_HDR_MERGE_t));
#endif

        // if CCF is not enabled we rely on sensor driver to put in default configuration
        // e.g. we never call Stream->mCapOps.mpCapSensorUpdate()
        memset(&DefHwConfig->mSensCfg, 0, sizeof(cca_SensPrm_t));
    }
    else
    {
        // extract initial settings from CCF library
        // NOTE: only partial overlay is extracted from CCA, each FrameTask is preloaded with APP defaults then upon new frame capture this CCA overlay is applied

        // At this point sensor is powered and we already communicate with it. This means that some sensor-specific parameters (e.g. calibration) can now be
        // updated by read from the sensor its self. We need to force CCA to recompute sensor's generic (CCA) parameters (first computed from defaults during ccl_init()).
        if (0 != cca_ConfigCam(aCamId, gCclState.mInitCfg.mpCams[aHwId][aCamId].mpParams, gCclState.mInitCfg.mpCams[aHwId][aCamId].mParamSize))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to update cca parameters. hwid: %d, camid: %d\n", aHwId, aCamId);
            return -1;
        }

        // point to new history item so the initial configuration will be stored there
        // Note: Only members valid for initial config are set
        cca_CcfIo_t lCcfIo;
        memset(&lCcfIo, 0, sizeof(cca_CcfIo_t));
        lCcfIo.mpSensPrmNew = &DefHwConfig->mSensCfg;
        lCcfIo.mpIspCtxOut  = &Stream->mIspCfg[Stream->mIspCfgIdxActive];

        /** cca_GetIniSet() allows extraction of default config for multiple streams,
         * here we want only single config, se we set all streams to NULL except the one we care for. */
        cca_CcfIoArr_t lCcfIoArr;
        memset(&lCcfIoArr, 0, sizeof(cca_CcfIoArr_t));
        lCcfIoArr.mppCcfIo[aCamId] = &lCcfIo;

        // *** get initial settings from CCA library ***
        if (cca_GetIniSet(&lCcfIoArr) != CCA_SUCCESS)
        {
            printf("Failed to get initial setting from camera abstraction.\n");
            return -1;
        }

        // allow client code to update initial values from CCA library
        ccl_UpdCtx_t lUpdCtx;
        lUpdCtx.mHwId        = aHwId;
        lUpdCtx.mCamId       = aCamId;
        lUpdCtx.mpNewCtx     = lCcfIo.mpIspCtxOut;
        lUpdCtx.mpNewSensPrm = lCcfIo.mpSensPrmNew;
        lUpdCtx.mFrameId     = 0U;
        UpdCtx(&lUpdCtx);


#if CCL_FEATURE_CONTEXT_MATCH
        // copy initial HDR cfg to first history item
        memcpy(&DefHwConfig->mHdrCfg, &lUpdCtx.mpNewCtx->mCtxHw.mRegs.mHDR_MERGE, sizeof(isp_HDR_MERGE_t));
#endif

        // *** enforce default configuration on sensor ***
        if (0 != Stream->mCapOps.mpCapSensorUpdate(aHwId, aCamId, false, &DefHwConfig->mSensCfg, &gCclState.mInitCfg.mpCams[aHwId][aCamId], DefHwConfig->mBasedOnId))
        {
            printf("Failed to send initial config to sensor hwid: %d, camid: %d\n", aHwId, aCamId);
            return -1;
        }
        LogSensCfg(aHwId, aCamId, "INIT", SENS_LOG_FRAME_ID_INITIAL_CFG, DefHwConfig->mBasedOnId, &DefHwConfig->mSensCfg);

        printf("Initial settings sent to sensor hwid: %d, camid: %d\n", aHwId, aCamId);
    }

#if CCL_FEATURE_CONTEXT_MATCH
    // consider new (default) cfg as ready for use
    Stream->mHwCfgHistory.mNewIdx = (Stream->mHwCfgHistory.mNewIdx + 1) % SENS_CFG_HISTORY_BUF_SIZE;
#endif
    return 0;
}

int32_t ccl_CcaSetDefaultCfg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    int32_t ret = SetDefaultCfg(aHwId, aCamId);

    return ret;
}

/***********************************************************************************/

void ccl_CcaUpdate(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrame)
{
    ccl_FrameStats_t* Stats = &gCclState.Streams[aHwId][aCamId].mCcfStats;

    // NOTE: this fcn must be called with stream's mutex locked

    if (Stats->mCcfgGroup == NULL || Stats->mCcfgGroup->mCcfGroupId == 0)
    {
        // there is no CCF processing over this stream, no need to store frames for it...
        ccl_SetFrameState(aHwId, aCamId, apFrame, CCL_FRAME_STATE_CCA_PROCESSED);
        return;
    }

    if (apFrame->mActSensCfg.mDataInvalid)
    {
        uint32_t FrameId = UINT32_MAX;
        if (apFrame->mInBuf != NULL)
        {
            FrameId = apFrame->mInBuf->mFrameNo;
        }

        // frame arrived but embedded data are invalid, we can't process CCF over this frame
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "Skipping frame %d with bad emb data. hwid: %d, camid: %d\n", FrameId, aHwId, aCamId);
        FrameDropped(aHwId, aCamId);
        ccl_SetFrameState(aHwId, aCamId, apFrame, CCL_FRAME_STATE_CCA_PROCESSED);
        return;
    }

    switch (Stats->mStatsState)
    {
        case CCL_STATS_STATE_NONE:
            // there is no frame scheduled for CCA -> store new frame
            Stats->mStatsState = CCL_STATS_STATE_READY;
            Stats->mpLastFrame = apFrame;
            // signal to CCA to check if processing can be done
            if (0 != ccl_sem_Give(&Stats->mCcfgGroup->mNewFrameEvent))
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to post CCA check event.\n");
            }
            break;

        case CCL_STATS_STATE_DROPPED:
            // there is already CCA state scheduled for CCA, replace with current frame
            Stats->mStatsState = CCL_STATS_STATE_READY;
            Stats->mpLastFrame = apFrame;
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "CCA -> previous drop still unprocessed. Possible skip of CCA pass on hwid: %d, camid: %d\n", aHwId, aCamId);
            break;

        case CCL_STATS_STATE_READY:
            // there is already CCA state scheduled for CCA, replace with current frame
            // remove older frame
            ccl_FreeFrameFromCca(aHwId, aCamId, Stats->mpLastFrame);
            // store current
            Stats->mpLastFrame = apFrame;
            ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "CCA -> previous frame still unprocessed. Possible skip of CCA pass on hwid: %d, camid: %d\n", aHwId, aCamId);
            break;

        default:
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Unexpected state for last frame %d. hwid: %d, camid: %d\n", Stats->mStatsState, aHwId, aCamId);
            break;
    }
}

/***********************************************************************************/

void ccl_CcaFrameDropped(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    // note this function can be called internally (with stream lock) as well as externally (no stream lock)
    // we need to make sure the stream is locked => we will double lock here (mutex recursion)

    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];

    if (!gCclState.mInitialized)
    {
        return;
    }

    if (0 != ccl_mutex_Lock(&Stream->mLock, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }

    FrameDropped(aHwId, aCamId);

    if (0 != ccl_mutex_Unlock(&Stream->mLock))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return;
    }
}

/***********************************************************************************/

ccl_Err_t ccl_CcaFindIspCfgM2M(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrame)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];

    if (ISP_HWID_CNT > 1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCA library currently supports only single instance of ISP.\n");
        return CCL_ERR_BAD_PARAM;
    }

    if (Stream->mCcfStats.mCcfgGroup != NULL)
    {

#if CCL_FEATURE_CONTEXT_MATCH
        ccl_HwCfg_t* apMatchedHistItem;
        if (0 != ContextMatchM2M(aHwId, aCamId, apFrame, &apMatchedHistItem))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to match context.\n");
            return CCL_ERR_GENERIC;
        }
#endif

        // copy matched context to the frame
        // NOTE: only partial ISP cfg is used, each FrameTask is preloaded with APP defaults, here we only apply partial ISP config overrides.
        ccl_CopyIspCfg(&apFrame->mIspCfg->mIspCtx.mCtxHw, &Stream->mIspCfg[Stream->mIspCfgIdxActive].mCtxHw);
#if CCL_FEATURE_CONTEXT_MATCH
        memcpy(&apFrame->mIspCfg->mIspCtx.mCtxHw.mRegs.mHDR_MERGE, &apMatchedHistItem->mHdrCfg, sizeof(isp_HDR_MERGE_t));
        memcpy(&apFrame->mExpSensCfg, &apMatchedHistItem->mSensCfg, sizeof(cca_SensPrm_t));
#endif
        LogSensCfg(aHwId, aCamId, "ISP", apFrame->FrameData.mMetaData.mFrameNo, Stream->mIspCfg[Stream->mIspCfgIdxActive].mCtxMeta.mFrameIdx, NULL);
    } // if ccf disabled for this stream

#if defined(IMX95) && defined(A0)
    FixIspRegs_RevA(&apFrame->mIspCfg->mIspCtx.mCtxHw);
#endif

    // update frame index in ISP structure (may not be necessary)
    apFrame->mIspCfg->mIspCtx.mCtxMeta.mFrameIdx = apFrame->FrameData.mMetaData.mFrameNo;

    return CCL_ERR_SUCCESS;
}

/***********************************************************************************/

ccl_Err_t ccl_CcaFindIspCfgStr(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrame, uint8_t aHdrDelay)
{
    ccl_Stream_t* Stream = &gCclState.Streams[aHwId][aCamId];

    if (ISP_HWID_CNT > 1)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCA library currently supports only single instance of ISP.\n");
        return CCL_ERR_BAD_PARAM;
    }

    if (Stream->mCcfStats.mCcfgGroup != NULL)
    {
#if CCL_FEATURE_CONTEXT_MATCH
        ccl_HwCfg_t* apMatchedHistItem;
        if (0 != ContextMatchStr(aHwId, aCamId, aHdrDelay, &apMatchedHistItem))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Fail to match context.\n");
            return CCL_ERR_GENERIC;
        }
#endif

        // NOTE: only partial ISP cfg is used, each FrameTask is preloaded with APP defaults, here we only apply partial ISP config overrides.
        ccl_CopyIspCfg(&apFrame->mIspCfg->mIspCtx.mCtxHw, &Stream->mIspCfg[Stream->mIspCfgIdxActive].mCtxHw);
#if CCL_FEATURE_CONTEXT_MATCH
        memcpy(&apFrame->mIspCfg->mIspCtx.mCtxHw.mRegs.mHDR_MERGE, &apMatchedHistItem->mHdrCfg, sizeof(isp_HDR_MERGE_t));
        memcpy(&apFrame->mExpSensCfg, &apMatchedHistItem->mSensCfg, sizeof(cca_SensPrm_t));
#endif
        LogSensCfg(aHwId, aCamId, "ISP", apFrame->FrameData.mMetaData.mFrameNo, Stream->mIspCfg[Stream->mIspCfgIdxActive].mCtxMeta.mFrameIdx, NULL);
    } // if ccf disabled for this stream

#if defined(IMX95) && defined(A0)
    FixIspRegs_RevA(&apFrame->mIspCfg->mIspCtx.mCtxHw);
#endif

    return CCL_ERR_SUCCESS;
}

/***********************************************************************************/

float ccl_GetCcaStats(uint32_t GroupId)
{
    if (GroupId == 0)
    {
        return gCclState.mStats.mCcaFpsAll;
    }
    else
    {
        for (uint32_t CcaGroupIdx = 0; CcaGroupIdx < MAX_CCF_GROUP_CNT; CcaGroupIdx++)
        {
            if (gCcfGroups[CcaGroupIdx].mCcfGroupId == GroupId)
            {
                return gCclState.mStats.mCca[CcaGroupIdx].mFps;
            }
        }
        return 0;
    }
}

/***********************************************************************************/
