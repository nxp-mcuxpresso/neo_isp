/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cca.c
 * @brief Implements generic camera abstraction behavior and APIs.
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <string.h>
#include <assert.h>
#include "cca.h"
#include "ilog.h"
#include "cca_internal.h"

/** @addtogroup cca_core_func Core functionality
 * Implementation of the public API and other CCF and sensor independent behavior.
 *
 *  @{
 */

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

/*==============================================================================
*                              LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/

DDR_NOINIT cca_Ctx_t gCcaCtx;
static uint32_t gCcaOpened = 0U; /**< 1 if opened already, 0 otherwise*/

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/**
 * @brief Check if embedded bypass is enabled
 *
 * @return true if embedded bypass delay is greater than 0, false otherwise
 */
static inline bool IsEmbeddedBypassEnabled(uint32_t aChnlIdx)
{
    assert(aChnlIdx < ISPDRV_CAM_PIPE_ID_CNT);
    return (gCcaCtx.mEmbdBypassItems[aChnlIdx].mEmbdBypassDelay > 0U);
}

/******************************************************************************/

/**
 * @brief Set embedded bypass delay value
 *
 * @param aChnlIdx[in] Channel index
 * @param aEmbdBypass[in] Embedded bypass delay value to set
 */
static inline void SetEmbeddedBypassDelay(uint32_t aChnlIdx, uint32_t aEmbdBypass)
{
    assert(aChnlIdx < ISPDRV_CAM_PIPE_ID_CNT);
    ccl_EmbdBypassHist_t* lpHist = &gCcaCtx.mEmbdBypassItems[aChnlIdx];

    if (aEmbdBypass > CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE)
    {
        lpHist->mEmbdBypassDelay = 0;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Embedded control history buffer size %d exceeds maximum %d\n", aEmbdBypass, CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE);
        return;
    }

    lpHist->mEmbdBypassDelay = aEmbdBypass;

    return;
}

/******************************************************************************/

/**
 * @brief Copy sensor configuration to embedded bypass history item
 *
 * @param[out] apDest Destination sensor configuration structure
 * @param[in] apSrc Source sensor configuration structure
 * @param[in] aDelay Delay value to add to frame count
 */
static inline void CopyEmbdHistoryItem(cca_SensPrm_t* apDest, const cca_SensPrm_t* apSrc, uint32_t aDelay)
{
    (void)memcpy(apDest, apSrc, sizeof(cca_SensPrm_t));

    // Adjust frame count based on delay
    if (apDest->mFrameCount > 0)
    {
        apDest->mFrameCount += aDelay;
    }
}

/******************************************************************************/

/**
 * @brief Initialize embedded bypass history buffer
 *
 * @param aEmbdBypass[in] Embedded bypass delay value to initialize the history buffer with
 */
static void InitEmbdBypassHistory(uint32_t aChnlIdx, const cca_SensPrm_t* apSensCfg)
{
    assert(aChnlIdx < ISPDRV_CAM_PIPE_ID_CNT);
    ccl_EmbdBypassHist_t* lpHist = &gCcaCtx.mEmbdBypassItems[aChnlIdx];

    memset(lpHist->mItems, 0, sizeof(lpHist->mItems));

    // Fill all history items with the default configuration
    for (int i = 0; i < CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE; i++)
    {
        CopyEmbdHistoryItem(&lpHist->mItems[i].mSensCfg, apSensCfg, lpHist->mEmbdBypassDelay);
        lpHist->mItems[i].mValid = true;
    }

    // Initialize write pointer and count
    lpHist->mWp = lpHist->mEmbdBypassDelay % CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE;
}

/******************************************************************************/

/**
 * @brief Add new item to embedded control history
 *
 * @param[in] aChnlIdx index of coresponding sensor channel
 * @param[in] apSensCfg - sensor configuration to store
 */
static void PushEmbdBypassItemToHistory(uint32_t aChnlIdx, const cca_SensPrm_t* apSensCfg)
{
    if (!IsEmbeddedBypassEnabled(aChnlIdx))
    {
        return;
    }

    ccl_EmbdBypassHist_t* lpHist = &gCcaCtx.mEmbdBypassItems[aChnlIdx];
    ccl_EmbdBypassHistItem_t* lpWriteItem = &lpHist->mItems[lpHist->mWp];

    if (!lpWriteItem->mValid) // invalid item -> run init
    {
        InitEmbdBypassHistory(aChnlIdx, apSensCfg);
    }

    CopyEmbdHistoryItem(&lpWriteItem->mSensCfg, apSensCfg, lpHist->mEmbdBypassDelay);

    // Update write pointer
    lpHist->mWp = (lpHist->mWp + 1) % CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE;
}

