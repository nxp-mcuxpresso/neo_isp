/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  utils_edma.cpp
 * @brief EDMA Freertos interface
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include "FreeRTOS.h"
#include "semphr.h"
#include "utils_edma.h"
#include "ilog.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
#include "projdefs.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define UTILS_DMA_BASEADDR EDMA5_2
#define UTILS_DMA_CHANNEL_0 0U
#define UTILS_DMA_IRQ DMA5_2_0_1_IRQn
#define UTILS_DMA_IRQ_HANDLER DMA5_2_0_1_IRQHandler

/*==============================================================================
*                         EXTERNAL GLOBAL variables
==============================================================================*/

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
/**
 * @brief driver handle.
 */
typedef struct utils_edma_handle
{
    SemaphoreHandle_t mutex; /*!< Mutex to lock the handle during a trasfer */
    SemaphoreHandle_t semaphore; /*!< Semaphore to notify and unblock task when transfer ends */
    StaticSemaphore_t mutexData;
    StaticSemaphore_t semaphoreData;
} utils_edma_handle_t;

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/

DDR_NOINIT static utils_edma_handle_t sgEdmaHandle;

/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/

/* EDMA transfer channel 0 IRQ handler */
void UTILS_DMA_IRQ_HANDLER(void)
{
    if ((EDMA_GetChannelStatusFlags(UTILS_DMA_BASEADDR, UTILS_DMA_CHANNEL_0) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_ClearChannelStatusFlags(UTILS_DMA_BASEADDR, UTILS_DMA_CHANNEL_0, kEDMA_InterruptFlag);

        utils_edma_handle_t* handle = &sgEdmaHandle;
        BaseType_t reschedule = pdFALSE;

        if (pdPASS == xSemaphoreGiveFromISR(handle->semaphore, &reschedule))
        {
            portYIELD_FROM_ISR(reschedule);
        }
    }
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/*******************************************************************************/
void utils_EdmaOpen()
{
    utils_edma_handle_t* handle = &sgEdmaHandle;

    /* Create mutex for resource protection */
    handle->mutex = xSemaphoreCreateMutexStatic(&handle->mutexData);
    if (handle->mutex == NULL)
    {
        return;
    }

    handle->semaphore = xSemaphoreCreateBinaryStatic(&handle->semaphoreData);
    if (handle->semaphore == NULL)
    {
        vSemaphoreDelete(handle->mutex);
        return;
    }

    /* Initialize EDMA module */
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(UTILS_DMA_BASEADDR, &userConfig);
    EnableIRQ(UTILS_DMA_IRQ);
} // utils_EdmaOpen()

/*******************************************************************************/
void utils_EdmaClose()
{
    utils_edma_handle_t* handle = &sgEdmaHandle;

    vSemaphoreDelete(&handle->mutexData);
    vSemaphoreDelete(&handle->semaphoreData);

    EDMA_Deinit(UTILS_DMA_BASEADDR);
    DisableIRQ(UTILS_DMA_IRQ);
} // utils_EdmaClose()

/*******************************************************************************/
int utils_EdmaTransfer(void* dest, const void* src, size_t n)
{
    if ((dest == NULL) || (src == NULL) || (n == 0U))
    {
        return -1;
    }

    utils_edma_handle_t* handle = &sgEdmaHandle;

    /* Lock resource mutex */
    if (xSemaphoreTake(handle->mutex, portMAX_DELAY) != pdTRUE)
    {
        return -1;
    }

    edma_transfer_config_t transferConfig;

    EDMA_PrepareTransfer(&transferConfig, (void*)src, sizeof(uint32_t), (void*)dest, sizeof(uint32_t), n, n, kEDMA_MemoryToMemory);
    EDMA_SetTransferConfig(UTILS_DMA_BASEADDR, UTILS_DMA_CHANNEL_0, &transferConfig, NULL);
    EDMA_TriggerChannelStart(UTILS_DMA_BASEADDR, UTILS_DMA_CHANNEL_0);

    /* Wait for transfer to finish */
    if (xSemaphoreTake(handle->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return -1;
    }

    /* Unlock resource mutex */
    if (xSemaphoreGive(handle->mutex) != pdTRUE)
    {
        return -1;
    }

    return 0;
} // utils_EdmaTransfer()

/*******************************************************************************/
