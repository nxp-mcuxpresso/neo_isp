/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/******************************************************************************/
/**
 * @file  cca_ccf_custom.c
 * @brief Implements control function integration into camera abstraction.
 *******************************************************************************/

#include "cca.h"
#include "cca_ccf_custom.h"

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

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
* @brief    Initialize memory
*
* @return   0 - OK; otherwise error
*******************************************************************************/
static cca_Result_t Open(void)
{
    cca_Result_t lRet = CCA_SUCCESS;

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

    // for all cameras
    for (uint32_t lChnIdx = 0; lChnIdx < ISPDRV_CAM_PIPE_ID_CNT; lChnIdx++)
    {
        // create local variables
        cca_CamCtx_t*   lpCamCtx    = &(gCcaCtx.mpCamCtx[lChnIdx]);

        // check input parameter
        if (
            (NULL == lpCamCtx))
        {
            lRet = CCA_ERROR;
        }
        else
        {
            // skip inactive channels
            if (lpCamCtx->mpCcfIo == NULL)
            {
                continue;
            }

            // create temporary local variables
            isp_CtxHw_t*    lpIspCtxAcc = &(lpCamCtx->mIspCtxAcc.mCtxHw);
            isp_CtxHw_t*    lpOutCtx    = &(lpCamCtx->mpCcfIo->mpIspCtxOut->mCtxHw);
            cca_SensPrm_t*  lpSensSet   = lpCamCtx->mpCcfIo->mpSensPrmNew;

            // get default regs & mems config (filled in app)
            const   isp_CtxHw_t*   lpcDef  = gCcaCtx.mCfg.mpCam[lChnIdx].mcpExtDef;

            // set sensor initial settings
            if (NULL != lpSensSet)
            {
                lpSensSet->mExpType        = CCA_EXP_NRM;
                lpSensSet->mpExp[0].mExp   = 1000U;
                lpSensSet->mpExp[0].mAgain = 65536U;
                lpSensSet->mpExp[0].mDgain = 65536U;
                lpSensSet->mL2sRatio       = 16384U;
                lpSensSet->mL2vsRatio      = 262144U;

                lpSensSet->mWb.mWbLoc          = (uint8_t)CCA_WB_SEN; // WB type set in Open() function
                lpSensSet->mWb.mColorTemp      = 3577U;
                lpSensSet->mWb.mGains.mBlue    = 256U;
                lpSensSet->mWb.mGains.mGreen   = 256U;
                lpSensSet->mWb.mGains.mRed     = 256U;
                lpSensSet->mWb.mOffsets.mBlue  = 0;
                lpSensSet->mWb.mOffsets.mGreen = 0;
                lpSensSet->mWb.mOffsets.mRed   = 0;
            }
            else
            {
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

    return lRet;

} // Close()

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/
void cca_custom_GetOps(cca_CcfOps_t* const acpOps)
{
    if (acpOps != NULL)
    {
        acpOps->mOpen    = Open;
        acpOps->mGetInit = GenInit;
        acpOps->mProcess = Process;
        acpOps->mClose   = Close;
    }

} // cca_itec_GetOps()