/******************************************************************************/

/**
 * @brief Retrieve sensor configuration from embedded control history
 *
 * @param[in] aChnlIdx index of coresponding sensor channel
 * @param[out] apSensCfg - sensor configuration to store
 *
 * @return true on success, false if history doesn't contain enough items or item is invalid
 */
static bool GetEmbdBypassItemFromHistory(uint32_t aChnlIdx, cca_SensPrm_t* apSensCfg)
{
    if (!IsEmbeddedBypassEnabled(aChnlIdx))
    {
        return false;
    }

    ccl_EmbdBypassHist_t* lpHist = &gCcaCtx.mEmbdBypassItems[aChnlIdx];

    // Calculate index with delay
    uint32_t lIdx;
    if (lpHist->mEmbdBypassDelay > lpHist->mWp)
    {
        lIdx = lpHist->mWp + CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE - lpHist->mEmbdBypassDelay;
    }
    else
    {
        lIdx = lpHist->mWp - lpHist->mEmbdBypassDelay;
    }

    // Check validity
    assert(lIdx < CCA_FEATURE_EMBD_BYPASS_MAX_BUFF_SIZE);
    if (!lpHist->mItems[lIdx].mValid)
    {
        return false;
    }

    // Copy data
    memcpy(apSensCfg, &lpHist->mItems[lIdx].mSensCfg, sizeof(cca_SensPrm_t));

    return true;
}

static inline cca_Result_t CheckCcfOps(cca_CcfOps_t* apCcfOps)
{
    cca_Result_t lRet = CCA_SUCCESS;
    if (apCcfOps->mOpen == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF has no Open() operation defined.\n");
        lRet = CCA_ERROR;
    }
    if (apCcfOps->mGetInit == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF has no GenInit() operation defined.\n");
        lRet = CCA_ERROR;
    }
    if (apCcfOps->mProcess == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF has no Process() operation defined.\n");
        lRet = CCA_ERROR;
    }
    if (apCcfOps->mClose == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF has no Close() operation defined.\n");
        lRet = CCA_ERROR;
    }

    return lRet;
} // CheckCcfOps()

/******************************************************************************/

static inline cca_Result_t CheckCamOps(cca_CamOps_t* apCamOps, uint32_t aCamPipeId)
{
    cca_Result_t lRet = CCA_SUCCESS;
    if (apCamOps->mOpen == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no Open() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mConfig == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no Config() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mActuate == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no Actuate() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mParseEmbd == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no ParseEmbd() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mInterpretEmbd == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no InterpretEmbd() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mGetI2cTab == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no mGetI2cTab() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mMatchPrm == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no mMatchPrm() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    if (apCamOps->mClose == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CamID %u active but no Close() operation defined.\n", aCamPipeId);
        lRet = CCA_ERROR;
    }
    return lRet;
} // CheckCamOps()

/******************************************************************************/

static inline cca_Result_t OpenCca(cca_Cfg_t* apcCfg)
{
    cca_Result_t lRet;
    // init context to all 0
    (void)memset(&gCcaCtx, 0, sizeof(cca_Ctx_t));
    // remember initial configuration
    gCcaCtx.mCfg = *apcCfg;

    // check control functions ops
    lRet = CheckCcfOps(&(apcCfg->mCcfCfg.mOps));
    for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
    {
        cca_CamInit_t* lpCamInit = &(gCcaCtx.mCfg.mpCam[lIter]);
        // inactive camera init structs must be ignored
        if (lpCamInit->mEnabled == 1U)
        {
            cca_CamCtx_t* lpCamCtx = &gCcaCtx.mpCamCtx[lIter];
            // check camera ops
            if( CCA_SUCCESS != CheckCamOps(&(lpCamInit->mCamOps), lIter))
            {
                lRet = CCA_ERROR;
            }
            else
            {
                lpCamCtx->mCcfChnlIdx = gCcaCtx.mActiveCamCnt; // assign ccf index
                gCcaCtx.mActiveCamCnt++; // increment active camera cnt
            }

            // initialize context accumulator
            (void)memcpy(&lpCamCtx->mIspCtxAcc.mCtxHw, lpCamInit->mcpExtDef, sizeof(isp_CtxHw_t));
            // disable debugging by default
            lpCamCtx->mIspCtxAcc.mCtxHw.mRegs.mIDBG1.mLINE_NUM.mLINE_NUM = 0x1FFFF;
            lpCamCtx->mIspCtxAcc.mCtxHw.mRegs.mIDBG2.mLINE_NUM.mLINE_NUM = 0x1FFFF;

            SetEmbeddedBypassDelay(lIter, lpCamInit->mEmbdBypass);
        } // if camera enabled
    } // for all cameras
    return lRet;
} // OpenCca()

/******************************************************************************/

static inline cca_Result_t OpenCcf(void)
{
    cca_Result_t lRet;
    // open control functions
    if (gCcaCtx.mCfg.mCcfCfg.mOps.mOpen() == CCA_SUCCESS)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_NFO, "CCF opened successfully.\n");
        lRet = CCA_SUCCESS;
    }
    else
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to Open CCF.\n");
        lRet = CCA_ERROR;
    }

    return lRet;
} // OpenCcf()

