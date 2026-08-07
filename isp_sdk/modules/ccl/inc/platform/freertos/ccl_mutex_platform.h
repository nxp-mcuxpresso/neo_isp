/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

 /**
 * @file ccl_mutex_platform.h
 * @brief Includes and defines for mutex object specific to selected platform. Platform is
 * defined by the directory this file is in.
 */

#ifndef CCL_MUTEX_PLATFORM_H
#define CCL_MUTEX_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "semphr.h"

#include <stdbool.h>

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
// defines and statically initializes mutex variable
#define CCL_DEFINE_MUTEX_STATIC_INIT(_mutex) \
ccl_mutex_t (_mutex) = {false, false, NULL}

typedef struct
{
    /** @brief Indication if mutex can be called recursively */
    bool mRecursive;
    /** @brief Indication if mutex was already initialized */
    bool mInitialized;

    SemaphoreHandle_t mHandle;
    StaticSemaphore_t mMutex;
}ccl_mutex_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_MUTEX_PLATFORM_H */

