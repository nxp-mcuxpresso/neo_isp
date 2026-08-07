/*
 * Copyright (c) 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"

#include "tcpecho.h"
#include "lwip/netifapi.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

#include "board.h"
#include "app.h"
#include "fsl_phy.h"

#include "lwip/sockets.h"
#include "tcp_server.h"


/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/* Must be after include of app.h */
#if !defined(configMAC_ADDR) || (defined(BOARD_NETWORK_USE_DUAL_ENET) && !defined(configMAC_ADDR1))
#include "fsl_silicon_id.h"
#endif

// Max number of ports we will listen on
#define MAX_TCP_SERVERS 1
// Max number of concurrent connections per server
#define MAX_TCP_CONNECTIONS 1

// size of the only received buffer in this layer, there are also some rx buffers inside LwIP stack
#define DATA_BUFF_SIZE 1500

// wait for data to arrive over open TCP connection, note that upon timeout we simply wait again
#define TCP_RX_WAIT_TIMEOUT_MS 10000
// max wait for read of data from the connection once we receive notification that some data are already waiting
// NOTE: once we start reading we either receive DATA_BUFF_SIZE of bytes or wait for this timeout
#define TCP_RX_READ_TIMEOUT_US 100

// selected ETH interface for the stack
#ifndef EXAMPLE_NETIF_INIT_FN
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif

// number of PHYs we want to activate
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
#define BOARD_PHY_COUNT 2
#else
#define BOARD_PHY_COUNT 1
#endif

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

 /**
  * @brief Each client connection have its own thread. This info is passed to the thread upon client connection.
  *
  */
typedef struct
{
    bool active;
    int mPort;
    int mSck;
    int mConnections[MAX_TCP_CONNECTIONS];
    RxCallback_t *mRxCallback;
    uint8_t (*mRxBuffer)[MAX_TCP_CONNECTIONS][DATA_BUFF_SIZE];
}ServerState_t;

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

static phy_handle_t phyHandle;
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
static phy_handle_t phyHandle1;

#if LWIP_SINGLE_NETIF == 1
#error \
    "Single netif limitation in lwIP must be disabled if this example were to use both interfaces. (LWIP_SINGLE_NETIF = 0)"
#endif // LWIP_SINGLE_NETIF == 1
#endif

static bool StackReady = false;
static bool StackStarting = false;
static ServerState_t gServers[MAX_TCP_SERVERS];

DDR_NOINIT static uint8_t gTcpRxBuf[MAX_TCP_SERVERS][MAX_TCP_CONNECTIONS][DATA_BUFF_SIZE];

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/**
 * @brief Callback function invoked from inside LwIP TCP/IP stack once initialization of the stack is done
 *
 * @param arg Task handle to notify when stack initialization is complete
 */
static void tcpip_init_done_evt(void *arg)
{
    TaskHandle_t task_handle = (TaskHandle_t)arg;
    StackReady = true;
    StackStarting = false;
    xTaskNotifyGive(task_handle);
}

/**
 * @brief Read of data from TCP connection in a loop.
 *
 * @param apSvr Server info
 * @param apConIdx Connection index
 *
 * @details  If we allow more than single connection per server this needs to be run in separate thread per connection.
 * Currently it runs on the same thread as the server listening loop, thus this fcn prevents accepting any new connection.
 */
