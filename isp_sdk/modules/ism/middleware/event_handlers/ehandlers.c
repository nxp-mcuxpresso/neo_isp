/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "ctx_mngr.h"
#include "ehandlers.h"
#include "isp.h"
#include "imp_definition.h"
#include "qadapter.h"
#include "evtbroker.h"
#include "comm.h"
#include "version.h"
#include "timestamps.h"

#if defined(SDK_OS_FREE_RTOS)
    static uint8_t gRequestQueueData[MAX_CAMERA_CHANNELS * MAX_REQUESTS_PER_CHANNEL * sizeof(struct request)];
    static StaticQueue_t gRequestQueue;
    static uint8_t gEdmaQueueData[DMA_REQUEST_QUEUE_SIZE * sizeof(struct edma_request)];
    static StaticQueue_t gEdmaQueue;
#endif

/*
 * Local type definition
 */
struct event_handler
{
    /**
     * Internal queue for requests.
     */
    QueueHandle_t delayed_request_queue;

    /**
     * Internal queue for DMA requests.
     */
    QueueHandle_t dma_request_queue;

    /**
     * Response Handler.
     */
    response_function responseHandler;
};

/*
 * Private function declarations
 */
/**
 * @brief Processes a received request, is possible.
 *
 * @details A received request to process image is handled. If there is no room
 * to process the request immediately, it is put into a queue for future
 * processing.
 */
static void processRequest(void);

/**
 * @brief Generic function for processing context left behind.
 *
 * @details The firmware manages two contexts, however, only one can
 * be processed at time fully. In such a case the second context is
 * partially prepared. This function is invoked at multiple places
 * to "un-block" such situations.
 */
static void handleNextStep(void);

/**
 * @brief Handles event when registers configuration context part has been copied to ISP.
 *
 * @param[in] e Event information.
 */
static void onDdrToRegistersTransferDone(const struct event * e);

/**
 * @brief Handles event when memories configuration context part has been copied to ISP.
 *
 * @param[in] e Event information.
 */
static void onDdrToMemoriesTransferDone(const struct event * e);

/**
 * @brief Handles event when registers data has been transfered into ISP.
 *
 * @param[in] e Event information.
 */
static void onRegistersToDdrTransferDone(const struct event * e);

/**
 * @brief Handles event when registers and memories were transferred from ISP to DDR.
 *
 * @param[in] e Event information.
 */
static void onMemoriesToDdrTransferDone(const struct event * e);


/*
 * Global module variables
 */
static struct event_handler g_event_handler_object;

/*
 * Private function definitions
 */


/*
 * Public function definitions
 */
int EH_init(uint8_t clients_count, response_function responseHandler)
{
    int retval;

#if defined(SDK_OS_FREE_RTOS)
    retval = Q_create(&g_event_handler_object.delayed_request_queue, clients_count * MAX_REQUESTS_PER_CHANNEL, sizeof(struct request), gRequestQueueData, &gRequestQueue);
#else
    retval = Q_create(&g_event_handler_object.delayed_request_queue, clients_count * MAX_REQUESTS_PER_CHANNEL, sizeof(struct request));
#endif
    if (0 != retval)
    {
        goto end;
    }
#if defined(SDK_OS_FREE_RTOS)
    retval = Q_create(&g_event_handler_object.dma_request_queue, DMA_REQUEST_QUEUE_SIZE, sizeof(struct edma_request), gEdmaQueueData, &gEdmaQueue);
#else
    retval = Q_create(&g_event_handler_object.dma_request_queue, DMA_REQUEST_QUEUE_SIZE, sizeof(struct edma_request));
#endif
    if (0 != retval)
    {
        /* Free the first created queue and continue, the retval will contain
        the error if queue was NULL! We don't care about it, because in such
        case there was nothing to be destroyed. */
        (void) Q_destroy(&g_event_handler_object.delayed_request_queue);
    }

    g_event_handler_object.responseHandler = responseHandler;

end:
    return retval;
}

