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

#include "ccl_mutex.h"

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

static ccl_Err_t Initialize(ccl_mutex_t *apTarget)
{
    if (apTarget->mInitialized)
    {
        return CCL_ERR_SUCCESS;
    }

    if (apTarget->mRecursive)
    {
        apTarget->mHandle = xSemaphoreCreateRecursiveMutexStatic(&apTarget->mMutex);
    }
    else
    {
        apTarget->mHandle = xSemaphoreCreateMutexStatic(&apTarget->mMutex);
    }

    if (NULL == apTarget->mHandle)
    {
        return CCL_ERR_GENERIC;
    }

    apTarget->mInitialized = true;
    return CCL_ERR_SUCCESS;
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

ccl_Err_t ccl_mutex_Create(ccl_mutex_t *apTarget, bool aRecursive)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }
    if (apTarget->mInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }
    apTarget->mRecursive = aRecursive;

    return Initialize(apTarget);
}

/******************************************************************************/

ccl_Err_t ccl_mutex_Destroy(ccl_mutex_t *apTarget)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }
    if (!apTarget->mInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    vSemaphoreDelete(apTarget->mHandle);

    apTarget->mInitialized = false;

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_mutex_TryLock(ccl_mutex_t *apTarget)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }
    ccl_Err_t res = Initialize(apTarget);
    if (res != CCL_ERR_SUCCESS)
    {
        return res;
    }

    if (apTarget->mRecursive)
    {
        if (pdFALSE == xSemaphoreTakeRecursive(apTarget->mHandle, 0))
        {
            return CCL_ERR_TIMEOUT;
        }
    }
    else
    {
        if (pdFALSE == xSemaphoreTake(apTarget->mHandle, 0))
        {
            return CCL_ERR_TIMEOUT;
        }
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_mutex_Lock(ccl_mutex_t *apTarget, int aWaitMs)
{
    if (apTarget == NULL || aWaitMs < 0)
    {
        return CCL_ERR_BAD_PARAM;
    }

    ccl_Err_t res = Initialize(apTarget);
    if (res != CCL_ERR_SUCCESS)
    {
        return res;
    }

    if (apTarget->mRecursive)
    {
        if (pdFALSE == xSemaphoreTakeRecursive(apTarget->mHandle, pdMS_TO_TICKS(aWaitMs)))
        {
            return CCL_ERR_TIMEOUT;
        }
    }
    else
    {
        if (pdFALSE == xSemaphoreTake(apTarget->mHandle, pdMS_TO_TICKS(aWaitMs)))
        {
            return CCL_ERR_TIMEOUT;
        }
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/

ccl_Err_t ccl_mutex_Unlock(ccl_mutex_t *apTarget)
{
    if (apTarget == NULL)
    {
        return CCL_ERR_BAD_PARAM;
    }
    if (!apTarget->mInitialized)
    {
        return CCL_ERR_BAD_STATE;
    }

    if (apTarget->mRecursive)
    {
        if (pdFALSE == xSemaphoreGiveRecursive(apTarget->mHandle))
        {
            return CCL_ERR_GENERIC;
        }
    }
    else
    {
        if (pdFALSE == xSemaphoreGive(apTarget->mHandle))
        {
            return CCL_ERR_GENERIC;
        }
    }

    return CCL_ERR_SUCCESS;
}

/******************************************************************************/
