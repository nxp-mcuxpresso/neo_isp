/*
** ###################################################################
**     Processor:           _CM33
**     Compiler:            Keil ARM C/C++ Compiler
**     Build:               b230704
**
**     Abstract:
**         CMSIS Peripheral Access Layer for mx95_CM33
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**
** ###################################################################
*/

/*!
 * @file mx95_CAMERA_BLK_CTRL_CAMERAMIX.h
 * @version 0.0
 * @date 0-00-00
 * @brief CMSIS Peripheral Access Layer for mx95_CAMERA_BLK_CTRL_CAMERAMIX
 *
 * CMSIS Peripheral Access Layer for mx95
 */

/* Prevention from multiple including the same memory map */
#if !defined(mx95_CAMERA_BLK_CTRL_CAMERAMIX_H_)  /* Check if memory map has not been already included */
#define mx95_CAMERA_BLK_CTRL_CAMERAMIX_H_

#include "mx95_COMMON.h"

/* ----------------------------------------------------------------------------
   -- CAMERA_BLK_CTRL_CAMERAMIX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CAMERA_BLK_CTRL_CAMERAMIX_Peripheral_Access_Layer CAMERA_BLK_CTRL_CAMERAMIX Peripheral Access Layer
 * @{
 */

/** CAMERA_BLK_CTRL_CAMERAMIX - Register Layout Typedef */
typedef struct {
  __IO uint32_t ISP_CLOCK_GATING_CONTROL;          /**< ISP Clock gating control, offset: 0x0 */
  __IO uint32_t ISP_AXCACHE_CONTROL;               /**< ISP AxCache control, offset: 0x4 */
  __IO uint32_t ISP_QOS_SETTING;                   /**< ISP QoS setting, offset: 0x8 */
  __IO uint32_t ISI_AXCACHE_CONTROL;               /**< ISI AxCache control, offset: 0xC */
  __IO uint32_t ISI_QOS_SETTING;                   /**< ISI QoS setting, offset: 0x10 */
  __IO uint32_t PANIC_QOS;                         /**< ISI Panic QoS setting, offset: 0x14 */
  __I  uint32_t INIT_PENDING_TX;                   /**< Init_pending_Tx, offset: 0x18 */
       uint8_t RESERVED_0[4];
  __IO uint32_t CSI0_VC0_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x20 */
  __IO uint32_t CSI0_VC1_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x24 */
  __IO uint32_t CSI0_VC2_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x28 */
  __IO uint32_t CSI0_VC3_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x2C */
  __IO uint32_t CSI0_VC4_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x30 */
  __IO uint32_t CSI0_VC5_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x34 */
  __IO uint32_t CSI0_VC6_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x38 */
  __IO uint32_t CSI0_VC7_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x3C */
  __IO uint32_t CSI0_VC_INTERLACED_CTRL;           /**< CSI0 VC Interlaced Control, offset: 0x40 */
  __IO uint32_t CSI0_VC_INTERLACED_ERROR;          /**< CSI0 VC Interlaced Error, offset: 0x44 */
  __IO uint32_t CSI0_YUV420_FIRST_LINE_EVEN;       /**< CSI0 YUV420 First Line Even, offset: 0x48 */
       uint8_t RESERVED_1[4];
  __IO uint32_t CSI0_RAW32_CTR;                    /**< CSI0 RAW32 Control, offset: 0x50 */
  __IO uint32_t CSI0_STREAM_FENCING_CTRL;          /**< Stream fencing control., offset: 0x54 */
  __I  uint32_t CSI0_STREAM_FENCING_STATUS;        /**< Stream fencing status., offset: 0x58 */
       uint8_t RESERVED_2[4];
  __IO uint32_t CSI0_NP_DATA_TYPE_VC0;             /**< CSI0 VC0 non-pixel data type, offset: 0x60 */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC1;             /**< CSI0 VC1 non-pixel data type, offset: 0x64 */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC2;             /**< CSI0 VC2 non-pixel data type, offset: 0x68 */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC3;             /**< CSI0 VC3 non-pixel data type, offset: 0x6C */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC4;             /**< CSI0 VC4 non-pixel data type, offset: 0x70 */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC5;             /**< CSI0 VC5 non-pixel data type, offset: 0x74 */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC6;             /**< CSI0 VC6 non-pixel data type, offset: 0x78 */
  __IO uint32_t CSI0_NP_DATA_TYPE_VC7;             /**< CSI0 VC7 non-pixel data type, offset: 0x7C */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC0;          /**< CSI0 Pixel Data Control VC0, offset: 0x80 */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC1;          /**< CSI0 Pixel Data Control VC1, offset: 0x84 */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC2;          /**< CSI0 Pixel Data Control VC2, offset: 0x88 */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC3;          /**< CSI0 Pixel Data Control VC3, offset: 0x8C */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC4;          /**< CSI0 Pixel Data Control VC4, offset: 0x90 */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC5;          /**< CSI0 Pixel Data Control VC5, offset: 0x94 */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC6;          /**< CSI0 Pixel Data Control VC6, offset: 0x98 */
  __IO uint32_t CSI0_PIXEL_DATA_CTRL_VC7;          /**< CSI0 Pixel Data Control VC7, offset: 0x9C */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC0;    /**< CSI0 Route Pixel Data Type VC0, offset: 0xA0 */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC1;    /**< CSI0 Route Pixel Data Type VC1, offset: 0xA4 */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC2;    /**< CSI0 Route Pixel Data Type VC2, offset: 0xA8 */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC3;    /**< CSI0 Route Pixel Data Type VC3, offset: 0xAC */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC4;    /**< CSI0 Route Pixel Data Type VC4, offset: 0xB0 */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC5;    /**< CSI0 Route Pixel Data Type VC5, offset: 0xB4 */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC6;    /**< CSI0 Route Pixel Data Type VC6, offset: 0xB8 */
  __IO uint32_t CSI0_ROUTE_PIXEL_DATA_TYPE_VC7;    /**< CSI0 Route Pixel Data Type VC7, offset: 0xBC */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC0;      /**< CSI0 Non-Pixel Data Control VC0, offset: 0xC0 */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC1;      /**< CSI0 Non-Pixel Data Control VC1, offset: 0xC4 */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC2;      /**< CSI0 Non-Pixel Data Control VC2, offset: 0xC8 */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC3;      /**< CSI0 Non-Pixel Data Control VC3, offset: 0xCC */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC4;      /**< CSI0 Non-Pixel Data Control VC4, offset: 0xD0 */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC5;      /**< CSI0 Non-Pixel Data Control VC5, offset: 0xD4 */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC6;      /**< CSI0 Non-Pixel Data Control VC6, offset: 0xD8 */
  __IO uint32_t CSI0_NON_PIXEL_DATA_CTRL_VC7;      /**< CSI0 Non-Pixel Data Control VC7, offset: 0xDC */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC0;          /**< CSI0 Pixel Data Type VC0, offset: 0xE0 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC1;          /**< CSI0 Pixel Data Type VC1, offset: 0xE4 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC2;          /**< CSI0 Pixel Data Type VC2, offset: 0xE8 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC3;          /**< CSI0 Pixel Data Type VC3, offset: 0xEC */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC4;          /**< CSI0 Pixel Data Type VC4, offset: 0xF0 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC5;          /**< CSI0 Pixel Data Type VC5, offset: 0xF4 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC6;          /**< CSI0 Pixel Data Type VC6, offset: 0xF8 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_VC7;          /**< CSI0 Pixel Data Type VC7, offset: 0xFC */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC0;      /**< CSI0 Pixel Data Type Error VC0, offset: 0x100 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC1;      /**< CSI0 Pixel Data Type Error VC1, offset: 0x104 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC2;      /**< CSI0 Pixel Data Type Error VC2, offset: 0x108 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC3;      /**< CSI0 Pixel Data Type Error VC3, offset: 0x10C */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC4;      /**< CSI0 Pixel Data Type Error VC4, offset: 0x110 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC5;      /**< CSI0 Pixel Data Type Error VC5, offset: 0x114 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC6;      /**< CSI0 Pixel Data Type Error VC6, offset: 0x118 */
  __IO uint32_t CSI0_PIXEL_DATA_TYPE_ERR_VC7;      /**< CSI0 Pixel Data Type Error VC7, offset: 0x11C */
  __IO uint32_t CSI1_VC0_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x120 */
  __IO uint32_t CSI1_VC1_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x124 */
  __IO uint32_t CSI1_VC2_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x128 */
  __IO uint32_t CSI1_VC3_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x12C */
  __IO uint32_t CSI1_VC4_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x130 */
  __IO uint32_t CSI1_VC5_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x134 */
  __IO uint32_t CSI1_VC6_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x138 */
  __IO uint32_t CSI1_VC7_INTERLACED_LINE_COUNT_SET; /**< Virtual Channel interlaced Line count, offset: 0x13C */
  __IO uint32_t CSI1_VC_INTERLACED_CTRL;           /**< CSI1 VC Interlaced Control, offset: 0x140 */
  __IO uint32_t CSI1_VC_INTERLACED_ERROR;          /**< CSI1 VC Interlaced Error, offset: 0x144 */
  __IO uint32_t CSI1_YUV420_FIRST_LINE_EVEN;       /**< CSI1 YUV420 First Line Even, offset: 0x148 */
       uint8_t RESERVED_3[4];
  __IO uint32_t CSI1_RAW32_CTR;                    /**< CSI1 RAW32 Control, offset: 0x150 */
  __IO uint32_t CSI1_STREAM_FENCING_CTRL;          /**< Stream fencing control., offset: 0x154 */
  __I  uint32_t CSI1_STREAM_FENCING_STATUS;        /**< Stream fencing status., offset: 0x158 */
       uint8_t RESERVED_4[4];
  __IO uint32_t CSI1_NP_DATA_TYPE_VC0;             /**< CSI1 VC0 non-pixel data type, offset: 0x160 */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC1;             /**< CSI1 VC1 non-pixel data type, offset: 0x164 */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC2;             /**< CSI1 VC2 non-pixel data type, offset: 0x168 */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC3;             /**< CSI1 VC3 non-pixel data type, offset: 0x16C */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC4;             /**< CSI1 VC4 non-pixel data type, offset: 0x170 */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC5;             /**< CSI1 VC5 non-pixel data type, offset: 0x174 */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC6;             /**< CSI1 VC6 non-pixel data type, offset: 0x178 */
  __IO uint32_t CSI1_NP_DATA_TYPE_VC7;             /**< CSI1 VC7 non-pixel data type, offset: 0x17C */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC0;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x180 */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC1;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x184 */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC2;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x188 */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC3;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x18C */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC4;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x190 */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC5;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x194 */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC6;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x198 */
  __IO uint32_t CSI1_PIXEL_DATA_CTRL_VC7;          /**< Control of the routing of the pixel data on Pixel link virtual channel., offset: 0x19C */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC0;    /**< CSI1 Route Pixel Data Type VC0, offset: 0x1A0 */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC1;    /**< CSI1 Route Pixel Data Type VC1, offset: 0x1A4 */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC2;    /**< CSI1 Route Pixel Data Type VC2, offset: 0x1A8 */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC3;    /**< CSI1 Route Pixel Data Type VC3, offset: 0x1AC */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC4;    /**< CSI1 Route Pixel Data Type VC4, offset: 0x1B0 */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC5;    /**< CSI1 Route Pixel Data Type VC5, offset: 0x1B4 */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC6;    /**< CSI1 Route Pixel Data Type VC6, offset: 0x1B8 */
  __IO uint32_t CSI1_ROUTE_PIXEL_DATA_TYPE_VC7;    /**< CSI1 Route Pixel Data Type VC7, offset: 0x1BC */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC0;      /**< CSI1 Non-Pixel Data Control VC0, offset: 0x1C0 */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC1;      /**< CSI1 Non-Pixel Data Control VC1, offset: 0x1C4 */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC2;      /**< CSI1 Non-Pixel Data Control VC2, offset: 0x1C8 */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC3;      /**< CSI1 Non-Pixel Data Control VC3, offset: 0x1CC */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC4;      /**< CSI1 Non-Pixel Data Control VC4, offset: 0x1D0 */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC5;      /**< CSI1 Non-Pixel Data Control VC5, offset: 0x1D4 */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC6;      /**< CSI1 Non-Pixel Data Control VC6, offset: 0x1D8 */
  __IO uint32_t CSI1_NON_PIXEL_DATA_CTRL_VC7;      /**< CSI1 Non-Pixel Data Control VC7, offset: 0x1DC */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC0;          /**< CSI1 Pixel Data Type VC0, offset: 0x1E0 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC1;          /**< CSI1 Pixel Data Type VC1, offset: 0x1E4 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC2;          /**< CSI1 Pixel Data Type VC2, offset: 0x1E8 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC3;          /**< CSI1 Pixel Data Type VC3, offset: 0x1EC */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC4;          /**< CSI1 Pixel Data Type VC4, offset: 0x1F0 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC5;          /**< CSI1 Pixel Data Type VC5, offset: 0x1F4 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC6;          /**< CSI1 Pixel Data Type VC6, offset: 0x1F8 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_VC7;          /**< CSI1 Pixel Data Type VC7, offset: 0x1FC */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC0;      /**< CSI1 Pixel Data Type Error VC0, offset: 0x200 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC1;      /**< CSI1 Pixel Data Type Error VC1, offset: 0x204 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC2;      /**< CSI1 Pixel Data Type Error VC2, offset: 0x208 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC3;      /**< CSI1 Pixel Data Type Error VC3, offset: 0x20C */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC4;      /**< CSI1 Pixel Data Type Error VC4, offset: 0x210 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC5;      /**< CSI1 Pixel Data Type Error VC5, offset: 0x214 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC6;      /**< CSI1 Pixel Data Type Error VC6, offset: 0x218 */
  __IO uint32_t CSI1_PIXEL_DATA_TYPE_ERR_VC7;      /**< CSI1 Pixel Data Type Error VC7, offset: 0x21C */
  __IO uint32_t CSI0_PIXEL_LINK_RAW10_FORMAT;      /**< Pixel link RAW10 format, offset: 0x220 */
  __IO uint32_t CSI1_PIXEL_LINK_RAW10_FORMAT;      /**< Pixel link RAW10 format, offset: 0x224 */
  __IO uint32_t CSI0_BUFFER_OVERFLOW;              /**< Buffer Overflow, offset: 0x228 */
  __IO uint32_t CSI1_BUFFER_OVERFLOW;              /**< Buffer Overflow, offset: 0x22C */
       uint8_t RESERVED_5[208];
  __IO uint32_t CM0P_ADDR_OFFSET1;                 /**< Cortex-M0+ Address Offset 1, offset: 0x300 */
  __IO uint32_t CM0P_ADDR_OFFSET2;                 /**< Cortex-M0+ Address Offset 2, offset: 0x304 */
  __IO uint32_t CM0P_CPUWAIT;                      /**< Cortex-M0+ CPUWAIT, offset: 0x308 */
  __IO uint32_t CM0P_CTL;                          /**< Cortex-M0+ Control, offset: 0x30C */
  __I  uint32_t CM0P_STAT;                         /**< Cortex-M0+ Status, offset: 0x310 */
  __IO uint32_t LPCAC_ERROR;                       /**< LPCAC Error, offset: 0x314 */
  __IO uint32_t LPCAC_ERROR_ADDR;                  /**< LPCAC Error Address, offset: 0x318 */
  __IO uint32_t LPCAC_ERROR_DATA;                  /**< LPCAC Error Address, offset: 0x31C */
  __IO uint32_t ISP_CONFIG;                        /**< ISP Configuration, offset: 0x320 */
} CAMERA_BLK_CTRL_CAMERAMIX_Type;

