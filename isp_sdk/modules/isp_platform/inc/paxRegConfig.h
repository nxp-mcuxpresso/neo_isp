/*******************************************************************************
 * Copyright  2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef PAX_REG_CONFIG_H
#define PAX_REG_CONFIG_H

#include <stdint.h>
// switch target revision
#if defined(IMX95)
    #if defined(A0)
        #include "paxRegConfig_a0.h"
        typedef imx9x_Regs_t IspRegs_t;
    #elif defined(B0)
        #include "paxRegConfig_b0.h"
        typedef imx9x_Regs_t IspRegs_t;
    #else
        #error "unknown target revision. Please, define REV=[A0/B0] at make command line. E.g.: make REV=B0"
    #endif
#else
      #error "unknown hw platform. Please, check makefile for [IMX95]."
#endif
#endif // PAX_REG_CONFIG_H