/******************************************************************************/

static inline cca_Result_t OpenCams(void)
{
    cca_Result_t lRet = CCA_SUCCESS;
    for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
    {
        cca_CamInit_t* lpCamInit = &(gCcaCtx.mCfg.mpCam[lIter]);
        if (lpCamInit->mEnabled == 1U)
        {
            if(CCA_SUCCESS != lpCamInit->mCamOps.mOpen(lIter, lpCamInit->mpParams, lpCamInit->mParamSize))
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to open camera translation layer at pipe ID %u.\n", lIter);
                lRet = CCA_ERROR;
                break;
            } // if failed to open
        } // if enabled
        // inactive camera init structs must be ignored
    } // for all cameras

    return lRet;
} // OpenCams()

/******************************************************************************/

static inline cca_Result_t Measure(cca_CcfIoArr_t* apCcfIoArr)
{
    cca_Result_t lRet = CCA_SUCCESS;

    for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
    {
        cca_CcfIo_t* lpCcfIo = apCcfIoArr->mppCcfIo[lIter];
        if (lpCcfIo == NULL)
        {
            // ISP CTX == NULL identifies inactive channel
            gCcaCtx.mpCamCtx[lIter].mpCcfIo = NULL;
            continue; // skip inactive channels
        }

        // check if CAM ID enabled
        if (gCcaCtx.mCfg.mpCam[lIter].mEnabled != 1U)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Attempted to process disabled CID %u.\n", lIter);
            lRet = CCA_ERROR;
            break;
        } // if channel not active

        // if channel active
        if (lpCcfIo->mpIspCtxIn != NULL)
        {
            cca_SensPrm_t* const lpSensPrm = &(gCcaCtx.mpCamCtx[lIter].mSensPrmCcf);

            if (IsEmbeddedBypassEnabled(lIter))
            {
                // Use embedded control history instead of embedded data
                if (!GetEmbdBypassItemFromHistory(lIter, lpSensPrm))
                {
                    // If history retrieval fails, mark data as invalid
                    gCcaCtx.mpCamCtx[lIter].mEmbdValid = 0U;
                    ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Failed to get embedded control history, marking as invalid. chnl: %d\n", lIter);
                    lRet = CCA_ERROR;
                }
                else
                {
                    gCcaCtx.mpCamCtx[lIter].mEmbdValid = 1U;
                }
            }
            else if ((lpCcfIo->mpEmbdPrm != NULL) && (lpCcfIo->mpEmbdPrm->mDataInvalid == false))
            {
                // copy lens position, in case sensor support does not touch it
                lpSensPrm->mLensPos = lpCcfIo->mpEmbdPrm->mLensPos;
                // find out current sensor configuration

                if (gCcaCtx.mCfg.mpCam[lIter].mCamOps.mInterpretEmbd(lIter, lpCcfIo->mpEmbdPrm, lpSensPrm) != CCA_SUCCESS)
                {
                    ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to interpret embedded data for CID %u.\n", lIter);
                    gCcaCtx.mpCamCtx[lIter].mEmbdValid = 0U;
                    lRet                               = CCA_ERROR;
                }
                else
                {
                    gCcaCtx.mpCamCtx[lIter].mEmbdValid = 1U;
                }

            } // if embedded data provided
            else
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "No embedded data provided for CID %u.\n", lIter);
                gCcaCtx.mpCamCtx[lIter].mEmbdValid = 0U;
            } // else if embedded data provided

            // remember the IO data
            gCcaCtx.mpCamCtx[lIter].mpCcfIo = lpCcfIo;
        }
        else
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "NULL context in CID %u.\n", lIter);
            lRet = CCA_ERROR;
        }
        if (lRet != CCA_SUCCESS)
        {
            break;
        }
    } // for all CIDs

    return lRet;
} // Measure()

