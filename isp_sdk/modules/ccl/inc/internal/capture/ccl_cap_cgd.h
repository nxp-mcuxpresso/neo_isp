/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_CAP_CGD_H
#define CCL_CAP_CGD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ccl_cap_cgd.h
 * @brief Internal CCL header implementing CGD capture method
 */

#include "ccl_internal.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/
#define BUF_COUNT_MAX_CGD (10U)
/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/**
 * @brief Initializes capture method functions to CGD based capture
 *
 * @param apOps - target capture functions
 *
 * @returns 0 if all ok, -1 otherwise
 */
int32_t ccl_CapOps_Cgd(ccl_CapOps_t* apOps, ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_CAP_CGD_H */
