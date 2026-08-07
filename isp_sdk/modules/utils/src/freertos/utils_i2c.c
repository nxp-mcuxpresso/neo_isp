/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  i2c.cpp
 * @brief userspace i/f to access I2C devices.
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "FreeRTOS.h"
#include "semphr.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "fsl_lpi2c.h"
#include "hal_clock.h"
#include "utils_i2c.h"
#include "ilog.h"
#include "projdefs.h"

/*==============================================================================
*                         EXTERNAL GLOBAL variables
==============================================================================*/

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
 /**
 * @brief driver handle.
 */
typedef struct _i2c_handle
{
    LPI2C_Type* base; /*!< LPI2C base address */
    lpi2c_master_handle_t drv_handle; /*!< Handle of the underlying driver, treated as opaque by the RTOS layer */
    status_t async_status;
    SemaphoreHandle_t mutex; /*!< Mutex to lock the handle during a trasfer */
    SemaphoreHandle_t semaphore; /*!< Semaphore to notify and unblock task when transfer ends */
    StaticSemaphore_t mutexData;
    StaticSemaphore_t semaphoreData;
} i2c_handle_t;

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/

DDR_NOINIT static i2c_handle_t sgUtilI2CHandle[9];

/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/
static void LPI2C_RTOS_Callback(LPI2C_Type* base, lpi2c_master_handle_t* drv_handle, status_t status, void* userData)
{
    i2c_handle_t* handle  = (i2c_handle_t*)userData;
    BaseType_t reschedule = pdFALSE;
    handle->async_status  = status;

    if (pdPASS == xSemaphoreGiveFromISR(handle->semaphore, &reschedule))
    {
        portYIELD_FROM_ISR(reschedule);
    }
}

int32_t i2c_Open(i2c_Dev_t* apDev)
{
    int32_t lRet = I2C_LIB_FAILURE;

    if (apDev != NULL && apDev->mDescriptor > 0 && apDev->mDescriptor < 9)
    {
        i2c_handle_t* handle                 = &sgUtilI2CHandle[apDev->mDescriptor];
        clock_ip_name_t const kLpi2cClocks[] = LPI2C_CLOCKS;

        LPI2C_Type* const s_i2cBases[]              = LPI2C_BASE_PTRS;

        (void)memset(handle, 0, sizeof(i2c_handle_t));

        handle->mutex = xSemaphoreCreateMutexStatic(&handle->mutexData);
        if (handle->mutex == NULL)
        {
            return lRet;
        }

        handle->semaphore = xSemaphoreCreateBinaryStatic(&handle->semaphoreData);
        if (handle->semaphore == NULL)
        {
            vSemaphoreDelete(handle->mutex);
            return lRet;
        }

        /* Get device registers from descriptor */
        handle->base         = s_i2cBases[apDev->mDescriptor];
        hal_clk_id_e clockId = kLpi2cClocks[apDev->mDescriptor];

        uint64_t rate = HAL_ClockGetRate(clockId);
        assert(rate < UINT32_MAX);

        lpi2c_master_config_t masterConfig;

        LPI2C_MasterGetDefaultConfig(&masterConfig);

        masterConfig.baudRate_Hz = 400000U;

        /* Initialize the LPI2C master peripheral */
        LPI2C_MasterInit(handle->base, &masterConfig, rate);
        LPI2C_MasterTransferCreateHandle(handle->base, &handle->drv_handle, LPI2C_RTOS_Callback, (void*)handle);

        lRet = I2C_LIB_SUCCESS;
    }
    else
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Error: Invalid I2C descriptor. The descriptor value must be between 1 and 8 (LPI2C1 to LPI2C8).\r\n");
    }

    return lRet;
} // i2c_Open()

/*******************************************************************************/

void i2c_Close(i2c_Dev_t* apDev)
{
    assert(apDev->mDescriptor >= 0 && apDev->mDescriptor < 9);

    LPI2C_MasterDeinit(sgUtilI2CHandle[apDev->mDescriptor].base);
    vSemaphoreDelete(sgUtilI2CHandle[apDev->mDescriptor].semaphore);
    vSemaphoreDelete(sgUtilI2CHandle[apDev->mDescriptor].mutex);
} // i2c_Close()

/*******************************************************************************/

