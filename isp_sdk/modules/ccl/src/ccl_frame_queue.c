/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/


/******************************************************************************/
/**
 * @file  ccl_frame_queue.c
 * @brief implements simple cbuf queue
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "ilog.h"
#include "ccl_frame_queue.h"
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
*                               LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/


uint8_t ccl_FqGetCnt(ccl_FrameQueue_t *apQueue)
{
    int16_t ret = apQueue->mWp - apQueue->mRp;

    if (ret < 0)
    {
        ret += FRAME_QUEUE_BUF_SIZE;
    }

    return ret;
}

ccl_FrameTask_t *ccl_FqPop(ccl_FrameQueue_t *apQueue)
{
    if (ccl_FqGetCnt(apQueue) < 1)
    {
        return NULL;
    }

    ccl_FrameTask_t *ret = apQueue->mItems[apQueue->mRp];

    apQueue->mRp = (apQueue->mRp + 1) % FRAME_QUEUE_BUF_SIZE;

    return ret;
}


int ccl_FqPush(ccl_FrameQueue_t *apQueue, ccl_FrameTask_t *apNewItem)
{
    if (ccl_FqGetCnt(apQueue) >= CCL_CFG_MAX_BUF_PER_STREAM)
    {
        return -1;
    }

    apQueue->mItems[apQueue->mWp] = apNewItem;

    apQueue->mWp = (apQueue->mWp + 1) % FRAME_QUEUE_BUF_SIZE;

    return 0;
}

int ccl_FqDropCheck(ccl_FrameQueue_t *apQueue, ccl_FrameTask_t *apDropItem)
{
    bool First = true;
    // Start with oldest item
    uint8_t Idx = apQueue->mRp;

    while (Idx != apQueue->mWp)
    {
        if (apQueue->mItems[Idx] == apDropItem)
        {
            if (First)
            {
                // drop item is in the list, on first position, we can drop it...
                apQueue->mRp = (apQueue->mRp + 1) % FRAME_QUEUE_BUF_SIZE;
            }
            else
            {
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Can't drop item from the middle of FIFO queue!\n");
                return -1;
            }
        }

        Idx = (Idx + 1) % FRAME_QUEUE_BUF_SIZE;
        First = false;
    }

    return 0;
}

void ccl_FqClean(ccl_FrameQueue_t *apQueue)
{
    memset(apQueue, 0, sizeof(ccl_FrameQueue_t));
}
