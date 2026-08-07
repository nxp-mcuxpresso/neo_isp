/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_CCA_H
#define CCL_CCA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ccl_cca.h
 * @brief Internal CCL header implementing camera control organisation
 * 
 * @details CCA organisation and interface. Takes care of camera runtime parameter adjustment for CCL lib.
 */

#include <stdint.h>

#include "ccl.h"
#include "ccl_internal.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

#define SENS_LOG_FRAME_ID_INITIAL_CFG  (UINT32_MAX)
#define SENS_LOG_FRAME_ID_NOT_APPLICABLE  (9999)
/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/**
 * @brief Initializes CCA interface
 * 
 * @return 0 on success or negative error otherwise.
 * 
 * @details Initializes CCA related variables. Creates CCF groups and
 * CCF processing threads.
 * Initializes CCA library (=> CCF lib initialized as well).
 */
int32_t ccl_CcaInit(void);

/**
 * @brief Performs cleanup of CCA interface.
 * 
 * @details Stops CCF processing threads. Closes CCA library. Cleans CCA related variables
 */
void ccl_CcaDeinit(void);

/**
 * @brief removes all configuration items from stream history
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 */
void ccl_CcaCleanCfgHistory(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/**
 * @brief Enforces default configuration on sensor for given stream
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * 
 * @return 0 on success or negative error otherwise.
 * 
 * @details Reads default configuration from CCF library. Enforces the cfg on
 * sensor. Removes all cfg history for the stream. Stores the default cfg as
 * sole item in streams history.
 */
int32_t ccl_CcaSetDefaultCfg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/**
 * @brief Event handling -> New frame was processed on ISP, check CCF updates for sensor
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - newly processed frame
 * 
 * @details Checks if there is CCF processing on this stream (if not, marks the frame as CCF processed).
 * If CCF is already running marks the frame as CCF processed and ignores it. In all other cases stores
 * the frame as latest frame for the stream (freeing any previous one) and signals update to CCF group.
 * NOTE: this fcn must be called with stream's mutex locked
 */
void ccl_CcaUpdate(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame);

/**
 * @brief Event handling -> a frame was dropped during capture or processing.
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * 
 * @details Updates Last frame information for the stream allowing in some situations to run CCF without this stream.
 */
void ccl_CcaFrameDropped(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/**
 * @brief Designed for M2M mode. Gets latest ISP cfg and based on embedded data finds best HDR cfg (one of ISP cfg group)
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - Newly captured frame
 * 
 * @return 0 on success or negative error otherwise.
 * 
 * @details This fcn works in several steps:
 * - takes latest ISP cfg
 * - reconstructs original sensor cfg from embedded data
 * - searches history (latest->oldest) for reconstructed cfg (if no much found, oldest cfg is used)
 * - updates frame's ISP cfg -> only HDR group -> from this match (sensor cfg and ISP cfg is stored as pairs in history)
 */
ccl_Err_t ccl_CcaFindIspCfgM2M(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame);


/**
 * @brief Designed for streaming mode. Gets latest ISP cfg and based on configurable delay updates HDR cfg
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - Future frame task
 * @param aHdrDelay - delay in activation of new HDR cfg
 * 
 * @return 0 on success or negative error otherwise.
 * 
 * @details This fcn works in several steps:
 * - takes latest ISP cfg as baseline
 * - takes n-th configuration from cfg history (delay is one of open() parameters)
 * - updates frame's ISP cfg -> only HDR group -> from this match
 */
ccl_Err_t ccl_CcaFindIspCfgStr(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrame, uint8_t aHdrDelay);


#if CCL_FEATURE_LOG_SENS_CFG
/**
 * @brief Adds event to sensor cfg log
 * 
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apEvtStr - event string
 * @param FrameId - frame that caused this event (0 if N/A)
 * @param BasedOnId - frame id that generated this configuration
 * @param apSensCfg - sensor configuration (NULL if N/A)
 */
void LogSensCfg(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, const char* apEvtStr, uint32_t FrameId, uint32_t BasedOnId, cca_SensPrm_t *apSensCfg);

/**
 * @brief Fils given buffer with header for future serialized sensor configuration
 * 
 * @param apBufOut - buffer to fill
 * @param aBufSize - max header size
 * @return actual size or -1 on error
 */
int32_t SerializeSensPrmHeader(char *apBufOut, ssize_t aBufSize);

#else
    #define LogSensCfg(...)
    #define SerializeSensPrmHeader(...)
#endif

#ifdef __cplusplus
}
#endif


#endif /* #ifndef CCL_CCA_H */
