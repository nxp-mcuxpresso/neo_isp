/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/******************************************************************************/

#ifndef CAMERA_CTX_H
#define CAMERA_CTX_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  camera_ctx.h
 * @brief camera context and settings
 *******************************************************************************/

#include "FreeRTOS.h"
#include "semphr.h"

#include "cca.h"
#include "cca_ovx3c.h"
#include "new_sensor_cca.h"

#include "ccl.h"
#include "utils_api.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define MAX_FRAME_WAIT_MS 10000U ///< max time to wait for a frame in miliseconds

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
typedef struct _CameraCtx_t CameraCtx_t;


/******************************************************************************/
/**
 * @brief application stream context
 *******************************************************************************/
typedef struct
{
    CameraCtx_t  *mpCameraCtx; ///< pointer to application context context
    uint32_t mStrIdx; ///< stream index

    ispdrv_HwId_t mHwId; ///< id of "system wide" "statically allocated" isp hw
    ispdrv_CamPipeId_t mCamId; ///< id of "system wide" "statically allocated" camera pipe inside the isp hw selected above


    /** @brief New frame that just arrived from sensor */
    ccl_Frame_t *mpFrameNew;



    /** @brief New frame that just arrived from sensor */
    ccl_Frame_t *mpFrameOnDisplay;

    /** @brief Handle of a mutex that protects mpFrameNew and mpFrameOnDisplay member */
    SemaphoreHandle_t mLockHandle;
    /** @brief Memory allocation for streams's mutex */
    StaticSemaphore_t mLockData;
}StreamCtx_t;

/******************************************************************************/
/**
 * @brief application context
 *******************************************************************************/
typedef struct _CameraCtx_t
{
    bool mpStop; ///< false if processing should run, true to stop
    uint32_t mStrCnt; ///< number of active streams
    utils_IniCfg_t mIniCfg; ///< parsed ini file information

    StreamCtx_t mpStreamArr[ISPDRV_CAM_PIPE_ID_CNT];

    // input
    ccl_CapMode_t mCapMode; ///< mode of ISP operation
    isp_CtxHw_t *mDefCtx[ISPDRV_CAM_PIPE_ID_CNT]; ///< default CTX

    uint32_t mSensorId[ISPDRV_CAM_PIPE_ID_CNT]; ///< now expected to be common to all streams

    cca_ovx3c_Params_t mOvx3cParams[ISPDRV_CAM_PIPE_ID_CNT]; ///< ovx3c parameters

    new_sensor_Params_t mNewSensorParams[ISPDRV_CAM_PIPE_ID_CNT]; ///< TODO: New sensor parameters

    // output
    uint32_t mFrameCnt; ///< number of frames processed
}CameraCtx_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/
extern unsigned char gIspCfg[];
/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CAMERA_CTX_H */
