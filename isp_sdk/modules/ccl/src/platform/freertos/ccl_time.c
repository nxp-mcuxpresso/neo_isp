/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ccl_time.c
 * @brief implements ccl_time for FreeRtos
 *******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

#include "ccl_time.h"
#include "ilog.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
// Copied from McuXSdk/rtos/freertos/freertos-kernel/portable/GCC/ARM_CM4F/port.c
#define portNVIC_SYSTICK_LOAD_REG (*((volatile uint32_t*)0xe000e014))
#define portNVIC_SYSTICK_CURRENT_VALUE_REG (*((volatile uint32_t*)0xe000e018))

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

uint64_t ccl_GetTime(void)
{
    return ((xTaskGetTickCount() * 1e6 / configTICK_RATE_HZ) + ((portNVIC_SYSTICK_LOAD_REG - portNVIC_SYSTICK_CURRENT_VALUE_REG) * 1e6 / configCPU_CLOCK_HZ));
}

void ccl_SleepUs(uint64_t aTimeUs)
{
    uint32_t ticks = aTimeUs * configTICK_RATE_HZ / 1e6;
    if (ticks == 0)
    {
        ticks = 1;
        ILOG(ILOG_MOD_CCL, ILOG_LEV_WRN, "ccl_SleepUs() - interval below resolution of %ums.\n", portTICK_PERIOD_MS);
    }
    vTaskDelay(ticks);
}

void ccl_Sleep(uint64_t aTime)
{
    vTaskDelay(aTime * configTICK_RATE_HZ);
}

/******************************************************************************/
