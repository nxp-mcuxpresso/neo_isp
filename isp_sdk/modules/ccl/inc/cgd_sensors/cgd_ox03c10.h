/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_OVX3C_H
#define CGD_OVX3C_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file cgd_ox03c10.h
 * @brief implements user-space setup of ox03c10 camera
 */

#include <stdint.h>
#include <stdlib.h>

#include "cca.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/**
 * @brief Enum with device addresses used on I2C bus
 * @details some addresses are default (0x27 deser, 0x40 ser, 0x36 sensor)
 * and another are remapped values with use address remaping feature on serializer chip
 * Is possible to access each sensors individually or broadcast to all (0x36)
 */
typedef enum
{
    OX03C_I2C_DESERIALIZER = 0x27,
    OX03C_I2C_SERIALIZER   = 0x40,
    OX03C_I2C_SERIALIZER1  = 0x44, // new serializer addresses remaped after inicialization
    OX03C_I2C_SERIALIZER2  = 0x45,
    OX03C_I2C_SERIALIZER3  = 0x46,
    OX03C_I2C_SERIALIZER4  = 0x47,
    OX03C_I2C_CAMERA       = 0x36, // universal camera address
    OX03C_I2C_CAMERA1      = 0x48, // new camera addresses remaped after inicialization
    OX03C_I2C_CAMERA2      = 0x49,
    OX03C_I2C_CAMERA3      = 0x4A,
    OX03C_I2C_CAMERA4      = 0x4B,
} cgd_Ox03cI2CAddress_t;

/**
 * @brief I2C hardware address ID used in command tables (cgd_Cmd_t)
 *
 */
typedef enum
{
    OX03C_I2C_DESERIALIZER_IDX = 0U,
    OX03C_I2C_SERIALIZER_IDX   = 1U,
    OX03C_I2C_SERIALIZER1_IDX  = 2U,
    OX03C_I2C_SERIALIZER2_IDX  = 3U,
    OX03C_I2C_SERIALIZER3_IDX  = 4U,
    OX03C_I2C_SERIALIZER4_IDX  = 5U,
    OX03C_I2C_CAMERA_IDX       = 6U, /** Camera broadcast */
    OX03C_I2C_CAMERA1_IDX      = 7U, /** Single camera control for 4x OVX3C */
    OX03C_I2C_CAMERA2_IDX      = 8U,
    OX03C_I2C_CAMERA3_IDX      = 9U,
    OX03C_I2C_CAMERA4_IDX      = 10U,
} cgd_Ox03cDeviceI2CId_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Initialize sensor for the Camera Control Library (CCL). Called for each sensor separately.
 *
 * @details Called by CCL internally. Do not invoke separately
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 * @param[in, out]  apParams  sensor init parameter structure, e.g. the new_sensor_Params_t (e.g. OTP data)
 * @param[in]  aParamSize  size of parameter structure in bytes
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
cgd_Result cgd_ovx3c_Init(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apParams, size_t mParamSize);

/******************************************************************************/
/**
 * @brief Initialize sensor for the Camera Control Library (CCL).
 *
 * @details Called by CCL internally. Do not invoke separately
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 * @param[in, out]  apParams  sensor init parameter structure, e.g. the new_sensor_Params_t (e.g. OTP data)
 * @param[in]  aParamSize  size of parameter structure in bytes
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
cgd_Result cgd_ovx3c_Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_SensPrm_t* apSensPrm);

/******************************************************************************/
/**
 * @brief Start sensor from the Camera Control Library (CCL).
 *
 * @details Called by CCL internally. Do not invoke separately
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
cgd_Result cgd_ovx3c_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/******************************************************************************/
/**
 * @brief Configure CGD system for selected sensor, called once before Init
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
cgd_Result cgd_ovx3c_ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cgd_SystemExtCfg_t* newExternalConfig);

/******************************************************************************/
/**
 * @brief Translate CamPipeId to CamIdx adress used for I2C command
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return mpI2cAdr camera address on I2C bus
 *******************************************************************************/
uint8_t cgd_ovx3c_getCamIdx(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_OVX3C_H */