/* ----------------------------------------------------------------------------
   -- CAMERA_BLK_CTRL_CAMERAMIX Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CAMERA_BLK_CTRL_CAMERAMIX_Register_Masks CAMERA_BLK_CTRL_CAMERAMIX Register Masks
 * @{
 */

/*! @name ISP_CLOCK_GATING_CONTROL - ISP Clock gating control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_0_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_0_SHIFT (0U)
/*! CSI2_pixel_formatting_0 - Pixel link clock gate control of the CSI2 pixel formatting 0 (Standalone)
 *  0b0..Do not gate the Pixel link clock of the CSI2 pixel formatting 0
 *  0b1..Gate the Pixel link clock of the CSI2 pixel formatting 0
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_1_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_1_SHIFT (1U)
/*! CSI2_pixel_formatting_1 - Pixel link clock gate control of the CSI2 pixel formatting 1 (Combo)
 *  0b1..Gate the Pixel link clock of the CSI2 pixel formatting 1
 *  0b0..Do not gate the Pixel link clock of the CSI2 pixel formatting 1
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_CSI2_PIXEL_FORMATTING_1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_AXI_CLOCK_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_AXI_CLOCK_SHIFT (4U)
/*! ISP_AXI_clock - AXI clock gate control of the ISP
 *  0b1..Gate the AXI clock of the ISP
 *  0b0..Do not gate the AXI clock of the ISP
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_AXI_CLOCK(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_AXI_CLOCK_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_AXI_CLOCK_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_PIXEL_CLOCK_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_PIXEL_CLOCK_SHIFT (5U)
/*! ISP_Pixel_clock - Pixel clock gate control of the ISP
 *  0b1..Gate the Pixel clock of the ISP
 *  0b0..Do not gate the Pixel clock of the ISP
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_PIXEL_CLOCK(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_PIXEL_CLOCK_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_PIXEL_CLOCK_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_ISP_CLOCK_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_ISP_CLOCK_SHIFT (6U)
/*! ISP_ISP_clock - ISP clock gate control of the ISP
 *  0b0..Do not gate the ISP clock of the ISP
 *  0b1..Gate the ISP clock of the ISP
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_ISP_CLOCK(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_ISP_CLOCK_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_CLOCK_GATING_CONTROL_ISP_ISP_CLOCK_MASK)
/*! @} */

