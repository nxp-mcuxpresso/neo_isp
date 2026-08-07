/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_GPIO_H
#define UTILS_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  utils_gpio.h
 * @brief GPIO accessibility from user space
 *
 * Example of usage:
 *
 *   gpio_Dev_t dev;
 *   dev.mChipId = 4; // select gpiochip4 device
 *
 *   gpio_Open(&dev); // open FD
 *
 *   printf("Chip name: %s\n", dev.mChipInfo.name);
 *   printf("Chip label: %s\n", dev.mChipInfo.label);
 *   printf("Number of lines: %d\n", dev.mChipInfo.lines); // count of all pins on selected device
 *
 *   gpio_SetLineOutput(&dev, 2); // setup output mode and write value to line/pin no. 2
 *   gpio_Write(&dev, 2, 1);
 *
 *   gpio_Close(&dev);
 *
 *******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*
 * The maximum size of name and label arrays.
 *
 * Must be a multiple of 8 to ensure 32/64-bit alignment of structs.
 */
#define UTILS_GPIO_MAX_NAME_SIZE 32

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/***************************************************************************
 * @brief          Defines return codes, -1 => failure, 0 => success
 ***************************************************************************/
typedef enum GPIO_LIB_RESULT
{
    GPIO_LIB_SUCCESS = 0,
    GPIO_LIB_FAILURE = -1
} GPIO_LIB_RESULT;
/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/******************************************************************************/
/**
 * @brief GPIO device defining structure
 *******************************************************************************/
typedef struct
{
#if defined(LINUX_EMB) || defined(LINUX_PC)
    uint8_t mChipId; /** @brief chip id, need be initialized manually - target to /dev/gpiochipX file*/
    int32_t mFd; /** @brief file descriptor, initialized automatically after Open() */
#elif SDK_OS_FREE_RTOS
    uint32_t mGpioHandle[((16U + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
    uint8_t direction; /** direction: 0 - in, 1 - out */
    uint8_t port;
    uint8_t pin;
#endif
} gpio_Dev_t;

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
 * @brief Opens interaction with specified GPIO device. Note: ChipId can be different on different Linux builds
 *
 * @param apDev pointer to the device structure, mChipId need to be initialized
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
GPIO_LIB_RESULT gpio_Open(gpio_Dev_t* apDev);

/******************************************************************************/
/**
 * @brief Find and opens GPIO device by chip label. Note: ChipId can be different on different Linux builds
 *
 * @param aLabel[IN] desired chip label to find
 * @param apDev[OUT] pointer to the device structure, need to be initialized
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
GPIO_LIB_RESULT gpio_FindChip(char aLabel[UTILS_GPIO_MAX_NAME_SIZE], gpio_Dev_t* apDev);

/******************************************************************************/
/**
 * @brief Terminates interaction with specified GPIO device
 *
 * @param apDev pointer to the device
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
void gpio_Close(gpio_Dev_t* apDev);

/******************************************************************************/
/**
 * @brief Check if line is in the OUTPUT mode (GPIO_V2_LINE_FLAG_OUTPUT)
 *
 * @param line_info  pointer to data structure with line info
 *
 * @returns true if is output, otherwise false
 *******************************************************************************/
bool gpio_IsLineOutput(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Check if line is Active LOW (GPIO_V2_LINE_FLAG_ACTIVE_LOW)
 *
 * @param line_info  pointer to data structure with line info
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
bool gpio_IsLineActiveLow(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Check if line is open drain (GPIO_V2_LINE_FLAG_OPEN_DRAIN)
 *
 * @param line_info  pointer to data structure with line info
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
bool gpio_IsLineOpenDrain(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Check if line is open source (GPIO_V2_LINE_FLAG_OPEN_SOURCE)
 *
 * @param line_info  pointer to data structure with line info
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
bool gpio_IsLineOpenSource(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Check if line is used (GPIO_V2_LINE_FLAG_USED)
 *
 * @param line_info  pointer to data structure with line info
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
bool gpio_IsLineUsed(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Set line to the OUTPUT mode (GPIO_V2_LINE_FLAG_OUTPUT)
 *
 * @param apcDev pointer to the device
 * @param acLineOffset  pointer to data structure with line info
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
GPIO_LIB_RESULT gpio_SetLineOutput(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Set line to the INPUT mode (GPIO_V2_LINE_FLAG_INPUT)
 *
 * @param apcDev pointer to the device
 * @param acLineOffset  pointer to data structure with line info
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
GPIO_LIB_RESULT gpio_SetLineInput(const gpio_Dev_t* apcDev, const uint32_t acLineOffset);

/******************************************************************************/
/**
 * @brief Set line flags
 *
 * @param apcDev pointer to the device
 * @param acLineOffset  pointer to data structure with line info
 * @param acFlags  desired flags on line
 *
 * @returns true if yes, otherwise false
 *******************************************************************************/
GPIO_LIB_RESULT gpio_SetLineFlags(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, const uint64_t acFlags);

/******************************************************************************/
/**
 * @brief Write requested @value to the pin (OUTPUT mode)
 *
 * @param apcDev pointer to the device
 * @param acLineOffset pin offset
 * @param acValue value to write
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
GPIO_LIB_RESULT gpio_Write(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, const uint8_t acValue);

/******************************************************************************/
/**
 * @brief Read requested @value from the pin (READ mode)
 *
 * @param apcDev pointer to the device
 * @param acLineOffset pin offset
 * @param apValue [out] value read
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
GPIO_LIB_RESULT gpio_Read(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, uint8_t* apValue);

/******************************************************************************/
/**
 * @brief GPIO line device poll
 *
 * @param apcDev pointer to the device
 * @param acLineOffset pin offset
 * @param acTimeout poll timeout in [ms], -1 for infinite
 *
 * @returns 0 when POLLIN, -1 otherwise (inc timeout)
 *******************************************************************************/
GPIO_LIB_RESULT gpio_Poll(const gpio_Dev_t* apcDev, const uint32_t acLineOffset, const uint32_t acTimeout);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UTILS_GPIO_H */
