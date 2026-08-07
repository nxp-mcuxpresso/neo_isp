/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  new_sensor_ccl_i2c.c
 * @brief Sensor i2c registry configuration class
 *******************************************************************************/
#include "new_sensor_ccl_i2c.h"
#include "cgd_types.h"
// #include "cgd_config.h"

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

/******************************************************************************/
cgd_Cmd_t sgCgdNewSensorInitializationArr[] =
    {
        {NEW_SENSOR_I2C_CAMERA_IDX, CGD_CMD_DELAY, {90}}, /* wait for start-up after gpio reset */
        /*Power on sequence*/
};

cgd_CmdTable_t sgCgdNewSensorInitialization =
    {
        .ympI2cTable = sgCgdNewSensorInitializationArr,
        .mI2cTableSz = sizeof(sgCgdNewSensorInitializationArr),
};

/******************************************************************************/
cgd_Cmd_t sgCgdNewSensorVideoEnabledArr[] =
    {
        {NEW_SENSOR_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x01}}, /* Streaming bit enable */
};

cgd_CmdTable_t sgCgdNewSensorVideoEnabled =
    {
        .ympI2cTable = sgCgdNewSensorVideoEnabledArr,
        .mI2cTableSz = sizeof(sgCgdNewSensorVideoEnabledArr),
};

/******************************************************************************/
cgd_Cmd_t sgCgdNewSensorVideoDisableArr[] =
    {
        {NEW_SENSOR_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x00}}, /* Streaming bit clear */
        {NEW_SENSOR_I2C_CAMERA_IDX, CGD_CMD_DELAY,                     {34U}}, /* Wait a maximum of 1 time frame. Worst case is 33.33ms. */
};

cgd_CmdTable_t sgCgdNewSensorVideoDisable =
    {
        .ympI2cTable = sgCgdNewSensorVideoDisableArr,
        .mI2cTableSz = sizeof(sgCgdNewSensorVideoDisableArr),
};

/******************************************************************************/

cgd_Cmd_t sgCgdNewSensorCloseArr[] =
    {
        {NEW_SENSOR_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x02, 0xD6, 0x80}}, /* Sensor power disable */
};

/******************************************************************************/

cgd_CmdTable_t sgCgdNewSensorClose =
    {
        .ympI2cTable = sgCgdNewSensorCloseArr,
        .mI2cTableSz = sizeof(sgCgdNewSensorCloseArr),
};

/******************************************************************************/
