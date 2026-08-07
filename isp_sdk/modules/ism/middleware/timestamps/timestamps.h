/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TIMESTAMPS_HEADER_GUARD
#define TIMESTAMPS_HEADER_GUARD

#ifdef __KERNEL__
    #include "linux/types.h"
#else
    #include <stdint.h>
    #include <stddef.h>
#endif


enum measurement
{
    FRAME_NUMBER,
    START,
    TRIGGER_DDR_TO_REGS,
    DDR_TO_REGS_DONE,
    TRIGGER_DDR_TO_MEMS,
    DDR_TO_MEMS_DONE,
    TRIGGER_REGS_TO_DDR,
    TRIGGER_ISP,
    ISP_P1_STATS_DONE,
    REGS_TO_DDR_DONE,
    TRIGGER_MEMS_TO_DDR,
    MEMS_TO_DDR_DONE,
    FRAME_DONE,
    UNUSED1,
    MU_SEND_FRAME_DONE,
    TIMESTAMPS_ENTRIES_COUNT,   /* Determines the number of timestamps for each measurement entry*/
};

struct timestamps
{
    uint32_t channel;
    uint32_t data[TIMESTAMPS_ENTRIES_COUNT];
};

/**
 * @brief
 *
 * @param[in] buffer
 * @param[in] size
 */
int TS_init(uint8_t * buffer, size_t size);

/**
 * @brief Stores the time at index for camera_channel for which the measurement started.
 *
 * @details The table of measurements contains MEASUREMENT_COUNT entries/rows.
 * Each entry consists of a camera channel identifier followed by timestamps.
 * The number of timestamps is equal to TIMESTAMPS_ENTRIES_COUNT. This function
 * stores the camera channel and timestamp for START measurement.
 *
 * @param[in] index Entry index.
 * @param[in] camera_channel Camera channel idrentifier.
 * @param[in] start_time request start time
 */
int TS_startElapsedMeasurement(uint32_t index, uint32_t camera_channel, uint32_t start_time);

/**
 * @brief Stores the time for given measurement at index.
 *
 * @details
 *
 * @param[in] index Entry index.
 * @param[in] m Measurement to be captured.
 */
int TS_elapsedTime(uint32_t index, enum measurement m);

/**
 * @brief Zeroes out all values for entry at index.
 *
 * @details All values including camera channel, measurement stat etc. are
 * cleared and set to zero.
 *
 * @param[in] index Entry index.
 */
int TS_resetMeasurement(uint32_t index);

/**
 * @brief Sets the frame number for measurement at index.
 *
 * @param[in] index Entry index.
 * @param[in] frame_number Image frame number.
 */
int TS_setFrameNumber(uint32_t index, uint32_t frame_number);

/**
 * @brief Returns measured value for entry at index.
 *
 * @param[in] index Entry index.
 * @param[out] ts Returned timestamps for provided index.
 */
int TS_getMeasurementInfo(uint32_t index, struct timestamps * ts);

/**
 *
 */
uint32_t TS_getISPProcessingTime(uint32_t index);

uint8_t TS_getEntriesCnt(void);

#endif /* TIMESTAMPS_HEADER_GUARD */
