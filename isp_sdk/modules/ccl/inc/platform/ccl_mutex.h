/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

 /**
 * @file ccl_mutex.h
 * @brief Definitions for mutex object common to all platforms. 
 */

#ifndef CCL_MUTEX_H
#define CCL_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ccl_mutex_platform.h"

// only needed for ccl_err_t
#include "ccl.h"

#define CCL_MUTEX_WAIT_ININITE INT32_MAX

/**
 * @brief Creates mutex object.
 * 
 * @param apTarget Target object.
 * @param aRecursive Set to true if mutex can be acquired more than once by the thread already owning it.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_mutex_Create(ccl_mutex_t *apTarget, bool aRecursive);

/**
 * @brief Deinitialize mutex object. (cleanup)
 * 
 * @param apTarget Target object.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_mutex_Destroy(ccl_mutex_t *apTarget);

/**
 * @brief Acquires object without waiting.
 * 
 * @param apTarget Target object.
 * @return 0 on success, negative number on error. If mutex already owned by different thread CCL_ERR_BAD_STATE.
 */
ccl_Err_t ccl_mutex_TryLock(ccl_mutex_t *apTarget);

/**
 * @brief Waits for mutex availability
 * 
 * @param apTarget Target object.
 * @param aWaitMs Maximum number of miliseconds to wait for the mutex. Use CCL_MUTEX_WAIT_ININITE for longest possible time.
 * @return 0 on success, negative number on error. If mutex is not acquired in given time CCL_ERR_TIMEOUT.
 */
ccl_Err_t ccl_mutex_Lock(ccl_mutex_t *apTarget, int aWaitMs);

/**
 * @brief Frees mutex for use.
 * 
 * @param apTarget Target object.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_mutex_Unlock(ccl_mutex_t *apTarget);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_MUTEX_H */

