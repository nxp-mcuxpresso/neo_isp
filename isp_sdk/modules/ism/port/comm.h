/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file comm.h
 *
 */

#ifndef COMM_HEADER_GUARD_
#define COMM_HEADER_GUARD_

#include "platform.h"

/**
 * @brief Read message.
 *
 * @details This interface is used to create input into the ISM module.
 *
 * @param[in] channel Source of data.
 * @param[in] data Data received.
 * @param[in] size Size of bytes received.data
 *
 * @note On ISP FW, this interface uses the Messaging Units hardware interface,
 * in which case the size of data is 16 bytes (4 words).
 *
 * @note The communication interface uses the MU protocol defined for ISP FW.
 */
int COMM_readMessage(uint32_t channel, char * data, size_t size);

/**
 * @brief Send message.
 *
 * @param[in] channel Source of data.
 * @param[in] data Data received.
 * @param[in] size Size of bytes received.data
 *
 * @note On ISP FW, this interface uses the Messaging Units hardware interface,
 * in which case the size of data is 16 bytes (4 words).
 *
 * @note The communication interface uses the MU protocol defined for ISP FW.
 *
 * @note Use this function for IRQ only.
 */
int COMM_sendMessageFromIRQ(uint32_t channel, const char * data, size_t size);

/**
 * @brief Send message.
 *
 * @param[in] channel Source of data.
 * @param[in] data Data received.
 * @param[in] size Size of bytes received.data
 *
 * @note On ISP FW, this interface uses the Messaging Units hardware interface,
 * in which case the size of data is 16 bytes (4 words).
 *
 * @note The communication interface uses the MU protocol defined for ISP FW.
 *
 * @note Don't invoke this function from IRQ.
 */
int COMM_sendMessage(uint32_t channel, const char * data, size_t size);

#endif  /* COMM_HEADER_GUARD_ */
