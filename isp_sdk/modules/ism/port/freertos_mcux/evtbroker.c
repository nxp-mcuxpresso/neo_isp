/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "evtbroker.h"
#include "qadapter.h"
#include <time.h>

#define MAX_EVENTS              ((uint32_t) 20U)
#define EVENT_QUEUE_LENGTH      ((uint32_t) 20U)
#define EVENT_QUEUE_ITEM_SIZE   ((size_t) sizeof(struct event))



struct pair
{
    uint32_t        event_id;
    eventHandler    handler;
};

struct pair g_event_handlers[MAX_EVENTS];

struct evtbroker
{
    /**
     *
     */
    QueueHandle_t event_queue;

    response_function responeHandler;
};

static struct evtbroker g_event_broker;

static uint8_t gEventQueueData[EVENT_QUEUE_LENGTH * EVENT_QUEUE_ITEM_SIZE];
static StaticQueue_t gEventQueue;


int EB_init(response_function responseHandler)
{
    g_event_broker.responeHandler = responseHandler;

    return Q_create(&g_event_broker.event_queue, EVENT_QUEUE_LENGTH, EVENT_QUEUE_ITEM_SIZE, gEventQueueData, &gEventQueue);;
}

int EB_deinit(void)
{
    return Q_destroy(&g_event_broker.event_queue);
}

int EB_registerEvent(uint32_t id, eventHandler handler)
{
    if (MAX_EVENTS <= id)
    {
        /* As our implementation is using an array and the events and it's handlers
        are stored at array index positions, we need to make sure the index doesn't
        exceed the array boundaries. */
        return -1;
    }

    if (NULL != g_event_handlers[id].handler)
    {
        /* An event is already registered there! */
        return -1;
    }

    g_event_handlers[id].handler = handler;
    return 0;
}


/* ========================== */
/*      platform specific??   */
int EB_publishEventFromISR(const struct event * ev)
{
    return Q_sendSyncedFromISR(g_event_broker.event_queue, (void *) ev);
}

int EB_publishEvent(const struct event * ev)
{
    return Q_sendSynced(g_event_broker.event_queue, (void *) ev, NULL);
}

void EB_handleEvent(void * input)
{
    (void) input;
    struct event item;
    struct timespec t = {__INT32_MAX__, __INT32_MAX__};

    while (1)
    {
        /* On FreeRTOS this function is used as a thread. From that perspective,
        the Q_receive "blocks" meaning it wait's indefinitely, until
        an event is published (queued). */
        if (0 != Q_receive(g_event_broker.event_queue, (void *) &item, &t))
        {
            DEBUG_PRINT("Warning - EB_handleEvent() Q_receive() timeout.\n");
            continue;
        }

        plt_assert(item.id < MAX_EVENTS);

        if (NULL != g_event_handlers[item.id].handler)
        {
            g_event_handlers[item.id].handler(&item);
        }
        else
        {
            /* We have received an invalid ID or the corresponding handler has
            not been registered. In either case this is a system error or bug!*/
            uint32_t data[4U] = {0U, 0U, IMP_ERROR_CMD, IMP_ERROR};
            g_event_broker.responeHandler(MASTER_ENTITY_CHANNEL, (char *) data, sizeof(data));
        }
    }
}
