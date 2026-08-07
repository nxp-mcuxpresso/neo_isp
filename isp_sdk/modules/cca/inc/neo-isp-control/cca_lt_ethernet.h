/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCA_LT_ETHERNET_H
#define CCA_LT_ETHERNET_H

/******************************************************************************/
/**
 * @file  cca_lt_ethernet.h
 * @brief Communication module header file for CCF
 *******************************************************************************/

#include <stdio.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define ETHERNET_PORT   12345
#define MAX_PACKET_SIZE 4096

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/

/*===============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************
 * @brief Checks for incoming Ethernet data from a connected client.
 *
 * @details This function handles the full communication flow:
 * - Accepts a new client connection if none is active.
 * - Polls the client socket for incoming data with a specified timeout.
 * - Reads the data into the provided buffer if available.
 *
 * @param[out] apBuffer     Pointer to the buffer where received data will be stored.
 * @param[in]  aMaxLength   Maximum number of bytes to read into the buffer.
 * @param[in]  aTimeoutMs   Timeout for polling in milliseconds.
 *
 * @return Number of bytes read on success (>0),
 *         0 if no data or connection closed,
 *        -1 on error.
 *******************************************************************************/
int32_t eth_CheckForTtData(uint8_t* apBuffer, size_t aMaxLength, int32_t aTimeoutMs);

/******************************************************************************
 * @brief   Initializes the Ethernet server socket for live tuning communication.
 *
 * @details This function creates a non-blocking TCP socket, binds it to the predefined
 * port, and starts listening for incoming client connections.
 *
 * @return  0 on success, -1 on failure.
 *******************************************************************************/
int32_t eth_Init(void);

/******************************************************************************
 * @brief   Deinitializes the Ethernet sockets.
 *
 * Closes both client and server sockets if they are open.
 *
 * @return  0 always.
 *******************************************************************************/
int32_t eth_Deinit(void);

/******************************************************************************
 * @brief   Sends data to the connected client.
 *
 * @details Writes aLength bytes from apData to the client socket.
 *
 * @param[in] apData   Pointer to the data to send.
 * @param[in] aLength  Number of bytes to send.
 *
 * @return  0 on success, -1 on failure or if no client is connected.
 *******************************************************************************/
int32_t eth_Write(const uint8_t* apData, size_t aLength);

#ifdef __cplusplus
}
#endif

#endif // CCA_LT_ETHERNET_H
