/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/


/******************************************************************************/
/**
 * @file  ccl_semaphore.c
 * @brief implements ccl_semaphore for linux
 *******************************************************************************/

#include "ccl_thread.h"
#include "ilog.h"
#include "isp_platform.h"

#include "ccl_mutex.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/


// Number of threads created by CCL = ("number of streams" * (cca_processing + schedule + capture + sensor)) + ccl_stats
// From this only cca_processing needs large stack
#define MAX_STREAMS_ON_RTOS_PLATFORM CONFIG_ISP_SDK_CAMERA_COUNT
#if (!defined(ISPDRV_CAM_PIPE_ID_CNT) || ISPDRV_CAM_PIPE_ID_CNT > MAX_STREAMS_ON_RTOS_PLATFORM)
    #warning "Maximum supported streams for RTOS platform is lower that configured number of streams for CCL (ISPDRV_CAM_PIPE_ID_CNT)"
    #ifdef ISPDRV_CAM_PIPE_ID_CNT
        #warning "\nISPDRV_CAM_PIPE_ID_CNT is defined as: "STRINGIFY(ISPDRV_CAM_PIPE_ID_CNT)
    #else
        #warning "\nISPDRV_CAM_PIPE_ID_CNT is NOT defined"
    #endif
#endif

#define CCL_THREAD_CNT_SMALL ((MAX_STREAMS_ON_RTOS_PLATFORM * 2) + 1)
#define CCL_THREAD_CNT_NORMAL ((MAX_STREAMS_ON_RTOS_PLATFORM * 2) + 0)
#define CCL_THREAD_CNT_LARGE ((MAX_STREAMS_ON_RTOS_PLATFORM * 2) + 0)

// NOTE: at present only allocation possible, there is no free on stack stop
static StackType_t gCclStackSmall[CCL_THREAD_CNT_SMALL][CCL_TASK_STACK_SIZE_SMALL];
static StackType_t gCclStackNormal[CCL_THREAD_CNT_NORMAL][CCL_TASK_STACK_SIZE_NORMAL];
static StackType_t gCclStackLarge[CCL_THREAD_CNT_LARGE][CCL_TASK_STACK_SIZE_LARGE];

static uint32_t gCclStackUseadCntSmall = 0;
static uint32_t gCclStackUseadCntNormal = 0;
static uint32_t gCclStackUseadCntLarge = 0;

// Mutex to protect stack allocation
CCL_DEFINE_MUTEX_STATIC_INIT(gCclStackMutex);

/**
 * @brief Thread startup wrapper for linux.
 *
 * @param apArg Thread control structure.
 * @return void*
 */
static void TaskInit(void *apArg)
{
    ccl_thread_t *Th = apArg;

    (void)Th->mStartFcn(Th->mArg);

    ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "CCL Task exit - ");
    vTaskDelete(Th->mHandle);
    Th->mHandle = NULL;
}

static UBaseType_t TranslatePriority(uint32_t aPriority)
{
    UBaseType_t NewPriority;

    switch (aPriority)
    {
        case CCL_THREAD_PRIORITY_IDLE:
            NewPriority = 0;
            break;

        case CCL_THREAD_PRIORITY_LOW:
            NewPriority = (configMAX_PRIORITIES - 1) * 1 / 4;
            break;

        case CCL_THREAD_PRIORITY_NORMAL:
            NewPriority = (configMAX_PRIORITIES - 1) * 2 / 4;
            break;

        case CCL_THREAD_PRIORITY_HIGH:
            NewPriority = (configMAX_PRIORITIES - 1) * 3 / 4;
            break;

        case CCL_THREAD_PRIORITY_MAX:
            NewPriority = configMAX_PRIORITIES - 1;
            break;

        default:
            // Bad input, return lowest priority
            NewPriority = 0;
    }

    return NewPriority;
}

