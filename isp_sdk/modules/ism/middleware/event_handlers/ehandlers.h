/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EVENT_HANDLERS_GUARD
#define EVENT_HANDLERS_GUARD

#include "evtbroker.h"
#include "ismanager.h"

enum event_id
{
    RESERVED                            = 0x0U,
    EI_SOFTWARE_RESET                   = 0x1U,
    EI_GET_IDLE_PERCENTAGE              = 0x2U,
    EI_GET_SW_VERSION                   = 0x3U,
    EI_RESET_STREAM                     = 0x4U,
    EI_START_STREAMING                  = 0x5U,
    EI_STOP_STREAMING                   = 0x6U,
    EI_READ_ADDRESS_LOCATION            = 0x7U,
    EI_WRITE_ADDRESS_LOCATION           = 0x8U,
    EI_DMA_DONE                         = 0x9U,
    EI_PROCESSING_REQUEST               = 0xBU,
    EI_DDR_TO_REGISTERS_TRANSFER_DONE   = 0xCU,
    EI_DDR_TO_MEMORIES_TRANSFER_DONE    = 0xDU,
    EI_REGISTERS_TO_DDR_TRANSFER_DONE   = 0xEU,
    EI_MEMORIES_TO_DDR_TRANSFER_DONE    = 0xFU,
    EI_DMA_ERROR                        = 0x10U,
    EI_ISP_PIPELINE_STARTED             = 0x11U,
    EI_ISP_STATISTICS_DONE              = 0x12U,
    EI_ISP_FRAME_DONE                   = 0x13U,
    EI_EVENT_ID_MAX                     = 0x14U,
};


/**
 *
 */
int EH_init(uint8_t clients_count, response_function responseHandler);

/**
 *
 */
int EH_deinit(void);

/**
 * @brief Handles when a request to start streaming mode has been received.
 *
 * @param[in] e
 */
void onStartStreamingRequest(const struct event * e);

/**
 * @brief Handles when a request to stop streaming mode has been received.
 *
 * @param[in] e
 */
void onStopStreamingRequest(const struct event * e);

/**
 * @brief Handles a request to process next image.
 *
 * @param[in] e
 */
void onProcessingRequest(const struct event * e);

/**
 * @brief Handles event when ISP has started processing.
 *
 * @param[in] e Event information.
 */
void onIspPipelineStarted(const struct event * e);

/**
 * @brief Handles event when ISP generated ISP trigger.
 *
 * @param[in] e Event information.
 */
void onStatisticsDone(const struct event * e);

/**
 * @brief Handlers event when ISP has finished image processing.
 *
 * @param[in] e Event information.
 */
void onFrameDone(const struct event * e);

/**
 * @brief Handles events related DMA errors.
 *
 * @param[in] e Event information.
 */
void onDmaError(const struct event * e);

/**
 * @brief Not implemented!!!
 *
 * @param[in] e Event information.
 */
void onReboot(const struct event * e);

/**
 * @brief Handles request to get the SW version.
 *
 * @param[in] e Event information.
 */
void onGetSwVersion(const struct event * e);

/**
 * @brief Not implemented/used right now!!!
 */
void onDmaDone(const struct event * ev);

/**
 * @brief Returns the idle percentage.
 *
 * @note Implemented only for FreeRTOS port.
 */
void onGetIdlePercentage(const struct event * e);

/**
 * @brief Puts a request to process image into a queue.
 *
 * @param[in] req Processing request.
 */
int EH_queueRequestSyncedFromISR(const void * req);

/**
 * @brief Puts a request to process image into a queue.
 *
 * @param[in] req Processing request.
 */
int EH_queueRequestSynced(const void * req);

#endif /* EVENT_HANDLERS_GUARD */
