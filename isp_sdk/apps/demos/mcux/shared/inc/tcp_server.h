/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback prototype for TCP server connection
 *
 */
typedef void (RxCallback_t)(int Socket, const uint8_t *apcData, size_t aSize);

/**
 * @brief Starts TCP stack
 *
 * @return 0 on success, negative value on error
 */
int TcpStart(void);

/**
 * @brief Starts TCP server on given port
 *
 * @param aPort - port number to listen on
 * @param apRxCallback - callback function to handle received data for all connections
 * @return 0 on success, negative value on error
 */
int TcpServerStart(int aPort, RxCallback_t *apRxCallback);

#ifdef __cplusplus
}
#endif

#endif /* TCP_SERVER_H */
