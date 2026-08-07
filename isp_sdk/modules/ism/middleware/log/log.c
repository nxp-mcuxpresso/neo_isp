/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "log.h"

/**
 * @brief Log internal structure
 */
struct log_metadata
{
    const uint32_t  metadata_size;  /**< Metadata structure size in bytes. */
    const uint32_t  log_version;    /**< Log version. */
    uint32_t        end_index;      /**< Points to the end of log, the log starts at end_index - 1. */
    bool            full;           /**< Tells whether the log wraps around, i.e. the buffer has been overwritten at least once. */
    uint32_t        reserved[10U];  /**< 64 byte alignment. */
    const uint32_t  data_size;      /**< Log size in bytes. */
    uint8_t         * data_location;/**< Location of log. */
};

static uint8_t g_log[LOG_SIZE] = {0U};

#if !defined (ISIM)
static struct log_metadata g_log_meta __attribute__ ((section (".logmetadata"))) =
#else
/* Declaration for testing purposes so the variable is accessible from the outside,
this requires the log structure to be defined in the test! */
struct log_metadata g_log_meta =
#endif
{
    .metadata_size  = sizeof(struct log_metadata),
    .log_version    = LOGGING_PROTOCOL_VERSION,
    .end_index      = 0U,
    .full           = false,
    .data_size      = LOG_SIZE,
    .data_location  = g_log,
};

void LOG_init(void)
{
    g_log_meta.end_index = 0U;
    g_log_meta.full      = false;

    /* We initialize the log here, as it's in its own section, which is not
    part of data/bss section and may end up uninitialized. */
    for (uint32_t i = 0; i < LOG_SIZE; i++)
    {
        g_log[i] = 0U;
    }
}

void LOG_logEvent(const void * data, uint32_t data_size)
{
    /* Critical section beg?? */
    uint32_t end_idx = g_log_meta.end_index;
    uint8_t * tmp = &g_log[end_idx];
    g_log_meta.end_index = ((uint32_t)((uint32_t)(tmp + data_size) - (uint32_t)&g_log[0U])) % LOG_SIZE;
    /* Critical section end?? */

    if ((end_idx + data_size + 1U) > LOG_SIZE)
    {
        /* We won't fit the message into the buffer. Split transfer and continue
        from beginning after we reach the end. */
        uint32_t loop = LOG_SIZE - end_idx;
        while (0U < loop)
        {
            data_size--;
            /* Store data in reverse order. */
            *tmp = *(((const uint8_t *) data) + data_size);
            tmp++;
            loop--;
        }

        /* we reached the end of buffer, wrap around */
        tmp = &g_log[0];

        /* We had to split the transfer, set flag to indicate it. */
        g_log_meta.full = true;
    }

    /* Either we continue a split transfer or we store data without wrapping around. */
    while (0U < data_size)
    {
        data_size--;
        /* Store data in reverse order. */
        *tmp = *(((const uint8_t *) data) + data_size);
        tmp++;
    }
}
