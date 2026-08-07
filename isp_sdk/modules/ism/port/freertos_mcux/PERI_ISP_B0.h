/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!
 * @file PERI_ISP_B0.h
 * @version 1.0
 * @date 2026-02-11
 * @brief CMSIS Peripheral Access Layer for ISP
 *
 * CMSIS Peripheral Access Layer for ISP
 */

#if !defined(PERI_ISP_DEF_H_)
#define PERI_ISP_DEF_H_

#if (defined(CPU_MIMX9506AVZXN_ca55) || defined(CPU_MIMX9506XVZXN_ca55))
#include "MIMX9506_ca55_COMMON.h"
#elif (defined(CPU_MIMX9506AVZXN_cm33) || defined(CPU_MIMX9506XVZXN_cm33))
#include "MIMX9506_cm33_COMMON.h"
#elif (defined(CPU_MIMX9506AVZXN_cm7) || defined(CPU_MIMX9506XVZXN_cm7))
#include "MIMX9506_cm7_COMMON.h"
#elif (defined(CPU_MIMX9534AVZXN_ca55) || defined(CPU_MIMX9534CVTXN_ca55) || defined(CPU_MIMX9534CVZXN_ca55) || defined(CPU_MIMX9534DVTXN_ca55) || defined(CPU_MIMX9534DVZXN_ca55) || defined(CPU_MIMX9534XVTXN_ca55) || defined(CPU_MIMX9534XVZXN_ca55))
#include "MIMX9534_ca55_COMMON.h"
#elif (defined(CPU_MIMX9534AVZXN_cm33) || defined(CPU_MIMX9534CVTXN_cm33) || defined(CPU_MIMX9534CVZXN_cm33) || defined(CPU_MIMX9534DVTXN_cm33) || defined(CPU_MIMX9534DVZXN_cm33) || defined(CPU_MIMX9534XVTXN_cm33) || defined(CPU_MIMX9534XVZXN_cm33))
#include "MIMX9534_cm33_COMMON.h"
#elif (defined(CPU_MIMX9534AVZXN_cm7) || defined(CPU_MIMX9534CVTXN_cm7) || defined(CPU_MIMX9534CVZXN_cm7) || defined(CPU_MIMX9534DVTXN_cm7) || defined(CPU_MIMX9534DVZXN_cm7) || defined(CPU_MIMX9534XVTXN_cm7) || defined(CPU_MIMX9534XVZXN_cm7))
#include "MIMX9534_cm7_COMMON.h"
#elif (defined(CPU_MIMX9536AVZXN_ca55) || defined(CPU_MIMX9536CVTXN_ca55) || defined(CPU_MIMX9536CVZXN_ca55) || defined(CPU_MIMX9536DVTXN_ca55) || defined(CPU_MIMX9536DVZXN_ca55) || defined(CPU_MIMX9536DVZXQ_ca55) || defined(CPU_MIMX9536XVTXN_ca55) || defined(CPU_MIMX9536XVZXN_ca55))
#include "MIMX9536_ca55_COMMON.h"
#elif (defined(CPU_MIMX9536AVZXN_cm33) || defined(CPU_MIMX9536CVTXN_cm33) || defined(CPU_MIMX9536CVZXN_cm33) || defined(CPU_MIMX9536DVTXN_cm33) || defined(CPU_MIMX9536DVZXN_cm33) || defined(CPU_MIMX9536DVZXQ_cm33) || defined(CPU_MIMX9536XVTXN_cm33) || defined(CPU_MIMX9536XVZXN_cm33))
#include "MIMX9536_cm33_COMMON.h"
#elif (defined(CPU_MIMX9536AVZXN_cm7) || defined(CPU_MIMX9536CVTXN_cm7) || defined(CPU_MIMX9536CVZXN_cm7) || defined(CPU_MIMX9536DVTXN_cm7) || defined(CPU_MIMX9536DVZXN_cm7) || defined(CPU_MIMX9536DVZXQ_cm7) || defined(CPU_MIMX9536XVTXN_cm7) || defined(CPU_MIMX9536XVZXN_cm7))
#include "MIMX9536_cm7_COMMON.h"
#elif (defined(CPU_MIMX9546AVZXN_ca55) || defined(CPU_MIMX9546XVZXN_ca55))
#include "MIMX9546_ca55_COMMON.h"
#elif (defined(CPU_MIMX9546AVZXN_cm33) || defined(CPU_MIMX9546XVZXN_cm33))
#include "MIMX9546_cm33_COMMON.h"
#elif (defined(CPU_MIMX9546AVZXN_cm7) || defined(CPU_MIMX9546XVZXN_cm7))
#include "MIMX9546_cm7_COMMON.h"
#elif (defined(CPU_MIMX9554AVTXN_ca55) || defined(CPU_MIMX9554AVZXN_ca55) || defined(CPU_MIMX9554CVTXN_ca55) || defined(CPU_MIMX9554CVZXN_ca55) || defined(CPU_MIMX9554DVTXN_ca55) || defined(CPU_MIMX9554DVZXN_ca55) || defined(CPU_MIMX9554XVTXN_ca55) || defined(CPU_MIMX9554XVZXN_ca55))
#include "MIMX9554_ca55_COMMON.h"
#elif (defined(CPU_MIMX9554AVTXN_cm33) || defined(CPU_MIMX9554AVZXN_cm33) || defined(CPU_MIMX9554CVTXN_cm33) || defined(CPU_MIMX9554CVZXN_cm33) || defined(CPU_MIMX9554DVTXN_cm33) || defined(CPU_MIMX9554DVZXN_cm33) || defined(CPU_MIMX9554XVTXN_cm33) || defined(CPU_MIMX9554XVZXN_cm33))
#include "MIMX9554_cm33_COMMON.h"
#elif (defined(CPU_MIMX9554AVTXN_cm7) || defined(CPU_MIMX9554AVZXN_cm7) || defined(CPU_MIMX9554CVTXN_cm7) || defined(CPU_MIMX9554CVZXN_cm7) || defined(CPU_MIMX9554DVTXN_cm7) || defined(CPU_MIMX9554DVZXN_cm7) || defined(CPU_MIMX9554XVTXN_cm7) || defined(CPU_MIMX9554XVZXN_cm7))
#include "MIMX9554_cm7_COMMON.h"
#elif (defined(CPU_MIMX9556AVZXN_ca55) || defined(CPU_MIMX9556CVTXN_ca55) || defined(CPU_MIMX9556CVZXN_ca55) || defined(CPU_MIMX9556DVTXN_ca55) || defined(CPU_MIMX9556DVZXN_ca55) || defined(CPU_MIMX9556XVTXN_ca55) || defined(CPU_MIMX9556XVZXN_ca55))
#include "MIMX9556_ca55_COMMON.h"
#elif (defined(CPU_MIMX9556AVZXN_cm33) || defined(CPU_MIMX9556CVTXN_cm33) || defined(CPU_MIMX9556CVZXN_cm33) || defined(CPU_MIMX9556DVTXN_cm33) || defined(CPU_MIMX9556DVZXN_cm33) || defined(CPU_MIMX9556XVTXN_cm33) || defined(CPU_MIMX9556XVZXN_cm33))
#include "MIMX9556_cm33_COMMON.h"
#elif (defined(CPU_MIMX9556AVZXN_cm7) || defined(CPU_MIMX9556CVTXN_cm7) || defined(CPU_MIMX9556CVZXN_cm7) || defined(CPU_MIMX9556DVTXN_cm7) || defined(CPU_MIMX9556DVZXN_cm7) || defined(CPU_MIMX9556XVTXN_cm7) || defined(CPU_MIMX9556XVZXN_cm7))
#include "MIMX9556_cm7_COMMON.h"
#elif (defined(CPU_MIMX9574AVZXN_ca55) || defined(CPU_MIMX9574CVTXN_ca55) || defined(CPU_MIMX9574CVZXN_ca55) || defined(CPU_MIMX9574DVTXN_ca55) || defined(CPU_MIMX9574DVZXN_ca55) || defined(CPU_MIMX9574XVTXN_ca55) || defined(CPU_MIMX9574XVZXN_ca55))
#include "MIMX9574_ca55_COMMON.h"
#elif (defined(CPU_MIMX9574AVZXN_cm33) || defined(CPU_MIMX9574CVTXN_cm33) || defined(CPU_MIMX9574CVZXN_cm33) || defined(CPU_MIMX9574DVTXN_cm33) || defined(CPU_MIMX9574DVZXN_cm33) || defined(CPU_MIMX9574XVTXN_cm33) || defined(CPU_MIMX9574XVZXN_cm33))
#include "MIMX9574_cm33_COMMON.h"
#elif (defined(CPU_MIMX9574AVZXN_cm7) || defined(CPU_MIMX9574CVTXN_cm7) || defined(CPU_MIMX9574CVZXN_cm7) || defined(CPU_MIMX9574DVTXN_cm7) || defined(CPU_MIMX9574DVZXN_cm7) || defined(CPU_MIMX9574XVTXN_cm7) || defined(CPU_MIMX9574XVZXN_cm7))
#include "MIMX9574_cm7_COMMON.h"
#elif (defined(CPU_MIMX9576AVZXN_ca55) || defined(CPU_MIMX9576CVTXN_ca55) || defined(CPU_MIMX9576CVZXN_ca55) || defined(CPU_MIMX9576DVTXN_ca55) || defined(CPU_MIMX9576DVZXN_ca55) || defined(CPU_MIMX9576XVTXN_ca55) || defined(CPU_MIMX9576XVZXN_ca55))
#include "MIMX9576_ca55_COMMON.h"
#elif (defined(CPU_MIMX9576AVZXN_cm33) || defined(CPU_MIMX9576CVTXN_cm33) || defined(CPU_MIMX9576CVZXN_cm33) || defined(CPU_MIMX9576DVTXN_cm33) || defined(CPU_MIMX9576DVZXN_cm33) || defined(CPU_MIMX9576XVTXN_cm33) || defined(CPU_MIMX9576XVZXN_cm33))
#include "MIMX9576_cm33_COMMON.h"
#elif (defined(CPU_MIMX9576AVZXN_cm7) || defined(CPU_MIMX9576CVTXN_cm7) || defined(CPU_MIMX9576CVZXN_cm7) || defined(CPU_MIMX9576DVTXN_cm7) || defined(CPU_MIMX9576DVZXN_cm7) || defined(CPU_MIMX9576XVTXN_cm7) || defined(CPU_MIMX9576XVZXN_cm7))
#include "MIMX9576_cm7_COMMON.h"
#elif (defined(CPU_MIMX9586AVZXN_ca55) || defined(CPU_MIMX9586XVZXN_ca55))
#include "MIMX9586_ca55_COMMON.h"
#elif (defined(CPU_MIMX9586AVZXN_cm33) || defined(CPU_MIMX9586XVZXN_cm33))
#include "MIMX9586_cm33_COMMON.h"
#elif (defined(CPU_MIMX9586AVZXN_cm7) || defined(CPU_MIMX9586XVZXN_cm7))
#include "MIMX9586_cm7_COMMON.h"
#elif (defined(CPU_MIMX9594AVZXN_ca55) || defined(CPU_MIMX9594CVTXN_ca55) || defined(CPU_MIMX9594CVZXN_ca55) || defined(CPU_MIMX9594DVTXN_ca55) || defined(CPU_MIMX9594DVZXN_ca55) || defined(CPU_MIMX9594XVTXN_ca55) || defined(CPU_MIMX9594XVZXN_ca55))
#include "MIMX9594_ca55_COMMON.h"
#elif (defined(CPU_MIMX9594AVZXN_cm33) || defined(CPU_MIMX9594CVTXN_cm33) || defined(CPU_MIMX9594CVZXN_cm33) || defined(CPU_MIMX9594DVTXN_cm33) || defined(CPU_MIMX9594DVZXN_cm33) || defined(CPU_MIMX9594XVTXN_cm33) || defined(CPU_MIMX9594XVZXN_cm33))
#include "MIMX9594_cm33_COMMON.h"
#elif (defined(CPU_MIMX9594AVZXN_cm7) || defined(CPU_MIMX9594CVTXN_cm7) || defined(CPU_MIMX9594CVZXN_cm7) || defined(CPU_MIMX9594DVTXN_cm7) || defined(CPU_MIMX9594DVZXN_cm7) || defined(CPU_MIMX9594XVTXN_cm7) || defined(CPU_MIMX9594XVZXN_cm7))
#include "MIMX9594_cm7_COMMON.h"
#elif (defined(CPU_MIMX9596AVZXN_ca55) || defined(CPU_MIMX9596CVTXN_ca55) || defined(CPU_MIMX9596CVZXN_ca55) || defined(CPU_MIMX9596DVTXN_ca55) || defined(CPU_MIMX9596DVZXN_ca55) || defined(CPU_MIMX9596DVZXQ_ca55) || defined(CPU_MIMX9596XVTXN_ca55) || defined(CPU_MIMX9596XVZXN_ca55))
#include "MIMX9596_ca55_COMMON.h"
#elif (defined(CPU_MIMX9596AVZXN_cm33) || defined(CPU_MIMX9596CVTXN_cm33) || defined(CPU_MIMX9596CVZXN_cm33) || defined(CPU_MIMX9596DVTXN_cm33) || defined(CPU_MIMX9596DVZXN_cm33) || defined(CPU_MIMX9596DVZXQ_cm33) || defined(CPU_MIMX9596XVTXN_cm33) || defined(CPU_MIMX9596XVZXN_cm33))
#include "MIMX9596_cm33_COMMON.h"
#elif (defined(CPU_MIMX9596AVZXN_cm7) || defined(CPU_MIMX9596CVTXN_cm7) || defined(CPU_MIMX9596CVZXN_cm7) || defined(CPU_MIMX9596DVTXN_cm7) || defined(CPU_MIMX9596DVZXN_cm7) || defined(CPU_MIMX9596DVZXQ_cm7) || defined(CPU_MIMX9596XVTXN_cm7) || defined(CPU_MIMX9596XVZXN_cm7))
#include "MIMX9596_cm7_COMMON.h"
#elif (defined(CPU_MIMX95N4AVTXN_ca55) || defined(CPU_MIMX95N4AVZXN_ca55) || defined(CPU_MIMX95N4CVTXN_ca55) || defined(CPU_MIMX95N4CVZXN_ca55) || defined(CPU_MIMX95N4DVTXN_ca55) || defined(CPU_MIMX95N4DVZXN_ca55) || defined(CPU_MIMX95N4XVTXN_ca55) || defined(CPU_MIMX95N4XVZXN_ca55))
#include "MIMX95N4_ca55_COMMON.h"
#elif (defined(CPU_MIMX95N4AVTXN_cm33) || defined(CPU_MIMX95N4AVZXN_cm33) || defined(CPU_MIMX95N4CVTXN_cm33) || defined(CPU_MIMX95N4CVZXN_cm33) || defined(CPU_MIMX95N4DVTXN_cm33) || defined(CPU_MIMX95N4DVZXN_cm33) || defined(CPU_MIMX95N4XVTXN_cm33) || defined(CPU_MIMX95N4XVZXN_cm33))
#include "MIMX95N4_cm33_COMMON.h"
#elif (defined(CPU_MIMX95N4AVTXN_cm7) || defined(CPU_MIMX95N4AVZXN_cm7) || defined(CPU_MIMX95N4CVTXN_cm7) || defined(CPU_MIMX95N4CVZXN_cm7) || defined(CPU_MIMX95N4DVTXN_cm7) || defined(CPU_MIMX95N4DVZXN_cm7) || defined(CPU_MIMX95N4XVTXN_cm7) || defined(CPU_MIMX95N4XVZXN_cm7))
#include "MIMX95N4_cm7_COMMON.h"
#elif (defined(CPU_MIMX95N6AVTXN_ca55) || defined(CPU_MIMX95N6AVZXN_ca55) || defined(CPU_MIMX95N6CVTXN_ca55) || defined(CPU_MIMX95N6CVYXN_ca55) || defined(CPU_MIMX95N6CVZXN_ca55) || defined(CPU_MIMX95N6DVTXN_ca55) || defined(CPU_MIMX95N6DVZXN_ca55) || defined(CPU_MIMX95N6XVTXN_ca55) || defined(CPU_MIMX95N6XVZXN_ca55))
#include "MIMX95N6_ca55_COMMON.h"
#elif (defined(CPU_MIMX95N6AVTXN_cm33) || defined(CPU_MIMX95N6AVZXN_cm33) || defined(CPU_MIMX95N6CVTXN_cm33) || defined(CPU_MIMX95N6CVYXN_cm33) || defined(CPU_MIMX95N6CVZXN_cm33) || defined(CPU_MIMX95N6DVTXN_cm33) || defined(CPU_MIMX95N6DVZXN_cm33) || defined(CPU_MIMX95N6XVTXN_cm33) || defined(CPU_MIMX95N6XVZXN_cm33))
#include "MIMX95N6_cm33_COMMON.h"
#elif (defined(CPU_MIMX95N6AVTXN_cm7) || defined(CPU_MIMX95N6AVZXN_cm7) || defined(CPU_MIMX95N6CVTXN_cm7) || defined(CPU_MIMX95N6CVYXN_cm7) || defined(CPU_MIMX95N6CVZXN_cm7) || defined(CPU_MIMX95N6DVTXN_cm7) || defined(CPU_MIMX95N6DVZXN_cm7) || defined(CPU_MIMX95N6XVTXN_cm7) || defined(CPU_MIMX95N6XVZXN_cm7))
#include "MIMX95N6_cm7_COMMON.h"
#else
  #error "No valid CPU defined!"
