/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cgd_freertos_config.c
 * @brief FREERTOS board config class
 *******************************************************************************/

#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ilog.h>
#include "cgd_config.h"
#include "cgd.h"
#include "cgd_i2c.h"

#include "board.h"

#include "fsl_dwc_mipi_csi2rx.h"

//*********************************************************
// DEFINES
//*********************************************************

/**
 * @brief what i2c control file is used - /dev/i2c-%u
 */
#define CGD_I2C_LPI2C2 2
#define CGD_I2C_LPI2C3 3

/*==============================================================================
*                         LOCAL DATA
==============================================================================*/
/******************************************************************************/
/**
 * @brief Definition of I2C subsystem
 *
 *******************************************************************************/
cgd_PortHead_t sgCgdPort0Config =
    {
        .mI2cPortDev = {.mDescriptor = CGD_I2C_LPI2C3, .mBusId = CGD_I2C_MIPI_CSI, .mAddr = 0},
};

cgd_PortHead_t sgCgdPort1Config =
    {
        .mI2cPortDev = {.mDescriptor = CGD_I2C_LPI2C2, .mBusId = CGD_I2C_MIPI_COMBO, .mAddr = 0},
};

/******************************************************************************/
/**
 * @brief Memory maping for CSI device + I2C port def
 *
 *******************************************************************************/
cgd_CsiPort_t sgCgdCSI0Port =
    {
        .mPortState = CGD_STATE_OFF,
        .mcHead     = &sgCgdPort0Config,
};

cgd_CsiPort_t sgCgdCSI1Port =
    {
        .mPortState = CGD_STATE_OFF,
        .mcHead     = &sgCgdPort1Config,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI0 =
    {
        .mIsiId = 0,
        .mCsiId = CGD_MIPI_CSI_DEV, // CGD_MIPI_CSI_COMBO_DEV, CGD_MIPI_CSI_DEV
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI1 =
    {
        .mIsiId = 1,
        .mCsiId = CGD_MIPI_CSI_DEV,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI2 =
    {
        .mIsiId = 2,
        .mCsiId = CGD_MIPI_CSI_DEV,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI3 =
    {
        .mIsiId = 3,
        .mCsiId = CGD_MIPI_CSI_DEV,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI4 =
    {
        .mIsiId = 4,
        .mCsiId = CGD_MIPI_CSI_COMBO_DEV,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI5 =
    {
        .mIsiId = 5,
        .mCsiId = CGD_MIPI_CSI_COMBO_DEV,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI6 =
    {
        .mIsiId = 6,
        .mCsiId = CGD_MIPI_CSI_COMBO_DEV,
};

/******************************************************************************/
/**
 * @brief CGD internal structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiDevice_t sgCgdISI7 =
    {
        .mIsiId = 7,
        .mCsiId = CGD_MIPI_CSI_COMBO_DEV,
};

/******************************************************************************/
/**
 * @brief External CSI Configuration for cgd function
 *
 *******************************************************************************/
cgd_CsiExtCfg_t sgCgdCsi0ExtCfg =
    {
        .mLanes       = 4U,
        .mDataType    = {kCSI2RX_DataTypeRAW16, kCSI2RX_DataTypeRAW16, kCSI2RX_DataTypeRAW16, kCSI2RX_DataTypeRAW16, 0, 0, 0, 0},
        .mNPDataType  = {     CSI_NP_DT_EMB_12,      CSI_NP_DT_EMB_12,      CSI_NP_DT_EMB_12,      CSI_NP_DT_EMB_12, 0, 0, 0, 0},
        .mMbps        = 1500U,
        .mI2cAdrCnt   = 0,
        .mpI2cAdr     = NULL,
        .mpcInitArr   = NULL,
        .mpcCloseArr  = NULL,
        .mpcStreamEn  = NULL,
        .mpcStreamDis = NULL,
};

cgd_CsiExtCfg_t sgCgdCsi1ExtCfg =
    {
        .mLanes       = 4U,
        .mDataType    = {kCSI2RX_DataTypeRAW16, kCSI2RX_DataTypeRAW16, kCSI2RX_DataTypeRAW16, kCSI2RX_DataTypeRAW16, 0, 0, 0, 0},
        .mNPDataType  = {     CSI_NP_DT_EMB_12,      CSI_NP_DT_EMB_12,      CSI_NP_DT_EMB_12,      CSI_NP_DT_EMB_12, 0, 0, 0, 0},
        .mMbps        = 1500U,
        .mI2cAdrCnt   = 0,
        .mpI2cAdr     = NULL,
        .mpcInitArr   = NULL,
        .mpcCloseArr  = NULL,
        .mpcStreamEn  = NULL,
        .mpcStreamDis = NULL,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg0 =
    {
        .mVC              = 0U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg1 =
    {
        .mVC              = 1U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg2 =
    {
        .mVC              = 2U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg3 =
    {
        .mVC              = 3U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg4 =
    {
        .mVC              = 0U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg5 =
    {
        .mVC              = 1U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg6 =
    {
        .mVC              = 2U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief CGD external structure for one ISI channel
 *
 *******************************************************************************/
cgd_IsiExtCfg_t sgCgdIsiExtCfg7 =
    {
        .mVC              = 3U,
        .mHwId            = 0U,
        .mPPErrIrqMask    = 0x3CFC0000, /* reg CHNL_IER IRQ mask */
        .mImageHeight     = 1082,
        .mImageWidth      = 1920,
        .mEmbdLinesBefore = 2,
        .mEmbdLinesAfter  = 0,
        .mBPP             = 2,
};

/******************************************************************************/
/**
 * @brief Storage for external configuration for cgd - can be modified and provided back to CGD
 *
 *******************************************************************************/
cgd_SystemExtCfg_t sgCgdExtCfg =
    {
        .mpCsi = {&sgCgdCsi0ExtCfg, &sgCgdCsi1ExtCfg},
        .mpIsi = {
                  &sgCgdIsiExtCfg0,
                  &sgCgdIsiExtCfg1,
                  &sgCgdIsiExtCfg2,
                  &sgCgdIsiExtCfg3,
                  &sgCgdIsiExtCfg4,
                  &sgCgdIsiExtCfg5,
                  &sgCgdIsiExtCfg6,
                  &sgCgdIsiExtCfg7,
                  }
};

/******************************************************************************/
/**
 * @brief Whole context for CGD , contains configuration for all CSI devices and all ISI channels
 *
 *******************************************************************************/
cgd_System_t sgCgdSystem =
    {
        .sgState      = CGD_STATE_OFF,
        .mppCsiPort   = {&sgCgdCSI0Port, NULL},
        .mppIsiDevice = {&sgCgdISI0, &sgCgdISI1, &sgCgdISI2, &sgCgdISI3, NULL, NULL, NULL, NULL}
};
