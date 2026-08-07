/*******************************************************************************
 * Copyright 2022, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  endians.c
 * @brief Brief
 *******************************************************************************/

#include "endians.h"
#include "assert.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/
inline void BE_WS32(uint8_t* apDst, uint8_t* aPtr, int32_t aValue)
{
    assert(*aPtr < (UINT8_MAX - 4));
    apDst[(*aPtr)++] = (*((uint32_t*) &aValue) >> 24) % 0x100;
    apDst[(*aPtr)++] = (*((uint32_t*) &aValue) >> 16) % 0x100;
    apDst[(*aPtr)++] = (*((uint32_t*) &aValue) >> 8) % 0x100;
    apDst[(*aPtr)++] = (*((uint32_t*) &aValue) >> 0) % 0x100;
}

inline void BE_WU32(uint8_t* apDst, uint8_t* aPtr, uint32_t aValue)
{
    assert(*aPtr < (UINT8_MAX - 4));
    apDst[(*aPtr)++] = (aValue >> 24) % 0x100;
    apDst[(*aPtr)++] = (aValue >> 16) % 0x100;
    apDst[(*aPtr)++] = (aValue >> 8) % 0x100;
    apDst[(*aPtr)++] = (aValue >> 0) % 0x100;
}

inline void BE_WS16(uint8_t* apDst, uint8_t* aPtr, int16_t aValue)
{
    assert(*aPtr < (UINT8_MAX - 2));
    apDst[(*aPtr)++] = (*((uint16_t*) &aValue) >> 8) % 0x100;
    apDst[(*aPtr)++] = (*((uint16_t*) &aValue) >> 0) % 0x100;
}

inline void BE_WU16(uint8_t* apDst, uint8_t* aPtr, uint16_t aValue)
{
    assert(*aPtr < (UINT8_MAX - 2));
    apDst[(*aPtr)++] = (aValue >> 8) % 0x100;
    apDst[(*aPtr)++] = (aValue >> 0) % 0x100;
}

inline void BE_WS8(uint8_t* apDst, uint8_t* aPtr, int8_t aValue)
{
    assert(*aPtr < (UINT8_MAX));
    apDst[(*aPtr)++] = *((uint8_t*) &aValue);
}

inline void BE_WU8(uint8_t* apDst, uint8_t* aPtr, uint8_t aValue)
{
    assert(*aPtr < (UINT8_MAX));
    apDst[(*aPtr)++] = (aValue >> 0) % 0x100;
}

inline int32_t BE_RS32(uint8_t* apSrc, uint8_t* aPtr)
{
    int32_t ret;
    assert(*aPtr < (UINT8_MAX - 4));
    ret = apSrc[(*aPtr)++] << 24;
    ret |= apSrc[(*aPtr)++] << 16;
    ret |= apSrc[(*aPtr)++] << 8;
    ret |= apSrc[(*aPtr)++] << 0;
    return ret;
}

inline uint32_t BE_RU32(uint8_t* apSrc, uint8_t* aPtr)
{
    uint32_t ret;
    assert(*aPtr < (UINT8_MAX - 4));
    ret = apSrc[(*aPtr)++] << 24;
    ret |= apSrc[(*aPtr)++] << 16;
    ret |= apSrc[(*aPtr)++] << 8;
    ret |= apSrc[(*aPtr)++] << 0;
    return ret;
}

inline int16_t BE_RS16(uint8_t* apSrc, uint8_t* aPtr)
{
    int16_t ret;
    assert(*aPtr < (UINT8_MAX - 2));
    ret = apSrc[(*aPtr)++] << 8;
    ret |= apSrc[(*aPtr)++] << 0;
    return ret;
}

inline uint16_t BE_RU16(uint8_t* apSrc, uint8_t* aPtr)
{
    uint16_t ret;
    assert(*aPtr < (UINT8_MAX - 2));
    ret = apSrc[(*aPtr)++] << 8;
    ret |= apSrc[(*aPtr)++] << 0;
    return ret;
}

inline int8_t BE_RS8(uint8_t* apSrc, uint8_t* aPtr)
{
    int8_t ret;
    assert(*aPtr < (UINT8_MAX));
    ret = (((int8_t *)apSrc)[(*aPtr)++]);
    return ret;
}

inline uint8_t BE_RU8(uint8_t* apSrc, uint8_t* aPtr)
{
    uint8_t ret;
    assert(*aPtr < (UINT8_MAX));
    ret = apSrc[(*aPtr)++] << 0;
    return ret;
}