#endif

/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
  #else
    #pragma push
    #pragma anon_unions
  #endif
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- ISP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ISP_Peripheral_Access_Layer ISP Peripheral Access Layer
 * @{
 */
/** ISP - Register Layout Typedef */
 typedef struct
 {
    __IO uint32_t SOFT_RESET;           /**< ; off: 0x00 */
    __IO uint32_t BUS_TXPARAM;          /**< ; 0ff: 0x04 */
    __IO uint32_t REG_XFR_DIS;          /**< ; off: 0x08 */
    __I uint32_t  reserved0;            /**< reserved; off: 0x0C */
    __IO uint32_t CSI_CTRL;             /**< ; off: 0x10 */
    __IO uint32_t FRAME_NUM;            /**< ; off: 0x14 */
    __IO uint32_t REG_SHD_CTRL;         /**< ; off: 0x18 */
    __IO uint32_t REG_SHD_CMD;          /**< ; off: 0x1C */
    __IO uint32_t TRIG_CAM0;            /**< ; off: 0x20 */
    __IO uint32_t INT_EN0;              /**< ; off: 0x24 */
    __IO uint32_t INT_STAT0;            /**< ; off: 0x28 */
    __IO uint32_t CSI_STAT;             /**< ; off: 0x2C */
    __IO uint32_t IMG_CONF_CAM0;        /**< ; off: 0x30 */
    __IO uint32_t IMG_SIZE_CAM0;        /**< ; off: 0x34 */
    __I uint32_t  reserved2;            /**< ; off: 0x38 */
    __IO uint32_t IMG0_IN_ADDR_CAM0;    /**< ; off: 0x3C */
    __IO uint32_t IMG1_IN_ADDR_CAM0;    /**< ; off: 0x40 */
    __IO uint32_t OUTCH0_ADDR_CAM0;     /**< ; off: 0x44 */
    __IO uint32_t OUTCH1_ADDR_CAM0;     /**< ; off: 0x48 */
    __IO uint32_t OUTIR_ADDR_CAM0;      /**< ; off: 0x4C */
    __IO uint32_t IMG0_IN_LS_CAM0;      /**< ; off: 0x50 */
    __IO uint32_t IMG1_IN_LS_CAM0;      /**< ; off: 0x54 */
    __IO uint32_t OUTCH0_LS_CAM0;       /**< ; off: 0x58 */
    __IO uint32_t OUTCH1_LS_CAM0;       /**< ; off: 0x5C */
    __IO uint32_t OUTIR_LS_CAM0;        /**< ; off: 0x60 */
    __IO uint32_t SKIP_CTRL0;           /**< ; off: 0x64 */
    __IO uint32_t reserved3[22];        /**< ; off: 0x68 */
    /* 0x4AE0_00C0: Head Color Control */
    struct
    {
        /* 0x2000 - 0xC0 = 0x1F40 => 8000 / 4 = 2000 */
        __O uint32_t unused[2000];
    } CTX_START;
    /* 0x4AE0_0100: HDR Decompression 0 */
    /* 0x4AE0_0180: HDR Decompression 1 */
    /* 0x4AE0_0200: Optical Black Correction and White Balance Unit 0 */
    /* 0x4AE0_0240: Optical Black Correction and White Balance Unit 1 */
    /* 0x4AE0_0280: Optical Black Correction and White Balance Unit 2 */
    /* 0x4AE0_0300: HDR Merge Unit */
    /* 0x4AE0_0400: Color Temperature Unit */
    /* 0x4AE0_0600: RGBIR to RGGB, IR */
    /* 0x4AE0_0700: Statistics and Histograms */
    /* 0x4AE0_0780: IR Compression Unit */
    /* 0x4AE0_0800: Bayer Noise Reduction Unit */
    /* 0x4AE0_0900: Vignetting */
    /* 0x4AE0_0FC0: IDBG1 */
    /* 0x4AE0_1180: Demosaic Unit */
    /* 0x4AE0_11C0: Color Space Converstion (RGB -> YUV) Unit */
    /* 0x4AE0_1300: Dynamic Range Compression */
    /* 0x4AE0_1400: Noise Reduction */
    /* 0x4AE0_1440: Direction Filter */
    /* 0x4AE0_1480: Edge Enhancement */
    /* 0x4AE0_14C0: Color Convolution and Median Filter Unit */
    /* 0x4AE0_1500: Color Adaptive Saturation Unit */
    /* 0x4AE0_1580: Packetizer */
    /* 0x4AE0_1600: Gamma Correction Matrix Unit */
    /* 0x4AE0_1700: Auto Focus */
    /* 0x4AE0_1FC0: IDBG2 */
    /* 0x4AE0_2000: Alias Region */
    struct
    {
        __O uint32_t unused[84];    /* 0x2000 - 0x214C */
    } ALIAS;
} ISP_TypeDef;


/*!
 *  @brief ISP memories
 */
typedef struct
{
    uint32_t BLK_STAT_R[64U];               /* off: 0x0     size: 8x8 = 64 x 4 = 256 = 0x100         */
    uint32_t BLK_STAT_G[64U];               /* off: 0x100   size: 8x8 = 64 x 4 = 256 = 0x100         */
    uint32_t BLK_STAT_B[64U];               /* off: 0x200   size: 8x8 = 64 x 4 = 256 = 0x100         */
    uint16_t BLK_STAT_CNT[64U];             /* off: 0x300   size: 8x8 = 64 x 2 = 128 = 0x080         */
    uint32_t unused0[32];                   /* off: 0x380   size: 32 x 4 = 128 = 0x080               */
    uint32_t RGBIR_HISTOGRAM[256];          /* off: 0x400   size: 2x2x64 = 256 x 4 = 1024 = 0x400    */
    uint32_t HISTOGRAM_STATISTICS[512];     /* off: 0x800   size: 4x2x64 = 512 x 4 = 2048 = 0x800    */
    uint32_t DRC_GLOBAL_HIST_ROI0[416];     /* off: 0x1000  size: 416 x 4 = 1664 = 0x680             */
    uint32_t unused1[32];                   /* off: 0x1680  size: 32 x 4 = 128 = 0x80                */
    uint32_t DRC_GLOBAL_HIST_ROI1[416];     /* off: 0x1700  size: 416 x 4 = 1664 = 0x680             */
    uint32_t unused2[32];                   /* off: 0x1D80  size: 32 x 4 = 128 = 0x80                */
    uint32_t DRC_LOCAL_SUM[1024];           /* off: 0x1E00  size: 32 x 32 = 1024 x 4 = 4096 = 0x1000 */
    uint16_t VIGNETTING_TABLE[3072];        /* off: 0x2E00  size: 3072 x 2 = 6144 = 0x1800           */
    uint16_t DRC_GLOBAL_TONEMAP[416];       /* off: 0x4600  size: 416 x 2 = 832 = 0x340              */
    uint32_t unused3[48];                   /* off: 0x4940  size: 48 x 4 = 192 = 0xC0                */
    uint8_t  DRC_LOCAL_TONEMAP[1024];       /* off: 0x4A00  size: 32 x 32 = 1024 = 0x400             */
                                            /* end: 0x4E00 */
} ISP_MEMS_TypeDef;

/* ----------------------------------------------------------------------------
   -- ISP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ISP_Register_Masks ISP Register Masks
 * @{
 */
/**
 * @name Soft Reset Register (SOFT_RESET) masks
 * @details Provides mechanisms for software generated resets to the pipeline
 * for all contexts. Using this register, a soft or a hard reset can be provided
 * to the pipeline for flushing or resetting the operation. The soft and hard
 * resets differ in how the bus operations are aborted.\n
 * In soft reset, the bus protocol is finished with bus protocol compliance,
 * while in hard reset the bus operation is simply reset. For initialization of
 * the pipeline, the software must initiate a soft reset and then wait for soft
 * reset cycle completion, followed by a hard reset request. Each reset request
 * finishes in 100 cycles of the register configuration clock from the cycle
 * when the register write to the corresponding bit is performed.
 * @{
 */
#define ISP_SOFT_RESET_HARD_RESET_Pos   (1U)
#define ISP_SOFT_RESET_HARD_RESET_Msk   ((uint32_t) ((uint32_t) 1U << (ISP_SOFT_RESET_HARD_RESET_Pos)))
/**
 * Hard reset control.
 * 0: no action\n
 * 1: hard resets the pipeline. This bit is cleared after hard reset cycle is
 * completed.
 */
#define ISP_SOFT_RESET_HARD_RESET       ISP_SOFT_RESET_HARD_RESET_Msk

#define ISP_SOFT_RESET_SOFT_RESET_Pos   (0U)
#define ISP_SOFT_RESET_SOFT_RESET_Msk   ((uint32_t) ((uint32_t) 1U << (ISP_SOFT_RESET_SOFT_RESET_Pos)))
/**
 * Soft reset control.
 * 0: no action
 * 1: soft resets the pipeline. This bit is cleared after soft reset cycle is
 * completed.
 */
#define ISP_SOFT_RESET_SOFT_RESET       ISP_SOFT_RESET_SOFT_RESET_Msk
/** @} */ /* SOFT_RESET definitions */

/**
 * @name Bus Transaction Parameter Register (BUS_TXPARAM) masks
 * @details Provides the parameters for controlling the bus transactions on the
 * read bus interface for all the context.
 * @{
 */
#define ISP_BUS_TXPARAM_BSIZE_Pos       (24U)
#define ISP_BUS_TXPARAM_BSIZE_Msk       ((uint32_t) ((uint32_t) 0xFFU << (ISP_BUS_TXPARAM_BSIZE_Pos)))
/**
 * Burst size.
 * 0: each burst has maximum of 4 beats.\n
 * 1: each burst has maximum of 8 beats.\n
 * 2: each burst has maximum of 16 beats.
 */
#define ISP_BUS_TXPARAM_BSIZE(x)        (((uint32_t)(((uint32_t)(x)) << ISP_BUS_TXPARAM_BSIZE_Pos)) & ISP_BUS_TXPARAM_BSIZE_Msk)

#define ISP_BUS_TXPARAM_POSTQOS_Pos     (16U)
#define ISP_BUS_TXPARAM_POSTQOS_Msk     ((uint32_t) ((uint32_t) 0xFFU << (ISP_BUS_TXPARAM_POSTQOS_Pos)))
/**
 * Post Quality of Service (QoS) Escalation.
 * 0x0000: Post threshold is 10.\n
 * 0x0001 - 0xFFFE: Post threshold is 64 * value.\n
 * 0xFFFF: Post QoS is disabled.
 */
#define ISP_BUS_TXPARAM_POSTQOS(x)      (((uint32_t)(((uint32_t)(x)) << ISP_BUS_TXPARAM_POSTQOS_Pos)) & ISP_BUS_TXPARAM_POSTQOS_Msk)

#define ISP_BUS_TXPARAM_OTHT_Pos        (8U)
#define ISP_BUS_TXPARAM_OTHT_Msk        ((uint32_t) ((uint32_t) 0xFFU << (ISP_BUS_TXPARAM_OTHT_Pos)))
/**
 * Outstanding Transaction High Threshold.
 */
#define ISP_BUS_TXPARAM_OTHT(x)        (((uint32_t)(((uint32_t)(x)) << ISP_BUS_TXPARAM_OTHT_Pos)) & ISP_BUS_TXPARAM_OTHT_Msk)

#define ISP_BUS_TXPARAM_OTLT_Pos        (0U)
#define ISP_BUS_TXPARAM_OTLT_Msk        ((uint32_t) ((uint32_t) 0xFFU << (ISP_BUS_TXPARAM_OTLT_Pos)))
/**
 * Outstanding Transaction Low Threshold.
 * 0xFFFF: disable QoS
 */
#define ISP_BUS_TXPARAM_OTLT(x)        (((uint32_t)(((uint32_t)(x)) << ISP_BUS_TXPARAM_OTLT_Pos)) & ISP_BUS_TXPARAM_OTLT_Msk)
/** @} */ /* BUS_TXPARAM definitions */

/**
 * @name Register Transfer Error Disable Register (REG_XFR_DIS) masks
 * @details Provides mechanisms for disabling the transfer error on register
 * access. When enabled read/write access to address holes, write access to
 * read-only registers do not provide transfer error. Using this mechanism, the
 * single external DMA descriptor can save/restore context processing registers.
 * This register does not effect transfer error response of partial writes i.e.
 * partial writes (less than 32-bit writes) always provide an error response.
 * @{
 */
#define ISP_REG_XFR_DIS_XFR_ERR_DIS_Pos (31U)
#define ISP_REG_XFR_DIS_XFR_ERR_DIS_Msk ((uint32_t) ((uint32_t) 1U << (ISP_REG_XFR_DIS_XFR_ERR_DIS_Pos)))
/**
 * Transfer Error Disable.
 * 0: transfer error enabled.\n
 * 1: disables error response all registers of pipeline 1 & 2.
 */
#define ISP_REG_XFR_DIS_XFR_ERR_DIS(x)  (((uint32_t)(((uint32_t)(x)) << ISP_REG_XFR_DIS_XFR_ERR_DIS_Pos)) & ISP_REG_XFR_DIS_XFR_ERR_DIS_Msk)
/* @} */ /* REG_XFR_DIS definitions */

/**
 * @name CSI Control Register (CSI_CTRL) masks
 * @details Provides virtual channel ID for linepath 0 and 1, using which the
 * CSI interface transfers pixel data. Also, this enables CSI stream mode versus
 * reading input images from system memory. If the Neo receives data corresponding
 * to virtual channel ID that do not match the IDs programmed in this register,
 * then the corresponding data is ignored.
 * @{
 */
#define ISP_CSI_CTRL_CSI_EN_Pos         (31U)
#define ISP_CSI_CTRL_CSI_EN_Msk         ((uint32_t) ((uint32_t) 1U << (ISP_CSI_CTRL_CSI_EN_Pos)))
/**
 * CSI stream control into ISP enable.
 * 0: CSI streaming is disabled. Input is read from system memory.\n
 * 1: CSI streaming is enabled. No read is done from system memory. Minimum image width 256 pixels.
 */
#define ISP_CSI_CTRL_CSI_EN(x)          (((uint32_t)(((uint32_t)(x)) << ISP_CSI_CTRL_CSI_EN_Pos)) & ISP_CSI_CTRL_CSI_EN_Msk)

#define ISP_CSI_CTRL_DEVL_Pos           (30U)
#define ISP_CSI_CTRL_DEVL_Msk           ((uint32_t) ((uint32_t) 1U << (ISP_CSI_CTRL_DEVL_Pos)))
/*
 * Dual Exposure VSYNC lock.
 * Setting this field to 1, enables ISP to lock onto ongoing CSI frame. This
 * field should always be set to 1 if the CSI interface is active when the
 * ISP block is triggered. The locking incurs a latency of 1 frame.\n
 * If the CSI interface is inactive when the ISP is triggered and CSI is enabled
 * later then this bit can be set to 0 and this incurs no frame latency.
 */
#define ISP_CSI_CTRL_DEVL(x)            (((uint32_t)(((uint32_t)(x)) << ISP_CSI_CTRL_DEVL_Pos)) & ISP_CSI_CTRL_DEVL_Msk)

#define ISP_CSI_CTRL_SSEN_Pos           (29U)
#define ISP_CSI_CTRL_SSEN_Msk           ((uint32_t) ((uint32_t) 1U << (ISP_CSI_CTRL_SSEN_Pos)))
/*
 * Single Step Frame.
 * Setting this field to 1 only single CSI is outputted then CSI frame is
 * stopped. A re-trigger is required for restarting the CSI frame processing.\n
 * If this bit is set to 0, then every CSI frame is processed.
 * 0: process every CSI frame.\n
 * 1: process single CSI frame.\n
 */
#define ISP_CSI_CTRL_SSEN(x)            (((uint32_t)(((uint32_t)(x)) << ISP_CSI_CTRL_SSEN_Pos)) & ISP_CSI_CTRL_SSEN_Msk)

#define ISP_CSI_CTRL_VID1_Pos           (8U)
#define ISP_CSI_CTRL_VID1_Msk           ((uint32_t) ((uint32_t) 0x1FU << (ISP_CSI_CTRL_VID1_Pos)))
/*
 * Virtual channel ID for line path 1.
 */
#define ISP_CSI_CTRL_VID1(x)            (((uint32_t)(((uint32_t)(x)) << ISP_CSI_CTRL_VID1_Pos)) & ISP_CSI_CTRL_VID1_Msk)

#define ISP_CSI_CTRL_VID0_Pos           (0U)
#define ISP_CSI_CTRL_VID0_Msk           ((uint32_t) ((uint32_t) 0x1FU << (ISP_CSI_CTRL_VID0_Pos)))
/*
 * Virtual channel ID for line path 0.
 */
#define ISP_CSI_CTRL_VID0(x)            (((uint32_t)(((uint32_t)(x)) << ISP_CSI_CTRL_VID0_Pos)) & ISP_CSI_CTRL_VID0_Msk)
/** @} */ /* CSI_CTRL definitions */

/**
 * @name Register Shadowing Control Register (REG_SHD_CTRL) masks
 * @brief Provides the register shadowing control for all the registers of the ISP.
 * @{
 */
#define ISP_REG_SHD_CTRL_CTRL_Pos       (31U)
#define ISP_REG_SHD_CTRL_CTRL_Msk       ((uint32_t) ((uint32_t) 1U << (ISP_REG_SHD_CTRL_CTRL_Pos)))
/**
 * Register Shadowing Control.
 * 0: register shadowing is done at every frame start.\n
 * 1: register shadowing is done at frame start if shadowing command register is set to 1.
 */
#define ISP_REG_SHD_CTRL_CTRL(x)        (((uint32_t)(((uint32_t)(x)) << ISP_REG_SHD_CTRL_CTRL_Pos)) & ISP_REG_SHD_CTRL_CTRL_Msk)
/** @} */ /* REG_SHD_CTRL definitions */

/**
 * @name Register Shadowing Command Register (REG_SHD_CMD) masks
 * @brief Provides the register shadowing command for all the registers of the
 * ISP. Writing a non-zero value to this register set the register shadow
 * command (all 1s). Writing a zero value has no effect. When the register
 * shadowing is done by hardware, the register is cleared to 0.
 * @{
 */
#define ISP_REG_SHD_CMD_CMD_Pos         (0U)
#define ISP_REG_SHD_CMD_CMD_Msk         ((uint32_t) ((uint32_t) 0xFFFFFFFFU << (ISP_REG_SHD_CMD_CMD_Pos)))
/** @} */ /* REG_SHD_CMD definitions */

/**
 * @name Camera Trigger Register (TRIG_CAM0) masks
 * @brief Provides trigger signaling for the ISP pipeline. Performing a write
 * operation with any value 0 to 31 to this register provides a trigger (start
 * signal) for the corresponding context. When the trigger signal has started
 * the ISP pipeline operation, this register is cleared to all 0s. Each camera
 * image is treated as a context for ISP pipeline, and for starting an operation
 * corresponding to a complete camera frame respective trigger is required.
 * @{
 */
#define ISP_TRIG_CAM0_TRIGGER_Pos       (0U)
#define ISP_TRIG_CAM0_TRIGGER_Msk       ((uint32_t) ((uint32_t) 0xFFFFFFFFU << (ISP_TRIG_CAM0_TRIGGER_Pos)))
/** @} */ /* TRIG_CAM0 definitions */

/**
 * @name NEO Camera Interrupt Enable Register (INT_EN0) masks
 * @{
 */
#define ISP_INT_EN0_EN_TRIG_TERR_Pos    (30U)
#define ISP_INT_EN0_EN_TRIG_TERR_Msk    ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_TRIG_TERR_Pos)))

#define ISP_INT_EN0_EN_CSI_TERR_Pos     (29U)
#define ISP_INT_EN0_EN_CSI_TERR_Msk     ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_CSI_TERR_Pos)))

#define ISP_INT_EN0_EN_BUS_ERR3_Pos     (19U)
#define ISP_INT_EN0_EN_BUS_ERR3_Msk     ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_BUS_ERR3_Pos)))

#define ISP_INT_EN0_EN_BUS_ERR2_Pos     (18U)
#define ISP_INT_EN0_EN_BUS_ERR2_Msk     ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_BUS_ERR2_Pos)))

#define ISP_INT_EN0_EN_BUS_ERR1_Pos     (17U)
#define ISP_INT_EN0_EN_BUS_ERR1_Msk     ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_BUS_ERR1_Pos)))

#define ISP_INT_EN0_EN_BUS_ERR0_Pos     (16U)
#define ISP_INT_EN0_EN_BUS_ERR0_Msk     ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_BUS_ERR0_Pos)))

#define ISP_INT_EN0_EN_BUS_ERR_Pos      (16U)
#define ISP_INT_EN0_EN_BUS_ERR_Msk      ((uint32_t) ((uint32_t) 0xFU << (ISP_INT_EN0_EN_BUS_ERR_Pos)))

#define ISP_INT_EN0_EN_DRCD_Pos         (5U)
#define ISP_INT_EN0_EN_DRCD_Msk         ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_DRCD_Pos)))

#define ISP_INT_EN0_EN_STATD_Pos        (4U)
#define ISP_INT_EN0_EN_STATD_Msk        ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_STATD_Pos)))

#define ISP_INT_EN0_EN_FD2_Pos          (3U)
#define ISP_INT_EN0_EN_FD2_Msk          ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_FD2_Pos)))

#define ISP_INT_EN0_EN_FD1_Pos          (2U)
#define ISP_INT_EN0_EN_FD1_Msk          ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_FD1_Pos)))

#define ISP_INT_EN0_EN_FS2_Pos          (1U)
#define ISP_INT_EN0_EN_FS2_Msk          ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_FS2_Pos)))

#define ISP_INT_EN0_EN_FS1_Pos          (0U)
#define ISP_INT_EN0_EN_FS1_Msk          ((uint32_t) ((uint32_t) 1U << (ISP_INT_EN0_EN_FS1_Pos)))
/** @} */ /* INT_EN0 definitions */

/**
 * @name NEO Camera Interrupt Status Register (INT_STAT0)
 * @{
 */
#define ISP_INT_STAT0_BUSY_Pos          (31U)
#define ISP_INT_STAT0_BUSY_Msk          ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_BUSY_Pos)))

#define ISP_INT_STAT0_S_TRIG_ERR_Pos    (30U)
#define ISP_INT_STAT0_S_TRIG_ERR_Msk    ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_TRIG_ERR_Pos)))

#define ISP_INT_STAT0_S_CSI_TERR_Pos    (29U)
#define ISP_INT_STAT0_S_CSI_TERR_Msk    ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_CSI_TERR_Pos)))

#define ISP_INT_STAT0_S_BUS_ERR_Pos     (16U)
#define ISP_INT_STAT0_S_BUS_ERR_Msk     ((uint32_t) ((uint32_t) 0xFU << (ISP_INT_STAT0_S_BUS_ERR_Pos)))
#define ISP_INT_STAT0_S_BUS_ERR_Bit0    ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_BUS_ERR_Pos + 0U)))
#define ISP_INT_STAT0_S_BUS_ERR_Bit1    ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_BUS_ERR_Pos + 1U)))
#define ISP_INT_STAT0_S_BUS_ERR_Bit2    ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_BUS_ERR_Pos + 2U)))
#define ISP_INT_STAT0_S_BUS_ERR_Bit3    ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_BUS_ERR_Pos + 3U)))

#define ISP_INT_STAT0_S_DRCD_Pos        (5U)
#define ISP_INT_STAT0_S_DRCD_Msk        ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_DRCD_Pos)))

#define ISP_INT_STAT0_S_STATD_Pos       (4U)
#define ISP_INT_STAT0_S_STATD_Msk       ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_STATD_Pos)))

#define ISP_INT_STAT0_S_FD2_Pos         (3U)
#define ISP_INT_STAT0_S_FD2_Msk         ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_FD2_Pos)))

#define ISP_INT_STAT0_S_FD1_Pos         (2U)
#define ISP_INT_STAT0_S_FD1_Msk         ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_FD1_Pos)))

#define ISP_INT_STAT0_S_FS2_Pos         (1U)
#define ISP_INT_STAT0_S_FS2_Msk         ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_FS2_Pos)))

#define ISP_INT_STAT0_S_FS1_Pos         (0U)
#define ISP_INT_STAT0_S_FS1_Msk         ((uint32_t) ((uint32_t) 1U << (ISP_INT_STAT0_S_FS1_Pos)))
/** @} */ /* INT_STAT0 definitions */

/*!
 * @}
 */ /* end of group ISP_Register_Masks */


/*!
 * @}
 */ /* end of group ISP_Peripheral_Access_Layer */


/*!
 * @}
 */ /* end of group Peripheral_access_layer */

/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
  #else
    #pragma pop
  #endif
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

#endif  /* PERI_ISP_DEF_H_ */
