/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file clock.h
 */

#ifndef CLOCK_HEADER_GUARD_
#define CLOCK_HEADER_GUARD_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

uint32_t xPortGetTimeUs(void);

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_HEADER_GUARD_ */
