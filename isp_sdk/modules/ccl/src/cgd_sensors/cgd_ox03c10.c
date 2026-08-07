/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cgd_ox03c10.c
 * @brief implements user-space setup of ox03c10 camera
 *******************************************************************************/

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "ilog.h"

#if defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))

#include "cgd_i2c.h"
#include "cgd_ox03c10.h"
#include "cca_ovx3c.h"
#include "cgd_types.h"
#include "cgd_config.h"
#include "cgd_ox03c10_i2c.h"

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
static const uint8_t sgOx03cI2cAdrTable[] = {
    [OX03C_I2C_DESERIALIZER_IDX] = OX03C_I2C_DESERIALIZER,
    [OX03C_I2C_SERIALIZER_IDX]   = OX03C_I2C_SERIALIZER,
    [OX03C_I2C_SERIALIZER1_IDX]  = OX03C_I2C_SERIALIZER1,
    [OX03C_I2C_SERIALIZER2_IDX]  = OX03C_I2C_SERIALIZER2,
    [OX03C_I2C_SERIALIZER3_IDX]  = OX03C_I2C_SERIALIZER3,
    [OX03C_I2C_SERIALIZER4_IDX]  = OX03C_I2C_SERIALIZER4,
    [OX03C_I2C_CAMERA_IDX]       = OX03C_I2C_CAMERA,
    [OX03C_I2C_CAMERA1_IDX]      = OX03C_I2C_CAMERA1,
    [OX03C_I2C_CAMERA2_IDX]      = OX03C_I2C_CAMERA2,
    [OX03C_I2C_CAMERA3_IDX]      = OX03C_I2C_CAMERA3,
    [OX03C_I2C_CAMERA4_IDX]      = OX03C_I2C_CAMERA4,
};

/*==============================================================================
*                                  VARIABLES
==============================================================================*/
static bool sSensorInitialized = false;

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/******************************************************************************/
uint8_t cgd_ovx3c_getCamIdx(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    (void)aHwId;

    uint8_t cameraId = OX03C_I2C_CAMERA_IDX;

    /* is necessary to translate camId to address on I2C bus
       because CGD use broadcasting and redefine camera (& serializer) address */
    switch (aCamId)
    {
        case ISPDRV_CAM_PIPE_ID_0:
        case ISPDRV_CAM_PIPE_ID_4:
        {
            cameraId = OX03C_I2C_CAMERA1_IDX;
        }
        break;
        case ISPDRV_CAM_PIPE_ID_1:
        case ISPDRV_CAM_PIPE_ID_5:
        {
            cameraId = OX03C_I2C_CAMERA2_IDX;
        }
        break;
        case ISPDRV_CAM_PIPE_ID_2:
        case ISPDRV_CAM_PIPE_ID_6:
        {
            cameraId = OX03C_I2C_CAMERA3_IDX;
        }
        break;
        case ISPDRV_CAM_PIPE_ID_3:
        case ISPDRV_CAM_PIPE_ID_7:
        {
            cameraId = OX03C_I2C_CAMERA4_IDX;
        }
        break;
        default:
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Unknown camera pipe!\n");
        }
    }

    return cameraId;
} // cgd_ovx3c_getCamIdx


/* Table for reading 9 bytes from OTP memory to provided buffer */
DDR_INIT static cgd_Cmd_t sgCgdOx03c10Otp[] =
{
    {0, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x01}}, /* start streaming for OTP load*/
    {0, CGD_CMD_DELAY,                      {33}}, /* wait for otp prepare */
    {0, CGD_CMD_WRITE,       {0x02U, 0x70, 0x57}}, /* seek to 0x7057 */
    {0,  CGD_CMD_READ,                      {3U}}, /* read 3 bytes */
    {0, CGD_CMD_WRITE,       {0x02U, 0x70, 0x5B}},
    {0,  CGD_CMD_READ,                      {3U}},
    {0, CGD_CMD_WRITE,       {0x02U, 0x70, 0x5F}},
    {0,  CGD_CMD_READ,                      {3U}},
    {0, CGD_CMD_WRITE,       {0x02U, 0x30, 0x0A}}, /* Read Sensor ID*/
    {0,  CGD_CMD_READ,                      {3U}},

    {0, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0x00}}, // GH start
    {0, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x00}}, // Streaming off
    {0, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0x10}},
    {0, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0xA0}}, // GH run

    {0, CGD_CMD_DELAY,                      {33}}, /* wait for stop */
};