/*! @name ISP_AXCACHE_CONTROL - ISP AxCache control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_ARCACHE_MASK (0xFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_ARCACHE_SHIFT (0U)
/*! ArCache - Set the AXI ArCache signal for the AXI read master ports */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_ARCACHE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_ARCACHE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_ARCACHE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_AWCACHE_MASK (0xF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_AWCACHE_SHIFT (8U)
/*! AwCache - Set the AXI AwCache signal for the AXI write master ports */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_AWCACHE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_AWCACHE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_AXCACHE_CONTROL_AWCACHE_MASK)
/*! @} */

/*! @name ISP_QOS_SETTING - ISP QoS setting */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AWQOS_MASK (0x7U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AWQOS_SHIFT (0U)
/*! AwQoS - Set the AXI AwQoS signal for all ISP AXI write ports. */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AWQOS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AWQOS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AWQOS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_ARQOS_MASK (0x700U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_ARQOS_SHIFT (8U)
/*! ArQoS - Set the AXI ArQoS signal for all ISP AXI write ports. */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_ARQOS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_ARQOS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_ARQOS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AR_LOCAL_PANIC_QOS_MASK (0x7000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AR_LOCAL_PANIC_QOS_SHIFT (12U)
/*! Ar_local_panic_QoS - Set the AXI Hurry/Pressure QoS signal when the o_arpostqos of the AXI read port are set to '1'. */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AR_LOCAL_PANIC_QOS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AR_LOCAL_PANIC_QOS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_QOS_SETTING_AR_LOCAL_PANIC_QOS_MASK)
/*! @} */

/*! @name ISI_AXCACHE_CONTROL - ISI AxCache control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_ARCACHE_MASK (0xFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_ARCACHE_SHIFT (0U)
/*! ArCache - Set the AXI ArCache signal for the AXI read master ports */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_ARCACHE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_ARCACHE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_ARCACHE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_AWCACHE_MASK (0xF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_AWCACHE_SHIFT (8U)
/*! AwCache - Set the AXI AwCache signal for the AXI write master ports */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_AWCACHE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_AWCACHE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISI_AXCACHE_CONTROL_AWCACHE_MASK)
/*! @} */

/*! @name ISI_QOS_SETTING - ISI QoS setting */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_QOS_SETTING_AWQOS_MASK (0x7U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_QOS_SETTING_AWQOS_SHIFT (0U)
/*! AwQoS - Set the AXI AwQoS signal for all ISI AXI read and write ports. */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISI_QOS_SETTING_AWQOS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISI_QOS_SETTING_AWQOS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISI_QOS_SETTING_AWQOS_MASK)
/*! @} */

/*! @name PANIC_QOS - ISI Panic QoS setting */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_PANIC_QOS_PANIC_QOS_MASK (0x7U)
#define CAMERA_BLK_CTRL_CAMERAMIX_PANIC_QOS_PANIC_QOS_SHIFT (0U)
/*! Panic_QoS - Set the AXI Hurry AwQoS signal for all ISI and ISP AXI write ports, when corresponding panic signals are active. */
#define CAMERA_BLK_CTRL_CAMERAMIX_PANIC_QOS_PANIC_QOS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_PANIC_QOS_PANIC_QOS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_PANIC_QOS_PANIC_QOS_MASK)
/*! @} */

/*! @name INIT_PENDING_TX - Init_pending_Tx */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_0_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_0_SHIFT (0U)
/*! Read_AXI_ISP_0 - Read_AXI_ISP_0 pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_1_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_1_SHIFT (1U)
/*! Read_AXI_ISP_1 - Read_AXI_ISP_1 pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_AXI_ISP_1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_0_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_0_SHIFT (2U)
/*! Write_AXI_ISP_0 - Write_AXI_ISP_0 pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_1_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_1_SHIFT (3U)
/*! Write_AXI_ISP_1 - Write_AXI_ISP_1 pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_AXI_ISP_1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_ISI_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_ISI_SHIFT (4U)
/*! Read_ISI - Read_ISI pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_ISI(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_ISI_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_READ_ISI_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_Y_ISI_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_Y_ISI_SHIFT (5U)
/*! Write_Y_ISI - Write_Y_ISI pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_Y_ISI(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_Y_ISI_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_Y_ISI_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_U_ISI_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_U_ISI_SHIFT (6U)
/*! Write_U_ISI - Write_U_ISI pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_U_ISI(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_U_ISI_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_U_ISI_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_V_ISI_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_V_ISI_SHIFT (7U)
/*! Write_V_ISI - Write_V_ISI pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_V_ISI(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_V_ISI_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_WRITE_V_ISI_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_CM0P_MASK (0x100U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_CM0P_SHIFT (8U)
/*! CM0P - CM0P pending transaction
 *  0b0..No pending transaction
 *  0b1..Pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_CM0P(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_CM0P_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_CM0P_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_EDMA_MASK (0x200U)
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_EDMA_SHIFT (9U)
/*! eDMA - eDMA pending transaction
 *  0b1..Pending transaction
 *  0b0..No pending transaction
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_EDMA(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_EDMA_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_INIT_PENDING_TX_EDMA_MASK)
/*! @} */

/*! @name CSI0_VC0_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC1_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC2_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC3_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC4_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC5_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC6_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC7_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI0_VC_INTERLACED_CTRL - CSI0 VC Interlaced Control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC0_INTERLACE_MODE_MASK (0x3U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC0_INTERLACE_MODE_SHIFT (0U)
/*! CSI0_VC0_interlace_mode - CSI0 VC0 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC0_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC0_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC0_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC1_INTERLACE_MODE_MASK (0xCU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC1_INTERLACE_MODE_SHIFT (2U)
/*! CSI0_VC1_interlace_mode - CSI0 VC1 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC1_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC1_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC1_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC2_INTERLACE_MODE_MASK (0x30U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC2_INTERLACE_MODE_SHIFT (4U)
/*! CSI0_VC2_interlace_mode - CSI0 VC2 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC2_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC2_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC2_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC3_INTERLACE_MODE_MASK (0xC0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC3_INTERLACE_MODE_SHIFT (6U)
/*! CSI0_VC3_interlace_mode - CSI0 VC3 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC3_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC3_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC3_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC4_INTERLACE_MODE_MASK (0x300U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC4_INTERLACE_MODE_SHIFT (8U)
/*! CSI0_VC4_interlace_mode - CSI0 VC4 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC4_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC4_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC4_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC5_INTERLACE_MODE_MASK (0xC00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC5_INTERLACE_MODE_SHIFT (10U)
/*! CSI0_VC5_interlace_mode - CSI0 VC5 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC5_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC5_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC5_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC6_INTERLACE_MODE_MASK (0x3000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC6_INTERLACE_MODE_SHIFT (12U)
/*! CSI0_VC6_interlace_mode - CSI0 VC6 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC6_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC6_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC6_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC7_INTERLACE_MODE_MASK (0xC000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC7_INTERLACE_MODE_SHIFT (14U)
/*! CSI0_VC7_interlace_mode - CSI0 VC7 interlace mode
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC7_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC7_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_CTRL_CSI0_VC7_INTERLACE_MODE_MASK)
/*! @} */

/*! @name CSI0_VC_INTERLACED_ERROR - CSI0 VC Interlaced Error */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC0_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC0_SHIFT (0U)
/*! CSI0_VC0 - CSI0 VC1 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC1_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC1_SHIFT (1U)
/*! CSI0_VC1 - CSI0 VC1 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC2_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC2_SHIFT (2U)
/*! CSI0_VC2 - CSI0 VC2 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC2(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC2_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC2_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC3_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC3_SHIFT (3U)
/*! CSI0_VC3 - CSI0 VC3 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC3(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC3_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC3_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC4_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC4_SHIFT (4U)
/*! CSI0_VC4 - CSI0 VC4 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC4(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC4_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC4_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC5_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC5_SHIFT (5U)
/*! CSI0_VC5 - CSI0 VC5 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC5(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC5_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC5_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC6_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC6_SHIFT (6U)
/*! CSI0_VC6 - CSI0 VC6 line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC6(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC6_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC6_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC7_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC7_SHIFT (7U)
/*! CSI0_VC7 - CSI0 VC7 line count mismatch
 *  0b1..Line count mismatch error
 *  0b0..No line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC7(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC7_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_VC_INTERLACED_ERROR_CSI0_VC7_MASK)
/*! @} */

/*! @name CSI0_YUV420_FIRST_LINE_EVEN - CSI0 YUV420 First Line Even */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC0_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC0_SHIFT (0U)
/*! VC0 - CSI0 VC0 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC1_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC1_SHIFT (1U)
/*! VC1 - CSI0 VC1 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC2_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC2_SHIFT (2U)
/*! VC2 - CSI0 VC2 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC2_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC2_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC3_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC3_SHIFT (3U)
/*! VC3 - CSI0 VC3 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC3_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC3_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC4_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC4_SHIFT (4U)
/*! VC4 - CSI0 VC4 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC4(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC4_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC4_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC5_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC5_SHIFT (5U)
/*! VC5 - CSI0 VC5 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC5(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC5_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC5_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC6_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC6_SHIFT (6U)
/*! VC6 - CSI0 VC6 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC6(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC6_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC6_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC7_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC7_SHIFT (7U)
/*! VC7 - CSI0 VC7 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC7(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC7_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_YUV420_FIRST_LINE_EVEN_VC7_MASK)
/*! @} */

/*! @name CSI0_RAW32_CTR - CSI0 RAW32 Control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC0_RAW32_MODE_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC0_RAW32_MODE_SHIFT (0U)
/*! CSI0_VC0_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC0_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC0_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC0_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC1_RAW32_MODE_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC1_RAW32_MODE_SHIFT (1U)
/*! CSI0_VC1_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC1_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC1_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC1_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC2_RAW32_MODE_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC2_RAW32_MODE_SHIFT (2U)
/*! CSI0_VC2_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC2_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC2_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC2_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC3_RAW32_MODE_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC3_RAW32_MODE_SHIFT (3U)
/*! CSI0_VC3_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC3_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC3_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC3_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC4_RAW32_MODE_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC4_RAW32_MODE_SHIFT (4U)
/*! CSI0_VC4_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC4_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC4_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC4_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC5_RAW32_MODE_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC5_RAW32_MODE_SHIFT (5U)
/*! CSI0_VC5_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC5_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC5_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC5_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC6_RAW32_MODE_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC6_RAW32_MODE_SHIFT (6U)
/*! CSI0_VC6_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC6_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC6_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC6_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC7_RAW32_MODE_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC7_RAW32_MODE_SHIFT (7U)
/*! CSI0_VC7_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC7_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC7_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_VC7_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_RAW_SWAP_MODE_MASK (0xFF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_RAW_SWAP_MODE_SHIFT (8U)
/*! CSI0_RAW_SWAP_MODE - Defines if a virtual channel is set in RAW SWAP mode when index corresponding to the virtual channel is set to '1'. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_RAW_SWAP_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_RAW_SWAP_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_RAW32_CTR_CSI0_RAW_SWAP_MODE_MASK)
/*! @} */

/*! @name CSI0_STREAM_FENCING_CTRL - Stream fencing control. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_FENCING_CONTROL_MASK (0xFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_FENCING_CONTROL_SHIFT (0U)
/*! CSI0_Fencing_Control - [0] Fence VC0...[7] Fence VC7 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_FENCING_CONTROL(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_FENCING_CONTROL_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_FENCING_CONTROL_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_RESET_FENCING_SM_MASK (0xFF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_RESET_FENCING_SM_SHIFT (8U)
/*! CSI0_Reset_Fencing_SM - SW reset of Fencing state machine. One per Virtual channel.[8] : Virtual channel 0...[15]: Virtual channel 7 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_RESET_FENCING_SM(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_RESET_FENCING_SM_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_CTRL_CSI0_RESET_FENCING_SM_MASK)
/*! @} */

/*! @name CSI0_STREAM_FENCING_STATUS - Stream fencing status. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_STATUS_CSI0_FENCING_STATUS_MASK (0xFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_STATUS_CSI0_FENCING_STATUS_SHIFT (0U)
/*! CSI0_Fencing_Status - Indicates when a virtual channel is fenced when set to '1'. [0] VC0 is fenced...[7] VC7 is fenced */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_STATUS_CSI0_FENCING_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_STATUS_CSI0_FENCING_STATUS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_STREAM_FENCING_STATUS_CSI0_FENCING_STATUS_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC0 - CSI0 VC0 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC0_CSI0_VC0_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC0_CSI0_VC0_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC0_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC0_CSI0_VC0_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC0_CSI0_VC0_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC0_CSI0_VC0_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC1 - CSI0 VC1 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC1_CSI0_VC1_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC1_CSI0_VC1_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC1_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC1_CSI0_VC1_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC1_CSI0_VC1_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC1_CSI0_VC1_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC2 - CSI0 VC2 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC2_CSI0_VC2_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC2_CSI0_VC2_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC2_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC2_CSI0_VC2_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC2_CSI0_VC2_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC2_CSI0_VC2_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC3 - CSI0 VC3 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC3_CSI0_VC3_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC3_CSI0_VC3_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC3_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC3_CSI0_VC3_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC3_CSI0_VC3_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC3_CSI0_VC3_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC4 - CSI0 VC4 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC4_CSI0_VC4_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC4_CSI0_VC4_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC4_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC4_CSI0_VC4_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC4_CSI0_VC4_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC4_CSI0_VC4_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC5 - CSI0 VC5 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC5_CSI0_VC5_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC5_CSI0_VC5_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC5_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC5_CSI0_VC5_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC5_CSI0_VC5_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC5_CSI0_VC5_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC6 - CSI0 VC6 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC6_CSI0_VC6_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC6_CSI0_VC6_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC6_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC6_CSI0_VC6_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC6_CSI0_VC6_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC6_CSI0_VC6_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_NP_DATA_TYPE_VC7 - CSI0 VC7 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC7_CSI0_VC7_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC7_CSI0_VC7_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI0_VC7_NP_data_type_en - Enables transport of non-pixel data on pixel link. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC7_CSI0_VC7_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC7_CSI0_VC7_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NP_DATA_TYPE_VC7_CSI0_VC7_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC0 - CSI0 Pixel Data Control VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b0..Pixel data are not re-routed.
 *  0b1..Pixel are routed to VC defined by New_VC.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC1 - CSI0 Pixel Data Control VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b1..Pixel are routed to VC defined by New_VC.
 *  0b0..Pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC2 - CSI0 Pixel Data Control VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b1..Pixel are routed to VC defined by New_VC.
 *  0b0..Pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC3 - CSI0 Pixel Data Control VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b1..Pixel are routed to VC defined by New_VC.
 *  0b0..Pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC4 - CSI0 Pixel Data Control VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b0..Pixel data are not re-routed.
 *  0b1..Pixel are routed to VC defined by New_VC.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC5 - CSI0 Pixel Data Control VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b0..Pixel data are not re-routed.
 *  0b1..Pixel are routed to VC defined by New_VC.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC6 - CSI0 Pixel Data Control VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b0..Pixel data are not re-routed.
 *  0b1..Pixel are routed to VC defined by New_VC.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_CTRL_VC7 - CSI0 Pixel Data Control VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the pixel data are routed to another VC or not
 *  0b0..Pixel data are not re-routed.
 *  0b1..Pixel are routed to VC defined by New_VC.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC0 - CSI0 Route Pixel Data Type VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC1 - CSI0 Route Pixel Data Type VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC2 - CSI0 Route Pixel Data Type VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC3 - CSI0 Route Pixel Data Type VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC4 - CSI0 Route Pixel Data Type VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC5 - CSI0 Route Pixel Data Type VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC6 - CSI0 Route Pixel Data Type VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_MASK)
/*! @} */

/*! @name CSI0_ROUTE_PIXEL_DATA_TYPE_VC7 - CSI0 Route Pixel Data Type VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC0 - CSI0 Non-Pixel Data Control VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC1 - CSI0 Non-Pixel Data Control VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC2 - CSI0 Non-Pixel Data Control VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC3 - CSI0 Non-Pixel Data Control VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC4 - CSI0 Non-Pixel Data Control VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC5 - CSI0 Non-Pixel Data Control VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC6 - CSI0 Non-Pixel Data Control VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_NON_PIXEL_DATA_CTRL_VC7 - CSI0 Non-Pixel Data Control VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC0 - CSI0 Pixel Data Type VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC1 - CSI0 Pixel Data Type VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC2 - CSI0 Pixel Data Type VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC3 - CSI0 Pixel Data Type VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC4 - CSI0 Pixel Data Type VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC5 - CSI0 Pixel Data Type VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC6 - CSI0 Pixel Data Type VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_VC7 - CSI0 Pixel Data Type VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC0 - CSI0 Pixel Data Type Error VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC1 - CSI0 Pixel Data Type Error VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC2 - CSI0 Pixel Data Type Error VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC3 - CSI0 Pixel Data Type Error VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC4 - CSI0 Pixel Data Type Error VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC5 - CSI0 Pixel Data Type Error VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC6 - CSI0 Pixel Data Type Error VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_DATA_TYPE_ERR_VC7 - CSI0 Pixel Data Type Error VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_VC0_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC0_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC1_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC1_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC2_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC2_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC3_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC3_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC4_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC4_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC5_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC5_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC6_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC6_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC7_INTERLACED_LINE_COUNT_SET - Virtual Channel interlaced Line count */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK (0x3FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT (0U)
/*! Odd_line_count - Odd line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_ODD_LINE_COUNT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK (0x3FFF0000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT (16U)
/*! Even_line_count - Even line count */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC7_INTERLACED_LINE_COUNT_SET_EVEN_LINE_COUNT_MASK)
/*! @} */

/*! @name CSI1_VC_INTERLACED_CTRL - CSI1 VC Interlaced Control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC0_INTERLACE_MODE_MASK (0x3U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC0_INTERLACE_MODE_SHIFT (0U)
/*! CSI1_VC0_interlace_mode - CSI1 VC0 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC0_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC0_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC0_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC1_INTERLACE_MODE_MASK (0xCU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC1_INTERLACE_MODE_SHIFT (2U)
/*! CSI1_VC1_interlace_mode - CSI1 VC1 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC1_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC1_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC1_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC2_INTERLACE_MODE_MASK (0x30U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC2_INTERLACE_MODE_SHIFT (4U)
/*! CSI1_VC2_interlace_mode - CSI1 VC2 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC2_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC2_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC2_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC3_INTERLACE_MODE_MASK (0xC0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC3_INTERLACE_MODE_SHIFT (6U)
/*! CSI1_VC3_interlace_mode - CSI1 VC3 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC3_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC3_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC3_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC4_INTERLACE_MODE_MASK (0x300U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC4_INTERLACE_MODE_SHIFT (8U)
/*! CSI1_VC4_interlace_mode - CSI1 VC4 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC4_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC4_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC4_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC5_INTERLACE_MODE_MASK (0xC00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC5_INTERLACE_MODE_SHIFT (10U)
/*! CSI1_VC5_interlace_mode - CSI1 VC5 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC5_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC5_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC5_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC6_INTERLACE_MODE_MASK (0x3000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC6_INTERLACE_MODE_SHIFT (12U)
/*! CSI1_VC6_interlace_mode - CSI1 VC6 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC6_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC6_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC6_INTERLACE_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC7_INTERLACE_MODE_MASK (0xC000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC7_INTERLACE_MODE_SHIFT (14U)
/*! CSI1_VC7_interlace_mode - CSI1 VC7 interlace mode
 *  0b00..Virtual Channel is not interlaced : pixel link interlaced/not control field is set 2b00
 *  0b01..Virtual Channel is interlaced and generation done according to the CSI2 frame number
 *  0b10..Virtual Channel is interlaced and generation done according to the number of lines received during one frame
 *  0b11..Virtual Channel is interlaced. First received frame is considered as ODD.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC7_INTERLACE_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC7_INTERLACE_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_CTRL_CSI1_VC7_INTERLACE_MODE_MASK)
/*! @} */

/*! @name CSI1_VC_INTERLACED_ERROR - CSI1 VC Interlaced Error */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC0_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC0_SHIFT (0U)
/*! CSI1_VC0 - CSI1 VC0 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC1_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC1_SHIFT (1U)
/*! CSI1_VC1 - CSI1 VC1 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC2_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC2_SHIFT (2U)
/*! CSI1_VC2 - CSI1 VC2 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC2(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC2_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC2_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC3_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC3_SHIFT (3U)
/*! CSI1_VC3 - CSI1 VC3 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC3(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC3_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC3_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC4_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC4_SHIFT (4U)
/*! CSI1_VC4 - CSI1 VC4 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC4(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC4_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC4_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC5_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC5_SHIFT (5U)
/*! CSI1_VC5 - CSI1 VC5 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC5(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC5_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC5_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC6_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC6_SHIFT (6U)
/*! CSI1_VC6 - CSI1 VC6 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC6(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC6_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC6_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC7_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC7_SHIFT (7U)
/*! CSI1_VC7 - CSI1 VC7 Line count mismatch
 *  0b0..No line count mismatch error
 *  0b1..Line count mismatch error
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC7(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC7_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_VC_INTERLACED_ERROR_CSI1_VC7_MASK)
/*! @} */

/*! @name CSI1_YUV420_FIRST_LINE_EVEN - CSI1 YUV420 First Line Even */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC0_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC0_SHIFT (0U)
/*! VC0 - CSI1 VC0 first line even
 *  0b1..First line is EVEN
 *  0b0..First line is ODD
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC0_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC0_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC1_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC1_SHIFT (1U)
/*! VC1 - CSI1 VC1 first line even */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC1_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC1_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC2_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC2_SHIFT (2U)
/*! VC2 - CSI1 VC2 first line even
 *  0b1..First line is EVEN
 *  0b0..First line is ODD
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC2_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC2_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC3_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC3_SHIFT (3U)
/*! VC3 - CSI1 VC3 first line even
 *  0b1..First line is EVEN
 *  0b0..First line is ODD
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC3_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC3_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC4_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC4_SHIFT (4U)
/*! VC4 - CSI1 VC4 first line even
 *  0b1..First line is EVEN
 *  0b0..First line is ODD
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC4(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC4_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC4_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC5_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC5_SHIFT (5U)
/*! VC5 - CSI1 VC5 first line even
 *  0b1..First line is EVEN
 *  0b0..First line is ODD
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC5(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC5_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC5_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC6_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC6_SHIFT (6U)
/*! VC6 - CSI1 VC6 first line even
 *  0b1..First line is EVEN
 *  0b0..First line is ODD
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC6(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC6_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC6_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC7_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC7_SHIFT (7U)
/*! VC7 - CSI1 VC7 first line even
 *  0b0..First line is ODD
 *  0b1..First line is EVEN
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC7(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC7_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_YUV420_FIRST_LINE_EVEN_VC7_MASK)
/*! @} */

/*! @name CSI1_RAW32_CTR - CSI1 RAW32 Control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC0_RAW32_MODE_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC0_RAW32_MODE_SHIFT (0U)
/*! CSI1_VC0_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC0_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC0_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC0_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC1_RAW32_MODE_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC1_RAW32_MODE_SHIFT (1U)
/*! CSI1_VC1_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC1_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC1_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC1_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC2_RAW32_MODE_MASK (0x4U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC2_RAW32_MODE_SHIFT (2U)
/*! CSI1_VC2_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC2_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC2_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC2_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC3_RAW32_MODE_MASK (0x8U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC3_RAW32_MODE_SHIFT (3U)
/*! CSI1_VC3_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC3_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC3_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC3_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC4_RAW32_MODE_MASK (0x10U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC4_RAW32_MODE_SHIFT (4U)
/*! CSI1_VC4_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC4_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC4_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC4_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC5_RAW32_MODE_MASK (0x20U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC5_RAW32_MODE_SHIFT (5U)
/*! CSI1_VC5_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC5_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC5_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC5_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC6_RAW32_MODE_MASK (0x40U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC6_RAW32_MODE_SHIFT (6U)
/*! CSI1_VC6_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC6_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC6_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC6_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC7_RAW32_MODE_MASK (0x80U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC7_RAW32_MODE_SHIFT (7U)
/*! CSI1_VC7_RAW32_MODE - Defines if a virtual channel is set in RAW32 mode
 *  0b0..VC is not set in RAW32 mode
 *  0b1..VC is set in RAW32 mode
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC7_RAW32_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC7_RAW32_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_VC7_RAW32_MODE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_RAW_SWAP_MODE_MASK (0xFF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_RAW_SWAP_MODE_SHIFT (8U)
/*! CSI1_RAW_SWAP_MODE - Defines if a virtual channel is set in RAW SWAP mode when index corresponding to the virtual channel is set to '1'. */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_RAW_SWAP_MODE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_RAW_SWAP_MODE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_RAW32_CTR_CSI1_RAW_SWAP_MODE_MASK)
/*! @} */

