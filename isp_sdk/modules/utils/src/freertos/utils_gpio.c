/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  utils_gpio.cpp
 * @brief userspace i/f to access GPIO devices.
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include "utils_gpio.h"
#include "ilog.h"
#include "fsl_adapter_gpio.h"
#include "fsl_debug_console.h"

/*==============================================================================
*                         EXTERNAL GLOBAL variables
==============================================================================*/

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/*******************************************************************************/
GPIO_LIB_RESULT gpio_Open(gpio_Dev_t* apDev)
{
    GPIO_LIB_RESULT lRet         = GPIO_LIB_SUCCESS;
    hal_gpio_status_t gpioStatus = kStatus_HAL_GpioSuccess;

    hal_gpio_pin_config_t cfg = {.direction = (hal_gpio_direction_t)apDev->direction,
                                 .port      = apDev->port,
                                 .pin       = apDev->pin};

    gpioStatus = HAL_GpioInit(apDev->mGpioHandle, &cfg);

    if (gpioStatus != kStatus_HAL_GpioSuccess)
    {
        lRet = GPIO_LIB_FAILURE;
        PRINTF("HAL_GpioInit failed.\r\n");
    }

    return lRet;
} // gpio_Open()

/*******************************************************************************/
GPIO_LIB_RESULT gpio_FindChip(char aLabel[UTILS_GPIO_MAX_NAME_SIZE], gpio_Dev_t* apDev)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_FindChip method not implemented \r\n");

    return lRet;
} // gpio_Open()

/*******************************************************************************/

void gpio_Close(gpio_Dev_t* apDev)
{
    (void)HAL_GpioDeinit(apDev->mGpioHandle);
} // gpio_Close()

/*******************************************************************************/
bool gpio_IsLineOutput(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    PRINTF("Utils GPIO gpio_IsLineOutput method not implemented \r\n");

    return false;
} // gpio_IsLineOutput()

/*******************************************************************************/
bool gpio_IsLineActiveLow(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    PRINTF("Utils GPIO gpio_IsLineActiveLow method not implemented \r\n");

    return false;
} // gpio_IsLineActiveLow()

/******************************************************************************/
GPIO_LIB_RESULT gpio_SetLineOutput(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_SetLineOutput method not implemented \r\n");

    return lRet;
}

/******************************************************************************/
GPIO_LIB_RESULT gpio_SetLineInput(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_SetLineOutput method not implemented \r\n");

    return lRet;
}

/*******************************************************************************/
GPIO_LIB_RESULT gpio_SetLineFlags(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, const uint64_t acFlags)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_SetLineFlags method not implemented \r\n");

    return lRet;
}

/*******************************************************************************/
bool gpio_IsLineOpenDrain(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_IsLineOpenDrain method not implemented \r\n");

    return lRet;
} // gpio_IsLineOpenDrain()

/*******************************************************************************/
bool gpio_IsLineOpenSource(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_IsLineOpenSource method not implemented \r\n");

    return lRet;
} // gpio_IsLineOpenSource()

/*******************************************************************************/
bool gpio_IsLineUsed(const gpio_Dev_t* apcDev, const uint32_t acLineOffset)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    PRINTF("Utils GPIO gpio_IsLineUsed method not implemented \r\n");

    return lRet;
} // gpio_IsLineUsed()

/*******************************************************************************/
GPIO_LIB_RESULT gpio_Write(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, const uint8_t acValue)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_SUCCESS;

    (void)HAL_GpioSetOutput(((gpio_Dev_t*)apcDev)->mGpioHandle, acValue);

    return lRet;
} // gpio_Write()

/*******************************************************************************/
GPIO_LIB_RESULT gpio_Read(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, uint8_t* apValue)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_SUCCESS;

    (void)HAL_GpioGetInput(((gpio_Dev_t*)apcDev)->mGpioHandle, apValue);

    return lRet;
} // gpio_Read

/*******************************************************************************/
GPIO_LIB_RESULT gpio_Poll(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, const uint32_t acTimeout)
{
    GPIO_LIB_RESULT lRet = GPIO_LIB_FAILURE;

    return lRet;
} // gpio_Poll
