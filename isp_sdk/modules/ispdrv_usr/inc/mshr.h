/*******************************************************************************
 * Copyright 2021-2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  mshr.h
 * @brief Shared memory
 *******************************************************************************/

#ifndef MSHR_H
#define MSHR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#if defined(GHS)
    #include <sys/types.h>
#endif

#include "isp_platform.h"

typedef enum
{
    MSHR_MODE_UNKNOWN,
    MSHR_MODE_OWNER,
    MSHR_MODE_USER
} MShr_Mode_t;

typedef enum
{
    MSHR_DIR_FROM_PC,
    MSHR_DIR_TO_PC,
    MSHR_DIR_BOTH
} Mshr_BufDir_t;

/******************************************************************************/
/**
 * @brief    Address mapping request container
 *******************************************************************************/
typedef struct
{
    uint64_t mPhysAddr;
    uint64_t mPhysAddrAligned;
    size_t   mSize;
    size_t   mSizeAligned;
    void*    mpVirtAddr;
    void*    mpVirtAddrAligned;
} Mshr_MmapReq_t;


/******************************************************************************/
/**
 * @brief    Initialize this module
 *
 * @param[in]   aMode - mode in which this lib should be used in this process
 *
 * @return   true on success or false on error
 *
 * @details  Prepares module for operation. Should be called only once per process.
 *      aMode is either MSHR_MODE_OWNER or MSHR_MODE_USER. Process with MSHR_MODE_OWNER
 *      must be only one and only it can create new memory blocks. There can be any
 *      number of MSHR_MODE_USER.
 *******************************************************************************/
bool MShr_Init(MShr_Mode_t aMode);

/******************************************************************************/
/**
 * @brief    Deinitialize this module
 *
 * @return   none
 *
 * @details  Unmap all allocated blocks from all streams. If mode is MSHR_MODE_OWNER
 *      all shared files are unlined (deleted once no precess uses them).
 *      Deinitialize this module (it cen later be initialized again).
 *******************************************************************************/
void MShr_DeInit(void);

/******************************************************************************/
/**
 * @brief    Allocates new block of shared memory
 *
 * @param[in]    aId - Id of the Hw block
 * @param[in]    aStreamId - steam id
 * @param[in]    aSize - minimum size of new memory block
 * @param[in]    aHandle -
 *                  MSHR_MODE_USER -> handle (offset) of block to map
 *                  MSHR_MODE_OWNER -> -1 (always)
 *
 * @return   memory pointer that process can use on success or NULL on error
 *
 * @details  Behavior depends on mode given during init call. For MSHR_MODE_OWNER
 *      shared file is enlarged and new shared memory is created. For MSHR_MODE_USER
 *      already existing block is mapped (handle must be known)
 *******************************************************************************/
void* MShr_malloc(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, size_t aSize, ssize_t aHandle);

/******************************************************************************/
/**
 * @brief    frees block of shared memory
 *
 * @param[in]    aId - Id of the Hw block
 * @param[in]    aStreamId - steam id
 * @param[in]    apPtr - work pointer returned by MShr_malloc()
 *
 * @return   none
 *
 * @details  Behavior depends on mode given during init call.
 *      For both modes the memory is unmapped (freed). Additionally for MSHR_MODE_OWNER
 *      if all blocks are freed, the shared file is unlinked (deleted once no
 *      other precess uses it).
 *******************************************************************************/
void MShr_free(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, void* apPtr);

/******************************************************************************/
/**
 * @brief    Translates local work pointer to global handle
 *
 * @param[in]   aStreamId - steam id
 * @param[in]   apPtr - work pointer returned by MShr_malloc()
 * @param[out]  aOutHandle - global handle identifying given memory block
 *
 * @return   true on success or false on error
 *
 * @details  Translates local work pointer to global handle that can be used
 *      by other precess to map the same memory.
 *      On success aOutHandle holds the identifier.
 *      On error aOutHandle is not modified.
 *      NOTE: while global identifier is fixed you still need to "transfer"
 *      all memory to "ISP HW" before any use (task scheduling). Use MShr_Transfer2remote().
 *      The same goes for "transfer" back to local control after
 *      remote use (task finished). Use MShr_Transfer2local().
 *******************************************************************************/
bool MShr_GetGlobalMemHandle(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, void* apPtr, uint64_t* aOutHandle);

/******************************************************************************/
/**
 * @brief    Moves data ownership out of the OS (flush write cache or copy to some external location)
 *
 * @param[in]   aId - Id of the Hw block
 * @param[in]   aStreamId - steam id
 * @param[in]   aBufDir - buffer data direction
 * @param[in]   apPtr - work pointer returned by MShr_malloc()
 * @param[out]  aOutHandle - global handle identifying given memory block
 *
 * @return   true on success or false on error
 *
 *******************************************************************************/
int MShr_Transfer2remote(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, Mshr_BufDir_t aBufDir, void* apPtr, uint64_t* aOutHandle);

/******************************************************************************/
/**
 * @brief    Moves data ownership into the OS (invalidates read cache or copy from external destination)
 *
 * @param[in]   aId - Id of the Hw block
 * @param[in]   aStreamId - steam id
 * @param[in]   aBufDir - buffer data direction
 * @param[in]   apPtr - work pointer returned by MShr_malloc()
 *
 * @return   true on success or false on error
 *
 *******************************************************************************/
int MShr_Transfer2local(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, Mshr_BufDir_t aBufDir, void* apPtr);

/******************************************************************************/
/**
 * @brief Makes Ioctl call to imx95_isp.ko to extract physical location of DMA buffer
 * 
 * @param[in] aFd Shared DMA descriptor of the buffer. (exported from different driver)
 * @param[out] aPhyAddr Location where to store the location (expected size is uint64_t[1])
 * @return Upon success the return value will be 0. On failure negative error code should be returned.
 */
int Mshr_GetDmaBufLocation(int aFd, uint64_t *apPhyAddr);

/******************************************************************************/
/**
 * @brief Provides cached virtual mapping for given physical address
 * 
 * Uses /dev/cammix_io on the background. Handles cache allignment internally.
 * 
 * @param[out] apMmapReq mapping request with prefilled mPhysAddr and mSize values - the pysical address to be mapped.
 * @return Upon success the virtual address mapping is returned. On failure NULL is returned.
 */
void* Mshr_MapPhysAddrCached(Mshr_MmapReq_t *apMmapReq);

/******************************************************************************/
/**
 * @brief Cancles previous mapping by \ref Mshr_MapPhysAddrCached
 * 
 * @param[out] apMmapReq mapping request filled by previous call to \ref Mshr_MapPhysAddrCached
 * @return 0 if all ok, -1 otherwise.
 */
int Mshr_UnmapPhysAddrCached(Mshr_MmapReq_t *apMmapReq);

#ifdef __cplusplus
}
#endif

#endif // MSHR_H
