/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_TIME_H
#define CCL_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Returns current time
 *
 * @return Current time in us
 */
uint64_t ccl_GetTime(void);

/**
 * @brief puts thread to sleep on given time
 *
 * @param aTimeUs - time to sleep in us
 * @details The sleep might not be exactly precise, at the moment 1ms resolution should be enough.
 * This might change in future.
 */
void ccl_SleepUs(uint64_t aTimeUs);

/**
 * @brief puts thread to sleep on given time
 *
 * @param aTimeUs - time to sleep
 */
void ccl_Sleep(uint64_t aTime);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_TIME_H */

