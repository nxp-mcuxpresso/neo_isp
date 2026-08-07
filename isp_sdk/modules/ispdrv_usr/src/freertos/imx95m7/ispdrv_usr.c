/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ispdrv_usr.c
 *
 * @brief Isp driver for FreeRtos (no firmware)
 *
 * @details Isp driver. This file implements common API for FreeRtos, including direct
 * control of ISP.
 *******************************************************************************/

#include "FreeRTOS.h"
#include "fsl_irqsteer.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// ispdrv_usr definitions
#include "ispdrv_usr.h"
#include "ispdrv_usr_internal.h"

// utils
#include "ilog.h"

//ism headers
#include "ismanager.h"
#include "evtbroker.h"

// TODO nxf90122 this include should be upstreamed to modules/mxsdk-imx95_m7/devices/MIMX9596/MIMX9596_cm7.h
#include "mx95_CAMERA_BLK_CTRL_CAMERAMIX.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
/** @brief struct dealing with event delivery from ispdrv_usr to ISM */
typedef struct
{
    /** @brief Task control structure (actual memory block) */
    StaticTask_t mTaskBuffer;
    /** @brief Task stack*/
    StackType_t mStack[ISM_EB_THREAD_STACK_SIZE];
    /** @brief Task handle (pointer to give api fcns)*/
    TaskHandle_t mEventDeliveryTask;
}ispdrv_ism_task_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/** @brief Constant array mapping ISM mode to isp_platform mode */
static enum isp_mode gIspModeMapping[] = {
    [ISPDRV_MODE_M2M] = MEMORY_TO_MEMORY,
    [ISPDRV_MODE_STREAM] = STREAMING,
};

/*==============================================================================
*                                  VARIABLES
==============================================================================*/
/****************************************************************************
 * @brief States and associated data for individual processing pipes
 ********************************************************************************/
ispdrv_CamPipe_t gCamPipes[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];
uint8_t gMeasurementBuffer[64 * 64];

ispdrv_EvtCtl_t gEvtDelivery =
{
    .mEventDeliveryTask = NULL,
};

ispdrv_ism_task_t gIsmTask =
{
    .mEventDeliveryTask = NULL,
};

/****************************************************************************
 * @brief The only instance of ispdrv_usr control structure
 ********************************************************************************/
ispdrv_Usr_t gUsr =
    {
        .mInitialized = false,
};

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/


static void IspdrvEvtTask(void *apParameters)
{
    ispdrv_Event_t evt;

    while (1)
    {
        if (pdPASS  == xQueueReceive(gEvtDelivery.mDrvEvents, &evt, INT32_MAX))
        {
            int32_t HwId = evt.mHwId;
            int32_t CamId = evt.mCamId;
            assert(HwId < ISP_HWID_CNT);
            assert(CamId < ISPDRV_CAM_PIPE_ID_CNT);
            ispdrv_EventCback_t ClientCback = gCamPipes[HwId][CamId].mEventCback;
            void* ClientPtr                 = gCamPipes[HwId][CamId].mpClientPtr;

            if (ClientCback != NULL)
            {
                ClientCback(HwId, CamId, evt.mId, &evt.mParam, ClientPtr);
            }
        }
    }
}

