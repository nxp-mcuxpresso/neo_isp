/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_FRAME_QUEUE_H
#define CCL_FRAME_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ccl_buf.h
 * @brief implements linked-list fifo buffers. Each item have dynamic allocation.
 */

#include <stdint.h>
#include "ccl_internal.h"

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

uint8_t ccl_FqGetCnt(ccl_FrameQueue_t *apQueue);

ccl_FrameTask_t *ccl_FqPop(ccl_FrameQueue_t *apQueue);

int ccl_FqPush(ccl_FrameQueue_t *apQueue, ccl_FrameTask_t *apNewItem);

int ccl_FqDropCheck(ccl_FrameQueue_t *apQueue, ccl_FrameTask_t *apDropItem);

void ccl_FqClean(ccl_FrameQueue_t *apQueue);

#ifdef __cplusplus
}
#endif


#endif /* #ifndef CCL_FRAME_QUEUE_H */
