
/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef NEW_SENSOR_CCL_I2C_H
#define NEW_SENSOR_CCL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * @file     new_sensor_ccl_i2c.h
 * @brief    The CGD command tables for NEW_SENSOR camera sensor
 ****************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <stddef.h>
#include "cgd_types.h"
#include "new_sensor_ccl.h"

//*********************************************************
// DEFINES
//*********************************************************

/*****************************************************************************
 * TYPES
 *****************************************************************************/

/**
 * @brief Enum with device addresses used on I2C bus
 * @details some addresses are default (0x27 deser, 0x40 ser, 0x36 sensor)
 * and another are remapped values with use address remaping feature on serializer chip
 * Is possible to access each sensors individually or broadcast to all (0x36)
 */
typedef enum
{
    NEW_SENSOR_I2C_CAMERA = 0x36, // universal camera address
} cgd_NewSensorI2CAddress_t;

/**
 * @brief I2C hardware address ID used in command tables (cgd_Cmd_t)
 *
 */
typedef enum
{
    NEW_SENSOR_I2C_CAMERA_IDX = 0U,
} cgd_NewSensorDeviceI2CId_t;

//*********************************************************
// ENUMS
//*********************************************************

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/
/**
 * @brief CGD command tables for the NEW_SENSOR camera.
 *
 * @details Each table is a sequence of CGD commands (see cgd_CmdIds_t) executed
 *          on the I2C bus by cgd_ExecTable(). The CCL flow uses one table per
 *          stage of the camera lifecycle:
 *          - sgCgdNewSensorInitialization: configures the whole I2C chain
 *            (deserializer, serializer, sensor) once during initialization,
 *            before streaming. Assigned to cgd_CsiExtCfg_t::mpcInitArr and run
 *            by cgd_Config().
 *          - sgCgdNewSensorVideoEnabled: starts data streaming from the
 *            devices. Assigned to cgd_CsiExtCfg_t::mpcStreamEn and run by
 *            cgd_StreamOn().
 *          - sgCgdNewSensorVideoDisable: stops data streaming. Assigned to
 *            cgd_CsiExtCfg_t::mpcStreamDis and run by cgd_StreamOff().
 *          - sgCgdNewSensorClose: closing sequence that powers down or resets
 *            the devices. Assigned to cgd_CsiExtCfg_t::mpcCloseArr and run by
 *            cgd_Close().
 */

/**
 * @brief Configure the whole I2C chain for streaming, run once at initialization.
 */
extern cgd_CmdTable_t sgCgdNewSensorInitialization;

/**
 * @brief Start streaming on the I2C devices.
 */
extern cgd_CmdTable_t sgCgdNewSensorVideoEnabled;

/**
 * @brief Stop streaming on the I2C devices.
 */
extern cgd_CmdTable_t sgCgdNewSensorVideoDisable;

/**
 * @brief Camera closing sequence.
 */
extern cgd_CmdTable_t sgCgdNewSensorClose;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef NEW_SENSOR_CCL_I2C_H */