/*! @name CSI1_STREAM_FENCING_CTRL - Stream fencing control. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_FENCING_CONTROL_MASK (0xFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_FENCING_CONTROL_SHIFT (0U)
/*! CSI1_Fencing_Control - [0] Fence VC0...[7] Fence VC7 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_FENCING_CONTROL(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_FENCING_CONTROL_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_FENCING_CONTROL_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_RESET_FENCING_SM_MASK (0xFF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_RESET_FENCING_SM_SHIFT (8U)
/*! CSI1_Reset_Fencing_SM - SW reset of Fencing state machine. One per Virtual channel.[8] : Virtual channel 0...[15]: Virtual channel 7 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_RESET_FENCING_SM(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_RESET_FENCING_SM_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_CTRL_CSI1_RESET_FENCING_SM_MASK)
/*! @} */

/*! @name CSI1_STREAM_FENCING_STATUS - Stream fencing status. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_STATUS_CSI1_FENCING_STATUS_MASK (0xFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_STATUS_CSI1_FENCING_STATUS_SHIFT (0U)
/*! CSI1_Fencing_Status - Indicates when a virtual channel is fenced when set to '1'. [0] VC0 is fenced...[7] VC7 is fenced */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_STATUS_CSI1_FENCING_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_STATUS_CSI1_FENCING_STATUS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_STREAM_FENCING_STATUS_CSI1_FENCING_STATUS_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC0 - CSI1 VC0 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC0_CSI1_VC0_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC0_CSI1_VC0_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC0_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC0_CSI1_VC0_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC0_CSI1_VC0_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC0_CSI1_VC0_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC1 - CSI1 VC1 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC1_CSI1_VC1_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC1_CSI1_VC1_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC1_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC1_CSI1_VC1_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC1_CSI1_VC1_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC1_CSI1_VC1_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC2 - CSI1 VC2 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC2_CSI1_VC2_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC2_CSI1_VC2_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC2_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC2_CSI1_VC2_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC2_CSI1_VC2_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC2_CSI1_VC2_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC3 - CSI1 VC3 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC3_CSI1_VC3_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC3_CSI1_VC3_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC3_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC3_CSI1_VC3_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC3_CSI1_VC3_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC3_CSI1_VC3_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC4 - CSI1 VC4 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC4_CSI1_VC4_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC4_CSI1_VC4_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC4_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC4_CSI1_VC4_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC4_CSI1_VC4_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC4_CSI1_VC4_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC5 - CSI1 VC5 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC5_CSI1_VC5_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC5_CSI1_VC5_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC5_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC5_CSI1_VC5_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC5_CSI1_VC5_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC5_CSI1_VC5_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC6 - CSI1 VC6 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC6_CSI1_VC6_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC6_CSI1_VC6_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC6_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC6_CSI1_VC6_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC6_CSI1_VC6_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC6_CSI1_VC6_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_NP_DATA_TYPE_VC7 - CSI1 VC7 non-pixel data type */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC7_CSI1_VC7_NP_DATA_TYPE_EN_MASK (0x3FFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC7_CSI1_VC7_NP_DATA_TYPE_EN_SHIFT (0U)
/*! CSI1_VC7_NP_data_type_en - Enables transport of non-pixel data on pixel link */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC7_CSI1_VC7_NP_DATA_TYPE_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC7_CSI1_VC7_NP_DATA_TYPE_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NP_DATA_TYPE_VC7_CSI1_VC7_NP_DATA_TYPE_EN_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC0 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC1 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC2 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC3 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC4 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC5 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC6 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_CTRL_VC7 - Control of the routing of the pixel data on Pixel link virtual channel. */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT (0U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC0 - CSI1 Route Pixel Data Type VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_SHIFT (0U)
/*! Others - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC0_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC1 - CSI1 Route Pixel Data Type VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC1_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC2 - CSI1 Route Pixel Data Type VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC2_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC3 - CSI1 Route Pixel Data Type VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC3_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC4 - CSI1 Route Pixel Data Type VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC4_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC5 - CSI1 Route Pixel Data Type VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC5_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC6 - CSI1 Route Pixel Data Type VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC6_RAW_MASK)
/*! @} */

/*! @name CSI1_ROUTE_PIXEL_DATA_TYPE_VC7 - CSI1 Route Pixel Data Type VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_MASK (0x1FFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_SHIFT (0U)
/*! Others - Route other pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_OTHERS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_MASK (0xFF8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_SHIFT (15U)
/*! RAW - Route RAW pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_RAW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_ROUTE_PIXEL_DATA_TYPE_VC7_RAW_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC0 - CSI1 Non-Pixel Data Control VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC0_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC1 - CSI1 Non-Pixel Data Control VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC1_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC2 - CSI1 Non-Pixel Data Control VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC2_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC3 - CSI1 Non-Pixel Data Control VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC3_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC4 - CSI1 Non-Pixel Data Control VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC4_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC5 - CSI1 Non-Pixel Data Control VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC5_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC6 - CSI1 Non-Pixel Data Control VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC6_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_NON_PIXEL_DATA_CTRL_VC7 - CSI1 Non-Pixel Data Control VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT (0U)
/*! Reroute_VC - Defines if the non-pixel data are routed to another VC or not
 *  0b1..Non-pixel data are routed to VC defined by New_VC.
 *  0b0..Non-pixel data are not re-routed.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_REROUTE_VC_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK (0xEU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT (1U)
/*! New_VC - Defines the virtual channel on which the non-pixel data are transported */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_NEW_VC(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_NON_PIXEL_DATA_CTRL_VC7_NEW_VC_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC0 - CSI1 Pixel Data Type VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC0_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC1 - CSI1 Pixel Data Type VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC1_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC2 - CSI1 Pixel Data Type VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC2_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC3 - CSI1 Pixel Data Type VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC3_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC4 - CSI1 Pixel Data Type VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC4_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC5 - CSI1 Pixel Data Type VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC5_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC6 - CSI1 Pixel Data Type VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC6_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_VC7 - CSI1 Pixel Data Type VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_SHIFT (0U)
/*! type_enable - Pixel data type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_VC7_TYPE_ENABLE_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC0 - CSI1 Pixel Data Type Error VC0 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC0_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC1 - CSI1 Pixel Data Type Error VC1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_SHIFT (0U)
/*! type_error - type enable */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC1_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC2 - CSI1 Pixel Data Type Error VC2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC2_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC3 - CSI1 Pixel Data Type Error VC3 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC3_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC4 - CSI1 Pixel Data Type Error VC4 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC4_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC5 - CSI1 Pixel Data Type Error VC5 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC5_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC6 - CSI1 Pixel Data Type Error VC6 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC6_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI1_PIXEL_DATA_TYPE_ERR_VC7 - CSI1 Pixel Data Type Error VC7 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_MASK (0xFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_SHIFT (0U)
/*! type_error - Pixel data type error */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_DATA_TYPE_ERR_VC7_TYPE_ERROR_MASK)
/*! @} */

/*! @name CSI0_PIXEL_LINK_RAW10_FORMAT - Pixel link RAW10 format */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_LINK_RAW10_FORMAT_RAW10P_MASK (0xFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_LINK_RAW10_FORMAT_RAW10P_SHIFT (0U)
/*! RAW10P - RAW10P */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_LINK_RAW10_FORMAT_RAW10P(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_LINK_RAW10_FORMAT_RAW10P_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_PIXEL_LINK_RAW10_FORMAT_RAW10P_MASK)
/*! @} */

/*! @name CSI1_PIXEL_LINK_RAW10_FORMAT - Pixel link RAW10 format */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_LINK_RAW10_FORMAT_RAW10P_MASK (0xFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_LINK_RAW10_FORMAT_RAW10P_SHIFT (0U)
/*! RAW10P - RAW10P */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_LINK_RAW10_FORMAT_RAW10P(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_LINK_RAW10_FORMAT_RAW10P_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_PIXEL_LINK_RAW10_FORMAT_RAW10P_MASK)
/*! @} */

/*! @name CSI0_BUFFER_OVERFLOW - Buffer Overflow */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_BUFFER_OVERFLOW_BUFFER_OVERFLOW_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_BUFFER_OVERFLOW_BUFFER_OVERFLOW_SHIFT (0U)
/*! Buffer_Overflow - Set when the rate adaptation buffer overflows */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI0_BUFFER_OVERFLOW_BUFFER_OVERFLOW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI0_BUFFER_OVERFLOW_BUFFER_OVERFLOW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI0_BUFFER_OVERFLOW_BUFFER_OVERFLOW_MASK)
/*! @} */

/*! @name CSI1_BUFFER_OVERFLOW - Buffer Overflow */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_BUFFER_OVERFLOW_BUFFER_OVERFLOW_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_BUFFER_OVERFLOW_BUFFER_OVERFLOW_SHIFT (0U)
/*! Buffer_Overflow - Set when the rate adaptation buffer overflows */
#define CAMERA_BLK_CTRL_CAMERAMIX_CSI1_BUFFER_OVERFLOW_BUFFER_OVERFLOW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CSI1_BUFFER_OVERFLOW_BUFFER_OVERFLOW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CSI1_BUFFER_OVERFLOW_BUFFER_OVERFLOW_MASK)
/*! @} */

/*! @name CM0P_ADDR_OFFSET1 - Cortex-M0+ Address Offset 1 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET1_OFFSET_MASK (0xFFFFFF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET1_OFFSET_SHIFT (8U)
/*! OFFSET - CM0P_ADDR_OFFSET1 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET1_OFFSET(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET1_OFFSET_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET1_OFFSET_MASK)
/*! @} */

/*! @name CM0P_ADDR_OFFSET2 - Cortex-M0+ Address Offset 2 */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET2_OFFSET_MASK (0xFFFFFF00U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET2_OFFSET_SHIFT (8U)
/*! OFFSET - CM0P_ADDR_OFFSET2 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET2_OFFSET(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET2_OFFSET_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_ADDR_OFFSET2_OFFSET_MASK)
/*! @} */

/*! @name CM0P_CPUWAIT - Cortex-M0+ CPUWAIT */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_CPW_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_CPW_SHIFT (0U)
/*! CPW - Controls CM0P CPUWAIT input signal
 *  0b0..Processor is running
 *  0b1..Processor is waiting
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_CPW(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_CPW_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_CPW_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_RST_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_RST_SHIFT (1U)
/*! RST - Software reset of CM0P core and LPCAC
 *  0b0..CM0P logic is not reset
 *  0b1..CM0P logic is reset
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_RST(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_RST_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CPUWAIT_RST_MASK)
/*! @} */

/*! @name CM0P_CTL - Cortex-M0+ Control */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_ILT_MASK (0x3FCU)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_ILT_SHIFT (2U)
/*! ILT - Controls the CM0Px IRQLATENCY[7:0] input */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_ILT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_ILT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_ILT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_CLR_MASK (0x400U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_CLR_SHIFT (10U)
/*! CLR - Clear LPCAC data cache (clr_lpcac)
 *  0b0..Disable clear of LPCAC data cache
 *  0b1..Enable clear LPCAC data cache
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_CLR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_CLR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_CLR_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_NAL_MASK (0x800U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_NAL_SHIFT (11U)
/*! NAL - Disable LPCAC data cache allocation (frc_no_alloc)
 *  0b0..Enable LPCAC data cache allocation
 *  0b1..Disable LPCAC data cache allocation (Bit DWB also needs to be set to '1')
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_NAL(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_NAL_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_NAL_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DWB_MASK (0x1000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DWB_SHIFT (12U)
/*! DWB - Disable LPCAC write buffer (dis_lpcac_wtbf)
 *  0b0..Enable write buffer
 *  0b1..Disable write buffer
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DWB(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DWB_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DWB_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DIS_MASK (0x2000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DIS_SHIFT (13U)
/*! DIS - Disable LPCAC (dis_lpcac)
 *  0b0..Enable LPCAC
 *  0b1..Disable LPCAC
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DIS(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DIS_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_DIS_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_BOOT_MASK (0x8000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_BOOT_SHIFT (15U)
/*! Boot - Boot source
 *  0b0..Boot from OCRAM_L
 *  0b1..Boot from DDR
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_BOOT(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_BOOT_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_BOOT_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_LIM_LPCAC_WTBF_MASK (0x10000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_LIM_LPCAC_WTBF_SHIFT (16U)
/*! lim_lpcac_wtbf - Limit write buffer (lim_lpcac_wtbf)
 *  0b0..If write buffer is enabled, buffer all writes to spaces that are bufferable.
 *  0b1..If write buffer is enabled, buffer all writes to spaces that are both bufferable and cacheable.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_LIM_LPCAC_WTBF(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_LIM_LPCAC_WTBF_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_LIM_LPCAC_WTBF_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_MISS_EN_MASK (0x20000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_MISS_EN_SHIFT (17U)
/*! parity_miss_en - Miss on error (parity_miss_en)
 *  0b0..Disable parity generation, parity checking, and recovery from parity faults
 *  0b1..Enable parity generation, parity checking, and recovery from parity faults
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_MISS_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_MISS_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_MISS_EN_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_FAULT_EN_MASK (0x40000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_FAULT_EN_SHIFT (18U)
/*! parity_fault_en - Enable parity error reporting
 *  0b0..Disable parity errors and parity error related information reporting
 *  0b1..Enable parity errors and parity error related information reporting
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_FAULT_EN(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_FAULT_EN_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_PARITY_FAULT_EN_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_OCRAM__IN_INIT_REQ_MASK (0x80000U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_OCRAM__IN_INIT_REQ_SHIFT (19U)
/*! OCRAM__in_init_req - OCRAM Initialization request */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_OCRAM__IN_INIT_REQ(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_OCRAM__IN_INIT_REQ_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_CTL_OCRAM__IN_INIT_REQ_MASK)
/*! @} */

/*! @name CM0P_STAT - Cortex-M0+ Status */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_STAT_LKP_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_STAT_LKP_SHIFT (0U)
/*! LKP - CM0P LOCKUP output signal value
 *  0b0..Processor is not in lockup state
 *  0b1..Processor is in lockup state, as a result of an unrecoverable exception
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_CM0P_STAT_LKP(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_CM0P_STAT_LKP_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_CM0P_STAT_LKP_MASK)
/*! @} */

/*! @name LPCAC_ERROR - LPCAC Error */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_PE_MASK (0x1U)
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_PE_SHIFT (0U)
/*! CACHE_PE - Cache data parity error (cache_data_pe). Output of that register is connected to mix output signal 'lpcac_cache_data_pe' */
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_PE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_PE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_PE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_WE_MASK (0x2U)
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_WE_SHIFT (1U)
/*! CACHE_WE - Cache write-buffer error (cache_wb_error). Output of that register is connected to
 *    mix output signal 'lpcac_cache_wb_error'
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_WE(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_WE_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_CACHE_WE_MASK)

#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_BUSERR_MASK (0xCU)
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_BUSERR_SHIFT (2U)
/*! BUSERR - Bus Error size. In case of Cache write-buffer error or Cache data parity error the
 *    LPCAC. Be_size is latched in that register.
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_BUSERR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_BUSERR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_BUSERR_MASK)
/*! @} */

/*! @name LPCAC_ERROR_ADDR - LPCAC Error Address */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_ADDR_BUSERR_ADDR_MASK (0xFFFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_ADDR_BUSERR_ADDR_SHIFT (0U)
/*! BUSERR_ADDR - In case of Cache write-buffer error or Cache data parity error the bus error address is latched in this register */
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_ADDR_BUSERR_ADDR(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_ADDR_BUSERR_ADDR_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_ADDR_BUSERR_ADDR_MASK)
/*! @} */

/*! @name LPCAC_ERROR_DATA - LPCAC Error Address */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_DATA_BUSERR_DATA_MASK (0xFFFFFFFFU)
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_DATA_BUSERR_DATA_SHIFT (0U)
/*! BUSERR_DATA - In case of Cache write-buffer error or Cache data parity error the bus error address is latched in this register */
#define CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_DATA_BUSERR_DATA(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_DATA_BUSERR_DATA_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_LPCAC_ERROR_DATA_BUSERR_DATA_MASK)
/*! @} */

/*! @name ISP_CONFIG - ISP Configuration */
/*! @{ */

#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CONFIG_PIXEL_LINK_SEL_MASK (0x3U)
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CONFIG_PIXEL_LINK_SEL_SHIFT (0U)
/*! Pixel_link_sel - Select the pixel link connected to the ISP
 *  0b00..CSI Pixel Link 0 is connected to the ISP
 *  0b01..CSI Pixel Link 1 is connected to the ISP
 */
#define CAMERA_BLK_CTRL_CAMERAMIX_ISP_CONFIG_PIXEL_LINK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CAMERA_BLK_CTRL_CAMERAMIX_ISP_CONFIG_PIXEL_LINK_SEL_SHIFT)) & CAMERA_BLK_CTRL_CAMERAMIX_ISP_CONFIG_PIXEL_LINK_SEL_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CAMERA_BLK_CTRL_CAMERAMIX_Register_Masks */


/* CAMERA_BLK_CTRL_CAMERAMIX - Peripheral instance base addresses */
/** Peripheral CAMERA__BLK_CTRL_CAMERAMIX base address */
#define CAMERA__BLK_CTRL_CAMERAMIX_BASE          (0x4AC10000u)
/** Peripheral CAMERA__BLK_CTRL_CAMERAMIX base pointer */
#define CAMERA__BLK_CTRL_CAMERAMIX               ((CAMERA_BLK_CTRL_CAMERAMIX_Type *)CAMERA__BLK_CTRL_CAMERAMIX_BASE)
/** Array initializer of CAMERA_BLK_CTRL_CAMERAMIX peripheral base addresses */
#define CAMERA_BLK_CTRL_CAMERAMIX_BASE_ADDRS     { CAMERA__BLK_CTRL_CAMERAMIX_BASE }
/** Array initializer of CAMERA_BLK_CTRL_CAMERAMIX peripheral base pointers */
#define CAMERA_BLK_CTRL_CAMERAMIX_BASE_PTRS      { CAMERA__BLK_CTRL_CAMERAMIX }

/*!
 * @}
 */ /* end of group CAMERA_BLK_CTRL_CAMERAMIX_Peripheral_Access_Layer */

#endif  /* #if !defined(mx95_CAMERA_BLK_CTRL_CAMERAMIX_H_) */
