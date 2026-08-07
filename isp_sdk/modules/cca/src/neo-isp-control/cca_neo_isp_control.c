/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cca_neo_isp_control.c
 * @brief Implements control function integration into camera abstraction.
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <string.h>
#include <math.h>

#include "ilog.h"
#include "cca.h"
#include "cca_neo_isp_control.h"
#include "engine/ccf_api.h"
#include "imx95/interface.h"
#include "utils_api.h"

#ifdef LIVE_TUNE
#include "cca_lt_connect.h"
#endif

/** @addtogroup CCA (CameraControlAbstraction) core functionality
 *  @{
 */

/*
    NOTE:
    - CCF_CAMERA_CNT defined in ccf_api.h for CCF purpose;
    - ISPDRV_CAM_PIPE_ID_CNT defined in isp_platform.h for ISP driver;;
    - if ISPDRV_CAM_PIPE_ID_CNT < CCF_CAMERA_CNT, use ISPDRV_CAM_PIPE_ID_CNT as max. camera count;
    - otherwise use CCF_CAMERA_CNT as max. camera count.
*/
#if (ISPDRV_CAM_PIPE_ID_CNT < CCF_CAMERA_CNT)
    #define CAMERA_CNT ISPDRV_CAM_PIPE_ID_CNT
#else
    #define CAMERA_CNT CCF_CAMERA_CNT
#endif


/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
/******************************************************************************/
/**
 * @brief   Contains state of the control functions.
 *******************************************************************************/
typedef struct
{
    cca_Ctx_t*          mpCtx;      /**< pointer to camabs global context */

    /* CCF config */
    uint32_t            mCameras;   /**< number of active cameras */
    cca_NeocontrolParams_t    mParams;    /**< external params */
} cca_CcfCtx_t;

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/
#define HIST_CHANNELS_CNT_MAX   (4U)
#define HIST_H0_ROI1_START_IDX  (64U)
#define HIST_H1_ROI1_START_IDX  (192U)
#define HIST_H2_ROI1_START_IDX  (320U)
#define HIST_H3_ROI1_START_IDX  (448U)

/*
    - histogram with scale 4 has 64 bins equally placed between values 0 - 1048575
    - this is our "base" every other histogram is compared to this base relatively
    - e.g. 4/4 = 1; 64/4 = 16; 1024/4 = 256, etc.
    - above values are used as scale factor for relative picture average calculation
*/
#define HIST_BASIC_SCALE        (4U)

/*==============================================================================
*                              LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/
static cca_CcfCtx_t     sCcfCtx;                    ///< control function context
static ccf_SensData_t   sSensData  [CAMERA_CNT];    ///< sensor related data
static ccf_CtrlParams_t sCtrlParams[CAMERA_CNT];    ///< parameters shared between alorithms for each camera

/*==============================================================================
*                   TESTING FUNCTION PROTOTYPES AND VARIABLES
==============================================================================*/

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/
static  cca_Result_t    GetEmbedData(   cca_CamCtx_t*   apCamCtx,
                                        ccf_SensData_t* apSensData);

static  cca_Result_t    CombineHist(    isp_CtxHw_t     *apCtxHwIn,
                                        ccf_SensData_t  *apSensData);

static  cca_Result_t    ProcessCcf(     void);

static  cca_Result_t    ConvertOutput(  void);

// APIs
static cca_Result_t Open    (void);
static cca_Result_t GenInit (void);
static cca_Result_t Process (void);
static cca_Result_t Close   (void);

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
* @brief    Get embedded data
*
* @param    apCamCtx - pointer to struct. containing camera channel data
* @param    apSensData - pointer to struct. containing exposure data (exp. & A+D gains)
*
* @return   summary of one histogram
*******************************************************************************/
static cca_Result_t GetEmbedData(   cca_CamCtx_t*   apCamCtx,
                                    ccf_SensData_t* apSensData)
{
    cca_Result_t lRet = CCA_SUCCESS;

    // check input validity
    if ((NULL == apCamCtx) ||
        (NULL == apSensData))
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "apCamCtx OR apExp = NULL\n");
        lRet = CCA_ERROR;
    }
    else
    {
        /*
            NOTE:
            - CCF ITEC algorithms shall be independent from HW;
            - every camera has at least one exposure (LONG) => only one exp. data are used for AEC;
            - cam. data from other exposures have very small contribution to the resulted exposure => they could be ignored;
            - LONG exposure has index 0 in array.
        */
        apSensData->mExpCcfIn      = apCamCtx->mSensPrmCcf.mpExp[0].mExp;
        apSensData->mAGainFixCcfIn = apCamCtx->mSensPrmCcf.mpExp[0].mAgain;
        apSensData->mDGainFixCcfIn = apCamCtx->mSensPrmCcf.mpExp[0].mDgain;

        // get WB gains from embedded data
        apSensData->mSensGainRIn = apCamCtx->mSensPrmCcf.mWb.mGains.mRed;
        apSensData->mSensGainGIn = apCamCtx->mSensPrmCcf.mWb.mGains.mGreen;
        apSensData->mSensGainBIn = apCamCtx->mSensPrmCcf.mWb.mGains.mBlue;

        lRet = CCA_SUCCESS;
    }

    return lRet;

}   // GetEmbedData()

