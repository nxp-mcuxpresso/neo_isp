/*******************************************************************************
 * Copyright 2022, 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_CONTAINER_H
#define UTILS_CONTAINER_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  utils_container.h
 * @brief Brief provides declaration of the isp_sdk common containers
 *
 * @details Implements:
 *          - image buffer container
 *          - embedded data container
 *******************************************************************************/

#include <stdint.h>

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
/******************************************************************************/
/**
 * @brief Image pixel data container
 *******************************************************************************/
typedef struct
{
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mBpp;
    uint16_t mChan;
    uint32_t mSizeBytes;
    uint32_t* mpData;
    uint64_t mHwAddr;
    uint32_t mIdx;
    uint64_t mTimeStamp;
} utils_ImgBuf_t;

/******************************************************************************/
/**
 * @brief Image embedded data container
 *******************************************************************************/
typedef struct
{
    uint32_t mFrmSeqNum; ///< frame sequence number
    uint8_t* mpEmbd0; ///< pointer to embedded lines before image data
    uint32_t mEmbd0Sz; ///< number of bytes in the Embd0 array
    uint8_t* mpEmbd1; ///< pointer to embedded lines after image data
    uint32_t mEmbd1Sz; ///< number of bytes in the Embd1 array
} utils_EmbdData_t;

/*******************************************************************************/
/**
 * @brief Struct for picture header parameters
 *
 *******************************************************************************/
typedef struct
{
    uint32_t    mRawType;
    uint32_t    mBayerPattern;
    uint32_t    mEmbd0Sz;
    uint32_t    mEmbd1Sz;
    uint32_t    mExp[4];
    uint32_t    mAgain[4];
    uint32_t    mDgain[4];
    uint32_t    mL2sRatio;
    uint32_t    mL2vsRatio;
    uint16_t    mWbGainR;
    uint16_t    mWbGainG;
    uint16_t    mWbGainB;
    int16_t     mWbOffsetR;
    int16_t     mWbOffsetG;
    int16_t     mWbOffsetB;
    uint16_t    mWbCTemp;
    int32_t     mSensTemp;
    bool        mDataInvalid;
    const char* mSensIdString;
} utils_PicParams_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef #define UTILS_CONTAINER_H */
