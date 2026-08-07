/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/


/******************************************************************************/
/**
 * @file  ccl_semaphore.c
 * @brief implements ccl_semaphore for FreeRtos
 *******************************************************************************/

#include "ccl_semaphore.h"

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


ccl_Err_t ccl_sem_Init(ccl_sem_t *apTarget, int aInitCnt)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }

    apTarget->mHandle = xSemaphoreCreateCountingStatic(((UBaseType_t) ~0U), aInitCnt, &apTarget->mSem);
    if (apTarget->mHandle == NULL)
    {
        return CCL_ERR_GENERIC;
    }
    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_sem_Destroy(ccl_sem_t *apTarget)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }

    if (apTarget->mHandle != NULL)
    {
        vSemaphoreDelete(apTarget->mHandle);
        apTarget->mHandle = NULL;
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_sem_Take(ccl_sem_t *apTarget, int aWaitMs)
{
    if (apTarget == NULL || aWaitMs < 0)
    {
        return CCL_ERR_BAD_PARAM;
    }

    if (pdTRUE != xSemaphoreTake(apTarget->mHandle, pdMS_TO_TICKS(aWaitMs)))
    {
        return CCL_ERR_TIMEOUT;
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_sem_Give(ccl_sem_t* apTarget)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }

    if (pdTRUE != xSemaphoreGive(apTarget->mHandle))
    {
        return CCL_ERR_GENERIC;
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_sem_GiveFromISR(ccl_sem_t* apTarget)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }

    if (pdTRUE != xSemaphoreGiveFromISR(apTarget->mHandle, pdFALSE))
    {
        return CCL_ERR_GENERIC;
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_sem_GetCnt(ccl_sem_t *apTarget, int *apOutCnt)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }

    uint32_t semCount = uxSemaphoreGetCount(apTarget->mHandle);
    *apOutCnt = semCount % INT32_MAX;

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/
