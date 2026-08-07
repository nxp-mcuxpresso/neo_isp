/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EVENT_BROKER_HEADER_GUARD_
#define EVENT_BROKER_HEADER_GUARD_

#include "platform.h"
#include "ismanager.h"

/* TODO Not sure whether to hardcode the events into the event broker or have
some sort of generic function to register events... */
struct event
{
    uint32_t data2;
    uint32_t data1;
    uint32_t data0;
    uint32_t id;
};

/**
 *
 */
typedef void (* eventHandler)(const struct event * ev);

/**
 *
 */
int EB_init(response_function responseHandler);

/**
 *
 */
int EB_deinit(void);

/**
 *
 */
int EB_registerEvent(uint32_t id, eventHandler handler);

/**
 *
 */
int EB_publishEventFromISR(const struct event * ev);

/**
 *
 */
int EB_publishEvent(const struct event * ev);

/**
 *
 */
void EB_handleEvent(void * input);

#endif /* EVENT_BROKER_HEADER_GUARD_ */
