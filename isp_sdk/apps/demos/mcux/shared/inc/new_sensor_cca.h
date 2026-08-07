/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef NEW_SENSOR_CCA_H
#define NEW_SENSOR_CCA_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file new_sensor.h
 * @brief new sensor API
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include "cca.h"

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define NEW_SENSOR                   (0xABC) ///< TODO: define new sensor ID
#define NEW_SENSOR_DTP_ID            (2010U) ///< ID used in DTP for the sensor

/*==============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/
/******************************************************************************/
/**
 * @brief   OVX03C sensor specific parameters.
 * TODO: define new sensor specific parameters
 *******************************************************************************/
typedef struct
{
    uint32_t mI2CAddIdx; /**< sensor address used for read/write values from i2c bus*/
    uint32_t mOtpSensHCG; /**< HCG one time programmable value*/
    uint32_t mOtpSensLCG; /**< LCG one time programmable value*/
    uint32_t mOtpSensSPD; /**< SPD one time programmable value*/
    uint8_t mFps; /**< desired fps (ovx3c with AD ser/deser 30fps, TI serdeser 60fps) */
} new_sensor_Params_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Initialize interaction with camera control abstraction (CCA) library.
 *
 * @param[in]  acpOps  CCA camera operations container
 * TODO: define get ops API for the new sensor
 *******************************************************************************/
void new_sensor_GetCcaOps(cca_CamOps_t* const acpOps);

/******************************************************************************/
/**
 * @brief Gives opportunity to modify the system wide default configuration of CSI and ISI modules before being passed to CGD.
 *
 * @details Invoked from ccl_Open() before the cgd_Config() is executed.
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @param[in]  newExternalConfig  Camera CCL pipeline id
 *
 * @return CGD_LIB_SUCCESS if all ok, CGD_FAILURE otherwise
 *
 * TODO: Define CSI and ISI parameters in scope of cgd_CsiExtCfg_t and cgd_IsiExtCfg_t structures.
 *******************************************************************************/
cgd_Result new_sensor_ConfigureCgd(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cgd_SystemExtCfg_t* newExternalConfig);

/******************************************************************************/
/**
 * @brief Gives opportunity to interact with the sensor after it was initialized.
 *
 * @details Invoked from ccl_Open() after cgd_Config(). Do not invoke separately.
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 * @param[in, out]  apParams  sensor init parameter structure, e.g. the new_sensor_Params_t (e.g. OTP data)
 * @param[in]  aParamSize  size of parameter structure in bytes
 *
 * @return CGD_LIB_SUCCESS if all ok, CGD_FAILURE otherwise
 *
 * TODO: Follow the example implementation for the ox03c10 sensor, where OTP data are fetched
 * and populated to the apParams, which later become available in the Open CCA operation
 * defined in the new_sensor_cca.c.
 *******************************************************************************/
cgd_Result new_sensor_Init(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apParams, size_t mParamSize);

/******************************************************************************/
/**
 * @brief Invoked from the Camera Control Library (CCL) in response to ccl_Start().
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return CGD_LIB_SUCCESS if all ok, CGD_FAILURE otherwise
 *******************************************************************************/
cgd_Result new_sensor_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/******************************************************************************/
/**
 * @brief Updates sensor settings through v4l2 driver interaction for the Camera Control Library (CCL).
 *
 * @details Called by CCL internally. Do not invoke separately
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 * @param[in]  apParams  pointer to sensor control parameter structure cca_SensPrm_t, for AEC, AWB, ...
 * @param[in]  aParamSize  size of parameter structure in bytes
 *
 * @return CGD_LIB_SUCCESS if all ok, CGD_FAILURE otherwise
 *
 * TODO: Implement parameter update API.  for the new sensor
 * Use the cca_GetI2cTab() to convert the apParams to CGD command table and execute it which applies new settings to camera sensor.
 * Follow the example implementation for the ox03c10 sensor
 *******************************************************************************/
cgd_Result new_sensor_Update(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_SensPrm_t* apSensPrm);

/******************************************************************************/
/**
 * @brief Invoked from the Camera Control Library (CCL) in response to ccl_Stop().
 *
 * @param[in]  aHwId  ISP HW id corresponding to the sensor (kept always to ISP_HWID_0)
 * @param[in]  aCamId  Camera CCL pipeline id
 *
 * @return CGD_LIB_SUCCESS if all ok, CGD_FAILURE otherwise
 *******************************************************************************/
cgd_Result new_sensor_Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

#ifdef __cplusplus
}
#endif
#endif /* #ifndef NEW_SENSOR_CCA_H */
