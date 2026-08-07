/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_FREERTOS_CONFIG
#define CGD_FREERTOS_CONFIG

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * @file     cgd_config.h
 * @brief    The system config of FREERTOS
 ****************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <stddef.h>
#include "cgd_types.h"

//*********************************************************
// DEFINES
//*********************************************************

/**
 * @brief what i2c control file is used - /dev/i2c-%u
 */
#define CGD_I2C_MIPI_COMBO 1
#define CGD_I2C_MIPI_CSI 2

/*****************************************************************************
 * TYPES
 *****************************************************************************/

//*********************************************************
// ENUMS
//*********************************************************
/**
 * @brief Enum with pixel data types
 */
typedef enum
{
    CSI_DT_YUV420_8B          = 1 << 0,
    CSI_DT_YUV420_8B_LEGACY   = 1 << 1,
    CSI_DT_YUV420_8B_SHIFTED  = 1 << 4,
    CSI_DT_YUV420_10B_SHIFTED = 1 << 5,
    CSI_DT_YUV422_8B          = 1 << 6,
    CSI_DT_YUV422_10B         = 1 << 7,
    CSI_DT_RGB444             = 1 << 8,
    CSI_DT_RGB555             = 1 << 9,
    CSI_DT_RGB565             = 1 << 10,
    CSI_DT_RGB666             = 1 << 11,
    CSI_DT_RGB888             = 1 << 12,
    CSI_DT_RAW24              = 1 << 15,
    CSI_DT_RAW6               = 1 << 16,
    CSI_DT_RAW7               = 1 << 17,
    CSI_DT_RAW8               = 1 << 18,
    CSI_DT_RAW10              = 1 << 19,
    CSI_DT_RAW12              = 1 << 20,
    CSI_DT_RAW14              = 1 << 21,
    CSI_DT_RAW16              = 1 << 22,
    CSI_DT_RAW20              = 1 << 23,
} cgd_PixelDataTypes_t;

/**
 * @brief Enum with non-pixel data types, used in CSI0_NP_data_type_VCx
 */
typedef enum
{
    CSI_NP_DT_GEN_17   = 1 << 21,
    CSI_NP_DT_USR_37   = 1 << 20,
    CSI_NP_DT_USR_36   = 1 << 19,
    CSI_NP_DT_USR_35   = 1 << 18,
    CSI_NP_DT_USR_34   = 1 << 17,
    CSI_NP_DT_USR_33   = 1 << 16,
    CSI_NP_DT_USR_32   = 1 << 15,
    CSI_NP_DT_USR_31   = 1 << 14,
    CSI_NP_DT_USR_30   = 1 << 13,
    CSI_NP_DT_GEN_4    = 1 << 12,
    CSI_NP_DT_GEN_3    = 1 << 11,
    CSI_NP_DT_GEN_2    = 1 << 10,
    CSI_NP_DT_GEN_1    = 1 << 9,
    CSI_NP_DT_EMB_12   = 1 << 8,
    CSI_NP_DT_SHORT_0F = 1 << 7,
    CSI_NP_DT_SHORT_0E = 1 << 6,
    CSI_NP_DT_SHORT_0D = 1 << 5,
    CSI_NP_DT_SHORT_0C = 1 << 4,
    CSI_NP_DT_SHORT_0B = 1 << 3,
    CSI_NP_DT_SHORT_0A = 1 << 2,
    CSI_NP_DT_SHORT_09 = 1 << 1,
    CSI_NP_DT_SHORT_08 = 1 << 0,
} cgd_NonPixelDataTypes_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Whole  system together
 */
extern cgd_System_t sgCgdSystem;

/******************************************************************************/
/**
 * @brief ISI channel setup - buffers, methods etc
 */
extern cgd_SystemExtCfg_t sgCgdExtCfg;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_FREERTOS_CONFIG */
