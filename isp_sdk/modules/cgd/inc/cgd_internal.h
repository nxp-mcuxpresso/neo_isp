/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_INTERNAL_H
#define CGD_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  cgd_internal.h
 * @brief Camera Generic Driver internal header
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "cgd_types.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                    ENUMS
==============================================================================*/

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
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Method for checking all errors triggered in device registers
 *
 * @details It's necessary to fill error table port->mpErrorTable
 *          Method is called through gpCsiErrorHandler, but can be manually called
 *
 * @param  aDevId  CSI device
 *
 * @return       none
 *******************************************************************************/
void cgd_CsiChannelErrorCheck(cgd_CsiDeviceId_t aDevId);

/******************************************************************************/
/**
 * @brief Method for checking all errors triggered in device registers
 *
 * @details It's necessary to fill error table port->mpErrorTable
 *          Method is called through gpIsiErrorHandler, but can be manually called
 *
 * @return       none
 *******************************************************************************/
void cgd_IsIChannelErrorCheck(cgd_IsiChannel_t aChnId);

/******************************************************************************/
/**
 * @brief Check if buffer 1 is active for the specified ISI channel.
 *
 * @param aChnId ISI channel identifier
 *
 * @return true if buffer 1 is active, false otherwise
 *******************************************************************************/
bool cgd_IsiBuffer1Active(cgd_IsiChannel_t aChnId);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_H */