int EH_queueRequestSyncedFromISR(const void * req)
{
    return Q_sendSyncedFromISR(g_event_handler_object.delayed_request_queue, req);
}

int EH_queueRequestSynced(const void * req)
{
    return Q_sendSynced(g_event_handler_object.delayed_request_queue, req, NULL);
}

int EH_deinit(void)
{
    /* We silently ignore the result, if we try to destroy a NULL array. */
    (void) Q_destroy(&g_event_handler_object.delayed_request_queue);
    (void) Q_destroy(&g_event_handler_object.dma_request_queue);
    return 0;
}

/*
 * Public function definitions
 */
void onStartStreamingRequest(const struct event * e)
{
    uint32_t channel = e->data0;
    /* is channel correct? */
    if ((1U > channel) || (8U < channel))
    {
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, IMP_START_STREAMING_INVALID_STREAM, channel, IMP_START_STREAMING};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }
    else if ((CM_getStreamingChannel() == channel) && (((uint32_t) STREAMING_MODE) == CM_getIspMode()))
    {
        /* channel is same */
        /* report that streaming is already running on that channel */
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, IMP_START_STREAMING_ALREADY_STEAMING, channel, IMP_START_STREAMING};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }
    else
    {
        /* channel differs, block incomming messages for all channels +
        block requests to switch to m2m/stream mode */
        CM_startStreaming(channel);

        /* flush the request queue */
        (void) Q_resetSynced(g_event_handler_object.delayed_request_queue);
        /* deinit NEXT request */
        if (UNINITIALIZED < CM_getContextState(NEXT))
        {
            CM_deinitContext(NEXT);
            CM_resetRequestCounts();
        }

        /* finish active request if there is any */
        if (UNINITIALIZED < CM_getContextState(ACTIVE))
        {
            CM_enableStreaming();
        }
        else
        {
            CM_disableStreaming();
            CM_modeSwitchDone();
            uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, IMP_START_STREAMING_OK, CM_getStreamingChannel(), IMP_START_STREAMING};
            (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
        }
    }
}

void onStopStreamingRequest(const struct event * e)
{
    (void) e;

    if (((uint32_t) MEMORY_TO_MEMORY_MODE) == CM_getIspMode())
    {
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, 0U, IMP_STOP_STREAMING_NOTHING_TO_STOP, IMP_STOP_STREAMING};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }
    else
    {
        /* We disable the streaming mode, this should stop ISP processing next requests,
        we set a flag, so communication is blocked and doesn't receive new requests or
        triggers on streaming mode enable/disable.
        At this point we reset the ISP to make sure it's in a healthy state - no error
        flags raised etc., however, there might be unfinished contexts, so we deinitialize
        them as well. As there might be any waiting requests in the queue, we must reset the
        queue as well. At that point we can switch the mode and report to host, that
        we have switched the mode.
        */

        CM_disableStreaming();
        CM_stopStreaming();

        /* deinit NEXT request */
        CM_resetISP();
        CM_deinitContext(NEXT);
        CM_deinitContext(ACTIVE);
        CM_resetRequestCounts();
        (void) Q_resetSynced(g_event_handler_object.delayed_request_queue);

        CM_modeSwitchDone();
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, 0U, IMP_STOP_STREAMING_STOPPED, IMP_STOP_STREAMING};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }
}

void onProcessingRequest(const struct event * e)
{
    DEBUG_PRINT("onProcessingRequest entered\n");
    uint32_t channel = e->data2 & 0x0000000FU;

    if (channel < MUP_CHANNEL_COUNT)
    {
        if ((STREAMING_MODE == CM_getIspMode()) && (channel != CM_getStreamingChannel()))
        {
            uint32_t data[MU_PACKET_BYTE_SIZE] = {IMP_FRAME_DONE_ERROR_STREAM, e->data1, e->data0, IMP_FRAME_DONE};
            (void) g_event_handler_object.responseHandler(channel, (const char *) data, sizeof(data));
            CM_decreaseRequests(channel);
        }
        else
        {
            processRequest();
        }
    }
    else
    {
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, IMP_SYSTEM_OUT_OF_BOUNDS, IMP_ERROR_SYSTEM, IMP_ERROR};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }

}