static int InitIsm(void)
{

    /*
        Setup clock for ISP subsystem
        Clock configuration of ISP Subdomain Manager is intentionally controlled
        outside the ISM library.
        The clock domain is considered as a separate resource/domain like IRQ's
        which are configured separately and not by the driver.
    */
    CAMERA_BLK_CTRL_CAMERAMIX_Type * csr = (CAMERA_BLK_CTRL_CAMERAMIX_Type *) CAMERA_CSR_BASE;
    uint32_t isp_cgc = csr->ISP_CLOCK_GATING_CONTROL;
    isp_cgc &= 0xFFFFFF0FU;
    csr->ISP_CLOCK_GATING_CONTROL = isp_cgc;

#ifdef USE_EDMA_DESCR
    ISM_init(ISP, EDMA, EDMA_DESCRIPTORS, gMeasurementBuffer, sizeof(gMeasurementBuffer), ispdrv_IsmEvt);
#else
    ISM_init(ISP, EDMA, NULL, gMeasurementBuffer, sizeof(gMeasurementBuffer), ispdrv_IsmEvt);
#endif

    gIsmTask.mEventDeliveryTask = xTaskCreateStatic(EB_handleEvent, "ISM_EB", ISM_EB_THREAD_STACK_SIZE, NULL, 3, gIsmTask.mStack, &gIsmTask.mTaskBuffer);
    if (gIsmTask.mEventDeliveryTask == NULL)
    {
        return -1;
    }

    EnableIRQ(ISP_IRQn);
    NVIC_SetPriority(ISP_IRQn, 5);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_ERROR_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_0_1_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_2_3_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_4_5_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_6_7_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_8_9_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_10_11_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_12_13_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_14_15_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_16_17_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_18_19_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_20_21_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_22_23_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_24_25_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_26_27_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_28_29_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA5_4_30_31_IRQn);

    return 0;
}

/******************************************************************************/
/**
 * @brief Make sure that pipe control structure is initialized.
 *
 * @return  none
 *******************************************************************************/
static void ispdrv_InitIfNot()
{
    if (!gUsr.mInitialized)
    {
        gUsr.mInitialized = true;

        for (ispdrv_HwId_t HwId = 0; HwId < ISP_HWID_CNT; HwId++)
        {
            for (ispdrv_CamPipeId_t CamId = 0; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
            {
                ispdrv_CamPipe_t* pipe       = &gCamPipes[HwId][CamId];
                pipe->mHwId                  = HwId;
                pipe->mCamId                 = CamId;
                pipe->mAvailable             = true;
                pipe->mEventCback            = NULL;
                pipe->mpClientPtr            = NULL;
                pipe->mState                 = ISP_STREAM_STATE_CLOSED;

                pipe->mLock = xSemaphoreCreateRecursiveMutexStatic(&pipe->mLock_Mem);
                if (NULL == pipe->mLock)
                {
                    ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "mutex init error\n");
                }
            }
        }

        gEvtDelivery.mDrvEvents = xQueueCreateStatic(MAX_PENDING_EVENTS_FROM_ISM, sizeof(ispdrv_Event_t), (uint8_t *)gEvtDelivery.mEventBuf, &gEvtDelivery.mDrvEventsQueue);

        InitIsm();

        gEvtDelivery.mEventDeliveryTask = xTaskCreateStatic(IspdrvEvtTask, "IspdrvEvts", DELIVERY_THREAD_STACK_SIZE, NULL, configMAX_PRIORITIES/2, gEvtDelivery.mStack, &gEvtDelivery.mTaskBuffer);
        if (NULL == gEvtDelivery.mEventDeliveryTask)
        {
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ISP drvier event task creation failed!.\r\n");
            while (1)
                ;
        }
    }
}

static ispdrv_Result_t Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    ispdrv_CamPipe_t* Pipe = &gCamPipes[aHwId][aCamId];

    /* resource availability check - if already closed do nothing, unless we want to force cleanup procedure*/
    if (gCamPipes[aHwId][aCamId].mState != ISP_STREAM_STATE_READY)
    {
        return ISPDRV_RES_BAD_STATE;
    }

    Pipe->mState = ISP_STREAM_STATE_CLOSING;

    if (Pipe->mInputMode == ISPDRV_MODE_STREAM)
    {
        assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
        uint8_t channel = aCamId + 1;
        if (0 != ISM_changeISPMode(channel, MEMORY_TO_MEMORY))
        {
            ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Failed to change ISP mode back to Memory-to-Memory\n");
            return ISPDRV_RES_ERROR_HW;
        }
    }

    Pipe->mAvailable  = true;
    Pipe->mEventCback = NULL;
    Pipe->mpClientPtr = NULL;
    Pipe->mInputMode  = ISPDRV_MODE_M2M;
    Pipe->mState      = ISP_STREAM_STATE_CLOSED;

    return ISPDRV_RES_SUCCESS;
}