/******************************************************************************/
/**
* @brief    combine ISP output histograms
*
* @details  get histograms from ISP and combine them into 3 virtual histograms,
*           which are inputs into AEC control algorithm
*
* @param    apCtxHwIn - pointer to IPS regs & mems
* @param    apSensData - pointer to structure containing exposure data
*
* @return   0 - OK; otherwise - error
*******************************************************************************/
static cca_Result_t CombineHist(isp_CtxHw_t     *apCtxHwIn,
                                ccf_SensData_t  *apSensData)
{
    cca_Result_t lRetVal = CCA_SUCCESS;

    uint32_t lIdx;
    uint32_t lSumHistExpected;
    uint32_t lRoiChannelsCnt = 0U;
    uint32_t lSumHistMid = 0U, lSumHistLow = 0U, lSumHistHigh = 0U;
    uint32_t *lpHist0, *lpHist1, *lpHist2, *lpHist3, *lpHist4, *lpHist5;

    bool lMatchRoiErr = false;

    // Check the channel configuration of the histograms
    if (((apCtxHwIn->mRegs.mSTAT.mHIST0_CTRL_CAM0.mCHANNEL  != apCtxHwIn->mRegs.mSTAT.mHIST1_CTRL_CAM0.mCHANNEL)  ||
         (apCtxHwIn->mRegs.mSTAT.mHIST1_CTRL_CAM0.mCHANNEL  != apCtxHwIn->mRegs.mSTAT.mHIST2_CTRL_CAM0.mCHANNEL)) ||
        ((apCtxHwIn->mRegs.mSTAT.mHIST3_CTRL_CAM0.mCHANNEL  != apCtxHwIn->mRegs.mRGBIR.mHIST0_CTRL_CAM0.mCHANNEL) ||
         (apCtxHwIn->mRegs.mRGBIR.mHIST0_CTRL_CAM0.mCHANNEL != apCtxHwIn->mRegs.mRGBIR.mHIST1_CTRL_CAM0.mCHANNEL)))
    {
        // Should not be happening, data corruption or misconfiguration occured
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "WARN! Mismatching histogram channel config.\n");
    }

    // Check the ROI configuration of the histograms
    if ((apCtxHwIn->mRegs.mSTAT.mROI1_SIZE_CAM0.mWIDTH  != apCtxHwIn->mRegs.mRGBIR.mROI1_SIZE_CAM0.mWIDTH) ||
        (apCtxHwIn->mRegs.mSTAT.mROI1_SIZE_CAM0.mHEIGHT != apCtxHwIn->mRegs.mRGBIR.mROI1_SIZE_CAM0.mHEIGHT))
    {
        /*
            - data corruption or misconfiguration occured
            - print a warning and duplicate the STAT histograms
        */
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "WARN! Mismatching RGBIR and STAT hist. ROI config."
                                         "Discarding RGBIR and using duplicate STAT histograms.\n");
        lMatchRoiErr = true;
    }

    for (lIdx = 0; lIdx < HIST_CHANNELS_CNT_MAX; lIdx++)
    {
        lRoiChannelsCnt += ((apCtxHwIn->mRegs.mSTAT.mHIST0_CTRL_CAM0.mCHANNEL >> lIdx) & 1U);
    }

    // Accounting for RGBIR histograms
    lRoiChannelsCnt *= 2U;

    if (lMatchRoiErr == true)
    {
        /*
            NOTE:
            - ROI0 is background and it is located in bins 0-63;
            - ROI1 is foreground and it is located in bins 64-128;
            - foreground is used => bin indexes starts from 64 (0-63 skipped) and their multiples (192, 320, etc.);
        */
        // Duplicate the histograms from STATS module
        lpHist0 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H0_ROI1_START_IDX];
        lpHist1 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H1_ROI1_START_IDX];
        lpHist2 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H2_ROI1_START_IDX];
        lpHist3 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H0_ROI1_START_IDX];
        lpHist4 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H1_ROI1_START_IDX];
        lpHist5 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H2_ROI1_START_IDX];
    }
    else
    {
        // The histograms are configured correctly, use all of them
        lpHist0 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H0_ROI1_START_IDX];
        lpHist1 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H1_ROI1_START_IDX];
        lpHist2 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H2_ROI1_START_IDX];
        lpHist3 = &apCtxHwIn->mMems.HISTOGRAM_STATISTICS[HIST_H3_ROI1_START_IDX];
        lpHist4 = &apCtxHwIn->mMems.RGBIR_HISTOGRAM     [HIST_H0_ROI1_START_IDX];
        lpHist5 = &apCtxHwIn->mMems.RGBIR_HISTOGRAM     [HIST_H1_ROI1_START_IDX];

    }   // if (lMatchRoiErr == true)

    // calculate histogram summary
    for (lIdx = 0U; lIdx < (uint32_t)STAT_HIST_BINS; lIdx++)
    {
        apSensData->mHistMid[lIdx]     = *(lpHist0 + lIdx) + *(lpHist3 + lIdx);
        apSensData->mHistLow[lIdx]     = *(lpHist1 + lIdx) + *(lpHist4 + lIdx);
        apSensData->mHistHigh[lIdx]    = *(lpHist2 + lIdx) + *(lpHist5 + lIdx);

        lSumHistMid = (((uint64_t)lSumHistMid + (uint64_t)apSensData->mHistMid[lIdx]) > UINT32_MAX )?
                        UINT32_MAX : lSumHistMid + apSensData->mHistMid[lIdx];

        lSumHistLow = (((uint64_t)lSumHistLow + (uint64_t)apSensData->mHistLow[lIdx]) > UINT32_MAX )?
                        UINT32_MAX : lSumHistLow + apSensData->mHistLow[lIdx];

        lSumHistHigh = (((uint64_t)lSumHistHigh + (uint64_t)apSensData->mHistHigh[lIdx]) > UINT32_MAX )?
                        UINT32_MAX : lSumHistHigh + apSensData->mHistHigh[lIdx];

    }

    /*
        - get constants to be used for relative picture average calculation;
        - more in description of HIST_BASIC_SCALE;
    */
    apSensData->mHistScMid  = (uint32_t)(apCtxHwIn->mRegs.mSTAT.mHIST0_SCALE_CAM0.mSCALE / HIST_BASIC_SCALE);
    apSensData->mHistScLow  = (uint32_t)(apCtxHwIn->mRegs.mSTAT.mHIST1_SCALE_CAM0.mSCALE / HIST_BASIC_SCALE);
    apSensData->mHistScHigh = (uint32_t)(apCtxHwIn->mRegs.mSTAT.mHIST2_SCALE_CAM0.mSCALE / HIST_BASIC_SCALE);

    /*
        NOTE:
        Assuming that the stats ROI's width and height do not exceed
        the frame's width and height. (E.g for 1280x720 frame, the stats ROI's size
        should be at most 1280x720. And if X or Y offset is specified, the ROI's
        width and height will be decremented accordingly to fit the frame's size.
    */
    lSumHistExpected = ((uint32_t)apCtxHwIn->mRegs.mSTAT.mROI1_SIZE_CAM0.mWIDTH *
                        (uint32_t)apCtxHwIn->mRegs.mSTAT.mROI1_SIZE_CAM0.mHEIGHT /
                        HIST_CHANNELS_CNT_MAX) * lRoiChannelsCnt;

    if (lSumHistMid != lSumHistExpected)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR,
            "Mismatching Long histogram sum. Expected: %u, current: %u\n",
            lSumHistExpected,
            lSumHistMid);

        lRetVal = CCA_ERROR;
    }
    else if (lSumHistLow != lSumHistExpected)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR,
            "Mismatching Short histogram sum. Expected: %u, current: %u\n",
            lSumHistExpected,
            lSumHistLow);

        lRetVal = CCA_ERROR;
    }
    else if (lSumHistHigh != lSumHistExpected)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR,
            "Mismatching VShort histogram sum. Expected: %u, current: %u\n",
            lSumHistExpected,
            lSumHistHigh);

        lRetVal = CCA_ERROR;
    }
    else
    {
        // save histogram summary and number of bins for further processing
        apSensData->mHistBins   = STAT_HIST_BINS;
        apSensData->mHistSum    = lSumHistExpected;
    }

    return lRetVal;

}   // CombineHist()

