/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file clock.c
 *
 * FreeRTOS specific time implementation.
 */

#include "platform.h"


uint32_t xPortGetTimeUs(void)
{
    /* Critical section required if running on a 16 bit processor. */
    uint32_t Systick_cnt;

    TickType_t xTicks = 0;
    bool Overflow = true;
    while (Overflow)
    {
        Systick_cnt = SysTick->VAL;

        // leave this as a last -> it contains critical section exit which is not reentrant
        xTicks = xTaskGetTickCount();

        uint32_t Systick_cnt2 = SysTick->VAL;
        if (Systick_cnt2 < Systick_cnt)
        {
            Overflow = false;
        }
    }

    TickType_t ret = (xTicks * (1e6 / configTICK_RATE_HZ) + ((SysTick->LOAD - Systick_cnt) / (configCPU_CLOCK_HZ / 1e6)));
    return ret;
}
