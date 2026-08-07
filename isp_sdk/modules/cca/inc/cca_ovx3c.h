/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCA_OVX3C_H
#define CCA_OVX3C_H

/******************************************************************************/
/**
 * @file cca_ovx3c.h
 * @brief Camera abstraction library for ovx03c sensor
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "cca.h"

/** @addtogroup cca_ovx3c_sens ox03c10 sensor support code
 * Enables use of ox03c10 sensor.
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define SENSOR_OV_X3C                (0x3C)
#define CCA_X3C_4EXP /* define CCA_X3C_3EXP for 3exposure mode, CCA_X3C_4EXP for 4exposure mode*/

#define SENSOR_OV_X3C_TI (0x3C6) /* sensor with Texas Instruments ser/deser */

/*==============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/
/******************************************************************************/
/**
 * @brief   OVX03C sensor specific parameters.
 *******************************************************************************/
typedef struct
{
    uint8_t mI2CAddIdx; /**< sensor address idx used for read the OTP values from i2c bus*/
    uint32_t mOtpSensHCG; /**< HCG one time programmable value*/
    uint32_t mOtpSensLCG; /**< LCG one time programmable value*/
    uint32_t mOtpSensSPD; /**< SPD one time programmable value*/
    uint8_t mFps; /**< desired fps (ovx3c with AD ser/deser 30fps, TI serdeser 60fps) */
} cca_ovx3c_Params_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Initialize interaction with camera abstraction library.
 *
 * @param[in]  acpOps  camera operations container
 *******************************************************************************/
void cca_ovx3c_GetOps(cca_CamOps_t* const acpOps);

#ifdef __cplusplus
}
#endif

/** @} */ // addtogroup cca_ovx3c_sens
#endif /* #ifndef CCA_OVX3C_H */
