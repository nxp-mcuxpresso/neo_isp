/*******************************************************************************
 * Copyright 2023-2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_CRC32C_H
#define UTILS_CRC32C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*******************************************************************************/
/**
 * @brief Computes 32bit CRC value for the given payload based on CRC32-C
 *
 * @details if acpcPayload == NULL, the resulting CRC is 0 and warning is logged.
 *
 * @param acpcPayload payload pointer
 * @param acPayloadLength payload length in bytes
 *
 * @return  resulting CRC value
 *******************************************************************************/
uint32_t utils_CalcCrc32c(const uint8_t* const acpcPayload, const uint32_t acPayloadLength);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_CRC16_CCITT_LUT_H */
