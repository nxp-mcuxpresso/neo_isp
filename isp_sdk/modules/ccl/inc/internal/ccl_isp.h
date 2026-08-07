/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_ISP_H
#define CCL_ISP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ccl_isp.h
 * @brief
 */

#include <stdint.h>

#include "isp_platform.h"
#include "ccl_internal.h"

#include "ispdrv_usr.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

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
 * @brief Callback from ISP interface, delivering ISP events
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aEventId - Id of the event that happens
 * @param apParam - Parameters of the event (event specific format)
 * @param apClientPtr - client pointer supplied during stream init (pointer to ccl_Stream_t of this stream)
 */
void ccl_IspCallback(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_EventId_t aEventId, ispdrv_EventParam_t* apParam, void* apClientPtr);

/**
 * @brief Allocates ISP output image buffers for given frame task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Frame task to allocate the buffers for
 *
 * @return 0 on success or negative error otherwise.
 */
int32_t ccl_AllocImgBufsOut(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk);

/**
 * @brief Frees ISP output img buffers for given frame task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Frame task to free the buffers from
 *
 * @return 0 on success or negative error otherwise.
 */
int32_t ccl_FreeImgBufsOut(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrTsk);

/**
 * @brief Updates ISP regs so they will point to associated buffers
 *
 * @param apFrTsk - task to update
 * @return 0 on success, -1 on error.
 */
int32_t ccl_UpdateIspBufLocations(ccl_FrameTask_t *apFrTsk);

/**
 * @brief Allocates ISP task memory for given frame task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Frame task to allocate the buffers for
 *
 * @return 0 on success or negative error otherwise.
 */
int32_t ccl_AllocIspTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk);

/**
 * @brief Frees ISp task from given Frame task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Frame task to free the buffers from
 *
 * @return 0 on success or negative error otherwise.
 */
int32_t ccl_FreeIspTask(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk);

/**
 * @brief Flushes cache for ISP relevant memories (in-img, regs, mems,...) for given frame task
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrTsk - Frame task to flush
 *
 * @return 0 on success or negative error otherwise.
 */
int32_t ccl_FlushIspBufs(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t *apFrTsk);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_ISP_H */