/******************************************************************************/

static inline cca_Result_t Actuate(void)
{
    cca_Result_t lRet = CCA_SUCCESS;
    for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
    {
        if(gCcaCtx.mpCamCtx[lIter].mpCcfIo != NULL)
        {
            // check output struct provided
            if (gCcaCtx.mpCamCtx[lIter].mpCcfIo->mpSensPrmNew == NULL)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Sensor parameter structure is NULL (CID %u).\n", lIter);
                continue;
            } // if output struct not provided

            cca_SensPrm_t* const acpSensPrmCcf = gCcaCtx.mpCamCtx[lIter].mpCcfIo->mpSensPrmNew;

            if (IsEmbeddedBypassEnabled(lIter))
            {
                // Store sensor configuration to history
                PushEmbdBypassItemToHistory(lIter, acpSensPrmCcf);
            }

            if (gCcaCtx.mCfg.mpCam[lIter].mCamOps.mActuate(lIter,
                                                           gCcaCtx.mpCamCtx[lIter].mpCcfIo->mpSensPrmNew,
                                                           acpSensPrmCcf)
                != CCA_SUCCESS)
            {
                lRet = CCA_ERROR;
                break;
            } // if actuate failed
        }
    } // for all CIDs
    return lRet;
} // Actuate()

/******************************************************************************/

static cca_Result_t CheckGetIniSetCcf(void)
{
    cca_Result_t lRet;

    // only if initial config not fetched yet
    if(gCcaCtx.mInitFetched == 0U)
    {
        // for all enabled streams: fetch initial configuration from CCF
        cca_CcfIo_t lpCcfIoArr[ISPDRV_CAM_PIPE_ID_CNT];
        // setup output pointer to be filled by CCF GetInit() call
        for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
        {
            // check if disabled
            if(gCcaCtx.mCfg.mpCam[lIter].mEnabled == 0U)
            {
                gCcaCtx.mpCamCtx[lIter].mpCcfIo = NULL;
                continue; // skip disabled camera stream
            }
            (void)memset(&lpCcfIoArr[lIter], 0, sizeof(cca_CcfIo_t));
            lpCcfIoArr[lIter].mpIspCtxOut   = &gCcaCtx.mpCamCtx[lIter].mIspCtxInit; // output ISP ctx
            lpCcfIoArr[lIter].mpSensPrmNew  = &gCcaCtx.mpCamCtx[lIter].mSensPrmInit; // output sensor params
            gCcaCtx.mpCamCtx[lIter].mpCcfIo = &lpCcfIoArr[lIter];
        }

        // get CCF init
        if (gCcaCtx.mCfg.mCcfCfg.mOps.mGetInit() != CCA_SUCCESS)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to get initial settings from CCF.\n");
            lRet = CCA_ERROR;
        } // if processing failed
        else
        {
            if (Actuate() != CCA_SUCCESS)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to actuate initial sensor settings.\n");
                lRet = CCA_ERROR;
            }
            else
            {
                gCcaCtx.mInitFetched = 1U; // remember settings are fetched now

                ILOG(ILOG_MOD_CCA, ILOG_LEV_NFO, "Succesfully fetched initial settings from CCF.\n");
                lRet = CCA_SUCCESS;
            }
        } // else from if GetInit() failed
    } // if init settings not fetched yet
    else
    {
        lRet = CCA_SUCCESS;
    }

    return lRet;
} // CheckGetIniSetCcf()

/******************************************************************************/

