/*******************************************************************************
 * Copyright 2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_LOAD_BIN_H
#define UTILS_LOAD_BIN_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  utils_load_bin.h
 * @brief load binary file from file system to the memory buffer
 *******************************************************************************/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/******************************************************************************/
/**
 * @brief   Holds info about single binary file and its content
 *******************************************************************************/
typedef struct
{
    const char* mpcFilePath; /**< path and name of the file*/
    uint8_t* mpBuf; /**< preallocated buffer for the binary bytes*/
    size_t mSizeBytes; /**< real size of the loaded binary*/
    size_t mBufSizeBytes; /**< number of bytes in the buffer*/
} utils_BinaryBlob_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/
/******************************************************************************/
/**
 * @brief Gets size in bytes of the binary file identified by the file path.
 * 
 * @details supports size only up to INT32_MAX
 *
 * @param[in] apcFilePath path to the binary file.
 *
 * @return  size of the file if all ok; -INT32_MAX if size more than INT32_MAX; -1 otherwise
 *******************************************************************************/
int32_t utils_GetBinarySize(const char* apcFilePath);

/******************************************************************************/
/**
 * @brief Loads a binary file into a preallocated buffer.
 *
 * @details If successfull, the binary needs to be freed later by the caller.
 *
 * @param[in,out] acpBin binary file and content descriptor structure.
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadBinary(utils_BinaryBlob_t* const acpBin);

/******************************************************************************/
/**
 * @brief Stores a memory data into file.
 *
 * @param[in] acpBin binary file and data descriptor structure.
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveBinary(utils_BinaryBlob_t* const acpBin);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UTILS_LOAD_BIN_H */
