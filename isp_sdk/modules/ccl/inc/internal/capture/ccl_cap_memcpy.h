/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_CAP_MEMCPY_H
#define CCL_CAP_MEMCPY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ccl_cap_memcpy.h
 * @brief Internal CCL header implementing memory copy capture method
 */


#include "ccl_internal.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
#define PLANE_COUNT_MIN (1U)
#define PLANE_COUNT_MAX (CCL_INBUF_PLANE_CNT)
#define BUF_COUNT_MAX_MEMCPY (10U)

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/**
 * @brief Initializes capture method functions to file based capture
 *
 * @param apOps - target capture functions
 */
int32_t ccl_CapOps_Memcpy(ccl_CapOps_t *apOps);

#ifdef __cplusplus
}
#endif


#endif /* #ifndef CCL_CAP_MEMCPY_H */