/******************************************************************************/
cgd_Result cgd_ovx3c_Init(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apParams, size_t mParamSize)
{
    if (mParamSize != sizeof(cca_ovx3c_Params_t))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Bad size for ovx3c parameters. hwid: %d, camid: %d\n", aHwId, aCamId);
        return CGD_LIB_FAILURE;
    }

    cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice((cgd_IsiChannel_t)aCamId);

    if (isiDev == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    /* Provide configuration for OS08A camera, start streaming for OTP data load possible */
    if (sSensorInitialized == false)
    {
        sSensorInitialized = true;

        if (CGD_LIB_FAILURE == cgd_PrepareAndExecTable(isiDev->mCsiId, &sgCgdOX03C10Config))
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CGD: failed to exec table pCameraInitTable.\n");
            return CGD_LIB_FAILURE;
        }
    }

    /// read OTP data
    /*
    val1; [23:16] = reg(0x7057), [15:8] = reg(0x7058), [7:0] = reg(0x7059)
    val2; [23:16] = reg(0x705B), [15:8] = reg(0x705C), [7:0] = reg(0x705D)
    val3; [23:16] = reg(0x705F), [15:8] = reg(0x7060), [7:0] = reg(0x7061)
    */

    cca_ovx3c_Params_t* paramsOvx3c = apParams;
    paramsOvx3c->mI2CAddIdx         = cgd_ovx3c_getCamIdx(aHwId, aCamId);

    for (uint32_t i = 0; i < sizeof(sgCgdOx03c10Otp) / sizeof(cgd_Cmd_t); i++)
    {
        sgCgdOx03c10Otp[i].mDevId = paramsOvx3c->mI2CAddIdx;
    }

    uint8_t buffer[12];
    memset(&buffer, 0, sizeof(buffer));

    cgd_CmdTable_t table;
    memset(&table, 0, sizeof(table));
    table.mPortIdx    = isiDev->mCsiId;
    table.ympI2cTable = sgCgdOx03c10Otp;
    table.mI2cTableSz = sizeof(sgCgdOx03c10Otp);
    table.mpSaveBuff  = buffer;
    table.mSaveBuffSz = sizeof(buffer);

    if (CGD_LIB_FAILURE == cgd_ExecTable(&table))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "OX03C10 failed to read OTP data\n");
        paramsOvx3c->mOtpSensHCG = 0;
        paramsOvx3c->mOtpSensLCG = 0;
        paramsOvx3c->mOtpSensSPD = 0;
        return CGD_LIB_FAILURE;
    }
    else
    {
        if (buffer[9] != 0x58 || buffer[10] != 0x03 || buffer[11] != 0x43) // from manual page 170 - Sensor Id
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "OX03C10 wrong CHIP ID, please check sensor type, hwid: %d, camid: %d\n", aHwId, aCamId);
            return CGD_LIB_FAILURE;
        }
        else
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "OX03C10 correct CHIP ID found, hwid: %d, camid: %d\n", aHwId, aCamId);
        }

        uint32_t val1 = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
        uint32_t val2 = buffer[3] << 16 | buffer[4] << 8 | buffer[5];
        uint32_t val3 = buffer[6] << 16 | buffer[7] << 8 | buffer[8];

        paramsOvx3c->mOtpSensHCG = val1;
        paramsOvx3c->mOtpSensLCG = val2;
        paramsOvx3c->mOtpSensSPD = val3;
        ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "OX03C OTP readout success: HCG=0x%x LCG=0x%x SPD=0x%x hwid: %d, camid: %d\n", paramsOvx3c->mOtpSensHCG, paramsOvx3c->mOtpSensLCG, paramsOvx3c->mOtpSensSPD, aHwId, aCamId);
    }

    return CGD_LIB_SUCCESS;
} // v4l_ovx3c_Init()

/******************************************************************************/
cgd_Result cgd_ovx3c_Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_SensPrm_t* apSensPrm)
{
    cgd_IsiDevice_t* isiDev  = cgd_GetIsiDevice((cgd_IsiChannel_t)aCamId);

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
}

/******************************************************************************/
cgd_Result cgd_ovx3c_ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cgd_SystemExtCfg_t* newExternalConfig)
{
    (void)aHwId;

    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice((cgd_IsiChannel_t)aCamId);

    if (lpIsiDev == NULL)
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

    /* Set DPHY bit rate to 1600 instead 1500 Mbps */
    lpCsiCfg->mMbps  = 1500;
    lpCsiCfg->mLanes = 4; // use 4 lanes

    for (uint8_t index = 0; index < CGD_CSI_VC_CNT; index++)
    {
#if defined(LINUX_EMB) || defined(GHS)
        lpCsiCfg->mDataType[index] = CSI_DT_RAW16;
#elif SDK_OS_FREE_RTOS
        lpCsiCfg->mDataType[index] = 0x2EU; //<- kCSI2RX_DataTypeRAW16;
#endif
    }

    /* Assign address tables to the CSI configuration */
    lpCsiCfg->mI2cAdrCnt = sizeof(sgOx03cI2cAdrTable);
    lpCsiCfg->mpI2cAdr   = sgOx03cI2cAdrTable;

    /* setup conf tables for each sensor */
    lpCsiCfg->mpcInitArr   = &sgCgdOvx3cInitialization;
    lpCsiCfg->mpcStreamEn  = &sgCgdOvx3cVideoEnabled;
    lpCsiCfg->mpcStreamDis = &sgCgdOvx3cVideoDisable;
    lpCsiCfg->mpcCloseArr  = &sgCgdOvx3cClose;

    return CGD_LIB_SUCCESS;
}
/******************************************************************************/
cgd_Result cgd_ovx3c_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    uint8_t camIdx          = cgd_ovx3c_getCamIdx(aHwId, aCamId);
    cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice((cgd_IsiChannel_t)aCamId);

    if (isiDev == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    cgd_Cmd_t sgCgdOx03c10Otp[] =
        {
            {camIdx, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x01}}, /* start streaming for OTP load*/
    };

    cgd_CmdTable_t table;
    memset(&table, 0, sizeof(table));
    table.mPortIdx    = isiDev->mCsiId;
    table.ympI2cTable = sgCgdOx03c10Otp;
    table.mI2cTableSz = sizeof(sgCgdOx03c10Otp);

    if (CGD_LIB_FAILURE == cgd_ExecTable(&table))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "OX03C10 failed to start\n");
        return CGD_LIB_FAILURE;
    }

    return CGD_LIB_SUCCESS;
}
/******************************************************************************/


#endif // else from defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))