static ccl_Err_t ThreadCreate(ccl_thread_t *apTarget, ccl_thread_fcn aStart, void *aArg, const char *apThreadName, uint32_t aMinStackSize, ccl_ThreadPriority_t aPriority)
{
    apTarget->mStack = NULL;

    if (aMinStackSize <= CCL_TASK_STACK_SIZE_SMALL)
    {
        if (gCclStackUseadCntSmall >= CCL_THREAD_CNT_SMALL)
        {
            return CCL_ERR_ALLOC;
        }
        apTarget->mStack = gCclStackSmall[gCclStackUseadCntSmall++];
        apTarget->mStackSize = CCL_TASK_STACK_SIZE_SMALL;
    }
    else if (aMinStackSize <= CCL_TASK_STACK_SIZE_NORMAL)
    {
        if (gCclStackUseadCntNormal >= CCL_THREAD_CNT_NORMAL)
        {
            return CCL_ERR_ALLOC;
        }
        apTarget->mStack = gCclStackNormal[gCclStackUseadCntNormal++];
        apTarget->mStackSize = CCL_TASK_STACK_SIZE_NORMAL;
    }
    else if (aMinStackSize <= CCL_TASK_STACK_SIZE_LARGE)
    {
        if (gCclStackUseadCntLarge >= CCL_THREAD_CNT_LARGE)
        {
            return CCL_ERR_ALLOC;
        }
        apTarget->mStack = gCclStackLarge[gCclStackUseadCntLarge++];
        apTarget->mStackSize = CCL_TASK_STACK_SIZE_LARGE;
    }
    else
    {
        return CCL_ERR_BAD_PARAM;
    }

    apTarget->mStartFcn = aStart;
    apTarget->mArg = aArg;

    apTarget->mHandle = xTaskCreateStatic(TaskInit, apThreadName, apTarget->mStackSize, apTarget, TranslatePriority(aPriority), apTarget->mStack, &apTarget->mTaskBuffer);

    if (NULL == apTarget->mHandle)
    {
        //("Task creation failed! %s.\r\n", apThreadName);
        while (1)
            ;
    }

    return CCL_ERR_SUCCESS;
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

ccl_Err_t ccl_thread_Create(ccl_thread_t *apTarget, ccl_thread_fcn aStart, void *aArg, const char *apThreadName, uint32_t aMinStackSize, ccl_ThreadPriority_t aPriority)
{
    if (NULL == apThreadName || NULL == apTarget || aStart == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }
    if (0 != ccl_mutex_Lock(&gCclStackMutex, CCL_MUTEX_WAIT_ININITE))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Lock fail\n");
        return CCL_ERR_GENERIC;
    }

    ccl_Err_t ret = ThreadCreate(apTarget, aStart, aArg, apThreadName, aMinStackSize, aPriority);

    if (0 != ccl_mutex_Unlock(&gCclStackMutex))
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "ccl_mutex_Unlock fail\n");
        return CCL_ERR_GENERIC;
    }

    return ret;
}

/******************************************************************************/

ccl_Err_t ccl_thread_Cancel(ccl_thread_t *apTarget)
{
    if (NULL == apTarget)
    {
        return CCL_ERR_BAD_PARAM;
    }
    if (apTarget->mHandle != NULL)
    {
        vTaskDelete(apTarget->mHandle);
        apTarget->mHandle = NULL;
    }
    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_thread_SetPriority(ccl_thread_t *apTarget, ccl_ThreadPriority_t aPriority)
{
    TaskHandle_t CurrentTask;
    UBaseType_t NewPriority;

    if (apTarget == NULL)
    {
        CurrentTask = xTaskGetCurrentTaskHandle();
    }
    else
    {
        CurrentTask = apTarget->mHandle;
    }

    NewPriority = TranslatePriority(aPriority);

    vTaskPrioritySet(CurrentTask, NewPriority);

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

bool ccl_thread_IsRunning(ccl_thread_t *apTarget)
{
    return (apTarget->mHandle != NULL);
}

/******************************************************************************/

ccl_Err_t ccl_thread_ManualStopControl()
{
    // not supported on this platform - cancel available anytime
    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_thread_StopAllow()
{
    // not supported on this platform - cancel available anytime
    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_thread_StopBlocked()
{
    // not supported on this platform - cancel available anytime
    return CCL_ERR_SUCCESS;
}

/******************************************************************************/