static void onDdrToRegistersTransferDone(const struct event * e)
{
    (void) e;

    uint32_t tmp_state = CM_getContextState(NEXT);
    if ((INITIALIZED == tmp_state) || ((INITIALIZED | REGS_PRELOADED) == tmp_state))
    {
        CM_dmaDone(0);
        /* Registers have been preloaded. Update memories if possible. */
        if (true == CM_isIspMemoriesAvailable())
        {
            uint8_t channel = CM_getContextChannel(NEXT);
            struct edma_request dma_req =
                {
                    .req_id  = EDMA_DDR_TO_ISP_MEMORY_TRANSFER,
                    .channel = channel,
                };

            (void) TS_elapsedTime(CM_getContextFrameIndex(NEXT), TRIGGER_DDR_TO_MEMS);
            if (-1 == CM_startDmaTransfer(dma_req.req_id, dma_req.channel))
            {
                if (0 != Q_sendSynced(g_event_handler_object.dma_request_queue, (void*)&dma_req, NULL))
                {
                    uint32_t data[] = {0U, IMP_SYSTEM_EDMA_QUEUE_FULL, IMP_ERROR_SYSTEM, IMP_ERROR};
                    (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
                }
            }
        }
    }
}

static void onDdrToMemoriesTransferDone(const struct event * e)
{
    (void) e;
    /* If an error happened during DMA transfer, we receive a DMA error and
    handle it there. This state expects that the transfer finished properly. */
    if ((INITIALIZED | REGS_PRELOADED) == CM_getContextState(NEXT))
    {
        CM_dmaDone(1U);
        /* We were allowed to write memories, so lets try to trigger ISP to
        process the image. */
        DEBUG_PRINT("Trigerring ISP...\n");
        CM_triggerISP();
    }
}

void onIspPipelineStarted(const struct event * e)
{
    (void) e;
    /* As NEXT request became ACTIVE, we can start preparing NEXT request. The
    function tries to prepare next request, if available. */
    (void) TS_elapsedTime(CM_getContextFrameIndex(NEXT), TRIGGER_ISP);
    (void) TS_setFrameNumber(CM_getContextFrameIndex(NEXT), CM_getFrameNumber());
    /* We can set next request only in case the registers and memories were preloaded. */
    uint32_t expected_state = INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED;
    uint32_t mode = CM_getIspMode();
    if (STREAMING_MODE == mode)
    {
        if (UNINITIALIZED != CM_getContextState(ACTIVE))
        {
            /* The ACTIVE request should be already deinitialized prior ISP trigger,
            but it's not. We deinitialize it here, stop streaming mode and report it
            back.
            This might happen if the ISP FW is not able to R/W memories after ISP
            FRAME_DONE. */
            uint32_t data[MU_PACKET_BYTE_SIZE];
            uint64_t address    = CM_getContextAddress(ACTIVE);
            uint32_t channel    = (uint32_t) CM_getContextChannel(ACTIVE);
            uint32_t frame_index = CM_getContextFrameIndex(ACTIVE);
            uint32_t errors     = CM_getErrors(ACTIVE);
            data[0U] = IMP_FRAME_DONE_ERROR_ERTRIG | errors;
            data[1U] = ((uint32_t)(frame_index << 24U)) | GET_ADDRESS_HI(address);
            data[2U] = GET_ADDRESS_LO(address);
            data[3U] = IMP_FRAME_DONE;
            (void) g_event_handler_object.responseHandler(channel, (const char *) data, sizeof(data));
            CM_deinitContext(ACTIVE);
            onStopStreamingRequest(NULL);
        }
    }

    if (expected_state == CM_getContextState(NEXT))
    {
        CM_setActiveRequestFromISR();
    }
    /* The pipeline has started, so if context has been reused in streaming
    mode, let's reset the reuse. Reusing config blocks writing next configuration
    in situations, where the new configuration has arrived after frame done
    delivery to host side. */
    if (STREAMING_MODE == CM_getIspMode())
    {
        CM_resetContextReuse();
    }

    /* Try to process next request. */
    handleNextStep();
}

void onStatisticsDone(const struct event * e)
{
    (void) e;

    (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), ISP_P1_STATS_DONE);

    /* If there was no error, the active request has not been deinitialized and
    the state should be at least processing. */
    uint32_t expected_state = INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING;
    if (expected_state == (expected_state & CM_getContextState(ACTIVE)))
    {
        CM_statisticsDone();
        /* Request transfering memories into DDR. */
        uint8_t channel = CM_getContextChannel(ACTIVE);
        struct edma_request dma_req =
        {
            .req_id  = EDMA_ISP_TO_DDR_MEMORY_TRANSFER,
            .channel = channel,
        };

        (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), TRIGGER_MEMS_TO_DDR);
        if (-1 == CM_startDmaTransfer(dma_req.req_id, dma_req.channel))
        {
            if (0 != Q_sendSynced(g_event_handler_object.dma_request_queue, (void*)&dma_req, NULL))
            {
                uint32_t data[] = {0U, IMP_SYSTEM_EDMA_QUEUE_FULL, IMP_ERROR_SYSTEM, IMP_ERROR};
                (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
            }
        }
    }
    else
    {
        handleNextStep();
    }
}

