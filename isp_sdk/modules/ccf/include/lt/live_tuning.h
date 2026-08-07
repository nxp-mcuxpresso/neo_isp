/*******************************************************************************
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef LIVE_TUNING_H
#define LIVE_TUNING_H

/******************************************************************************/
/**
 * @file  live_tuning.h
 * @brief Live tuning public interface header file
 *******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define LT_CAMERA_CNT 4

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/******************************************************************************/
/**
 * @brief   Live tuning control service operation result
 *******************************************************************************/
typedef enum
{
    LT_OK          = 0x00U, /**< operation succeeded */
    LT_GEN_ERR     = 0x01U, /**< general error */
} lt_Result_t;

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Parses and processes a raw Live Tuning packet.
 *
 * @details This function extracts header, allocates a buffer for the payload,
 * and performs bounds checking. It then copies the payload, extracts the checksum
 * and calls the request packet processor.
 *
 * @param[in] apPacket   Pointer to the raw packet data.
 * @param[in] aLength    Length of the packet data in bytes.
 *
 * @return  0 on success,
 *          -1 on error (e.g., invalid size, allocation failure, processing error).
 ******************************************************************************/
int32_t lt_ProcessRawPacket(const uint8_t* apPacket, int32_t aLength);

/******************************************************************************
 * @brief   Initializes the Live tuning module.
 *
 * @details Sets up the buffer pool and write function callback.
 *
 * @param[in] WriteFunc Write function.
 *
 * @return  0 on success,
 *          otherwise if:
 *              - the buffer pool allocation fails,
 *              - write function is invalid or cannot be set,
 *******************************************************************************/
lt_Result_t lt_InitLtModule(int32_t (*WriteFunc)(const uint8_t* capData, size_t aSize));


#ifdef __cplusplus
}
#endif

#endif // LIVE_TUNING_H
