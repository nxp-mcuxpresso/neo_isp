/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file queue.h
 */

#ifndef QUEUE_HEADER_GUARD_
#define QUEUE_HEADER_GUARD_

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"


/**
 * Creates a queue and returns a queue handle.
 *
 * On Linux, the queue is named name with number of elements equal to queue_length.
 * Every queue element will have element_size.
 *
 * @param[in] name
 * @param[in] queue_length Maximum number of elements the queue can hold.
 * @param[in] element_size Size of each queue element. Max allowed size is 256.
 *
 * @note name is POSIX specific and is ignored on FreeRTOS.
 */
#if defined(SDK_OS_FREE_RTOS)
int Q_create(QueueHandle_t * queue, size_t queue_length, size_t element_size, uint8_t *apDataStorage, StaticQueue_t *apControlStorage);
#else
int Q_create(QueueHandle_t * queue, size_t queue_length, size_t element_size);
#endif

/**
 * Destroys a queue freeing all memory.
 *
 * @param[in] queue Pointer to queue handle to be destroyed.
 */
int Q_destroy(QueueHandle_t * queue);

/**
 * Resets the queue to initial state removing all allocated elements.
 *
 * @param[in] queue Hnadle to queue to be destroyed.
 */
void Q_resetSynced(QueueHandle_t queue);

/**
 * Sends item to back of the queue. The item is copied into queue. The number of
 * bytes copied are determined based on information provided during queue
 * creation.
 *
 * @param[in] queue Queue handle where to send message.
 * @param[in] item Item to be queued.
 */
int Q_sendSyncedFromISR(QueueHandle_t queue, const void * item);

/**
 * Sends item to back of the queue. The item is copied into queue. The number of
 * bytes copied are determined based on information provided during queue
 * creation.
 *
 * @param[in] queue Queue handle where to send message.
 * @param[in] item Item to be queued.
 */
int Q_sendSynced(QueueHandle_t queue, const void * item, const struct timespec * ts);

/**
 * Dequeues an item from a queue into buffer.The item is copied into the provided buffer.
 *
 * @param[in] queue Queue from which to dequeue an item.
 * @param[out] item
 * @param[in] ts Defines the time to wait until the function returns.
 * @param[in] priority
 */
int Q_receive(QueueHandle_t queue, void * const item, const struct timespec * ts);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_HEADER_GUARD_ */