/******************************************************************************/
/**
* @brief    Call processing CCF for every channel/camera
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t ProcessCcf(void)
{
    static uint8_t sCcfInitialized = 0U;
    cca_Result_t lRet = CCA_SUCCESS;

    if (NULL == sCcfCtx.mpCtx)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "sCcfCtx.mpCtx == NULL\n");
        return CCA_ERROR;
    }

    // for all channels
    for (uint32_t lChnIdx = 0U; lChnIdx < CAMERA_CNT; lChnIdx++)
    {
        cca_CamCtx_t*   lpCamCtx = &(sCcfCtx.mpCtx->mpCamCtx[lChnIdx]);

        // check input parameter
        if (NULL == lpCamCtx)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "lpCamCtx == NULL\n");
            lRet = CCA_ERROR;
        }
        else
        {
            // skip inactive channels
            if (lpCamCtx->mpCcfIo == NULL)
            {
                continue;
            }

            if (lpCamCtx->mpCcfIo->mpIspCtxIn == NULL)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "mpIspCtxIn == NULL\n");
                lRet = CCA_ERROR;
                break;
            }

            // combine ISP histograms to get virtual histograms
            if (CCA_SUCCESS != CombineHist( (void*)&lpCamCtx->mpCcfIo->mpIspCtxIn->mCtxHw,
                                            &sSensData[lChnIdx]))
            {
                lRet = CCA_ERROR;
            }
            // get embedded exposure data, A+D gains & exposure, and store them into internal structure
            else if (CCA_SUCCESS == GetEmbedData(   lpCamCtx,
                                                    &sSensData[lChnIdx]))
            {
                // Temporary solution to avoid sending initial configuration data in every iteration
                // and separate the first frame initialization from subsequent frames within CCF
                if (sCcfInitialized < sCcfCtx.mCameras)
                {
                    // First run initialization
                    if (0 != ccf_Init( (void*)&lpCamCtx->mpCcfIo->mpIspCtxIn->mCtxHw,
                                            (void*)&lpCamCtx->mIspCtxAcc.mCtxHw,
                                            sSensData[lChnIdx].mSensID,
                                            lChnIdx))
                    {
                        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "ccf_Init failed.\n");
                        lRet = CCA_ERROR;
                    }
                    else
                    {
                        sCcfInitialized++;
                    }
                }
                else
                {
                    // call ccf run
                    if (0 != ccf_Process(  (void*)&lpCamCtx->mpCcfIo->mpIspCtxIn->mCtxHw,
                                                (void*)&lpCamCtx->mIspCtxAcc.mCtxHw,
                                                &sSensData[lChnIdx],
                                                &sCtrlParams[lChnIdx],
                                                lChnIdx))
                    {
                        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "ccf_Process failed.\n");
                        lRet = CCA_ERROR;
                    }
    #ifdef LIVE_TUNE
                    if (sCcfCtx.mParams.mTuningToolCon != 0U)
                    {
                        if (CCA_SUCCESS != cca_LiveTune())
                        {
                            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Live tuning process failed.\n");
                        }
                    }
    #endif
                }
            }   // CCA_SUCCESS == GetEmbedData()

        }   // if ((NULL == sCcfCtx.mpCtx) || (NULL == lpCamCtx))

    }   // for sCcfCtx.mCameras

    return lRet;

}   // ProcessCcf()

/******************************************************************************/
/**
* @brief    Provide updated regs & mems to output
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t ConvertOutput(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if (NULL == sCcfCtx.mpCtx)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "sCcfCtx.mpCtx == NULL\n");
        return CCA_ERROR;
    }

    // for all channels
    for (uint32_t lChnIdx = 0U; lChnIdx < CAMERA_CNT; lChnIdx++)
    {
        // create local pointer variable
        cca_CamCtx_t*   lpCamCtx = &(sCcfCtx.mpCtx->mpCamCtx[lChnIdx]);

        // check input parameter
        if (NULL == lpCamCtx)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "lpCamCtx == NULL\n");
            lRet = CCA_ERROR;
        }
        else
        {
            // skip inactive channels
            if (lpCamCtx->mpCcfIo == NULL)
            {
                continue;
            }

            // local variables - last two needs to be exactly here afer check of inactive channels
            isp_CtxHw_t*    lpIspCtxAcc = &(lpCamCtx->mIspCtxAcc.mCtxHw);

            if (NULL == lpCamCtx->mpCcfIo->mpIspCtxOut)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "NULL == mpIspCtxOut\n");
                lRet = CCA_ERROR;
                break;
            }
            isp_CtxHw_t*    lpOutCtx    = &(lpCamCtx->mpCcfIo->mpIspCtxOut->mCtxHw);
            cca_SensPrm_t*  lpSensSet   = lpCamCtx->mpCcfIo->mpSensPrmNew;

            // set static sensor settings
            if (NULL != lpSensSet)
            {
                lpSensSet->mExpType        = CCA_EXP_NRM;
                lpSensSet->mpExp[0].mExp   = sSensData[lChnIdx].mExpCcfOut;
                lpSensSet->mpExp[0].mAgain = sSensData[lChnIdx].mAGainFixCcfOut;
                lpSensSet->mpExp[0].mDgain = sSensData[lChnIdx].mDGainFixCcfOut;
                lpSensSet->mL2sRatio       = 16384U;     // values have been measured with uguzzi & ovx3c and here adopted
                lpSensSet->mL2vsRatio      = 262144U;

                lpSensSet->mWb.mWbLoc          = sSensData[lChnIdx].mWbLoc; // WB type set in Open() function
                lpSensSet->mWb.mColorTemp      = 3577U;
                lpSensSet->mWb.mGains.mBlue    = sSensData[lChnIdx].mSensBlueGain;
                lpSensSet->mWb.mGains.mGreen   = sSensData[lChnIdx].mSensGreenGain;
                lpSensSet->mWb.mGains.mRed     = sSensData[lChnIdx].mSensRedGain;
                lpSensSet->mWb.mOffsets.mBlue  = 0;
                lpSensSet->mWb.mOffsets.mGreen = 0;
                lpSensSet->mWb.mOffsets.mRed   = 0;

            }
            else
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "NULL == lpSensSet\n");
                lRet = CCA_ERROR;
            }

            /*
                copy ISP regs & mems settings to output from accumulated context;
                copy only relevant config (e.g. skip buffer addresses and alias region)
            */
          // void cca_CopyIspCfg(isp_CtxHw_t *apDst, isp_CtxHw_t *apSrc)
           cca_CopyIspCfg(lpOutCtx, lpIspCtxAcc);

        }   // else of if ((NULL == sCcfCtx.mpCtx) || (NULL == lpCamCtx))

     }   // for sCcfCtx.mCameras

     return lRet;

}   // ConvertOutput()

