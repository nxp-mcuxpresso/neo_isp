/*******************************************************************************
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ispdrv_usr_msgs.c
 * @brief Handler for messages received from firmware
 *******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "ispdrv_usr_internal.h"
#include "isp_platform.h"
#include "internal/mu_msg_ids.h"
#include "ilog.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
typedef enum
{
    MU_MSG_INDEX_DATA_2,
    MU_MSG_INDEX_DATA_1,
    MU_MSG_INDEX_DATA_0,
    MU_MSG_INDEX_ID,
    MU_MSG_SIZE_WORDS
} Mu_MsgIndex_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

static void ispdrv_FrameDoneHandler(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, const uint32_t* apData, size_t aDataSize)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);

    if (aDataSize != MU_MSG_SIZE_WORDS)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Bad msg size. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    uint64_t DoneTaskAddr = ((uint64_t)apData[MU_MSG_INDEX_DATA_1] << 32) | (uint64_t)apData[MU_MSG_INDEX_DATA_0];
    uint8_t FwProfilingIdx = (DoneTaskAddr & 0xFF00000000000000U) >> 56;
    DoneTaskAddr &= 0x00FFFFFFFFFFFFFFU;
    uint32_t Error_id     = apData[MU_MSG_INDEX_DATA_2];
    if (DoneTaskAddr == 0)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_WRN, "Fw reports wrong task address. hwid: %d, camid: %d, TaskAddr: 0x%x\n", aHwId, aCamId, DoneTaskAddr);
        return;
    }

    if (gCamPipes[aHwId][aCamId].mEventCback == NULL)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_WRN, "No client callback to report task done. hwid: %d, camid: %d, Task: 0x%x\n", aHwId, aCamId, DoneTaskAddr);
        return;
    }

    ispdrv_Event_t evt;
    evt.mId = ISPDRV_EVENT_ID_FRAME_DONE;
    evt.mHwId = aHwId;
    evt.mCamId = aCamId;
    evt.mParam.mFrameDone.mTaskAddr = DoneTaskAddr;
    evt.mParam.mFrameDone.mErrorId  = Error_id;
    evt.mParam.mFrameDone.mProfilingIdx = FwProfilingIdx;

    if (pdPASS != xQueueSendToBack(gEvtDelivery.mDrvEvents, &evt, 0))
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "fail to queue ispdrv event. hwid: %d, camid: %d\n", aHwId, aCamId);
    }
}

static void ispdrv_ErrMsgHandler(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, const uint32_t* apData, size_t aDataSize)
{
    if (aCamId >= ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Invalid camera pipe id. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }
    if (aDataSize != MU_MSG_SIZE_WORDS)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Bad msg size. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    uint32_t Error_id     = apData[MU_MSG_INDEX_DATA_2];

    if (gCamPipes[aHwId][aCamId].mEventCback == NULL)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_WRN, "No client callback to report error msg. hwid: %d, camid: %d\n", aHwId, aCamId);
        return;
    }

    ispdrv_Event_t evt;
    evt.mId = ISPDRV_EVENT_ID_ERROR;
    evt.mHwId = aHwId;
    evt.mCamId = aCamId;
    evt.mParam.mError.mErrorId    = ISPDRV_ERROR_ID_ERR_MSG;
    evt.mParam.mError.mParam.ErrMsg = Error_id;

    if (pdPASS != xQueueSendToBack(gEvtDelivery.mDrvEvents, &evt, 0))
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "fail to queue ispdrv event. hwid: %d, camid: %d\n", aHwId, aCamId);
    }
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/**
 * @brief Callback from ISM indicating new event happened on ISM
 *
 * @param channel - MuId of channel that generated the event
 * @param data - event data
 * @param size - size of event data - should always be 16
 *
 * @return on normal run we should always return 0
 */
int ispdrv_IsmEvt(uint32_t channel, const char * data, size_t size)
{
    if (size != (MU_MSG_SIZE_WORDS*sizeof(uint32_t)))
    {
        return -1;
    }
    if (channel < 1 || channel > ISPDRV_CAM_PIPE_ID_CNT)
    {
        // should never get master MU as errors should pass into callback
        return -1;
    }

    uint8_t aCamId = channel - 1;
    uint32_t *apData = (uint32_t *)data;
    uint32_t aDataSize = size / sizeof(uint32_t);

    switch (apData[MU_MSG_INDEX_ID])
    {
        case FW_MSG_FRAME_DONE_NTF:
        {
            ispdrv_FrameDoneHandler(ISP_HWID_0, aCamId, apData, aDataSize);
        }
        break;

        case FW_MSG_ERROR_NTF:
        {
            ispdrv_ErrMsgHandler(ISP_HWID_0, aCamId, apData, aDataSize);
        }
        break;

        default:
        {
            ILOG(ILOG_MOD_ISP, ILOG_LEV_WRN, "Unknown msg from fw: 0x%x\n", apData[MU_MSG_INDEX_ID]);
        }
        break;
    }

    return 0;
}

/******************************************************************************/
