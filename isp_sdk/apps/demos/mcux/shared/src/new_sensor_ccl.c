/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  new_sensor_ccl.c
 * @brief implements user-space setup new sensor
 *******************************************************************************/

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "ilog.h"

#if defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))

#include "cgd_i2c.h"
#include "cgd_types.h"

#include "new_sensor_ccl.h"
#include "new_sensor_ccl_i2c.h"

#include "board.h"
#include "fsl_dwc_mipi_csi2rx.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define DT_RAW16 1 << 22

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/
/******************************************************************************/
/**
 * @brief Default I2C device address table for IMX95 ports.
 *
 *******************************************************************************/
static const uint8_t sgNewSensorI2cAdrTable[] = {
    [NEW_SENSOR_I2C_CAMERA_IDX] = NEW_SENSOR_I2C_CAMERA,
};
/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

cgd_Result cgd_New_Sensor_ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cgd_SystemExtCfg_t* newExternalConfig)
{
    (void)aHwId;

    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice((cgd_IsiChannel_t)aCamId);

    if (lpIsiDev == NULL) /* check if device is valid */
    {
        return CGD_LIB_FAILURE;
    }

    uint8_t Id = lpIsiDev->mCsiId;

    if (Id >= CGD_MIPI_CSI_CNT)
    {
        return CGD_LIB_FAILURE;
    }

    cgd_CsiExtCfg_t* lpCsiCfg = newExternalConfig->mpCsi[Id];

    if (lpCsiCfg == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    /* Set DPHY bit rate */
    lpCsiCfg->mMbps  = 1500;
    lpCsiCfg->mLanes = 4; // use 4 lanes

    for (uint8_t index = 0; index < CGD_CSI_VC_CNT; index++)
    {
        lpCsiCfg->mDataType[index] = kCSI2RX_DataTypeRAW16; // set data type for each pixel link
    }

    /* Assign address tables to the CSI configuration */
    lpCsiCfg->mI2cAdrCnt = sizeof(sgNewSensorI2cAdrTable);
    lpCsiCfg->mpI2cAdr   = sgNewSensorI2cAdrTable;

    /* setup i2c tables */
    lpCsiCfg->mpcInitArr   = &sgCgdNewSensorInitialization;
    lpCsiCfg->mpcStreamEn  = &sgCgdNewSensorVideoEnabled;
    lpCsiCfg->mpcStreamDis = &sgCgdNewSensorVideoDisable;
    lpCsiCfg->mpcCloseArr  = &sgCgdNewSensorClose;

    return CGD_LIB_SUCCESS;
} // new_sensor_ConfigureCgd()

/******************************************************************************/

cgd_Result cgd_New_Sensor_Init(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apParams, size_t mParamSize)
{
    (void)aHwId;
    (void)aCamId;
    (void)apParams;
    (void)mParamSize;

    return CGD_LIB_SUCCESS;
} // new_sensor_Init()

/******************************************************************************/
cgd_Result cgd_New_Sensor_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    (void)aHwId;
    (void)aCamId;
    return CGD_LIB_SUCCESS;
} // new_sensor_Start()

/******************************************************************************/
cgd_Result cgd_New_sensor_Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    (void)aHwId;
    (void)aCamId;
    return CGD_LIB_SUCCESS;
} // cgd_New_sensor_Stop()

/******************************************************************************/

cgd_Result cgd_New_Sensor_Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_SensPrm_t* apSensPrm)
{
    (void)aHwId;

    cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice((cgd_IsiChannel_t)aCamId);

    if (isiDev == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    // *** generate I2C table ***
    cgd_Cmd_t tab[128];
    memset(tab, 0, sizeof(tab));

    cca_I2cTab_t lI2cTab;
    lI2cTab.mpTable    = tab;
    lI2cTab.mSz        = sizeof(tab);
    lI2cTab.mUsedBytes = 0U;

    // CCA can now generate i2c update command table
    if (CCA_SUCCESS != cca_GetI2cTab(&lI2cTab, apSensPrm, NULL, aCamId))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Failed to get I2c table\n");
        return CGD_LIB_FAILURE;
    }

    cgd_CmdTable_t updateTable =
        {
            .mPortIdx    = isiDev->mCsiId,
            .ympI2cTable = (cgd_Cmd_t*)lI2cTab.mpTable,
            .mI2cTableSz = lI2cTab.mUsedBytes,
        };

    // and finally send commands to camera
    if (CGD_LIB_FAILURE == cgd_ExecTable(&updateTable))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Sensor update error (CamId: %d)\n", aCamId);
        return CGD_LIB_FAILURE;
    }

    return CGD_LIB_SUCCESS;
} // new_sensor_Update()

/******************************************************************************/

#endif // else from defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))
