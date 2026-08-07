/*
 * Copyright 2022, 2024, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EDMA_TRIGGER_GUARD
#define EDMA_TRIGGER_GUARD

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Platform specific implementation of DMA trigger.
 *
 * @param[in] edma Object representing the eDMA peripheral.
 * @param[in] channel channel to trigger
*/
void edma_trigger_plt(EDMA_TypeDef * edma, uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* EDMA_TRIGGER_GUARD */
