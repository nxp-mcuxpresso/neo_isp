/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file qadapter.c
 *
 * FreeRTOS specific queue implementation.
 */

#include "platform.h"

int Q_create(QueueHandle_t * queue, size_t queue_length, size_t element_size, uint8_t *apDataStorage, StaticQueue_t *apControlStorage)
{
    int retval = -ENOMEM;
    QueueHandle_t q;

    q =  xQueueCreateStatic(queue_length, element_size, apDataStorage, apControlStorage);

    if (NULL != q)
    {
        *queue = q;
        retval = 0;
    }

    return retval;
}

int Q_destroy(QueueHandle_t * queue)
{
    vQueueDelete(*queue);
    *queue = NULL;

    return 0;
}

void Q_resetSynced(QueueHandle_t queue)
{
    xQueueReset(queue);
}

int Q_sendSyncedFromISR(QueueHandle_t queue, const void * item)
{
    return (xQueueSendFromISR(queue, item, NULL) == pdTRUE) ? 0 : -1;
}

int Q_sendSynced(QueueHandle_t queue, const void * item, const struct timespec * ts)
{
    /* FreeRTOS works in ticks -> convert time into ticks. */
    TickType_t t;
#if 0
    /* Time measurement not implemented yet!! */
    t = ts->tv_sec * 1000000 + ts->tv_nsec / 1000;
#endif

    if (NULL == ts)
    {
        t = portMAX_DELAY;
    }
    else
    {
        plt_assert(ts->tv_sec >= 0);
        plt_assert(ts->tv_nsec > 0);
        plt_assert(ts->tv_sec < UINT32_MAX / configTICK_RATE_HZ );
        plt_assert(ts->tv_nsec < 1000000000UL);

        /* Convert timespec to FreeRTOS ticks */
        t = (TickType_t)(((uint64_t)ts->tv_sec * configTICK_RATE_HZ) + (((uint64_t)ts->tv_nsec) * configTICK_RATE_HZ / 1000000000UL));
    }

    if (pdPASS != xQueueSend(queue, item, t))
    {
        return -1;
    }
    return 0;
}

int Q_receive(QueueHandle_t queue, void * const item, const struct timespec * ts)
{
    /* FreeRTOS works in ticks -> convert time into ticks. */
    /* TODO replace this with a MACRO??? */
    TickType_t t;
#if 0
    /* Time measurement not implemented yet!! */
    t = ts->tv_sec * 1000000 + ts->tv_nsec / 1000;
#endif
    t = (NULL == ts) ? 0 : portMAX_DELAY;

    return (xQueueReceive(queue, item, t) == pdTRUE) ? 0 : -1;
}
