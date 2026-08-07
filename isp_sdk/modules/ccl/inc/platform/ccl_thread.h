/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_THREAD_H
#define CCL_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include <stdint.h>

// only needed for ccl_err_t
#include "ccl.h"


// Note that stack sizes below are in multiples of uint32_t not in bytes
#define CCL_TASK_STACK_SIZE_SMALL     256
#define CCL_TASK_STACK_SIZE_NORMAL    768
#define CCL_TASK_STACK_SIZE_LARGE     2048

/** @brief Thread start function. Will be called from platform specific wrapper.
 * @details This proxy call is a way to get by different thread function prototype on various
 * platforms and to make sure that when thread function returns (end) we have indication of it in our code.
 * NOTE: this definition needs to be before including platform specific header as it will be referenced there. (avoiding dedicated header file for one define) */
typedef void *(*ccl_thread_fcn)(void *);

#include "ccl_thread_platform.h"

typedef enum
{
    CCL_THREAD_PRIORITY_IDLE,
    CCL_THREAD_PRIORITY_LOW,
    CCL_THREAD_PRIORITY_NORMAL,
    CCL_THREAD_PRIORITY_HIGH,
    CCL_THREAD_PRIORITY_MAX,
}ccl_ThreadPriority_t;

/**
 * @brief Creates new thread.
 *
 * @param apTarget Thread control structure to use.
 * @param aStart Start function to call.
 * @param aArg Argument for start function.
 * @param apThreadName Optionall thread name. Use NULL for system default.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_thread_Create(ccl_thread_t *apTarget, ccl_thread_fcn aStart, void *aArg, const char *apThreadName, uint32_t aMinStackSize, ccl_ThreadPriority_t aPriority);

/**
 * @brief Stops thread by canaling it.
 *
 * @param apTarget Thread control structure to use.
 * @return 0 on success, negative number on error.
 *
 * @details Thread can be stopped in various cancelable points like semaphore wait if OS defaults are used or
 * if ccl_thread_ManualStopControl() is called, thread it self controls if it can be stopped or not by
 * ccl_thread_StopAllow() and ccl_thread_StopBlocked().
 * Do not call from within the thread you want to stop!
 */
ccl_Err_t ccl_thread_Cancel(ccl_thread_t *apTarget);

/**
 * @brief Increase thread priority to maximum
 *
 * @return 0 on success, negative number on error.
 * @details Call from within target thread.
 */
ccl_Err_t ccl_thread_SetPriority(ccl_thread_t *apTarget, ccl_ThreadPriority_t aPriority);

/**
 * @brief Returns thread running state.
 *
 * @param apTarget Thread control structure to use.
 * @return true if thread is running, false if not.
 */
bool ccl_thread_IsRunning(ccl_thread_t *apTarget);

/**
 * @brief Activates manual control of thread stop availability
 *
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_thread_ManualStopControl(void);

/**
 * @brief Allows calling thread to be stoped.
 *
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_thread_StopAllow(void);

/**
 * @brief Blocks any attempts future attempts to stop calling thread.
 *
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_thread_StopBlocked(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_THREAD_H */

