 /*******************************************************************************
 * Copyright 2020-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/
#ifndef DRC_DEFINES_H
#define DRC_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

// defines that are needed in case of compilation out of ISIM
#define DRC_INPUT_BPP   20u
#define DRC_OUTPUT_BPP  16u
#define DRC_MINCLZ      8u   // min count leading zero (clz) histogram category
#define DRC_BITCLZ      5u   // 2<<n bins per histogram clz category
#define DRC_BINCLZ      (1u << DRC_BITCLZ)
#define MASK_LSB        (DRC_BINCLZ - 1u)
#define DRC_HISTBINS    ((DRC_INPUT_BPP - DRC_MINCLZ + 1u) * DRC_BINCLZ)
#define DRC_XBLOCKS     (32u)
#define DRC_YBLOCKS     (32u)
#define DRC_BLOCKS      (DRC_XBLOCKS * DRC_YBLOCKS)
#define STAT_HIST_BINS  ((uint32_t) 64U)

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DRC_DEFINES_H */