void onFrameDone(const struct event * e)
{
    (void) e;
    /* We don't request ALIAS_REGS->DDR transfer here. This happens after the
    MEMS->DDR has finished in onMemoriesToDdrTransferDone function. */
    /* If there was an error during processing, the ACTIVE context will be
    deinitialized and thus we can handle next request. */

    /* Multiple IRQs might be scheduled if we interrupt the FW (e.g. with a debugger,
    or in ISIM, where IRQs are not scheduled as on real HW).
    Those might be invoked in different order than expected which will lead to
    different state than expected.
    Although it's not expected here in normal situation the state to indicate
    everything has been finished and we can confirm image processing, we will
    check it just in case. */

    (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), FRAME_DONE);

    uint32_t expected_state = INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING | STATISTICS_DONE | MEMS_READ;
    uint32_t active_state = CM_getContextState(ACTIVE);
#if defined (FREE_RTOS)
    /* TODO keep it like this until we design time measurement on LKM and other platforms */
    OS_incrementIspProcessingTime();
#endif

    if (expected_state == (expected_state & active_state))
    {
        uint64_t address = CM_getContextAddress(ACTIVE);
        uint32_t data[MU_PACKET_BYTE_SIZE];
        uint32_t frame_index = CM_getContextFrameIndex(ACTIVE);
        data[0U] = CM_getErrors(ACTIVE);
        data[1U] = ((uint32_t)(frame_index << 24U)) | GET_ADDRESS_HI(address);
        data[2U] = GET_ADDRESS_LO(address);
        data[3U] = IMP_FRAME_DONE;
        (void) TS_elapsedTime(frame_index, MU_SEND_FRAME_DONE);

        uint32_t state = CM_getContextState(NEXT);
        uint32_t mode  = CM_getIspMode();
        if ((STREAMING_MODE == mode) && (UNINITIALIZED == state))
        {
            /* We are in streaming mode, but there is no new configuration for ISP, so we must
            reuse the old one, indicate this state to the host side. */
            CM_reuseContext();
            data[0] |= IMP_FRAME_DONE_ERROR_REUSED;
        }
        else
        {
            CM_deinitContext(ACTIVE);
        }
        DEBUG_PRINT("Sending message frame done!!!\n");
        uint32_t channel = (uint32_t) CM_getContextChannel(ACTIVE);
        (void) g_event_handler_object.responseHandler(channel, (const char *) data, sizeof(data));
        handleNextStep();
    }
    else if ((INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING) == ((INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING) & active_state))
    {
        CM_ispFinished();
    }
    else
    {
        handleNextStep();
    }
}

