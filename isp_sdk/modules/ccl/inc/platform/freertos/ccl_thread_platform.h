/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

 /**
 * @file ccl_thread_platform.h
 * @brief Includes and defines for thread object specific to selected platform. Platform is
 * defined by the directory this file is in.
 */

#ifndef CCL_THREAD_PLATFORM_H
#define CCL_THREAD_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>


/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

typedef struct
{
    /** @brief Client thread function to call from platform specific wrapper. */
    ccl_thread_fcn mStartFcn;
    /** @brief Arguments to pass to the client thread function. */
    void *mArg;

    StaticTask_t mTaskBuffer;
    StackType_t *mStack;
    uint32_t mStackSize;
    TaskHandle_t mHandle;

}ccl_thread_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_THREAD_PLATFORM_H */