static void TcpReadLoop(ServerState_t *apSvr, int apConIdx)
{
    assert(apSvr != NULL);
    assert(apConIdx < MAX_TCP_CONNECTIONS);

    struct timeval timeout = {.tv_usec = TCP_RX_READ_TIMEOUT_US, .tv_sec = 0};
    int err = lwip_setsockopt(apSvr->mConnections[apConIdx], SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (err)
    {
        PRINTF("Failed to set socket receive timeout\r\n");
    }

    // prepare wait structure - once the file is readable, poll will return
    struct pollfd pfd;
    pfd.fd = apSvr->mConnections[apConIdx];
    pfd.events = POLLIN;
    pfd.revents = 0;

    while (1)
    {
        // await data
        int ready = poll(&pfd, 1, TCP_RX_WAIT_TIMEOUT_MS);
        if (ready == -1)
        {
            /** connection closed (error) */
            if (apSvr->mRxCallback)
            {
                apSvr->mRxCallback(-1, NULL, 0);
            }
            apSvr->mConnections[apConIdx] = -1;
            break;
        }
        else if (ready == 0)
        {
            // Timeout reached
            continue;
        }


        // read data from socket
        ssize_t bytes = read(apSvr->mConnections[0], (*apSvr->mRxBuffer)[apConIdx], DATA_BUFF_SIZE);
        if (bytes > 0)
        {
            // call client callback with received data
            if (apSvr->mRxCallback)
            {
                apSvr->mRxCallback(apSvr->mConnections[apConIdx], (const uint8_t *)(*apSvr->mRxBuffer)[apConIdx], (size_t)bytes);
            }
        }
        else if (errno == EWOULDBLOCK)
        {
            // possibility for periodic checks if no data arrive over active connection in MAX_TCP_RX_WAIT_TIMEOUT_MS
            continue;
        }
        else if (bytes < 0)
        {
            /** connection closed (or error) */
            if (apSvr->mRxCallback)
            {
                // notify client connection is closed
                apSvr->mRxCallback(-1, NULL, 0);
            }
            apSvr->mConnections[apConIdx] = -1;
            break;
        }
    }
}

/**
 * @brief Thread function that listens for incoming TCP connections accepts one and reads data from it.
 *
 * @param arg Server info to listen on.
 */
static void TcpListeningThread(void *arg)
{
    ServerState_t *Svr = (ServerState_t*)arg;

    // start listening on the socket, arg of zero -> to use the smallest connection backlog possible (see tcp_backlog_set)
    listen(Svr->mSck, 0);

#if MAX_TCP_CONNECTIONS > 1
    #error "This implementation supports only MAX_TCP_CONNECTIONS = 1"
#endif
    int Next_connection_index = 0;

    while (true)
    {
        Svr->mConnections[Next_connection_index] = accept(Svr->mSck, NULL, 0);
        if (Svr->mConnections[Next_connection_index] < 0)
        {
            // Nothing to accept. Wait 50ms and try it again. In theory we should never get here with blocking wait.
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        else
        {
            PRINTF("TCP connection accepted\r\n");
            // Start reading data from the new connection
            // for more connection new thread needs to be created here
            TcpReadLoop(Svr, Next_connection_index);
        }
    }

    close(Svr->mSck);

    vTaskDelete(NULL);
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/


int TcpStart(void)
{
    // TODO change to mutexwait() if more than one init place
    if (StackReady || StackStarting)
    {
        return -1;
    }
    StackStarting = true;

    memset(gServers, 0, sizeof(gServers));

    ip4_addr_t netif0_ipaddr, netif0_netmask, netif0_gw;
    static struct netif s_netif0;
    ethernetif_config_t enet0_config = {.phyHandle   = &phyHandle,
                                        .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                        .phyOps      = EXAMPLE_PHY_OPS,
                                        .phyResource = EXAMPLE_PHY_RESOURCE,
                                        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
                                        .macAddress = configMAC_ADDR
#endif
    };
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet0_config.macAddress);
#endif

    tcpip_init(tcpip_init_done_evt, xTaskGetCurrentTaskHandle());

    while (!StackReady)
    {
        (void)xTaskNotifyWait(0,0,0, portMAX_DELAY);
    }

    IP4_ADDR(&netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);
    netifapi_netif_add(&s_netif0, &netif0_ipaddr, &netif0_netmask, &netif0_gw, &enet0_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_up(&s_netif0);

#if defined(BOARD_NETWORK_USE_DUAL_ENET)
    ip4_addr_t netif1_ipaddr, netif1_netmask, netif1_gw;
    ethernetif_config_t enet1_config = {.phyHandle   = &phyHandle1,
                                        .phyAddr     = EXAMPLE_PHY1_ADDRESS,
                                        .phyOps      = EXAMPLE_PHY1_OPS,
                                        .phyResource = EXAMPLE_PHY1_RESOURCE,
                                        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR1
                                        .macAddress = configMAC_ADDR1
#endif
    };
    static struct netif s_netif1;
#ifndef configMAC_ADDR1
    (void)SILICONID_ConvertToMacAddr(&enet1_config.macAddress);
#endif
    IP4_ADDR(&netif1_ipaddr, configIP1_ADDR0, configIP1_ADDR1, configIP1_ADDR2, configIP1_ADDR3);
    IP4_ADDR(&netif1_netmask, configNET1_MASK0, configNET1_MASK1, configNET1_MASK2, configNET1_MASK3);
    IP4_ADDR(&netif1_gw, configGW1_ADDR0, configGW1_ADDR1, configGW1_ADDR2, configGW1_ADDR3);
    netifapi_netif_add(&s_netif1, &netif1_ipaddr, &netif1_netmask, &netif1_gw, &enet1_config, EXAMPLE_NETIF1_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_up(&s_netif1);
#else
    /*
     * Single netif is used, set is as default to avoid
     * the need to append zone indices to link-local IPv6 addresses.
     */
    netifapi_netif_set_default(&s_netif0);
#endif /* defined(BOARD_NETWORK_USE_DUAL_ENET) */

#if defined(LWIP_IPV6) && (LWIP_IPV6 == 1)
    LOCK_TCPIP_CORE();
    netif_create_ip6_linklocal_address(&s_netif0, 1);
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
    netif_create_ip6_linklocal_address(&s_netif1, 1);
#endif
    UNLOCK_TCPIP_CORE();
#endif

    struct netif *netif_array[BOARD_PHY_COUNT];
    netif_array[0] = &s_netif0;
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
    netif_array[1] = &s_netif1;
#endif

    while (ethernetif_wait_linkup_array(netif_array, BOARD_PHY_COUNT, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
        return -1;
    }

    return 0;
}

int TcpServerStart(int aPort, RxCallback_t *apRxCallback)
{
    int ret;

    ServerState_t *ServerInfo = NULL;
    for (int i = 0; i < ARRAY_SIZE(gServers); i++)
    {
        if (gServers[i].active == false)
        {
            ServerInfo = &gServers[i];
            ServerInfo->mRxBuffer = &(gTcpRxBuf[i]);
            break;
        }
    }
    if (ServerInfo == NULL)
    {
        return -3;
    }
    ServerInfo->active = true;
    ServerInfo->mRxCallback = apRxCallback;

    int af = AF_INET;
    int sck_type = SOCK_STREAM;

    ServerInfo->mSck = socket(af, sck_type, 0);
    if (ServerInfo->mSck)
    {
        return -1;
    }

    struct sockaddr_in ipv4;
    memset(&ipv4, 0, sizeof(struct sockaddr_in));
    ipv4.sin_len = sizeof(struct sockaddr_in);
    ipv4.sin_family = AF_INET;
    ipv4.sin_port = htons(aPort);
    IP4_ADDR((ip4_addr_t *)&(ipv4.sin_addr.s_addr), configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);

    ret = bind(ServerInfo->mSck, (struct sockaddr *)&ipv4, sizeof(ipv4));
    if (ret < 0)
    {
        close(ServerInfo->mSck);
        ServerInfo->mSck = -2;
        ServerInfo->active = false;
        ServerInfo->mRxCallback = NULL;
    }
    else
    {
        sys_thread_t thread = sys_thread_new("TcpListeningThread", TcpListeningThread, (void *)ServerInfo, 5120, DEFAULT_THREAD_PRIO);

        if (thread == NULL)
        {
            close(ServerInfo->mSck);
            ServerInfo->mSck = -2;
            ServerInfo->active = false;
            ServerInfo->mRxCallback = NULL;
        }
    }

    return ServerInfo->mSck;
}