static void OutputIniSet(cca_CcfIoArr_t* apCcfIoArr)
{
    // copy initial settings to requested outputs
    for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
    {
        cca_CcfIo_t* lpCcfIo = apCcfIoArr->mppCcfIo[lIter];
        if(lpCcfIo == NULL)
        {
            continue;
        } // if no settings requested -> skip

        if(gCcaCtx.mCfg.mpCam[lIter].mEnabled == 0U)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Requested initial settings for disabled stream (CID %u).\n", lIter);
            continue;
        } // if not enabled

        if (lpCcfIo->mpSensPrmNew == NULL)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Sensor parameter structure is NULL (CID %u).\n", lIter);
            continue;
        } // if output struct not provided -> skip
        else
        {
            // output sensor configuration
            (void)memcpy(lpCcfIo->mpSensPrmNew, &gCcaCtx.mpCamCtx[lIter].mSensPrmInit, sizeof(cca_SensPrm_t));
        }

        if (lpCcfIo->mpIspCtxOut == NULL)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Isp context structure is NULL (CID %u).\n", lIter);
            continue;
        } // if output struct not provided -> skip
        else
        {
            // output ISP configuration
            isp_CtxHw_t* lpCtxHwDst = &lpCcfIo->mpIspCtxOut->mCtxHw;
            isp_CtxHw_t* lpCtxHwSrc = &gCcaCtx.mpCamCtx[lIter].mIspCtxInit.mCtxHw;
            // copy only relevant config (e.g. skip buffer addresses)
            cca_CopyIspCfg(lpCtxHwDst, lpCtxHwSrc);


        } // else from if output struct not provided
    } // for all CIDs
} // OutputIniSet()

/******************************************************************************/

static inline cca_Result_t ProcCcf(cca_CcfIoArr_t* apCcfIoArr)
{
    cca_Result_t lRet;

    // Measure step (parse inputs, remember output structures inside corresponding camera context)
    if (Measure(apCcfIoArr) != CCA_SUCCESS)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF IO array is NULL.\n");
        lRet = CCA_ERROR;
    } // if Measure() step failed
    else
    {
        // CCF processing step
        if (gCcaCtx.mCfg.mCcfCfg.mOps.mProcess() != CCA_SUCCESS)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to execute control fuctions processing.\n");
            lRet = CCA_ERROR;
        } // if processing failed
        else
        {
            if (Actuate() != CCA_SUCCESS)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to actuate sensor settings.\n");
                lRet = CCA_ERROR;
            }
            else
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_NFO, "Control functions processing successfully finished.\n");
                lRet = CCA_SUCCESS;
            }
        } // else from if processing failed
    } // else from if Measure() step failed

    return lRet;
} // ProcCcf()

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/
cca_Result_t cca_Open(cca_Cfg_t* apcCfg)
{
    cca_Result_t lRet;

    if (apcCfg != NULL)
    {
        ///< TODO: lock
        if (gCcaOpened != 1U)
        {
            // init camera abstraction internals
            lRet = OpenCca(apcCfg);
        } // if not initialized yet
        else
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Initialized already.\n");
            lRet = CCA_ERROR;
        } // else from if not initialized yet

        if(gCcaCtx.mActiveCamCnt == 0U)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Number of CCF active cameras is 0.\n");
        }

        // if all ok so far and at least one active camera
        if((lRet == CCA_SUCCESS) && (gCcaCtx.mActiveCamCnt > 0U))
        {
            // init control functions
            if (OpenCcf() == CCA_SUCCESS)
            {
                if(OpenCams() == CCA_SUCCESS)
                {
                    gCcaOpened = 1U;
                    lRet            = CCA_SUCCESS;
                    ILOG(ILOG_MOD_CCA, ILOG_LEV_NFO, "CCA opened successfully.\n");
                }
                else
                {
                    ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to initialize camera translation layers.\n");
                    lRet = CCA_ERROR;
                } // else from if OpenCcf succeeded
            } // if OpenCcf succeeded
            else
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to initialize control functions.\n");
                lRet = CCA_ERROR;
            } // else from if OpenCcf succeeded
        } // if all ok so far
        ///< TODO: unlock
    } // if not NULL
    else
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Config structure is NULL.\n");
        lRet = CCA_ERROR;
    } // else from if not NULL

    return lRet;
} // cca_Open()


/******************************************************************************/

cca_Result_t cca_ConfigCam(uint32_t aChnlIdx, const void* const acpcData, uint32_t aDataSzBytes)
{
    cca_Result_t lRet;

    if(gCcaOpened == 0U)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCA not initialized yet.\n");
        lRet = CCA_ERROR;
    }
    else if(aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera stream index is out of range (%u >= %u).\n", aChnlIdx, ISPDRV_CAM_PIPE_ID_CNT);
        lRet = CCA_ERROR;
    }
    else if(gCcaCtx.mCfg.mpCam[aChnlIdx].mEnabled != 1U)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera at index [%u] is not enabled.\n", aChnlIdx);
        lRet = CCA_ERROR;
    }
    else if(acpcData == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Data pointer is NULL.\n");
        lRet = CCA_ERROR;
    }
    else
    {
        lRet = gCcaCtx.mCfg.mpCam[aChnlIdx].mCamOps.mConfig(aChnlIdx, acpcData, aDataSzBytes);
    }
    return lRet;
} // cca_ConfigCam()

