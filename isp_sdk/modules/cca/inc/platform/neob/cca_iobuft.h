/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCA_IOBUFT_H
#define CCA_IOBUFT_H

/******************************************************************************/
/**
 * @file cca_iobuft.h
 * @brief CCA definitions of i.MX 95 specific input/output buffer types.
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include <stddef.h>

/** @addtogroup cca_pub_api CCA (CameraControlAbstraction) public API
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                    ENUMS
==============================================================================*/

/******************************************************************************/
/**
* @brief   image IO buffer types
*
* The buffer types do not have serve for simple SW identification of available buffers.
* They do not have to follow the HW input/output paths and registers 1:1.
*******************************************************************************/
typedef enum
{
    CCA_IOBUFT_IN0   = 0x00U,   /**< 1st raw data input, usually combined or long exposure */
    CCA_IOBUFT_IN1    = 0x01U,   /**< 2nd raw data input, usually very short exposure or not used */
    CCA_IOBUFT_C012  = 0x02U,   /**< output buffer for interleaved image */
    CCA_IOBUFT_C0    = 0x03U,   /**< output buffer with first component of the semiplanar image, usually Y plane */
    CCA_IOBUFT_C12   = 0x04U,   /**< output buffer with the second and third components interleaved from the semiplanar image format, usually UV plane */
    CCA_IOBUFT_IR    = 0x05U,   /**< output buffer with infrared image = monochrome */
#ifdef ISIM
    CCA_IOBUFT_RGB   = 0x06U,   /**< output buffer specific to ISIM with RGB888 interleaved image */
    CCA_IOBUFT_OUT_LAST  = CCA_IOBUFT_RGB, /**< last output buffer type */
    CCA_IOBUFT_CNT       = 0x07, /**< number of I/O buffer types */
#else
    CCA_IOBUFT_OUT_LAST  = CCA_IOBUFT_IR, /**< last output buffer type */
    CCA_IOBUFT_CNT       = 0x06, /**< number of I/O buffer types */
#endif
    CCA_IOBUFT_IN_FIRST  = CCA_IOBUFT_IN0, /**< first input buffer type index */
    CCA_IOBUFT_IN_LAST   = CCA_IOBUFT_IN1, /**< last input buffer type index */
    CCA_IOBUFT_OUT_FIRST = CCA_IOBUFT_C012, /** first output buffer type index */
} cca_IoBufTypes_t;

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */ // addtogroup cca_pub_api
#endif /* #ifndef CCA_IOBUFT_H */
