
/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_OX03C10_I2C_H
#define CGD_OX03C10_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * @file     cgd_ox03c10_i2c.h
 * @brief    The i2c config tables for OVX3C camera sensor
 ****************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <stddef.h>
#include "cgd_types.h"
#include "cgd_ox03c10.h"

//*********************************************************
// DEFINES
//*********************************************************

/*****************************************************************************
 * TYPES
 *****************************************************************************/

//*********************************************************
// ENUMS
//*********************************************************

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/
/**
 * @brief Prepare whole I2C system for streaming.
 */
extern cgd_CmdTable_t sgCgdOvx3cInitialization;

/**
 * @brief Start IMX95 I2C devices straming
 */
extern cgd_CmdTable_t sgCgdOvx3cVideoEnabled;

/**
 * @brief Start IMX95 I2C devices straming, used for testing
 */
extern cgd_CmdTable_t sgCgdOvx3cVideoEnableOnAllCameras;

/**
 * @brief Stop IMX95 I2C devices streaming
 */
extern cgd_CmdTable_t sgCgdOvx3cVideoDisable;

/**
 * @brief Camera initialization sequence
 */
extern cgd_CmdTable_t sgCgdOX03C10Config;

/**
 * @brief Camera closing sequence
 */
extern cgd_CmdTable_t sgCgdOvx3cClose;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_OX03C10_I2C_H */