static void onMemoriesToDdrTransferDone(const struct event * e)
{
    (void) e;
    uint32_t expected_state = INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING | STATISTICS_DONE | PROCESSED;
    uint32_t active_state = CM_getContextState(ACTIVE);
#if !defined (ISIM)
    if (expected_state == (expected_state & active_state) && (true == CM_dmaTransferUsesDescriptors()))
#else
    if (expected_state == (expected_state & active_state))
#endif
    {
        uint64_t address = CM_getContextAddress(ACTIVE);
        uint32_t channel = CM_getContextChannel(ACTIVE);
        plt_assert((0 != channel) && (channel < MUP_CHANNEL_COUNT));
        uint32_t data[MU_PACKET_BYTE_SIZE];
        uint32_t errors = CM_getErrors(ACTIVE);
        data[0U] = errors;
        data[1U] = ((uint32_t) (CM_getContextFrameIndex(ACTIVE) << 24U)) | GET_ADDRESS_HI(address);
        data[2U] = GET_ADDRESS_LO(address);
        data[3U] = IMP_FRAME_DONE;
        uint32_t frame_index = CM_getContextFrameIndex(ACTIVE);
        (void) TS_elapsedTime(frame_index, MU_SEND_FRAME_DONE);

        uint32_t state = CM_getContextState(NEXT);
        uint32_t mode  = CM_getIspMode();
        if ((STREAMING_MODE == mode) && (UNINITIALIZED == state))
        {
            /* We are in streaming mode, but there is no new configuration for ISP, so we must
            reuse the old one, indicate this state to the host side. */
            CM_reuseContext();
            data[0] |= IMP_FRAME_DONE_ERROR_REUSED;
        }
        else
        {
            CM_deinitContext(ACTIVE);
        }
        DEBUG_PRINT("Sending message frame done!!!\n");
        (void) g_event_handler_object.responseHandler(channel, (const char *) data, sizeof(data));
        handleNextStep();
    }
    else if ((INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING | STATISTICS_DONE) < active_state)
    {
        CM_dmaDone(3U);
        uint8_t channel = CM_getContextChannel(ACTIVE);
        struct edma_request dma_req =
        {
            .req_id  = EDMA_ISP_TO_DDR_REGISTER_ALIAS_TRANSFER,
            .channel = channel,
        };

        (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), TRIGGER_REGS_TO_DDR);
        if (-1 == CM_startDmaTransfer(dma_req.req_id, dma_req.channel))
        {
            if (0 != Q_sendSynced(g_event_handler_object.dma_request_queue, (void*)&dma_req, NULL))
            {
                uint32_t data[] = {0U, IMP_SYSTEM_EDMA_QUEUE_FULL, IMP_ERROR_SYSTEM, IMP_ERROR};
                (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
            }
        }
    }
    else
    {
        handleNextStep();
    }
}

static void onRegistersToDdrTransferDone(const struct event * e)
{
    (void) e;
    uint32_t state = CM_getContextState(ACTIVE);
    if ((INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED | PROCESSING | STATISTICS_DONE | PROCESSED | MEMS_READ) == state)
    {
        uint32_t data[4U];
        uint32_t errors          = CM_getErrors(ACTIVE);
        uint32_t frame_index     = CM_getContextFrameIndex(ACTIVE);
        uint64_t context_address = CM_getContextAddress(ACTIVE);
        uint32_t channel         = (uint32_t) CM_getContextChannel(ACTIVE);
        plt_assert((0 != channel) && (channel < MUP_CHANNEL_COUNT));
        data[0U] = errors;
        data[1U] = ((uint32_t) (frame_index << 24U)) | GET_ADDRESS_HI(context_address);
        data[2U] = GET_ADDRESS_LO(context_address);
        data[3U] = IMP_FRAME_DONE;
        DEBUG_PRINT("Sending message frame done!!!\n");
        (void) g_event_handler_object.responseHandler(channel, (const char *) data, sizeof(data));
        (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), MU_SEND_FRAME_DONE);
        CM_deinitContext(ACTIVE);
    }

    handleNextStep();
}

