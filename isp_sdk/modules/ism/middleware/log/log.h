/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief Simple logging library.
 * @details This logging library allows a simple logging of messages into
 * internal buffer located at in RAM. The data has a metadata structure describing
 * the log. See linker script for details about the placement.
 * The log metadata structure is following:
 * +----------------+----------+
 * | structure size | uint32_t |    size of the metadata structure
 * +----------------+----------+
 * |   log version  | uint32_t |    version of log
 * +----------------+----------+
 * |  end index     | uint32_t |    index into log where logging ended
 * +----------------+----------+
 * |       full     | uint32_t |    if buffer has been overwritten at least once
 * +----------------+----------+
 * |   reserved[10] | uint32_t |    reserved for future use
 * +----------------+----------+
 * |    data size   | uint32_t |    size of data buffer
 * +----------------+----------+
 * | data location  | uint32_t |    location of data buffer
 * +----------------+----------+
 *
 * The log values values are in reversed order. So to read them out, read them
 * byte by byte in reversed order starting from the end index -1, i.e.
 * +---------------------+
 * | d0 | d1 | len | n/a |
 * +---------------------+
 *                    ^
 *                    | end index
 *
 * The implementation allows to store the data in any way. However, a simplified
 * protocol is used throughout the code:
 * +---------------------------------------------+
 * | uint32  | uint32 | uint32 | uint32 | uint32 |
 * |---------+--------+--------+--------+--------|
 * | log len |  ID    | data0  | ...... | dataN  |
 * +---------------------------------------------+
 *
 * This protocol allows to read data back in a deterministic way.
 *
 * @warning The data is stored in reversed byte order. So read the log byte by byte
 * in reversed order first starting from the end index - 1, to get them back in
 * little endian. Afterwards it's possible to read the values as uint32_t.
 */
#ifndef MY_LOG_GUARD
#define MY_LOG_GUARD

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"
#include "logging_protocol.h"

#define LOG_DISABLED    0
#define LOG_ENABLED     1

/**
 * @brief Log size in bytes.
 */
#define LOG_SIZE        ((uint32_t)((uint32_t) 1024U * 4U))

#if !defined(LOGGING)
    #define LOGGING     LOG_ENABLED
    #pragma message ("Logging has been ENABLED by default!")
#endif

/* Enable this part of code just to verify, that the interconnection between
makefile and code works as intended. */
#if 0
    #if LOGGING == LOG_ENABLED
        #pragma message ("Logging is enabled!")
    #elif LOGGING == LOG_DISABLED
        #pragma message ("Logging is disabled")
    #else
    #endif
#endif

/**
 * @brief Initialize logging structure.
 */
void LOG_init(void);

/**
 * @brief Logs data into internal structure.
 */
void LOG_logEvent(const void * data, uint32_t data_size);

#ifdef __cplusplus
}
#endif


#endif   /* MY_LOG_GUARD */
