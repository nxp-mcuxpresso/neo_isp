/*******************************************************************************
 * Copyright 2025 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_HAL
#define CGD_HAL

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * @file     cgd_hal.h
 * @brief    hardware abstraction layer file for CGD
 ****************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "cgd.h"
#include "cgd_types.h"

/*==============================================================================
*                                    GLOBAL VARIABLES
==============================================================================*/
/** @brief Handler for ISI frame done event */
extern cgd_IsiEventHandler_t gpCgdFrameDoneEvtHandler;
extern cgd_IsiEventHandler_t gpCgdFirstLineEvtHandler;
extern cgd_ErrorEventHandler_t gpCgdErrorEvtHandler;

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Reset GPIO pin for specified CSI port
 *
 * This function resets the GPIO pin associated with the given CSI port. It finds
 * the appropriate GPIO chip based on the port type, sets the line to output mode,
 * and performs a reset sequence if requested.
 *
 * @param aPortIdx CSI device ID (CGD_MIPI_CSI_DEV or CGD_MIPI_CSI_COMBO_DEV)
 * @param aRestart If true, performs a reset pulse (low->high), if false sets pin low
 *
 * @return CGD_LIB_SUCCESS on success, CGD_LIB_FAILURE on error
 ****************************************************************************/
cgd_Result cgd_ResetGpioPin(cgd_CsiDeviceId_t aPortIdx, bool aRestart);

/******************************************************************************/
/**
 * @brief          Initialize variables, registers etc
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_IsiPrepare(cgd_IsiDevice_t* apIsiDevice);

/******************************************************************************/
/**
 * @brief          (Re)Configure ISI channel
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_IsiChannelConfigure(cgd_IsiChannel_t aChnId);

/******************************************************************************/
/**
 * @brief          Enable ISI channel processing
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_IsiChannelStreamOn(cgd_IsiChannel_t aChnId);

/******************************************************************************/
/**
 * @brief          Stop ISI channel streaming
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_IsiChannelStreamOff(cgd_IsiChannel_t aChnId);

/******************************************************************************/
/**
 * @brief          Disable ISI channel
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_IsiChannelClose(cgd_IsiChannel_t aChnId);

/******************************************************************************/
/**
 * @brief          Initialize variables, registers etc
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_CsiPrepare(cgd_CsiDeviceId_t aPortIdx);

/******************************************************************************/
/**
 * @brief          Open CSI port
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_CsiOpen(cgd_CsiDeviceId_t aPortIdx);

/******************************************************************************/
/**
 * @brief          Close CSI device
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_CsiClose(cgd_CsiDeviceId_t aPortIdx);

/******************************************************************************/
/**
 * @brief          Setup CSI interruption masks in registry
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 ****************************************************************************/
cgd_Result cgd_CsiEnableInterruption(cgd_CsiDeviceId_t aPortIdx);

/******************************************************************************/
/**
 * @brief           Clean all
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_CleanUp(void);

/******************************************************************************/
/**
 * @brief Check if empty buffer is available for use.
 *
 * @param  aIsiIdx  ISI index
 *
 * @return false - no empty buffer available, true - empty buffer is available
 *
 ****************************************************************************/
bool cgd_IsiIsEmptyBufferPrepared(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief           Submit new free buffer to ISI device queue, should be used before cgd_GetFullBuffer
 *
 * @param  aIsiIdx  ISI index
 * @param  apEmptyBuffer  free buffer
 *
 * @return cgd_State_t enum
 *
 ****************************************************************************/
cgd_Result cgd_IsiSubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apEmptyBuffer);

/******************************************************************************/
/**
 * @brief           Return full frame buffer. NULL in case of error, also NULL when empty buffer queue is empty. Is recommended to use cgd_SubmitEmptyBuffer first.
 *
 * @param  aIsiIdx  ISI index
 *
 * @param  apOutBuffer  (out) full buffer output
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_IsiGetFullBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer);

/******************************************************************************/
/**
 * @brief Get full frame buffer and submit empty buffer in single operation.
 *
 * @param  aIsiIdx  ISI index
 *
 * @param  apOutBuffer  (out) full buffer output
 * @param  apEmptyBuffer (in)  free buffer to be used
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_IsiGetFullBufferSubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer, utils_ImgBuf_t* apEmptyBuffer);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_HAL */