static ispdrv_Result_t Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_OpenCfg_t* const acpConfig)
{
    ispdrv_CamPipe_t* Pipe = &gCamPipes[aHwId][aCamId];

    /* resource availability check */
    if (!Pipe->mAvailable)
    {
        return ISPDRV_RES_BAD_STATE;
    }

    Pipe->mState = ISP_STREAM_STATE_OPENING;

    Pipe->mAvailable  = false;
    Pipe->mEventCback = acpConfig->mEventCback;
    Pipe->mpClientPtr = acpConfig->mpClientPtr;
    Pipe->mState      = ISP_STREAM_STATE_READY;

    return ISPDRV_RES_SUCCESS;
}

static ispdrv_Result_t TaskSchedule(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint64_t aTaskAddr)
{
    assert(aHwId < ISP_HWID_CNT);
    ispdrv_CamPipe_t* Pipe = &gCamPipes[aHwId][aCamId];

    /* verify pipe is ready to receive tasks */
    if (Pipe->mState != ISP_STREAM_STATE_READY)
    {
        return ISPDRV_RES_BAD_STATE;
    }

    // ISM works with MU id not cam id
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    uint8_t MuId = aCamId + 1;
    uint res = ISM_requestISPProcessing(MuId, aTaskAddr);
    if (res != 0)
    {
        return ISPDRV_RES_ERROR;
    }

    return ISPDRV_RES_SUCCESS;
}

static ispdrv_Result_t ChangeMode(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_Mode_t aNewMode)
{
    assert(aHwId < ISP_HWID_CNT);
    ispdrv_CamPipe_t* Pipe = &gCamPipes[aHwId][aCamId];

    /* verify pipe is ready to receive tasks */
    if (Pipe->mState != ISP_STREAM_STATE_READY)
    {
        return ISPDRV_RES_BAD_STATE;
    }

    enum isp_mode mode = gIspModeMapping[aNewMode];

    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    uint8_t channel = aCamId + 1;
    if (0 != ISM_changeISPMode(channel, mode))
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Failed to change ISP mode to %d\n", aNewMode);
        return ISPDRV_RES_ERROR_HW;
    }

    // Track pipe mode so we can automatically swich to M2M upon Close of Stream
    Pipe->mInputMode = aNewMode;

    return ISPDRV_RES_SUCCESS;
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

