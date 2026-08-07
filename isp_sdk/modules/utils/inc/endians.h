/*******************************************************************************
 * Copyright 2022, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ENDIANS_H
#define ENDIANS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  endians.h
 * @brief TODO
 *
 *******************************************************************************/

#include <stdint.h>

void BE_WS32(uint8_t* apDst, uint8_t* aPtr, int32_t aValue);

void BE_WU32(uint8_t* apDst, uint8_t* aPtr, uint32_t aValue);

void BE_WS16(uint8_t* apDst, uint8_t* aPtr, int16_t aValue);

void BE_WU16(uint8_t* apDst, uint8_t* aPtr, uint16_t aValue);

void BE_WS8(uint8_t* apDst, uint8_t* aPtr, int8_t aValue);

void BE_WU8(uint8_t* apDst, uint8_t* aPtr, uint8_t aValue);

int32_t BE_RS32(uint8_t* apSrc, uint8_t* aPtr);

uint32_t BE_RU32(uint8_t* apSrc, uint8_t* aPtr);

int16_t BE_RS16(uint8_t* apSrc, uint8_t* aPtr);

uint16_t BE_RU16(uint8_t* apSrc, uint8_t* aPtr);

int8_t BE_RS8(uint8_t* apSrc, uint8_t* aPtr);

uint8_t BE_RU8(uint8_t* apSrc, uint8_t* aPtr);

#ifdef __cplusplus
}
#endif

#endif /* ENDIANS_H */