#ifdef LIVE_TUNE
/******************************************************************************/
/**
* @brief    Connect live tuning module
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t ConnectLiveTuning(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if (CCA_SUCCESS != cca_LtConnect())
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to initialize Live Tuning Connect! LT support will not be available.\n");
        lRet = CCA_ERROR;
    }

    return lRet;
} // ConnectLiveTuning()
#endif

/******************************************************************************/
/**
* @brief    Initialize memory
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t Open(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

    // set whole cam ctl ctx to 0s
    (void)memset(&sCcfCtx, 0, sizeof(cca_CcfCtx_t));

    // remember pointer to toplevel ctx
    sCcfCtx.mpCtx       = &gCcaCtx;
    sCcfCtx.mCameras    = gCcaCtx.mActiveCamCnt;

    // parameter checks
    if (gCcaCtx.mCfg.mCcfCfg.mpParams == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF parameter structure is NULL.\n");

        lRet = CCA_ERROR;
    }
    // param size check
    else if (gCcaCtx.mCfg.mCcfCfg.mParamSize != sizeof(cca_NeocontrolParams_t))
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR,
            "Wrong ccf param structure size (%zu, expected %zu).\n",
            gCcaCtx.mCfg.mCcfCfg.mParamSize,
            sizeof(cca_NeocontrolParams_t));

        lRet = CCA_ERROR;
    }
    // checks ok, continue
    else
    {
        // remember pointer to toplevel ctx
        sCcfCtx.mpCtx       = &gCcaCtx;
        sCcfCtx.mCameras    = gCcaCtx.mActiveCamCnt;

        // retype to specific parameter structure - needed for tunning from *.yml & other "parameters"
        sCcfCtx.mParams = *(cca_NeocontrolParams_t*)gCcaCtx.mCfg.mCcfCfg.mpParams;

        // go through all cameras
        for (uint32_t lCamIdx = 0; lCamIdx < CAMERA_CNT; lCamIdx++)
        {
            cca_CamInit_t* lpCamInitCfg = &sCcfCtx.mpCtx->mCfg.mpCam[lCamIdx];

            // channel enabled?
            if (lpCamInitCfg->mEnabled == 1U)
            {
                // save sensor ID
                sSensData[lCamIdx].mSensID = lpCamInitCfg->mSensorId;

            } // if camera is active

            /*
                - initialize camera exposure related structure with any data
                - some of the variables are going to be reinitialized with more valid adata later
            */
            sSensData[lCamIdx].mExpCcfOut         = 1000U;  // start with time 1ms
            sSensData[lCamIdx].mAGainFixCcfOut    = 65536U; // fixed point 16.16 - start with gain 1.0
            sSensData[lCamIdx].mDGainFixCcfOut    = 65536U; // fixed point 16.16 - start with gain 1.0
            sSensData[lCamIdx].mExpCcfIn          = 0U;
            sSensData[lCamIdx].mAGainFixCcfIn     = 0U;
            sSensData[lCamIdx].mDGainFixCcfIn     = 0U;

            // init WB related data
            sSensData[lCamIdx].mWbLoc           = (uint8_t)CCA_WB_SEN; // white balancing on sensor = better pic. results
            sSensData[lCamIdx].mSensBlueGain    = 256U;  // start with gains 1.0 (8.8 fixed format)
            sSensData[lCamIdx].mSensGreenGain   = 256U;
            sSensData[lCamIdx].mSensRedGain     = 256U;

            for (uint32_t lIdx = 0U; lIdx < (uint32_t)STAT_HIST_BINS; lIdx++)
            {
                sSensData[lCamIdx].mHistLow[lIdx]  = 0U;
                sSensData[lCamIdx].mHistMid[lIdx]  = 0U;
                sSensData[lCamIdx].mHistHigh[lIdx] = 0U;
            }

            sSensData[lCamIdx].mHistScLow     = 0U;
            sSensData[lCamIdx].mHistScMid     = 0U;
            sSensData[lCamIdx].mHistScHigh    = 0U;
            sSensData[lCamIdx].mHistSum       = 0U;
            sSensData[lCamIdx].mHistBins      = 0U;


        } // for all camera channels