ispdrv_Result_t ispdrv_Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_OpenCfg_t* const acpConfig)
{

    /** param sanity check */
    if (aHwId >= ISP_HWID_CNT
        || aCamId >= ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Bad id hwid: %d, camid: %d\n", aHwId, aCamId);
        return ISPDRV_RES_BAD_PARAM;
    }
    if (acpConfig->mEventCback == NULL)
    {
        return ISPDRV_RES_BAD_PARAM;
    }

    ispdrv_InitIfNot();

    ispdrv_Result_t ret;

    if (pdTRUE != xSemaphoreTakeRecursive(gCamPipes[aHwId][aCamId].mLock, portMAX_DELAY ))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_ext_mutex_lock fail\n");
        return ISPDRV_RES_ERROR;
    }

    ret = Open(aHwId, aCamId, acpConfig);

    if (pdTRUE != xSemaphoreGiveRecursive(gCamPipes[aHwId][aCamId].mLock))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_mutex_unlock fail\n");
        return ISPDRV_RES_ERROR;
    }

    return ret;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    /** param sanity check */
    if (aHwId >= ISP_HWID_CNT
        || aCamId >= ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Bad id hwid: %d, camid: %d\n", aHwId, aCamId);
        return ISPDRV_RES_BAD_PARAM;
    }

    ispdrv_InitIfNot();

    if (pdTRUE != xSemaphoreTakeRecursive(gCamPipes[aHwId][aCamId].mLock, portMAX_DELAY ))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_ext_mutex_lock fail\n");
        return ISPDRV_RES_ERROR;
    }

    ispdrv_Result_t ret = Close(aHwId, aCamId);

    if (pdTRUE != xSemaphoreGiveRecursive(gCamPipes[aHwId][aCamId].mLock))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_mutex_unlock fail\n");
        return ISPDRV_RES_ERROR;
    }

    return ret;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_CloseForced(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    (void)aHwId;
    (void)aCamId;

    return ISPDRV_RES_NOT_IMPLEMENTED;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_TaskSchedule(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint64_t aTaskAddr)
{
    /** param sanity check */
    if (aHwId >= ISP_HWID_CNT
        || aCamId >= ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Bad id hwid: %d, camid: %d\n", aHwId, aCamId);
        return ISPDRV_RES_BAD_PARAM;
    }

    ispdrv_InitIfNot();

    if (pdTRUE != xSemaphoreTakeRecursive(gCamPipes[aHwId][aCamId].mLock, portMAX_DELAY ))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_mutex_unlock fail\n");
        return ISPDRV_RES_ERROR;
    }

    ispdrv_Result_t ret = TaskSchedule(aHwId, aCamId, aTaskAddr);

    if (pdTRUE != xSemaphoreGiveRecursive(gCamPipes[aHwId][aCamId].mLock))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_mutex_unlock fail\n");
        return ISPDRV_RES_ERROR;
    }

    return ret;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_TaskScheduleBlocking(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, isp_Task_t* const acpTask)
{
    (void)aHwId;
    (void)aCamId;
    (void)acpTask;

    return ISPDRV_RES_NOT_IMPLEMENTED;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_ChangeMode(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_Mode_t aNewMode)
{
    /** param sanity check */
    if (aHwId >= ISP_HWID_CNT
        || aCamId >= ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_ISP, ILOG_LEV_ERR, "Bad id hwid: %d, camid: %d\n", aHwId, aCamId);
        return ISPDRV_RES_BAD_PARAM;
    }

    ispdrv_InitIfNot();

    if (pdTRUE != xSemaphoreTakeRecursive(gCamPipes[aHwId][aCamId].mLock, portMAX_DELAY ))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_ext_mutex_lock fail\n");
        return ISPDRV_RES_ERROR;
    }

    ispdrv_Result_t ret = ChangeMode(aHwId, aCamId, aNewMode);

    if (pdTRUE != xSemaphoreGiveRecursive(gCamPipes[aHwId][aCamId].mLock))
    {
        ILOG(ILOG_MOD_ISP_SIM, ILOG_LEV_ERR, "pthread_mutex_unlock fail\n");
        return ISPDRV_RES_ERROR;
    }

    return ret;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_LoadFwMeta(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, isp_Task_t* apIspCfg, uint8_t aTableIdx)
{
    (void)aHwId;
    (void)aCamId;

    FwTimeStampLine_t* TimestampTable = (FwTimeStampLine_t*)(gMeasurementBuffer);
    assert(aTableIdx < (sizeof(gMeasurementBuffer) / sizeof(FwTimeStampLine_t)));

    apIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.mu_channel = TimestampTable[aTableIdx].mu_channel;
    apIspCfg->mIspCtx.mCtxMeta.mFrameIdx = TimestampTable[aTableIdx].ts[FW_TIME_FRAME_NUMBER];
    for (int i = 0; i < FW_TIME_TIMESTAMPS_ENTRIES_COUNT; i++)
    {
        apIspCfg->mIspCtx.mCtxMeta.mFwTimestamps.ts[i] = TimestampTable[aTableIdx].ts[i];
    }

    return 0;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_isifix(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool aStreamActive)
{
    (void)aHwId;
    (void)aCamId;
    (void)aStreamActive;

    return ISPDRV_RES_NOT_IMPLEMENTED;
}

/******************************************************************************/

ispdrv_Result_t ispdrv_GetFwIdleStats(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint32_t* apIspIdle, uint32_t* apCpuIdle)
{
    (void)aHwId;
    (void)aCamId;
    (void)apIspIdle;
    (void)apCpuIdle;

    return ISPDRV_RES_NOT_IMPLEMENTED;
}
