/*******************************************************************************
 * Copyright  2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ISP_MEMS_H
#define ISP_MEMS_H

// switch target revision
#if defined(IMX95)
    #if defined(A0)
        #include "imx9x_mem_a0.h"
        typedef imx9x_mem_t IspMems_t;
    #elif defined(B0)
        #include "imx9x_mem_b0.h"
        typedef imx9x_mem_t IspMems_t;
    #else
        #error "unknown target revision. Please, define REV=[A0/B0] at make command line. E.g.: make REV=B0"
    #endif
#else
      #error "unknown hw platform. Please, check makefile for [IMX95]."
#endif

#endif // ISP_MEMS_H
