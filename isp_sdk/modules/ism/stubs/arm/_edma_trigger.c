/*
 * Copyright 2022, 2024, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "_edma_trigger.h"

void edma_trigger_plt(EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    edma->Channel[channel].TCD.TCSR |= EDMA_TCDn_CSR_START(1);
}
