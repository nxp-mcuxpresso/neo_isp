/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cca_lt_connect_eth.c
 * @brief Implements connection with live tuning module
 *******************************************************************************/

 #include "cca.h"

#include "ilog.h"
#include "cca_lt_ethernet.h"
#include "cca_lt_connect.h"
#include "lt/live_tuning.h"

#include "tcp_server.h"
#include "lwip/sockets.h"

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

/*==============================================================================
*                              LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/
// server on which we run
static int ListeningSocket = -1;
// active client socket for live tuning
static int ClientSocket = -1;

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/**
 * @brief Sends given data to live tunning client side (if connected)
 *
 * @param apcData pointer to data buffer to send
 * @param aSize size of data to send in bytes
 * @return 0 if successful, -1 if invalid parameters, -2 if write failed
 */
static int32_t cca_LtTransmit(const uint8_t* apcData, size_t aSize)
{
    if ((NULL == apcData) || (0U == aSize) || (ListeningSocket < 0) || (ClientSocket < 0))
    {
        return -1;
    }

    int WrittenBytes = write(ClientSocket, apcData, aSize);
    if (WrittenBytes != aSize)
    {
        return -2;
    }

    return 0;
}

/**
 * @brief Handles incoming data events from live tunning client
 *
 * @param Socket socket id over which data arrived
 * @param apcData pointer to received data buffer
 * @param aSize size of received data in bytes
 */
void cca_LtEventMoreData(int Socket, const uint8_t *apcData, size_t aSize)
{
    if (Socket == -1)
    {
        // notification from eth code that client disconnected
        ClientSocket = -1;
        ILOG(ILOG_MOD_LT, ILOG_LEV_WRN, "Eth client disconnected.\n");
        return;
    }
    if (ClientSocket != -1 && ClientSocket != Socket)
    {
        // we have active connection and received new data on different socket
        ILOG(ILOG_MOD_LT, ILOG_LEV_WRN, "Received data on new socket connection - switching transmit target.\n");
    }
    ClientSocket = Socket;

    if (apcData == NULL || aSize == 0U)
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_WRN, "Invalid cca_LtEventMoreData event.\n");
        return;
    }

    // Data available -> send them to Live tuning
    if (0 != lt_ProcessRawPacket(apcData, aSize))
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_ERR, "Failed to process TT message.\n");
    }
}

cca_Result_t cca_LtConnect()
{
    // Init communication interface
    if (0U != TcpStart())
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_ERR, "Failed to init communication live tuning module.\n");
        return CCA_ERROR;
    }

    // start listening on live tuning port
    ListeningSocket = TcpServerStart(12345, cca_LtEventMoreData);
    if (ListeningSocket < 0)
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_ERR, "Failed to start TCP server for live tuning module.\n");
        return CCA_ERROR;
    }

    // Initialize the LT communication module and pass the write function to LT
    if (0U != lt_InitLtModule(cca_LtTransmit))
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_ERR, "Failed to init communication live tuning module.\n");
        return CCA_ERROR;
    }

    return CCA_SUCCESS;
}

/******************************************************************************/
cca_Result_t cca_LiveTune()
{
    // called after CCA process finished - not used

    return CCA_SUCCESS;
}