void onDmaError(const struct event * e)
{
    /* We have received an error during DMA transfer. Report it back. */
    plt_assert(e != NULL);
    plt_assert(e->data0 <= ACTIVE);
    enum requestId req       = (enum requestId) e->data0;
    uint64_t context_address = CM_getContextAddress(req);
    uint32_t frame_index     = CM_getContextFrameIndex(req);
    uint32_t channel         = (uint32_t) CM_getContextChannel(ACTIVE);
    uint32_t data[MU_PACKET_BYTE_SIZE];
    data[0U] = CM_getErrors(req);
    data[1U] = ((uint32_t) (frame_index << 24U)) | GET_ADDRESS_HI(context_address);
    data[2U] = GET_ADDRESS_LO(context_address);
    data[3U] = IMP_FRAME_DONE;
    (void) g_event_handler_object.responseHandler(channel, (const char *) data, sizeof(data));
    (void) TS_elapsedTime(CM_getContextFrameIndex(req), MU_SEND_FRAME_DONE);
    CM_deinitContext(req);

    /* TODO reinitialize corresponding DMA */

    handleNextStep();
}

void onGetSwVersion(const struct event * e)
{
    (void) e;
    uint32_t data[MU_PACKET_BYTE_SIZE] = {PATCH_ID, MINOR_ID, MAJOR_ID, IMP_GET_SW_VERSION};
    (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
}

void onReboot(const struct event * e)
{
    (void) e;
    /* TODO due to the change to support multiple systems, we need to move this
    to a system specific file?? */
    // NVIC_SystemReset();
}

void onGetIdlePercentage(const struct event * e)
{
    #if defined (FREE_RTOS)
    /* TODO keep it like this until we design time measurement on LKM and other platforms */
    uint32_t idle_percentage = OS_getIspIdlePercentage();
    uint32_t cpu_usage = OS_getTaskCpuUsage(IDLE_TASK_IDX);
    uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, idle_percentage, cpu_usage, IMP_GET_IDLE_PERCENTAGE};
    #else
    uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, 0U, 0U, IMP_GET_IDLE_PERCENTAGE};
    #endif
    (void) g_event_handler_object.responseHandler(e->data2, (const char *) data, sizeof(data));
}

void onDmaDone(const struct event * ev)
{
    struct edma_request req = {EDMA_DDR_TO_ISP_REGISTERS_TRANSFER};
    /* If there is something in the DMA delayed request queue, process it first. */

    int retval = Q_receive(g_event_handler_object.dma_request_queue, (void*)&req, NULL);
    if (0 == retval)
    {
        /* This is the only place where we trigger DMA! As this event is received
        when DMA has finished, we can be sure the DMA won't be busy! */
        (void) CM_startDmaTransfer(req.req_id, req.channel);
    }

    plt_assert(ev->data0 < EI_EVENT_ID_MAX);

    /* Now let's handle what caused the DMA done event. */
    if (EI_DDR_TO_REGISTERS_TRANSFER_DONE == (enum event_id) ev->data0)
    {
        (void) TS_elapsedTime(CM_getContextFrameIndex(NEXT), DDR_TO_REGS_DONE);
        onDdrToRegistersTransferDone(ev);
    }
    else if (EI_DDR_TO_MEMORIES_TRANSFER_DONE == (enum event_id) ev->data0)
    {
        (void) TS_elapsedTime(CM_getContextFrameIndex(NEXT), DDR_TO_MEMS_DONE);
        onDdrToMemoriesTransferDone(ev);
    }
    else if (EI_REGISTERS_TO_DDR_TRANSFER_DONE == (enum event_id) ev->data0)
    {
        (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), REGS_TO_DDR_DONE);
        onRegistersToDdrTransferDone(ev);
    }
    else
    {
        (void) TS_elapsedTime(CM_getContextFrameIndex(ACTIVE), MEMS_TO_DDR_DONE);
        onMemoriesToDdrTransferDone(ev);
    }
}

