/*******************************************************************************
 * Copyright 2022-2023,2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ISP_TASK_H
#define ISP_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  isp_task.h
 * @brief Defines organisation of a memory that is shared between firmware and higher
 * software layers (ispdrv, cca,...).
 * @details Defines organisation of a memory that is shared between firmware and higher
 * software layers (ispdrv, cca,...). Each frame processed on ISP needs this stuct.
 *******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "paxRegConfig.h"
#include "isp_mems.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
#if defined(GHS)
#else
#pragma pack(push, 1)
#endif

typedef struct
{
    /** @brief Metadata structure size in bytes */
    const uint32_t metadata_size;
    /** @brief Log version */
    const uint32_t log_version;
    /** @brief Points to the end of log, the log starts at end_index - 1. */
    uint32_t end_index;
    /** @brief Tells whether the log wraps around, i.e. the buffer has been overwritten at least once. */
    bool full;
    /** @brief 64 byte alignment. */
    uint32_t reserved[10U];
    /** @brief Log size in bytes. */
    const uint32_t data_size;
    /** @brief Location of log. */
    uint8_t* data_location;
} FwLogItem_t;


enum measurement
{
    FW_TIME_FRAME_NUMBER,
    FW_TIME_START,
    FW_TIME_TRIGGER_DDR_TO_REGS,
    FW_TIME_DDR_TO_REGS_DONE,
    FW_TIME_TRIGGER_DDR_TO_MEMS,
    FW_TIME_DDR_TO_MEMS_DONE,
    FW_TIME_TRIGGER_REGS_TO_DDR,
    FW_TIME_TRIGGER_ISP,
    FW_TIME_ISP_P1_STATS_DONE,
    FW_TIME_REGS_TO_DDR_DONE,
    FW_TIME_TRIGGER_MEMS_TO_DDR,
    FW_TIME_MEMS_TO_DDR_DONE,
    FW_TIME_FRAME_DONE,
    FW_TIME_UNUSED1,
    FW_TIME_MU_SEND_FRAME_DONE,
    FW_TIME_TIMESTAMPS_ENTRIES_COUNT,   /* Determines the number of timestamps for each measurement entry*/
};

typedef struct
{
    /** @brief MU channel/stream the timestamps belong to. */
    uint32_t mu_channel;
    /** @brief Array of timestamps. */
    uint32_t ts[FW_TIME_TIMESTAMPS_ENTRIES_COUNT];
} FwTimeStampLine_t;

typedef struct
{
    /** @brief Metadata structure size in bytes. */
    const uint32_t timestamp_size;
    /** @brief Timestamps version. */
    const uint32_t timestamp_version;
    /** @brief 64 byte alignment. */
    uint32_t reserved[12U];
    /** @brief Log size in bytes. */
    const uint32_t data_size;
    /** @brief Location of timestamps. */
    const uint32_t timestamps_location;
}FwTimestampHeader_t;

typedef struct
{
    IspRegs_t mRegs; /**< registers of the ISP */
    IspMems_t mMems; /**< ISP internal memories */

#ifdef ISIM
    vuint32_t mpIsimRgb;
#endif
} isp_CtxHw_t;

typedef struct
{
    // metadata
    uint64_t mTaskDuration; /**< time of processing (filed by firmware) */
    uint32_t mErrFlags; /**< error flags if any */
    uint32_t mFrameIdx; /**< ISP sequence number of the frame (set by fw in stream mode, set by ccl before SheduleTask() in M2M), do not confuse with ccl_Frame_t's meta info mFrameNo */
    uint64_t mCaptureTimestampNs; /**< frame time stamp in ns*/
    uint64_t mIspDoneTimestampNs; /**< frame time stamp in ns*/
    uint64_t mReadyTimestampNs; /**< frame time stamp in ns*/
    FwTimeStampLine_t mFwTimestamps;
} isp_CtxMeta_t;

typedef struct
{
    isp_CtxHw_t mCtxHw;
    isp_CtxMeta_t mCtxMeta;
} isp_Ctx_t;

typedef struct
{
    // ISP Context
    isp_Ctx_t mIspCtx;

    void* mClientCtx;
} isp_Task_t;

#if defined(GHS)
#else
#pragma pack(pop)
#endif

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

#endif /* #ifndef ISPDRV_TASK_H */
