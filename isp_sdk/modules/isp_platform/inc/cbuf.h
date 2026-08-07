/*******************************************************************************
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cbuf.h
 * @brief posix compatible thread safe circular buffers
 *******************************************************************************/

#ifndef CBUF_H
#define CBUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include "pthread_ext.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

typedef enum
{
    CBUF_ERR_SUCCESS = 0,
    CBUF_ERR_NO_SPACE,
    CBUF_ERR_NO_DATA,
    CBUF_ERR_MUTEX, /* fail to operate mutex */
    CBUF_ERR_BAD_PARAM, /* wrong parameter input */
    CBUF_ERR_BAD_STATE, /* Buffer in bad state (uninitialized?) */
    CBUF_ERR_EVENT_POST, /* data valid, however fail to post internal event (MORE_DATA or MORE_SPACE)*/
    CBUF_ERR_OTHER, /* un-categorized error*/
} cbuf_err_t;

/******************************************************************************
 *
 * @brief Conditionale variable struct
 *******************************************************************************/
typedef struct
{
    uint32_t mWp;
    uint32_t mRp;
    uint32_t mSize;
    uint8_t* mpData;

    bool SignalMoreData;
    bool SignalMoreSpace;
    sem_t mMoreDataEvent;
    sem_t mMoreSpaceEvent;
    pthread_mutex_t mLockWrite;
    pthread_mutex_t mLockRead;
} cbuf_t;

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
 * @brief    Initialize cbuf structure
 *
 * @param[in]    apBuf - structure over which to perform the operation
 * @param[in]    apMem - Memory block that will hold data
 * @param[in]    aMemSize - Size of memory block (size of circular buffer is aMemSize -1)
 * @param[in]    aMoreDataEvents - true if "MoreData" events should be generated
 * @param[in]    aMoreSpaceEvents - true if "MoreSpace" events should be generated
 *
 * @return   cbuf_err_t (CBUF_ERR_SUCCESS or numbered error)
 *******************************************************************************/
cbuf_err_t cbuf_Init(cbuf_t* apBuf, uint8_t* apMem, uint32_t aMemSize, bool aMoreDataEvents, bool aMoreSpaceEvents);

/******************************************************************************/
/**
 * @brief    De-Initialize cbuf structure
 *
 * @param[in]    apBuf - structure over which to perform the operation
 *
 * @return   none
 *******************************************************************************/
void cbuf_Destroy(cbuf_t* apBuf);

/******************************************************************************/
/**
 * @brief    Returns amount of free space in the buffer
 *
 * @param[in]    apBuf - structure over which to perform the operation
 *
 * @return   amount of free space (0 on error)
 *
 * @details If the buffer was not locked by cbuf_SetExclusiveWrite() return value may
 * no longer be valid at the time if return.
 *******************************************************************************/
uint32_t cbuf_GetSpace(cbuf_t* apBuf);

/******************************************************************************/
/**
 * @brief    Returns amount of data in the buffer
 *
 * @param[in]    apBuf - structure over which to perform the operation
 *
 * @return   amount of data (0 on error)
 *
 * @details If the buffer was not locked by cbuf_SetExclusiveRead() return value may
 * no longer be valid at the time if return.
 *******************************************************************************/
uint32_t cbuf_GetAvailable(cbuf_t* apBuf);

/******************************************************************************/
/**
 * @brief    Locks(or frees) given buffer's data for current thread
 *
 * @param[in]    apBuf - structure over which to perform the operation
 * @param[in]    aLock - true if buffer should be locked, false if freeing the buffer
 *
 * @return   cbuf_err_t (CBUF_ERR_SUCCESS or numbered error)
 *
 * @details Allows a thread to get exclusive access to the buffer. Also makes sure that
 * no changes are made, and data are valid, after call to cbuf_GetAvailable().
 *******************************************************************************/
cbuf_err_t cbuf_SetExclusiveRead(cbuf_t* apBuf, bool aLock);

/******************************************************************************/
/**
 * @brief    Locks(or frees) given buffer's space for current thread
 *
 * @param[in]    apBuf - structure over which to perform the operation
 * @param[in]    aLock - true if buffer should be locked, false if freeing the buffer
 *
 * @return   cbuf_err_t (CBUF_ERR_SUCCESS or numbered error)
 *
 * @details Allows a thread to get exclusive access to the buffer. Also makes sure that
 * no changes are made, and data are valid, after call to cbuf_GetSpace().
 *******************************************************************************/
cbuf_err_t cbuf_SetExclusiveWrite(cbuf_t* apBuf, bool aLock);

/******************************************************************************/
/**
 * @brief    Stores data inside a buffer
 *
 * @param[in]    apBuf - structure over which to perform the operation
 * @param[in]    apData - pointer to data to be stored in the buffer
 * @param[in]    aDataSize - size of the data to store
 *
 * @return   cbuf_err_t (CBUF_ERR_SUCCESS or numbered error)
 *******************************************************************************/
cbuf_err_t cbuf_Push(cbuf_t* apBuf, const uint8_t* apData, uint32_t aDataSize);

/******************************************************************************/
/**
 * @brief    Reads data from a buffer
 *
 * @param[in]    apBuf - structure over which to perform the operation
 * @param[in]    apData - pointer to memory block where to store the data
 * @param[in]    aDataSize - size of the data to move out  from the buffer
 *
 * @return   cbuf_err_t (CBUF_ERR_SUCCESS or numbered error)
 *
 * @details NOTE: Size of memory is not checked and it is a responsibility of a caller
 * to provide large enough memory.
 *******************************************************************************/
cbuf_err_t cbuf_Pop(cbuf_t* apBuf, uint8_t* apData, uint32_t aDataSize);

#ifdef __cplusplus
}
#endif

#endif /* CBUF_H  */