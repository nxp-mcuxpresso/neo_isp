/*
 * Copyright 2022, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief HW stub implementing the ISP pipeline triggering.
 */

#include "_isp_trigger.h"

#define SUCCESS     (0U)
#define FAILURE     (1U)

void isp_trigger_plt(ISP_TypeDef * isp)
{
    /* Any write into the TRIG_CAM0 register triggers the ISP pipeline, however
    if multiple trigger requests are received, an error is raised in INT_STAT0
    register and the writes to this register are ignored. */
    isp->TRIG_CAM0 = 0x50505050U;
}
