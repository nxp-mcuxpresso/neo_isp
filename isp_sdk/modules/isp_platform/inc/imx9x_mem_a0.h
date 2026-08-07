/*******************************************************************************
 * Copyright  2020-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/


//****************************************************************************
// File copied from ISIM (C_simulator/src/imx9x/arch/imx9x_mem.h)
// ISIM Commit: f29ca396e7f51309e9b5b9d1ee9182fa8b8c982d
//
// NOTE: Use following command to extract the commit ID:
// sed -E -n 's/\/\/ ISIM Commit: ([0-9,a-f]{40})/\1/p' imx9x_mem.h
//****************************************************************************

/**
* @file     imx9x_mem.h
* @brief    type definition of ISP pipe memories
****************************************************************************/

#ifndef PIPE_MEM_H
#define PIPE_MEM_H

#include <stdint.h>

//*****************************************************************************
// constants
//*****************************************************************************
#define BLOCKSTAT_XBLOCKS 8
#define BLOCKSTAT_YBLOCKS 8
#define BLOCKSTAT_BLOCKS (BLOCKSTAT_XBLOCKS * BLOCKSTAT_YBLOCKS)

#define STAT_HISTOGRAMS_CNT 4     // number of histograms
#define STAT_GIR_HISTOGRAMS_CNT 2 // number of histograms for IR and G
#define STAT_HIST_BINS 64         // per histogram

#define DRC_XBLOCKS (32)
#define DRC_YBLOCKS (32)
#define DRC_BLOCKS (DRC_XBLOCKS * DRC_YBLOCKS)

#define DRC_INPUT_BPP 20
#define DRC_LOG_HIST
#define DRC_MINCLZ 8 // min count leading zero (clz) histogram category
#define DRC_BITCLZ 5 // 2<<n bins per histogram clz category
#define DRC_BINCLZ (1 << DRC_BITCLZ)
#define DRC_HISTBINS ((DRC_INPUT_BPP - DRC_MINCLZ + 1) * DRC_BINCLZ)

//*****************************************************************************
// types
//*****************************************************************************

/**
 * @brief ISP pipe memories.
 *
 ****************************************************************************/

typedef struct
{
    // as defined in the HW (gaps missing)
    uint32_t BLK_STAT_R[BLOCKSTAT_BLOCKS];                                   /* off: 0x0     size: 8x8 = 64 x 4 = 256 = 0x100         */
    uint32_t BLK_STAT_G[BLOCKSTAT_BLOCKS];                                   /* off: 0x100   size: 8x8 = 64 x 4 = 256 = 0x100         */
    uint32_t BLK_STAT_B[BLOCKSTAT_BLOCKS];                                   /* off: 0x200   size: 8x8 = 64 x 4 = 256 = 0x100         */
    uint16_t BLK_STAT_CNT[BLOCKSTAT_BLOCKS];                                 /* off: 0x300   size: 8x8 = 64 x 2 = 128 = 0x080         */
    uint32_t unused0[32];                                                    /* off: 0x380   size: 32 x 4 = 128 = 0x080               */
    uint32_t RGBIR_HISTOGRAM[STAT_GIR_HISTOGRAMS_CNT * 2 * STAT_HIST_BINS];  /* off: 0x400   size: 2x2x64 = 256 x 4 = 1024 = 0x400    */
    uint32_t HISTOGRAM_STATISTICS[STAT_HISTOGRAMS_CNT * 2 * STAT_HIST_BINS]; /* off: 0x800   size: 4x2x64 = 512 x 4 = 2048 = 0x800    */
    uint16_t VIGNETTING_TABLE[3072];                                         /* off: 0x1000  size: 3072 x 2 = 6144 = 0x1800           */
    uint32_t unused1[1536];                                                  /* off: 0x2800  size: 1536 x 4 = 6144 = 0x1800           */
    uint16_t DRC_GLOBAL_TONEMAP[416];            // in 8.8                   /* off: 0x4000  size: 416 x 2 = 832 = 0x340              */
    uint32_t unused2[48];                                                    /* off: 0x4340  size: 48 x 4 = 192 = 0xC0                */
    uint8_t  DRC_LOCAL_TONEMAP[DRC_BLOCKS];      // in 0.8                   /* off: 0x4400  size: 32 x 32 = 1024 = 0x400             */
    uint32_t DRC_LOCAL_SUM[DRC_BLOCKS];          // in 32.0                  /* off: 0x4800  size: 32 x 32 = 1024 x 4 = 4096 = 0x1000 */
    uint32_t DRC_GLOBAL_HIST_ROI0[DRC_HISTBINS]; // in 32.0                  /* off: 0x5800  size: 416 x 4 = 1664 = 0x680             */
    uint32_t unused3[32];                                                    /* off: 0x5E80  size: 32 x 4 = 128 = 0x80                */
    uint32_t DRC_GLOBAL_HIST_ROI1[DRC_HISTBINS]; // in 32.0                  /* off: 0x5F00  size: 416 x 4 = 1664 = 0x680             */
} imx9x_mem_t;

//*****************************************************************************
// globals
//*****************************************************************************

#endif /* PIPE_MEM_H */