int32_t i2c_Recv(const i2c_Dev_t* apcDev, uint8_t* apBuf, const ssize_t acByteCnt)
{
    int32_t lRet = I2C_LIB_FAILURE;

    lpi2c_master_transfer_t xfer;

    if (apcDev->mDescriptor < 0 || apcDev->mDescriptor >= 9 || apBuf == NULL || acByteCnt <= 0)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Error: Invalid parameters for i2c_Recv.\r\n");
        return lRet;
    }

    xfer.flags          = 0;
    xfer.slaveAddress   = apcDev->mAddr;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = 0; // don't send address separately
    xfer.subaddressSize = 0;
    xfer.data           = (void*)apBuf;
    xfer.dataSize       = acByteCnt;

    status_t reVal = LPI2C_MasterTransferBlocking(sgUtilI2CHandle[apcDev->mDescriptor].base, &xfer);

    switch (reVal)
    {
        case kStatus_Success:
            lRet = I2C_LIB_SUCCESS;
            break;
        case kStatus_LPI2C_Busy:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_Busy) Another master is currently utilizing the bus.\r\n");
            break;
        case kStatus_LPI2C_Nak:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_Nak) The slave device sent a NAK in response to a byte.\r\n");
            break;
        case kStatus_LPI2C_FifoError:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_FifoError) FIFO under run or overrun.\r\n");
            break;
        case kStatus_LPI2C_ArbitrationLost:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_ArbitrationLost) Arbitration lost error.\r\n");
            break;
        case kStatus_LPI2C_PinLowTimeout:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_PinLowTimeout) SCL or SDA were held low longer than the timeout.\r\n");
            break;
    }

    return lRet;
} // i2c_Recv()

/*******************************************************************************/

int32_t i2c_Send(const i2c_Dev_t* apcDev, const uint8_t* apcBuf, const ssize_t acByteCnt)
{
    int32_t lRet = I2C_LIB_FAILURE;

    assert(apcDev->mDescriptor >= 0 && apcDev->mDescriptor < 9);
    if (acByteCnt < 0)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Error: Invalid parameters for i2c_Send.\r\n");
        return I2C_LIB_FAILURE;
    }

    i2c_handle_t* handle = &sgUtilI2CHandle[apcDev->mDescriptor];

    /* Lock resource mutex */
    if (xSemaphoreTake(handle->mutex, portMAX_DELAY) != pdTRUE)
    {
        return kStatus_LPI2C_Busy;
    }

    lpi2c_master_transfer_t xfer;

    xfer.flags          = 0;
    xfer.slaveAddress   = apcDev->mAddr;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = 0; // don't send address separately
    xfer.subaddressSize = 0;
    xfer.data           = (void*)apcBuf;
    xfer.dataSize       = acByteCnt;

    status_t reVal = LPI2C_MasterTransferNonBlocking(handle->base, &handle->drv_handle, &xfer);

    switch (reVal)
    {
        case kStatus_Success:
            lRet = I2C_LIB_SUCCESS;
            break;
        case kStatus_LPI2C_Busy:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_Busy) Another master is currently utilizing the bus.\r\n");
            break;
        case kStatus_LPI2C_Nak:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_Nak) The slave device sent a NAK in response to a byte.\r\n");
            break;
        case kStatus_LPI2C_FifoError:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_FifoError) FIFO under run or overrun.\r\n");
            break;
        case kStatus_LPI2C_ArbitrationLost:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_ArbitrationLost) Arbitration lost error.\r\n");
            break;
        case kStatus_LPI2C_PinLowTimeout:
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "(kStatus_LPI2C_PinLowTimeout) SCL or SDA were held low longer than the timeout.\r\n");
            break;
    }

    if (lRet == I2C_LIB_SUCCESS)
    {
        /* Wait for transfer to finish */
        (void)xSemaphoreTake(handle->semaphore, portMAX_DELAY);
    }

    /* Unlock resource mutex */
    (void)xSemaphoreGive(handle->mutex);

    return lRet;
} // i2c_Send()

/*******************************************************************************/

int32_t i2c_SetAdress(i2c_Dev_t* apDev, const long acAddress)
{
    apDev->mAddr = (u_int8_t)(acAddress & 0xFF);

    return I2C_LIB_SUCCESS;
} // i2c_SetAdress()