/*
 * Private function definitions
 */
static void processRequest(void)
{
    static uint8_t frame_index = 0U;
    DEBUG_PRINT("processRequest entered\n");
    /* A new request has been received over MU. Process it if possible.
    1. check that a next request can be scheduled.
    2. get new request from queue
    3. start new TS measurement and update frame_index
    4. set new request as next
    5. request DMA to transfer data into registers
    */
    plt_assert(frame_index < TS_getEntriesCnt());
    if ((MEMORY_TO_MEMORY_MODE == CM_getIspMode()) || ((STREAMING_MODE == CM_getIspMode()) && (false == CM_isContextReused())))
    {
        DEBUG_PRINT("ISP is in M2M or streaming!\n");
        if (UNINITIALIZED == CM_getContextState(NEXT))
        {
            DEBUG_PRINT("retreiving data from queue...\n");
            struct request req = {0};
            if (0 != Q_receive(g_event_handler_object.delayed_request_queue, (void *) &req, NULL))
            {
                DEBUG_PRINT("No new request in queue.\n");
                return;
            }
            // start new TS measurement on current frame_index
            req.frame_index = frame_index;
            TS_resetMeasurement(frame_index);
            TS_startElapsedMeasurement(frame_index, req.channel, req.start_time);
            frame_index++;
            frame_index %= TS_getEntriesCnt();
            DEBUG_PRINT("Setting next request...\n");
            if (true == CM_setNextRequest(&req))
            {
                DEBUG_PRINT("Next request set! Triggering DMA...\n");
                uint8_t channel = CM_getContextChannel(NEXT);
                struct edma_request dma_req =
                {
                    .req_id  = EDMA_DDR_TO_ISP_REGISTERS_TRANSFER,
                    .channel = channel,
                };

                /* We were able to schedule next request, request ISP registers preload. */
                (void) TS_elapsedTime(CM_getContextFrameIndex(NEXT), TRIGGER_DDR_TO_REGS);
                if (-1 == CM_startDmaTransfer(dma_req.req_id, dma_req.channel))
                {
                    DEBUG_PRINT("Triggering DMA failed!\n");
                    /* TODO this Q_sendSynced might be replaced with a non-synced version??? */
                    if (0 != Q_sendSynced(g_event_handler_object.dma_request_queue, (void *) &dma_req, NULL))
                    {
                        uint32_t data[4U] = {0U, IMP_SYSTEM_EDMA_QUEUE_FULL, IMP_ERROR_SYSTEM, IMP_ERROR};
                        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
                    }
                }
            }
        }
    }
    DEBUG_PRINT("Process request exit\n");
}

static void handleNextStep(void)
{
    /* this seems to be a good point to handle either next request or switch mode */
    if ((uint32_t) TRANSITION_TO_STREAMING == CM_getIspMode())
    {
        CM_modeSwitchDone();
        CM_enableStreaming();
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, IMP_START_STREAMING_OK, CM_getStreamingChannel(), IMP_START_STREAMING};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }
    else if ((uint32_t) TRANSITION_TO_MEMORY_TO_MEMORY == CM_getIspMode())
    {
        CM_modeSwitchDone();
        uint32_t data[MU_PACKET_BYTE_SIZE] = {0U, 0U, IMP_STOP_STREAMING_STOPPED, (uint32_t) IMP_STOP_STREAMING};
        (void) g_event_handler_object.responseHandler(MASTER_ENTITY_CHANNEL, (const char *) data, sizeof(data));
    }
    else
    {
        uint32_t state = CM_getContextState(NEXT);

        if ((INITIALIZED | REGS_PRELOADED | MEMS_PRELOADED) == state)
        {
            CM_triggerISP();
        }
        else if ((INITIALIZED | REGS_PRELOADED) == state)
        {
            onDdrToRegistersTransferDone(NULL);
        }
        else
        {
            processRequest();
        }
    }
}
