/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

 /**
 * @file ccl_semaphore_platform.h
 * @brief Includes and defines for semaphore object specific to selected platform. Platform is
 * defined by the directory this file is in.
 */

#ifndef CCL_SEMAPHORE_PLATFORM_H
#define CCL_SEMAPHORE_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "semphr.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

typedef struct
{
    SemaphoreHandle_t mHandle;
    StaticSemaphore_t mSem;
}ccl_sem_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_SEMAPHORE_PLATFORM_H */