/******************************************************************************/

cca_Result_t cca_GetIniSet(cca_CcfIoArr_t* apCcfIoArr)
{
    cca_Result_t lRet;
    if (apCcfIoArr == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF IO array is NULL.\n");
        lRet = CCA_ERROR;
    } // if input is NULL
    else
    {
        /// /TODO: lock
        if (gCcaOpened == 1U)
        {
            lRet = CheckGetIniSetCcf();

            if(lRet == CCA_SUCCESS)
            {
                OutputIniSet(apCcfIoArr);
            } // if all ok so far
        } // if opened already
        else
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera abstraction not initialized.\n");
            lRet = CCA_ERROR;
        } // else from if opened already
        /// /TODO: unlock
    } // else from if input is NULL

#ifdef ISIM
    // only for ISIM
    if (lRet == CCA_SUCCESS)
    {
        for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
        {
            cca_CcfIo_t* lpCcfIo = apCcfIoArr->mppCcfIo[lIter];
            isp_Ctx_t* lpCtxOut;
            if ((lpCcfIo == NULL) || (lpCcfIo->mpIspCtxIn == NULL))
            {
                continue;
            }

            lpCtxOut = (lpCcfIo->mpIspCtxOut != NULL)?lpCcfIo->mpIspCtxOut:lpCcfIo->mpIspCtxIn;
            ///< NOTE: pre and postskips forced skip set to 0 now
            lpCtxOut->mCtxHw.mRegs.mPIPE_CONF.mSKIP_CTRL0.mPRESKIP  = 0U;
            lpCtxOut->mCtxHw.mRegs.mPIPE_CONF.mSKIP_CTRL0.mPOSTSKIP = 0U;
        } // for all CIDs
    } // if all ok so far
#endif
    return lRet;
} // cca_GetIniSet()

/******************************************************************************/

cca_Result_t cca_ProcCcf(cca_CcfIoArr_t* apCcfIoArr)
{
    cca_Result_t lRet;

    if (apCcfIoArr == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF IO array is NULL.\n");
        lRet = CCA_ERROR;
    } // if input is NULL
    else
    {
        /// /TODO: lock
        if (gCcaOpened == 1U)
        {
            lRet = ProcCcf(apCcfIoArr);
        } // if opened already
        else
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera abstraction not initialized.\n");
            lRet = CCA_ERROR;
        } // else from if opened already
    } // else from if input is NULL

#ifdef ISIM
    // only for ISIM
    if (lRet == CCA_SUCCESS)
    {
        for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
        {
            cca_CcfIo_t* lpCcfIo = apCcfIoArr->mppCcfIo[lIter];
            isp_Ctx_t* lpOutCtx;
            if ((lpCcfIo == NULL) || (lpCcfIo->mpIspCtxIn == NULL))
            {
                continue;
            }

            lpOutCtx = (lpCcfIo->mpIspCtxOut != NULL)?lpCcfIo->mpIspCtxOut:lpCcfIo->mpIspCtxOut;

            ///< NOTE: pre and postskips forced skip set to 0 now
            lpOutCtx->mCtxHw.mRegs.mPIPE_CONF.mSKIP_CTRL0.mPRESKIP  = 0U;
            lpOutCtx->mCtxHw.mRegs.mPIPE_CONF.mSKIP_CTRL0.mPOSTSKIP = 0U;
        } // for all CIDs
    } // if all ok so far
#endif

    /// /TODO: unlock
    return lRet;
} // cca_ProcCcf()

/******************************************************************************/

