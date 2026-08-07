/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

// #include <stdbool.h>

#include "ismanager.h"
#include "evtbroker.h"
#include "ctx_mngr.h"
#include "ehandlers.h"
#include "imp_definition.h"
#include "timestamps.h"

int ISM_init(
    ISP_TypeDef *       isp,
    EDMA_TypeDef *      dma,
    EDMA_TCD_TypeDef *  edma_descriptors,
    uint8_t *           measurement_buffer,
    size_t              buffer_size,
    response_function   responseHandler
)
{
    int retval = 0;
    if (0 != (retval = EH_init(MAX_CAMERA_CHANNELS, responseHandler)))
    {
        DEBUG_PRINT("EH_init failed!\n");
        retval = -1;
        goto end;
    }

    if (0 != (retval = EB_init(responseHandler)))
    {
        DEBUG_PRINT("EB_init failed!\n");
        retval = -1;
        goto eb_init;
    }

    (void) EB_registerEvent((uint32_t) EI_SOFTWARE_RESET,                  NULL);
    (void) EB_registerEvent((uint32_t) EI_GET_IDLE_PERCENTAGE,             NULL);
    (void) EB_registerEvent((uint32_t) EI_GET_SW_VERSION,                  onGetSwVersion);
    (void) EB_registerEvent((uint32_t) EI_RESET_STREAM,                    NULL);
    (void) EB_registerEvent((uint32_t) EI_START_STREAMING,                 onStartStreamingRequest);
    (void) EB_registerEvent((uint32_t) EI_STOP_STREAMING,                  onStopStreamingRequest);
    (void) EB_registerEvent((uint32_t) EI_READ_ADDRESS_LOCATION,           NULL);
    (void) EB_registerEvent((uint32_t) EI_WRITE_ADDRESS_LOCATION,          NULL);
    (void) EB_registerEvent((uint32_t) EI_DMA_DONE,                        onDmaDone);
    (void) EB_registerEvent((uint32_t) EI_PROCESSING_REQUEST,              onProcessingRequest);
    (void) EB_registerEvent((uint32_t) EI_DMA_ERROR,                       onDmaError);
    (void) EB_registerEvent((uint32_t) EI_ISP_PIPELINE_STARTED,            onIspPipelineStarted);
    (void) EB_registerEvent((uint32_t) EI_ISP_STATISTICS_DONE,             onStatisticsDone);
    (void) EB_registerEvent((uint32_t) EI_ISP_FRAME_DONE,                  onFrameDone);

    if (0 != (retval = CM_init(isp, dma, edma_descriptors)))
    {
        DEBUG_PRINT("CM_init failed!\n");
        retval = -1;
        goto cm_init;
    }

    if (0 == (retval = TS_init(measurement_buffer, buffer_size)))
    {
        goto end;
    }
    DEBUG_PRINT("TS_init failed!\n");
    retval = -1;

    CM_deinit();
cm_init:
    EB_deinit();
eb_init:
    EH_deinit();
end:
    return retval;
}

int ISM_deinit(void)
{
    DEBUG_PRINT("Invoking ISM deinit\n");
    CM_deinit();
    EH_deinit();
    EB_deinit();
    return 0;
}

int ISM_changeISPMode(uint8_t channel, enum isp_mode mode)
{
    int retval      = 0;

    struct event ev = {0};
    ev.data0        = channel;

    if (mode == MEMORY_TO_MEMORY)
    {
        ev.id = IMP_STOP_STREAMING;
    }
    else
    {
        ev.id = IMP_START_STREAMING;
    }

    if (0 != EB_publishEvent(&ev))
    {
        retval = IMP_SYSTEM_EVENT_QUEUE_FULL;
    }

    return retval;
}

int ISM_changeISPModeFromISR(uint8_t channel, enum isp_mode mode)
{
    int retval      = 0;

    struct event ev = {0};
    ev.data0        = channel;

    if (mode == MEMORY_TO_MEMORY)
    {
        ev.id = IMP_STOP_STREAMING;
    }
    else
    {
        ev.id = IMP_START_STREAMING;
    }

    if (0 != EB_publishEventFromISR(&ev))
    {
        retval = IMP_SYSTEM_REQUEST_QUEUE_FULL;
    }

    return retval;
}

uint32_t ISM_requestISPProcessing(uint8_t channel, uint64_t context_address)
{
    uint32_t retval                  = 0;

    if (channel == 0U || channel > MUP_CHANNEL_COUNT)
    {
        retval = IMP_FRAME_DONE_ERROR_REJECT;
    }
    else if (true == CM_isModeTransitonActive())
    {
        /* We ignore any new requests while we are switching modes. */
        retval = IMP_FRAME_DONE_ERROR_STREAM;
    }
    else
    {
        if (true == CM_increaseRequests(channel - 1U))
        {

            struct event ev;
            ev.data2 = channel;
            ev.data1 = (uint32_t) (context_address >> 32U);
            ev.data0 = context_address & UINT32_MAX;
            ev.id    = IMP_REQUEST_ISP_PROCESSING;

            struct request req = {
                channel,
                0U, // frame index to be set in event handler
                context_address,
                xPortGetTime(), // get request start time
            };

            /* TODO We might remove storing the request into queue and postope it up
            to onProcessingRequest, but that would require to rework the logic
            and check, whether we haven't broken the logic elsewhere... */
            if (0 != EH_queueRequestSynced((void *) &req))
            {
                retval = IMP_SYSTEM_REQUEST_QUEUE_FULL;
            }

            if (0 != EB_publishEvent(&ev))
            {
                retval = IMP_SYSTEM_EVENT_QUEUE_FULL;
            }
        }
        else
        {
            /* There are already 2 requests from this client in the queue, report an error
            We keep the context address (byte 0 & 1), but rewrite the ID and the error. */
            retval = IMP_FRAME_DONE_ERROR_REJECT;
        }
    }
    return retval;
}

uint32_t ISM_requestISPProcessingFromISR(uint8_t channel, uint64_t address)
{
    plt_assert(channel < 8U);
    plt_assert(channel > 0);

    uint32_t retval                  = 0;

    if (true == CM_isModeTransitonActive())
    {
        /* We ignore any new requests while we are switching modes. */
        retval = IMP_FRAME_DONE_ERROR_STREAM;
    }
    else
    {
        if (true == CM_increaseRequestsFromISR(channel - 1U))
        {
            struct event ev;
            ev.data2 = channel;
            ev.data1 = (uint32_t) (address >> 32U);
            ev.data0 = (uint32_t) (address & UINT32_MAX);
            ev.id    = IMP_REQUEST_ISP_PROCESSING;

            struct request req = {
                channel,
                0U, // frame index to be set in event handler
                address,
                xPortGetTime(), // get request start time
            };

            /* TODO We might remove storing the request into queue and postope it up
            to onProcessingRequest, but that would require to rework the logic
            and check, whether we haven't broken the logic elsewhere... */
            if (0 != EH_queueRequestSyncedFromISR((void *) &req))
            {
                retval = IMP_SYSTEM_REQUEST_QUEUE_FULL;
            }

            if (0 != EB_publishEventFromISR(&ev))
            {
                retval = IMP_SYSTEM_EVENT_QUEUE_FULL;
            }
        }
        else
        {
            /* There are already 2 requests from this client in the queue, report an error
            We keep the context address (byte 0 & 1), but rewrite the ID and the error. */
            retval = IMP_FRAME_DONE_ERROR_REJECT;
        }
    }

    return retval;
}
