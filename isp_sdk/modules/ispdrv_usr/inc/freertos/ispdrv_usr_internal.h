/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ISPDRV_USR_INTERNAL_H
#define ISPDRV_USR_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  ispdrv_usr_internal.h
 * @brief Header for internal ispdrv_usr use
 *******************************************************************************/

#include "FreeRTOS.h"
#include "semphr.h"

#include "ispdrv_usr.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define DELIVERY_THREAD_STACK_SIZE 512
#define ISM_EB_THREAD_STACK_SIZE 256
#define MAX_PENDING_EVENTS_FROM_ISM (10)

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
typedef enum
{
    ISP_STREAM_STATE_UNDEFINED,
    ISP_STREAM_STATE_CLOSED,
    ISP_STREAM_STATE_OPENING,
    ISP_STREAM_STATE_READY,
    ISP_STREAM_STATE_CLOSING
} ispdrv_CamPipeState_t;

typedef struct ispdrv_CamPipe_t
{
    ispdrv_HwId_t mHwId;

    ispdrv_CamPipeId_t mCamId;

    /** current state of the pipe */
    ispdrv_CamPipeState_t mState;

    /** client event callback if any */
    ispdrv_EventCback_t mEventCback;

    /** Information if this pipe is already allocated or free for use*/
    bool mAvailable;

    /** @brief Indicates whether this pipe is opend as sterming => upon close of this stream a change mode is requested */
    ispdrv_Mode_t mInputMode;

    /** client pointer that should be passed to mEventCback upon invocation*/
    void* mpClientPtr;

    /** mutex -> protecting access to all members of this struct
     * Note: entire struct inited to 0s, which is default mutex value init */
    StaticSemaphore_t mLock_Mem;
    SemaphoreHandle_t mLock;
} ispdrv_CamPipe_t;

typedef struct
{
    ispdrv_EventId_t mId;
    ispdrv_EventParam_t mParam;
    ispdrv_HwId_t mHwId;
    ispdrv_CamPipeId_t mCamId;
}ispdrv_Event_t;

/** @brief struct dealing with event delivery from ispdrv_usr to client code */
typedef struct
{
    /** @brief Task control structure (actual memory block) */
    StaticTask_t mTaskBuffer;
    /** @brief Task stack*/
    StackType_t mStack[DELIVERY_THREAD_STACK_SIZE];
    /** @brief Task handle (pointer to give api fcns)*/
    TaskHandle_t mEventDeliveryTask;
    /** @brief Queue control structure (actual memory block) */
    StaticQueue_t mDrvEventsQueue;
    /** @brief Queue memory for events */
    ispdrv_Event_t mEventBuf[MAX_PENDING_EVENTS_FROM_ISM];
    /** @brief Queue handle (pointer to give api fcns) */
    QueueHandle_t mDrvEvents;
}ispdrv_EvtCtl_t;


/*******************************************************************************************************
 * @brief Main ispdrv_usr structure
 * @details Single instance. Used for proper initialization and internal ispdrv communication.
 ********************************************************************************************************/
typedef struct
{
    /** @brief statically initialized to false, so we can autoinit on first external call */
    bool mInitialized;
} ispdrv_Usr_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/** declaration of global structure for all pipes */
extern ispdrv_CamPipe_t gCamPipes[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];

extern ispdrv_Usr_t gUsr;

extern ispdrv_EvtCtl_t gEvtDelivery;

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

int ispdrv_IsmEvt(uint32_t channel, const char * data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ISPDRV_USR_INTERNAL_H */
