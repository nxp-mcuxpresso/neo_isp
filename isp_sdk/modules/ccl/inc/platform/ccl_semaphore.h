/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_SEMAPHORE_H
#define CCL_SEMAPHORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// only needed for ccl_err_t
#include "ccl.h"

#include "ccl_semaphore_platform.h"

#define CCL_SEM_WAIT_ININITE    INT32_MAX

/**
 * @brief Creates semaphore object.
 *
 * @param apTarget Target object.
 * @param aInitCnt Specify initial count the semaphore will hold.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_sem_Init(ccl_sem_t *apTarget, int aInitCnt);

/**
 * @brief Deinitialize sempahore object. (cleanup)
 *
 * @param apTarget Target object.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_sem_Destroy(ccl_sem_t *apTarget);

/**
 * @brief Waits for semaphore to signal availability
 *
 * @param apTarget Target object.
 * @param aWaitMs Maximum number of miliseconds to wait for the semaphore. Use CCL_SEM_WAIT_ININITE for longest possible time.
 * @return 0 on success, negative number on error. If mutex is not acquired in given time CCL_ERR_TIMEOUT.
 */
ccl_Err_t ccl_sem_Take(ccl_sem_t *apTarget, int aWaitMs);

/**
 * @brief Adds one to semaphore count.
 *
 * @param apTarget Target object.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_sem_Give(ccl_sem_t* apTarget);

/**
 * @brief Adds one to semaphore count, can be used from an ISR
 *
 * @param apTarget Target object.
 * @return 0 on success, negative number on error.
 */
ccl_Err_t ccl_sem_GiveFromISR(ccl_sem_t* apTarget);

/**
 * @brief Returns current count.
 *
 * @param apTarget Target object.
 * @param apOutCnt Variable to receive current count.
 * @return 0 on success, negative number on error.
 *
 * @details Note that without use of a mutex the count is potentially old value.
 */
ccl_Err_t ccl_sem_GetCnt(ccl_sem_t *apTarget, int *apOutCnt);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_SEMAPHORE_H */

