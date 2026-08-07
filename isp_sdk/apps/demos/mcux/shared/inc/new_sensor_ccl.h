/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef NEW_SENSOR_CCL_H
#define NEW_SENSOR_CCL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file new_sensor_ccl.h
 * @brief implements camera control library
 */

#include <stdint.h>
#include <stdlib.h>

#include "cca.h"

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
cgd_Result cgd_New_Sensor_Init(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apParams, size_t mParamSize);

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
cgd_Result cgd_New_Sensor_Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_SensPrm_t* apSensPrm);

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
cgd_Result cgd_New_Sensor_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/******************************************************************************/
/**
 * @brief Configure CGD system for selected sensor, called once before Init
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
cgd_Result cgd_New_Sensor_ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cgd_SystemExtCfg_t* newExternalConfig);

/******************************************************************************/
/**
 * @brief Stop sensor from the Camera Control Library (CCL).
 *
 * @details Called by CCL internally. Do not invoke separately
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
cgd_Result cgd_New_sensor_Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef NEW_SENSOR_CCL_H */