#ifdef LIVE_TUNE
        // Connect Live tuning
        if (sCcfCtx.mParams.mTuningToolCon != 0U)
        {
            if (CCA_SUCCESS != ConnectLiveTuning())
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to connect live tuning\n");
                // set tuning tool connection param to false
                sCcfCtx.mParams.mTuningToolCon = 0U;
                // not a fatal issue
                lRet = CCA_SUCCESS;
            }
        }
#endif

    } // else of parameter check

    return lRet;
} // Open()

/******************************************************************************/
/**
* @brief    Do a first regs & mems and sensor settings from default values; init CCF
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t GenInit(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if (NULL == sCcfCtx.mpCtx)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "sCcfCtx.mpCtx == NULL\n");
        return CCA_ERROR;
    }

    // for all cameras
    for (uint32_t lChnIdx = 0; lChnIdx < CAMERA_CNT; lChnIdx++)
    {
        // create local variables
        cca_CamCtx_t*   lpCamCtx    = &(sCcfCtx.mpCtx->mpCamCtx[lChnIdx]);

        // check input parameter
        if (NULL == lpCamCtx)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "lpCamCtx == NULL\n");
            lRet = CCA_ERROR;
        }
        else
        {
            // skip inactive channels
            if (lpCamCtx->mpCcfIo == NULL)
            {
                continue;
            }


            isp_CtxHw_t*    lpIspCtxAcc = &(lpCamCtx->mIspCtxAcc.mCtxHw);
            if (NULL == lpCamCtx->mpCcfIo->mpIspCtxOut)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "NULL == mpIspCtxOut\n");
                lRet = CCA_ERROR;
                break;
            }
            isp_CtxHw_t*    lpOutCtx    = &(lpCamCtx->mpCcfIo->mpIspCtxOut->mCtxHw);

            // get default regs & mems config (filled in app)
            const   isp_CtxHw_t*   lpcDef  = sCcfCtx.mpCtx->mCfg.mpCam[lChnIdx].mcpExtDef;
            if (NULL == lpcDef)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "NULL == lpcDef\n");
                lRet = CCA_ERROR;
                break;
            }

            cca_SensPrm_t*  lpSensSet   = lpCamCtx->mpCcfIo->mpSensPrmNew;
            // set sensor initial settings
            if (NULL != lpSensSet)
            {
                lpSensSet->mExpType        = CCA_EXP_NRM;
                lpSensSet->mpExp[0].mExp   = sSensData[lChnIdx].mExpCcfOut;
                lpSensSet->mpExp[0].mAgain = sSensData[lChnIdx].mAGainFixCcfOut;
                lpSensSet->mpExp[0].mDgain = sSensData[lChnIdx].mDGainFixCcfOut;
                lpSensSet->mL2sRatio       = 16384U;                 // measured with uguzzi & ovx3c camera
                lpSensSet->mL2vsRatio      = 262144U;                // measured with uguzzi & ovx3c camera

                lpSensSet->mWb.mWbLoc          = sSensData[lChnIdx].mWbLoc; // WB type set in Open() function
                lpSensSet->mWb.mColorTemp      = 3577U;
                lpSensSet->mWb.mGains.mBlue    = sSensData[lChnIdx].mSensBlueGain;
                lpSensSet->mWb.mGains.mGreen   = sSensData[lChnIdx].mSensGreenGain;
                lpSensSet->mWb.mGains.mRed     = sSensData[lChnIdx].mSensRedGain;
                lpSensSet->mWb.mOffsets.mBlue  = 0;
                lpSensSet->mWb.mOffsets.mGreen = 0;
                lpSensSet->mWb.mOffsets.mRed   = 0;
            }
            else
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "NULL == lpSensSet\n");
                lRet = CCA_ERROR;
            }

            /*
                copy default init settings to accumulator as a start point;
                copy only relevant config (e.g. skip buffer addresses and alias region)
            */
            cca_CopyIspCfg( lpIspCtxAcc,
                            lpcDef);
            /*
                copy default init settings to output;
                copy only relevant config (e.g. skip buffer addresses and alias region)
            */
            cca_CopyIspCfg( lpOutCtx,
                            lpcDef);

        }

    } // for all enabled cameras

    // call init of CCF
    ccf_Open(   sCcfCtx.mParams.mTuningToolCon,
                sCcfCtx.mParams.mpCcfAlgoConfig);

    return lRet;

} // GenInit()