cca_Result_t cca_Embd2Prm(uint32_t aChnlIdx, const cca_EmbdData_t* const acpcEmbd, cca_SensPrm_t* const acpSensPrmEmbd)
{
    cca_Result_t lRet;

    if(gCcaOpened == 0U)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCA not initialized yet.\n");
        lRet = CCA_ERROR;
    }
    else if(aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera stream index is out of range (%u >= %u).\n", aChnlIdx, ISPDRV_CAM_PIPE_ID_CNT);
        lRet = CCA_ERROR;
    }
    else if(gCcaCtx.mCfg.mpCam[aChnlIdx].mEnabled != 1U)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera at index [%u] is not enabled.\n", aChnlIdx);
        lRet = CCA_ERROR;
    }
    else
    {
        if (gCcaCtx.mCfg.mpCam[aChnlIdx].mCamOps.mParseEmbd != NULL
            && gCcaCtx.mCfg.mpCam[aChnlIdx].mCamOps.mParseEmbd(aChnlIdx, acpcEmbd, acpSensPrmEmbd) == CCA_SUCCESS)
        {
            lRet = CCA_SUCCESS;
        }
        else
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to parse embedded data for CID %u.\n", aChnlIdx);
            lRet = CCA_ERROR;
        }
    }
    return lRet;
} // cca_Embd2Prm();

/******************************************************************************/

uint32_t cca_MatchSensPrm(uint32_t aChnlIdx, const cca_SensPrm_t* const acpcPrmA, const cca_SensPrm_t* const acpcPrmB)
{
    uint32_t lRet;

    if(gCcaOpened == 0U)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCA not initialized yet.\n");
        lRet = 0U;
    }
    else if(aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera stream index is out of range (%u >= %u).\n", aChnlIdx, ISPDRV_CAM_PIPE_ID_CNT);
        lRet = 0U;
    }
    else if(gCcaCtx.mCfg.mpCam[aChnlIdx].mEnabled != 1U)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Camera at index [%u] is not enabled.\n", aChnlIdx);
        lRet = 0U;
    }
    else
    {
        lRet = gCcaCtx.mCfg.mpCam[aChnlIdx].mCamOps.mMatchPrm(acpcPrmA, acpcPrmB);
    }
    return lRet;
} // cca_MatchSensPrm();

/******************************************************************************/

cca_Result_t cca_GetI2cTab(cca_I2cTab_t* const acpI2cTab, const cca_SensPrm_t* apcSensPrmNew, const cca_SensPrm_t* apcSensPrmPrv, uint32_t aChnlIdx)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if(acpI2cTab == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "I2cTable is NULL\n");
        lRet = CCA_ERROR;
    } // if I2C table is NULL
    else
    {
        if (acpI2cTab->mSz < sizeof(cgd_Cmd_t))
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "I2cTable size is < single CGD command.\n");
            lRet = CCA_ERROR;
        }
    } // else from if I2C table is NULL
    if(apcSensPrmNew == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "New sensor parameter structure is NULL.\n");
        lRet = CCA_ERROR;
    } // if new sensor prm NULL
    if (aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Invalid channel index %u\n", aChnlIdx);
        lRet = CCA_ERROR;
    }
    if(lRet == CCA_SUCCESS)
    {
        if (gCcaCtx.mCfg.mpCam[aChnlIdx].mCamOps.mGetI2cTab(aChnlIdx, acpI2cTab, apcSensPrmNew, apcSensPrmPrv) != CCA_SUCCESS)
        {
            lRet = CCA_ERROR;
        }
    } // if all ok so far

    return lRet;
} // cca_GetI2cTab()

/******************************************************************************/

static inline cca_Result_t CloseCams(void)
{
    cca_Result_t lRet = CCA_SUCCESS;
    for (uint32_t lIter = 0U; lIter < ((uint32_t)ISPDRV_CAM_PIPE_ID_CNT); lIter++)
    {
        cca_CamInit_t* lpCamInit = &(gCcaCtx.mCfg.mpCam[lIter]);
        if (lpCamInit->mEnabled == 1U)
        {
            if(CCA_SUCCESS != lpCamInit->mCamOps.mClose(lIter))
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to close camera translation layer at pipe ID %u.\n", lIter);
                lRet = CCA_ERROR;
                break;
            } // if failed to open
        } // if enabled
        // inactive camera init structs must be ignored
    } // for all cameras

    return lRet;
} // CloseCams

/******************************************************************************/

