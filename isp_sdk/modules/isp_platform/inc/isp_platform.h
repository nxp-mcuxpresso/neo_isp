/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ISP_PLATFORM_H
#define ISP_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  isp_platform.h
 * @brief Defines common to all isp software.
 *******************************************************************************/

#include "isp_task.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define ISPDRV_TASKS_PER_STREAM (2u)

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
typedef enum
{
    ISP_HWID_0   = 0x00U, /**< ID of ISP HW instance 0 */
    ISP_HWID_CNT = 0x01U, /**< overall number of available ISP instances */
    ISP_HWID_INV = 0xFFU /**< invalid ID */
} ispdrv_HwId_t;

typedef enum
{
    MU_CHANNEL_ID_MASTER,
    MU_CHANNEL_ID_CLIENT_0,
    MU_CHANNEL_ID_CLIENT_1,
    MU_CHANNEL_ID_CLIENT_2,
    MU_CHANNEL_ID_CLIENT_3,
    MU_CHANNEL_ID_CLIENT_4,
    MU_CHANNEL_ID_CLIENT_5,
    MU_CHANNEL_ID_CLIENT_6,
    MU_CHANNEL_ID_CLIENT_7,
    MU_CHANNEL_COUNT
} Mu_ChannelId_t;

typedef enum
{
    ISPDRV_CAM_PIPE_ID_0 = 0,
    ISPDRV_CAM_PIPE_ID_1 = 1,
    ISPDRV_CAM_PIPE_ID_2 = 2,
    ISPDRV_CAM_PIPE_ID_3 = 3,
    ISPDRV_CAM_PIPE_ID_4 = 4,
    ISPDRV_CAM_PIPE_ID_5 = 5,
    ISPDRV_CAM_PIPE_ID_6 = 6,
    ISPDRV_CAM_PIPE_ID_7 = 7,
    ISPDRV_CAM_PIPE_ID_INVALID = 0xFF
} ispdrv_CamPipeId_t;
#define ISPDRV_CAM_PIPE_ID_CNT 8

typedef enum
{
    ISPDRV_RES_SUCCESS, /**< operation succeeded */
    ISPDRV_RES_ERROR, /**< operation failed */
    ISPDRV_RES_ERROR_HW, /**< hw refused operation or hw error */
    ISPDRV_RES_TIMEOUT, /**< operation timed-out */
    ISPDRV_RES_NO_RESOURCE, /** not enough resources to complete operation */
    ISPDRV_RES_BAD_STATE, /** bad state for given operation */
    ISPDRV_RES_BAD_PARAM, /** bad parameter for given operation */
    ISPDRV_RES_NOT_IMPLEMENTED, /** functionality not yet implemented */
    ISPDRV_RES_OS, /** OS reported internal error */
    ISPDRV_RES_FORBIDDEN, /** Insufficient privileges*/
} ispdrv_Result_t;

typedef enum
{
    ISP_M0_RST_HW_FULL,
    ISP_M0_RST_HW_FW, /**< reset HW */
    ISP_M0_RST_SW_FW, /**< reset SW */
} ispdrv_M0Rst_t;

typedef enum
{
    ISPDRV_MODE_M2M,
    ISPDRV_MODE_STREAM,
}ispdrv_Mode_t;

#pragma pack(push, 1)

/**
 * @brief Structure that defines contents of internal driver msg
 * @details Note that any member longer than one byte must be an array, not a multi-byte type (e.g. uint16_t)
 *          to avoid errors when sending between BE and LE architectures.
 **/
typedef struct
{
    uint8_t mHwId; // ispdrv_HwId_t type
    uint8_t mMuId; // Mu_ChannelId_t type
    uint8_t mData[14];
} ispdrv_CommMsg_t;

#pragma pack(pop)

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

#endif /* #ifndef ISP_PLATFORM_H */
