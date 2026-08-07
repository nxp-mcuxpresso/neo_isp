/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cca_neob.c
 * @brief Implements platform specific functions for NEOB.
 *******************************************************************************/

#include <string.h>
#include <stddef.h>
#include "isp_task.h"

#if defined(SDK_OS_FREE_RTOS)
#include "utils_edma.h"
#endif

void cca_CopyIspCfg(isp_CtxHw_t *apDst, const isp_CtxHw_t *apSrc)
{
    // copy only relevant config (e.g. skip buffer addresses and alias region)
#if defined(SDK_OS_FREE_RTOS)
    utils_EdmaTransfer((uint8_t*)(&apDst->mRegs) + offsetof(IspRegs_t, mHC), (uint8_t*)(&apSrc->mRegs) + offsetof(IspRegs_t, mHC), offsetof(IspRegs_t, mIDBG2) + sizeof(isp_IDBG2_t) - offsetof(IspRegs_t, mHC));
    apDst->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0   = apSrc->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0;
    apDst->mRegs.mPIPE_CONF.mIMG_CONF_CAM0   = apSrc->mRegs.mPIPE_CONF.mIMG_CONF_CAM0;
    apDst->mRegs.mPIPE_CONF.mSKIP_CTRL0      = apSrc->mRegs.mPIPE_CONF.mSKIP_CTRL0;

    utils_EdmaTransfer(apDst->mMems.VIGNETTING_TABLE, apSrc->mMems.VIGNETTING_TABLE, sizeof(apSrc->mMems.VIGNETTING_TABLE));
    utils_EdmaTransfer(apDst->mMems.DRC_GLOBAL_TONEMAP, apSrc->mMems.DRC_GLOBAL_TONEMAP, sizeof(apSrc->mMems.DRC_GLOBAL_TONEMAP));
    utils_EdmaTransfer(apDst->mMems.DRC_LOCAL_TONEMAP, apSrc->mMems.DRC_LOCAL_TONEMAP, sizeof(apSrc->mMems.DRC_LOCAL_TONEMAP));
#else
    memcpy((uint8_t*)(&apDst->mRegs) + offsetof(IspRegs_t, mHC), (uint8_t*)(&apSrc->mRegs) + offsetof(IspRegs_t, mHC), offsetof(IspRegs_t, mIDBG2) + sizeof(isp_IDBG2_t) - offsetof(IspRegs_t, mHC));
    apDst->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0   = apSrc->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0;
    apDst->mRegs.mPIPE_CONF.mIMG_CONF_CAM0   = apSrc->mRegs.mPIPE_CONF.mIMG_CONF_CAM0;
    apDst->mRegs.mPIPE_CONF.mSKIP_CTRL0      = apSrc->mRegs.mPIPE_CONF.mSKIP_CTRL0;

    memcpy(apDst->mMems.VIGNETTING_TABLE, apSrc->mMems.VIGNETTING_TABLE, sizeof(apSrc->mMems.VIGNETTING_TABLE));
    memcpy(apDst->mMems.DRC_GLOBAL_TONEMAP, apSrc->mMems.DRC_GLOBAL_TONEMAP, sizeof(apSrc->mMems.DRC_GLOBAL_TONEMAP));
    memcpy(apDst->mMems.DRC_LOCAL_TONEMAP, apSrc->mMems.DRC_LOCAL_TONEMAP, sizeof(apSrc->mMems.DRC_LOCAL_TONEMAP));
#endif
}
