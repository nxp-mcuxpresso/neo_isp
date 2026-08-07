/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

 /**
 * @file ccl_platform
 * @brief Adds includes and defines specific to FreeRtos platform.
 */

#ifndef CCL_PLATFORM_FREERTOS_H
#define CCL_PLATFORM_FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

#include "task.h"
#include "semphr.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
typedef SemaphoreHandle_t PLATFORM_SEMAPHORE_T;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_PLATFORM_FREERTOS_H */

