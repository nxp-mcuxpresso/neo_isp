/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file  timestamps.c
 * @brief Module allowing to capture timestamps for ISM processing.
 * @details
 */

#include "timestamps.h"
#include "platform.h"

/**
 * @brief Timestamp measurement version
 */
#define TIMESTAMPS_VERSION  (0x00010000)
/**
 * @brief Timestamps metadata
 */
struct timestamps_metadata
{
    const uint32_t  timestamp_size;         /**< Timestamps structure size in bytes. */
    const uint32_t  timestamp_version;      /**< Timestamps version. */
    uint32_t        reserved[12U];          /**< 64 byte alignment. */
    const uint32_t  data_size;              /**< Log size in bytes. */
    struct timestamps * timestamps_location;/**< Location of timestamps. */
};

#if 0
#if defined (ARM)
static struct timestamps_metadata g_timestamps_metadata __attribute__ ((section (".timestampsmetadata"))) =
#else
/* Declaration for testing purposes so the variable is accessible from the outside,
this requires the log structure to be defined in the test! */
struct timestamps_metadata g_timestamps_metadata =
#endif
{
    .timestamp_size     = sizeof(struct timestamps_metadata),
    .timestamp_version  = TIMESTAMPS_VERSION,
    .data_size          = MEASUREMENTS_COUNT * sizeof(struct timestamps_metadata),
    .timestamps_location  = &g_measurements[0],
};
#endif

/*
                        Private interface
*/
struct m
{
    uint32_t            entries_count;
    struct timestamps * time_entries;
};

/**
 * @brief Array of timestamps for most of the events happening during ISP processing.
 */
static struct m g_measurements;
// static struct timestamps g_measurements[MEASUREMENTS_COUNT];
// static struct timestamps * g_measurements;

/*
                        Public Interface
*/
int TS_init(uint8_t * buffer, size_t size)
{
    int retval = -EINVAL;
    /* Compute how many entries the buffer can hold, this must be at least 1
    to work! */
    size_t number_of_entries = size / sizeof(struct timestamps);

    if ((NULL != buffer) && (number_of_entries > 0U))
    {
        g_measurements.entries_count    = number_of_entries;
        g_measurements.time_entries     = (struct timestamps *) buffer;
        retval = 0;
    }

    return retval;
}

int TS_startElapsedMeasurement(uint32_t index, uint32_t camera_channel, uint32_t start_time)
{
    int retval = -EINVAL;
    if (index < g_measurements.entries_count)
    {
        g_measurements.time_entries[index].channel      = camera_channel;
        g_measurements.time_entries[index].data[START]  = start_time;
        retval = 0;
    }

    return retval;
}

int TS_elapsedTime(uint32_t index, enum measurement m)
{
    int retval = -EINVAL;
    if (index < g_measurements.entries_count)
    {
        g_measurements.time_entries[index].data[m] = xPortGetTime();
        retval = 0;
    }
    return retval;
}

int TS_resetMeasurement(uint32_t index)
{
    int retval = -EINVAL;
    if (index < g_measurements.entries_count)
    {
        retval = 0;
        g_measurements.time_entries[index].channel = 0U;
        for (size_t i = 0; i < (size_t) TIMESTAMPS_ENTRIES_COUNT; i++)
        {
            g_measurements.time_entries[index].data[i] = 0U;
        }
    }
    return retval;
}

int TS_setFrameNumber(uint32_t index, uint32_t frame_number)
{
    int retval = -EINVAL;
    if (index < g_measurements.entries_count)
    {
        retval = 0;
        g_measurements.time_entries[index].data[FRAME_NUMBER] = frame_number;
    }

    return retval;
}

int TS_getMeasurementInfo(uint32_t index, struct timestamps * ts)
{
    int retval = -EINVAL;
    uint8_t * tmp = (uint8_t *) ts;
    if ((index < g_measurements.entries_count) && (NULL != ts))
    {
        retval = 0;
        ts->channel = g_measurements.time_entries[index].channel;
        for (size_t i = 0U; i < sizeof(struct timestamps); i++)
        {
            tmp[i] = ((uint8_t *) (g_measurements.time_entries))[i];
        }
    }

    return retval;
}

uint32_t TS_getISPProcessingTime(uint32_t index)
{
    plt_assert(g_measurements.entries_count > index);
    plt_assert(g_measurements.time_entries[index].data[FRAME_DONE] > g_measurements.time_entries[index].data[TRIGGER_ISP]);
    return g_measurements.time_entries[index].data[FRAME_DONE] - g_measurements.time_entries[index].data[TRIGGER_ISP];
}

uint8_t TS_getEntriesCnt(void)
{
    return g_measurements.entries_count & UINT8_MAX;
}