cca_Result_t cca_Close(void)
{
    cca_Result_t lRet;

    ///< TODO: lock
    if (gCcaOpened == 1U)
    {
        // cameras deinitialization
        lRet = CloseCams();

        // CCF deinitialization step
        if (gCcaCtx.mCfg.mCcfCfg.mOps.mClose() != CCA_SUCCESS)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to close control fuctions.\n");
            lRet = CCA_ERROR;
        } // if processing failed

        if(lRet == CCA_SUCCESS)
        {
            gCcaOpened = 0U;
            ILOG(ILOG_MOD_CCA, ILOG_LEV_NFO, "Control functions successfully closed.\n");
        }
    } // if opened
    else
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Camera abstraction is closed already.\n");
        lRet = CCA_SUCCESS;
    } // else from if opened

    ///< TODO: un-lock
    return lRet;
} // cca_Close()

/*==============================================================================
*                           CCA INTERNAL FUNCTIONS
==============================================================================*/

cca_Result_t cca_AddI2cWrite(cca_I2cTab_t* const acpI2cTab, const cca_I2cCmd_t* apcCmd)
{
    assert(acpI2cTab->mUsedBytes <= acpI2cTab->mSz);

    cca_Result_t lRet;
    uint32_t lBytesNeeded;

    // Validate that mDevId fits in uint8_t range
    if (apcCmd->mDevId > 0xFFU)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Device ID %u exceeds valid range (0-255).\n", apcCmd->mDevId);
        return CCA_ERROR;
    }

    // bytes needed = number of commands needed * sizeof(command)
    // first command: 1byte transfer size, 2 address bytes and 1 payload byte
    // continued commands: 4 payload bytes each
    // number of commands needed = at least 1 command + (mByteCnt + 2 addr bytes + 1 transfer size byte - 1<4bytes == single command already accounted for>)/(4 bytes in command)
    lBytesNeeded = (1U + ((apcCmd->mByteCnt + 2U + 1U - 1U) >> 2U)) * ((uint32_t)sizeof(cgd_Cmd_t));
    // check if fitting cmd table space
    if (((size_t)acpI2cTab->mSz - (size_t)acpI2cTab->mUsedBytes) >= lBytesNeeded)
    {
        uint32_t lSrcIdx    = 0U;
        uint8_t lDevId      = (uint8_t)apcCmd->mDevId;
        cgd_Cmd_t* lpNewCmd = &(acpI2cTab->mpTable[acpI2cTab->mUsedBytes / sizeof(cgd_Cmd_t)]);
        // add first write command
        lpNewCmd->mDevId    = lDevId;
        lpNewCmd->mCmdId    = CGD_CMD_WRITE;
        lpNewCmd->mpData[0] = (uint8_t)(2U + apcCmd->mByteCnt);
        lpNewCmd->mpData[1] = (uint8_t)((apcCmd->mAddr & 0xff00U) >> 8); // take upper address byte
        lpNewCmd->mpData[2] = (uint8_t)(apcCmd->mAddr & 0xffU); // take lower address byte
        lpNewCmd->mpData[3] = apcCmd->mpData[lSrcIdx++];

        while (apcCmd->mByteCnt > lSrcIdx)
        {
            // if more than 4bytes to transfer, copy only 4bytes in each iteration to fit single CGD_CMD_CON space
            uint32_t lDataCnt = (apcCmd->mByteCnt - lSrcIdx >= 4U) ? 4U : apcCmd->mByteCnt - lSrcIdx;
            uint32_t lDstIdx  = 0U;
            lpNewCmd++;
            lpNewCmd->mDevId = lDevId;
            lpNewCmd->mCmdId = CGD_CMD_CON;

            while (lDstIdx < lDataCnt)
            {
                lpNewCmd->mpData[lDstIdx++] = apcCmd->mpData[lSrcIdx++];
            } // while space in command
        } // while data left
        acpI2cTab->mUsedBytes += lBytesNeeded;
        lRet = CCA_SUCCESS;
    } // if enough space in table
    else
    {
        lRet = CCA_ERROR;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Not enough space in the command table %u/%u (needed/remaining)).\n", lBytesNeeded, acpI2cTab->mSz - acpI2cTab->mUsedBytes);
    }

    return lRet;
} // cca_AddI2cWrite()

/******************************************************************************/

void cca_ConvUint32ToByteArray(uint32_t aVal32, uint8_t* apArr8, uint32_t aByteCnt)
{
    for (uint32_t lIter = aByteCnt - 1U; lIter > 0U; lIter--)
    {
        apArr8[lIter] = (uint8_t)(aVal32 & 0xFFU);
        aVal32 >>= 8;
    }
    apArr8[0] = (uint8_t)(aVal32 & 0xFFU);
} // ConvUint32ToByteArray()

/** @} */ // addtogroup cca_core_func
