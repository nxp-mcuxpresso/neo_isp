/*******************************************************************************
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_I2C_H
#define UTILS_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  utils_i2c.h
 * @brief I2C accessibility from user space
 *******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/***************************************************************************
 * @brief          Defines return codes, -1 => failure, 0 => success
 ***************************************************************************/
typedef enum I2C_LIB_RESULT
{
    I2C_LIB_SUCCESS = 0,
    I2C_LIB_FAILURE = -1
} I2C_LIB_RESULT;
/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
#ifdef GHS
    struct I2CDeviceStruct;
    struct I2CBusStruct;

    /* For each unique I2C target address a Device must be opened.
       Maximum Number of simultaneously used I2C Devices (addresses) */
    #define I2C_DEV_COUNT 32
#endif

/******************************************************************************/
/**
 * @brief I2C device defining structure
 *******************************************************************************/

typedef struct
{
    int32_t mDescriptor; ///< linux file descriptor/freertos registry descriptor
    int32_t mBusId; ///< I2C bus ID
    uint8_t mAddr; ///< I2C device address
#ifdef GHS
    struct I2CDeviceStruct *mDevice; /** currently selected Communication I2C device handle */
    struct I2CBusStruct *mBus; /** I2C bus handle */
    long mDevAddresses[I2C_DEV_COUNT]; /** all opened I2C addresses */
    struct I2CDeviceStruct *mDevices[I2C_DEV_COUNT]; /** all opened I2C Devices handles */
#endif
} i2c_Dev_t;

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
 * @brief Opens interaction with specified I2C device
 *
 * @param apDev pointer to the device
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t i2c_Open(i2c_Dev_t* apDev);

/******************************************************************************/
/**
 * @brief Terminates interaction with specified I2C device
 *
 * @param apDev pointer to the device
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
void i2c_Close(i2c_Dev_t* apDev);

/******************************************************************************/
/**
 * @brief Receives requested number of bytes from the device
 *
 * @param apDev pointer to the device
 * @param apBuf pointer to data buffer to be filled
 * @param acByteCnt number of bytes to read
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t i2c_Recv(const i2c_Dev_t* apcDev, uint8_t* apBuf, const ssize_t acByteCnt);

/******************************************************************************/
/**
 * @brief Sends requested number of bytes to the device
 *
 * @param apDev pointer to the device
 * @param apcBuf pointer to data to be written
 * @param acByteCnt number of bytes to write
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t i2c_Send(const i2c_Dev_t* apcDev, const uint8_t* apcBuf, const ssize_t acByteCnt);

/****************************************************************************/
/**
 * @brief Set device address with ioctl command
 *
 * @param apDev pointer to the device
 * @param  acAddress  requested address
 *
 * @returns 0 if all ok, -1 otherwise
 *
 ****************************************************************************/
int32_t i2c_SetAdress(i2c_Dev_t* apDev, const long acAddress);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UTILS_I2C_H */
