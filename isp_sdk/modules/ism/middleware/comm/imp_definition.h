/*
 * Copyright 2022, 2024, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file imp_definition.h
 * @version 0.1
 * @brief This file defines the protocol IDs and error flags of error messages.
 */

#ifndef IMP_DEFINITION_GUARD
#define IMP_DEFINITION_GUARD

/* Master messages IDs */
/**
 * @defgroup isp_manager_protocol Protocol definition
 * @{
 */

/**
 * @defgroup mup_channel_ids MUP channel IDs
 * @{
 */
#define MASTER_ENTITY_CHANNEL       (0U)
#define CLIENT_CHANNEL0             (1U)
#define CLIENT_CHANNEL1             (2U)
#define CLIENT_CHANNEL2             (3U)
#define CLIENT_CHANNEL3             (4U)
#define CLIENT_CHANNEL4             (5U)
#define CLIENT_CHANNEL5             (6U)
#define CLIENT_CHANNEL6             (7U)
#define CLIENT_CHANNEL7             (8U)
#define MUP_CHANNEL_COUNT           (9U)
/** @} */ /* MU channel IDs*/

/**
 * @name Packet helper identifiers.
 * @{
 */
typedef enum
{
    MU_PACKET_BYTE_DATA2,   /**< Data 2. */
    MU_PACKET_BYTE_DATA1,   /**< Data 1. */
    MU_PACKET_BYTE_DATA0,   /**< Data 0.  */
    MU_PACKET_BYTE_ID,      /**< Message ID. */
    MU_PACKET_BYTE_SIZE,    /**< Number of words in a packet. */
} mu_packet_byte_t;
/** @} */ /* packet description */

/**
 * @name Master channel message IDs
 * @{
 */
/**
 * @{
 */
#define IMP_SOFTWARE_RESET          ((uint32_t) 1U)   /**< Request restarting of the ISP manager software. */
#define IMP_GET_IDLE_PERCENTAGE     ((uint32_t) 2U)   /**< Request the average idle percentage of the ISP manager utilization */
#define IMP_GET_SW_VERSION          ((uint32_t) 3U)   /**< Request the ISP manager software version. */
#define IMP_RESET_STREAM            ((uint32_t) 4U)   /**< Request stream reset. */
#define IMP_START_STREAMING         ((uint32_t) 5U)   /**< Request to switch to streaming mode ID. */
#define IMP_STOP_STREAMING          ((uint32_t) 6U)   /**< Request to stop streaming mode ID. */
#define IMP_READ_ADDRESS_LOCATION   ((uint32_t) 7U)   /**< Request content from address A. */
#define IMP_WRITE_ADDRESS_LOCATION  ((uint32_t) 8U)   /**< Request writing data into address A. */
/** @} */

/**
 * @brief Messages from ISP manager to host.
 */
#define IMP_ERROR                   (9U)    /**< Error message.*/
#define IMP_READY_AFTER_BOOT        (10U)   /**< ISP manager booting finished. */
/** @} */ /* Master channel message IDs*/

/**
 * @name Client channel message IDs
 * @{
 */
/**
 * @brief Messages from host to ISP manager.
 */
#define IMP_REQUEST_ISP_PROCESSING  (11U)   /**< Request ISP manager to execute ISP processing with config C.*/

/**
 * @brief Messages from ISP manager to host.
 */
#define IMP_FRAME_DONE              (12U)   /**< Image processing done. */
/** @} */ /* Client channel message IDs */

/**
 * @name Streaming mode request status flags
 * @{
 */
#define IMP_START_STREAMING_OK                  (0U)
#define IMP_START_STREAMING_ALREADY_STEAMING    (1U)
#define IMP_START_STREAMING_INVALID_STREAM      (2U)
#define IMP_START_STREAMING_MODE_TRANSITION     (3U)
/** @} */ /* Streaming mode reques status flags */

/**
 * @name Stop streaming mode request status flags
 * @{
 */
#define IMP_STOP_STREAMING_STOPPED              (0U)
#define IMP_STOP_STREAMING_NOTHING_TO_STOP      (1U)
/** @} */ /* Stop streaming mode request status flags */

/**
 * @name Frame done response error flags
 * @{
 */
#define IMP_FRAME_DONE_ERROR_REJECT (0x80000000U)   /**< Reject flag indicating max requests per stream has been reached. */
#define IMP_FRAME_DONE_ERROR_STREAM (0x40000000U)   /**< Stream flag indicating either channel is not in steraming mode or switch to streaming is ongoing. */
#define IMP_FRAME_DONE_ERROR_REUSED (0x20000000U)   /**< Flag indicating reuse of current configuration in streaming mode. */
#define IMP_FRAME_DONE_ERROR_ERTRIG (0x10000000U)   /**< Flag indicating ISP trigger although memories were not read for active request and written for next request. */
/** @} */ /* Frame done response error flags */

/**
 * @name Ready after boot flags
 * @{
 */
#define IMP_SOFT_RESET          (1U)  /**< ISP manager has booted due to soft reset request. */
/** @} */ /* Ready after boot flags */

/**
 * @name Error message identifiers
 * @{
 */
#define IMP_ERROR_BOOT              (0U)    /**< System was not able to initialize required resources. */
#define IMP_ERROR_CMD               (1U)    /**< Invalid message ID received on Master Entity channel. */
#define IMP_ERROR_SYSTEM            (2U)    /**< System errors. These represend unexpected errors which are in most cases unrecoverable. */
/** @} */ /* Error message identifiers */

/**
 * @name Boot error arguments 0
 * @{
 */
#define IMP_CLIENT_RX_QUEUE_CREATION_FAILED                 (1U)    /**< Creation of queue failed, insufficient memory. */
#define IMP_EDMA_QUEUE_CREATION_FAILED                      (2U)    /**< Creation of queue failed, insufficient memory. */
#define IMP_EDMA_AVAILABLE_SEMAPHORE_CREATION_FAILED        (6U)    /**< Creation of semaphore failed, insufficient memory. */
#define IMP_EDMA_MANAGER_TASK_CREATION_FAILED               (9U)    /**< Task creation failed, insufficient memory. */
#define IMP_EVENT_HANDLER_TASK_CREATION_FAILED              (10U)   /**< Task creation failed, unsufficient memory. */
#define IMP_EVENT_BROKER_INIT_FAILED                        (11U)   /**< Event broker initialization failed, insufficient memory.  */
/** @} */ /* Boot */

/**
 * @name System error argument 0
 * @{
 */
#define IMP_SYSTEM_EDMA_AVAILABLE_SEMAPHORE_TIMEOUT         (2U)    /**< DMA transfer exceeded acceptable time. This is considered as fatal error. */
#define IMP_SYSTEM_EDMA_AVAILABLE_SEMAPHORE_NOTIFY_FAILURE  (3U)    /**< Fatal error related to signaling a semaphore. */

#define IMP_SYSTEM_REQUEST_QUEUE_FULL                       (100U)  /**< The camera client request queue is full. */
#define IMP_SYSTEM_EDMA_QUEUE_FULL                          (102U)  /**< The EDMA task queue is full. */
#define IMP_SYSTEM_EVENT_QUEUE_FULL                         (103U)  /**< The event queue is full. */
#define IMP_SYSTEM_MESSAGE_QUEUE_FULL                       (104U)  /**< The pending messages buffer is full. We can't transmit additional messages. */
#define IMP_SYSTEM_OUT_OF_BOUNDS                            (105U)  /**< A channel index used is out of bounds. */
/** @} */ /* System */

/** @} */ /* isp_manager_protocol group */

#endif /* IFP_DEFINITION_GUARD header guard */
