/*******************************************************************************
 * Copyright 2022-2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ISPDRV_EVENTS_H
#define ISPDRV_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  ispdrv_events.h
 * @brief Event definition for Isp driver (user part)
 *******************************************************************************/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

typedef enum
{
    ISPDRV_EVENT_ID_ERROR,
    ISPDRV_EVENT_ID_FRAME_DONE,
    ISPDRV_EVENT_ID_STATISTIC_UPDATE,
    ISPDRV_EVENT_ID_PIPE_CLOSED,
} ispdrv_EventId_t;

typedef enum
{
    ISPDRV_ERROR_ID_BAD_ACK,
    ISPDRV_ERROR_ID_ERR_MSG,
    ISPDRV_ERROR_ID_INTERNAL,
} ispdrv_error_id_t;

typedef enum
{
    ISPDRV_FW_ERR_ID_INV_CMD = 2,
}ispdrv_fw_ErrIds_t;

typedef enum
{
    ISPDRV_FW_ERR_INTERNAL_UNKNOWN_ISM_EVENT = 1,
}ispdrv_InternalErrIds_t;

typedef union
{
    uint8_t BadAck;
    ispdrv_fw_ErrIds_t ErrMsg;
    ispdrv_InternalErrIds_t Internal;
}ispdrv_error_param_t;


typedef struct
{
    /** @brief Origin of the error (module/message)  */
    ispdrv_error_id_t mErrorId;
    /** @brief Error id specific to the origin (module) */
    ispdrv_error_param_t mParam;
} ispdrv_ParamError_t;

typedef struct
{
    uint64_t mTaskAddr;
    uint32_t mErrorId;
    uint8_t mProfilingIdx;
} ispdrv_ParamFrameDone_t;

typedef struct
{
    uint64_t mTaskAddr;
} ispdrv_ParamStatisticUpdate_t;

typedef union
{
    ispdrv_ParamError_t mError;
    ispdrv_ParamFrameDone_t mFrameDone;
    ispdrv_ParamStatisticUpdate_t mStatisticUpdate;
} ispdrv_EventParam_t;

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

#endif /* #ifndef ISPDRV_EVENTS_H */