/******************************************************************************/
/**
* @brief    Process CCF and set output
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t Process(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

    // process CCF for all channels
    if (CCA_SUCCESS != ProcessCcf())
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "ProcessCcf() failed\n");
        lRet = CCA_ERROR;
    }
    else
    {
        // send updated regs & mems to output
        lRet = ConvertOutput();
    }

    return lRet;

} // Process()

/******************************************************************************/
/**
* @brief    Cleanup memory
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t Close(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

    // set whole cam ctl ctx to 0s
    (void)memset(&sCcfCtx, 0, sizeof(cca_CcfCtx_t));

    if (0 != ccf_Close())
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to close the CCF properly.\n");
        lRet = CCA_ERROR;
    }

    return lRet;

} // Close()

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/
void cca_ccf_GetOps(cca_CcfOps_t* const acpOps)
{
    if (acpOps != NULL)
    {
        acpOps->mOpen    = Open;
        acpOps->mGetInit = GenInit;
        acpOps->mProcess = Process;
        acpOps->mClose   = Close;
    }

} // cca_ccf_GetOps()

/******************************************************************************/
cca_Result_t cca_ccf_Config(uint32_t aChnlIdx, uint32_t aCmdId, int32_t aVal)
{
    (void)aChnlIdx;
    (void)aCmdId;
    (void)aVal;

    cca_Result_t lRet = CCA_SUCCESS;

    return lRet;

} // cca_ccf_Config()

/** @} */ // addtogroup cca_ug_integration
