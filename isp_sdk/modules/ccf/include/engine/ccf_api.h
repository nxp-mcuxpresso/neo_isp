/*******************************************************************************
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCF_API_H
#define CCF_API_H

/**
 * @file ccf_api.h
 * @brief
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "interface/algorithm.h"

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
/*
    NOTE:
    - maximum number of cameras supported by the CCF
    - final value should be 8 cameras
    - current limitation of CGD is 4 cameras (TODO)
*/
#define CCF_CAMERA_CNT 4U

/*===============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/**
 * @brief   mapping struct determining the camera ID on each channel index
 *
 * @note not used right now, but will be necessary for updates from TDS
 */
typedef struct {
    uint32_t mCameraId[CCF_CAMERA_CNT];
} ccf_CamMap_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/
extern ccf_Algorithm_t* gAlgorithms[];

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/**
 * @brief    open CCF functionality
 *
 * @details  enable/disable individual functionalitites
 *
 * @param[in]    aLtActive - live tuning flag: active (1) / non active (0)
 * @param[in]    apConfig  - algorithms configuration data
 *
 * @return   0 - OK; error otherwise
*******************************************************************************/
int     ccf_Open(   uint8_t         aLtActive,
                    void*           apConfig);

/**
* @brief    preprocess CCF
*
* @details  prepare and validate input data before main CCF processing
*
* @param[in]        apCtxHwIn   - regs & mems settings from HW ISP
* @param[out]       apCtxHwOut  - regs & mems to be copied to output
* @param[in]        aSensID     - sensor identification
* @param[in]        aChnIdx     - identification of channel/camera
*
* @return   0 - OK; error otherwise
*******************************************************************************/
int     ccf_Init(   void *    apCtxHwIn,
                    void *    apCtxHwOut,
                    uint32_t  aSensID,
                    uint32_t  aChnIdx);

/**
* @brief    process CCF
*
* @details  run control algorithms
*
* @param[in]        apCtxHwIn       Regs & mems settings from HW ISP.
* @param[out]       apCtxHwOut      Regs & mems to be copied to output.
* @param[in,out]    apSensData      Camera related data.
* @param[in]        apCtrlParams    Parameters shared between control functions.
* @param[in]        aChnIdx         Identification of channel/camera.
*
* @return   0 - OK; error otherwise
*******************************************************************************/
int ccf_Process(void *      apCtxHwIn,
                void *      apCtxhwOut,
                void *      apSensData,
                void *      apCtrlParams,
                uint32_t    aChnIdx);

/******************************************************************************/
/**
* @brief    Clean up of dynamically allocated memory in case of active Live tuning
*
* @return   0 - OK; error otherwise
*******************************************************************************/
int ccf_Close(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCF_API_H */
