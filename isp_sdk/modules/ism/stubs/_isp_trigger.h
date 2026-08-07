/*
 * Copyright 2022, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ISP_TRIGGER_GUARD
#define ISP_TRIGGER_GUARD

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Platrofm specific implementation of ISP pipeline trigger.
 *
 * @param[in] isp Object representing the ISP peripheral
 *
 */
void isp_trigger_plt(ISP_TypeDef * isp);

#ifdef __cplusplus
}
#endif

#endif /* ISP_TRIGGER_GUARD */
