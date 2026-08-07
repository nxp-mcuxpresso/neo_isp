 /*******************************************************************************
 * Copyright 2020-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/**
 * @file     interface.h
 * @brief    Interfaces used by algorithms.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "sensor.h"

/**
 * @brief   struct holds variables related to temperature color estimation
 */
typedef struct
{
    uint32_t mColorTemp;    // estimated color temperature of the scene; range: 0 - 10000K
    float       mLux;       // estimated scene illuminance in lux; range: 0 - 100000 lux
} ccf_CtrlParams_t;


#endif /* INTERFACE_H */
