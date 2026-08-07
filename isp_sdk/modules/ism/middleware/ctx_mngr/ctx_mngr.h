/*
 * Copyright 2022-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ISP_MW_GUARD
#define ISP_MW_GUARD

#include "platform.h"

/**
 * @addtogroup drivers
 * @{
 */

struct request
{
    uint8_t  channel;
    uint8_t  frame_index;
    uint64_t context_address;
    uint32_t start_time;
};

/**
 * @defgroup ctx_mngr Context manager (CM)
 * @details The context manager is a driver incorporating the eDMA and ISP
 * functionality to control the ISP peripheral. It manages metadata about
 * the requests being processed to keep track which camera client should be
 * notified and implements a mechanism to track the number of requests
 * per camera client. In case the count of requests exceeds the total allowed
 * count, this mechanism reports this and it's up to the user to react properly.
 * The context manager also initializes the eDMA peripheral for each camera client
 * and provides API to simplify the work with eDMA.
 * The context manager also implements the eDMA and ISP interrupts handlers. The
 * user must register his own IRQ handlers to be able to implement his own logic
 * base on the eDMA/ISP events.
 * @{
 */
/**
 * @defgroup cm_cfg_options Context manager types and configuration options
 * @{
 */
enum requestId
{
    NEXT    = 0U,  /**< @brief Next request scheduled for processing. */
    ACTIVE  = 1U,  /**< @brief Currently processed request. */
};

/**
 * @name Context states
 * @{
 */
#define UNINITIALIZED   ((uint32_t) 0x00U)
#define INITIALIZED     ((uint32_t) 0x01U)
#define REGS_PRELOADED  ((uint32_t) 0x02U)
#define MEMS_PRELOADED  ((uint32_t) 0x04U)
#define PROCESSING      ((uint32_t) 0x08U)
#define STATISTICS_DONE ((uint32_t) 0x10U)
#define PROCESSED       ((uint32_t) 0x20U)
#define MEMS_READ       ((uint32_t) 0x40U)
#define REGS_READ       ((uint32_t) 0x80U)
/** @} */

/**
 * @name ISP modes
 * @{
 */
#define MEMORY_TO_MEMORY_MODE            ((uint32_t) 0U)
#define TRANSITION_TO_STREAMING          ((uint32_t) 1U)
#define STREAMING_MODE                   ((uint32_t) 2U)
#define TRANSITION_TO_MEMORY_TO_MEMORY   ((uint32_t) 3U)
/** @} */

enum edma_channel_event
{
    MAJOR_ITERATION_COMPLETE,       /**< @brief The eDMa transfer has completed. */
    MAJOR_ITERATION_HALF_COMPLETE,  /**< @brief The eDMA has transfered half the data. */
};

/**
 * @details eDMA task request ID's.
 */
enum edma_request_id
{
    EDMA_DDR_TO_ISP_REGISTERS_TRANSFER      = 0U, /**< @brief Request to transfer registers configuration from DDR into ISP. Number corresponds to DMA channel to be used. */
    EDMA_DDR_TO_ISP_MEMORY_TRANSFER         = 1U, /**< @brief Request to transfer memory configuration from DDR into ISP. Number corresponds to DMA channel to be used. */
    EDMA_ISP_TO_DDR_REGISTER_ALIAS_TRANSFER = 2U, /**< @brief Request to transfer aliased registers from ISP into DDR. Number corresponds to the DMA channel to be used. */
    EDMA_ISP_TO_DDR_MEMORY_TRANSFER         = 3U, /**< @brief Request to transfer memory from ISP into DDR. Number corresponds to the DMA channel to be used. */
};

/**
 * @details eDMA request descriptor.
*/
struct edma_request
{
    enum edma_request_id    req_id;         /**< Request ID. */
    uint8_t                 channel;        /**< MU channel. */
};

enum edma_error_event
{
    DESTINATION_BUS_ERROR,
    SOURCE_BUS_ERROR,
    SCATHER_GATHER_CONFIG_ERROR,
    NBYTES_CITER_CONFIG_ERROR,
    DESTINATION_OFFSET_ERROR,
    DESTINATION_ADDRESS_ERROR,
    SOURCE_OFFSET_ERROR,
    SOURCE_ADDRESS_ERROR,
    TRANSFER_CANCELED,
    UNCORRECTABLE_TCD_ERROR
};

enum isp_event
{
    PIPELINE1_FRAME_START       = 0x00U,
    PIPELINE2_FRAME_START       = 0x01U,
    PIPELINE1_FRAME_DONE        = 0x02U,
    PIPELINE2_FRAME_DONE        = 0x03U,
    PIPELINE1_STATISTICS_DONE   = 0x04U,
    PIPELINE2_DRC_DONE          = 0x05U,
    READ_BUS_LINEPATH0_ERR      = 0x06U,
    READ_BUS_LINEPATH1_ERR      = 0x07U,
    WRITE_PORT0_ERR             = 0x08U,
    WRITE_PORT1_ERR             = 0x09U,
    CSI_TIMING_ERROR            = 0x10U,
    TRIGGER_ERROR               = 0x11U,
};
/** @} */ /* cm_cfg_options group */

/**
 * @defgroup irq_handlers Context manager user IRQ handlers
 * @brief IRQ handler prototypes.
 * @details The user must create and register IRQ handler for eDMA and ISP
 * during CM_init() to be able to react on eDMA and ISP events.
 * @{
 */
/**
 * @brief ISP event handler.
 *
 * @param[in] e Event.
 *
 * @note The flag corresponding to this event gets cleared automatically. Don't
 * clear it in the handler.
 * @see isp_event
 */
typedef void (* isp_event_handler)(enum isp_event e);
/**
 * @brief eDMA channel event handler.
 *
 * @param[in] channel eDMA channel where the event happened.
 * @param[in] e Event.
 *
 * @note The flag corresponding to this event gets cleared automatically. Don't
 * clear it in the handler.
 * @see edma_channel_event
 */
typedef void (* edma_channel_event_handler)(uint32_t channel, enum edma_channel_event e);
/**
 * @brief eDMA error event handler.
 *
 * @param[in] channel eDMA channel where the error happened.
 * @param[in] e Event.
 *
 * @note The flag corresponding to this event gets cleared automatically. Don't
 * clear it in the handler.
 * @see edma_error_event
 */
typedef void (* edma_error_event_handler)(uint32_t channel, enum edma_error_event e);
/** @} */ /* irq_handlers group */

/**
 * @defgroup cm_core Context manager core functions
 * @details Context manager initialization API.
 * @{
 */
/**
 * @brief Initializes the context manager.
 *
 * @param[in] isp ISP peripheral to use by context manager.
 * @param[in] edma eDMA peripheral to use by context manager.
 * @param[in] edma_descriptors Address of eDMA descriptors used by edma.
 *
 * @retval 0 if ok, -EINVAL if isp or edma are NULL.
 */
int CM_init(ISP_TypeDef * isp, EDMA_TypeDef * edma, EDMA_TCD_TypeDef * edma_descriptors);

/**
 * @brief Deinitializes the context manager.
 *
 * @details Restarts the ISP and sets the internals to default values.
 *
 * @warning Any ongoing DMA transfers are not stopped! The DMA descriptors are not
 * cleared.
 */
void CM_deinit(void);

/**
 * @brief Stores image processing time and triggers ISP.
 * @details The only way to get processing time is from registers prior the ISP
 * gets triggered. This processing time is later on used as timeouts for
 * processed images.
 *
 * @see CM_getCurrentImageProcessingTimeMs()
 */
void CM_triggerISP(void);

/**
 * @brief Deinitializes context.
 *
 * @param[in] req NEXT or ACTIVE request.
 */
void CM_deinitContext(enum requestId req);

/**
 * @brief Returns whether memories can be written.
 */
bool CM_isIspMemoriesAvailable(void);

/**
 * @brief Updates the context state.
 *
 * @param[in] dma_channel DMA channel which has finished.
 */
void CM_dmaDone(uint8_t dma_channel);

/**
 * @brief Updates the context state.
 */
void CM_ispFinished(void);

/**
 * @brief Updates the context state.
 */
void CM_statisticsDone(void);

/**
 * @brief Resets the request count for each channel.
 */
void CM_resetRequestCounts(void);

/**
 * @brief Returns channel which is in streaming mode.
 *
 * @retval streaming channel
 */
uint32_t CM_getStreamingChannel(void);

/**
 * @brief Returns the context state.
 *
 * @param[in] req NEXT or ACTIVE request.
 *
 * @retval context state.
 */
uint32_t CM_getContextState(enum requestId req);

/**
 * @brief  Returns whether mode transition is ongoing.
 *
 * @returns true if mode change is ongoing, false otherwise.
 */
bool CM_isModeTransitonActive(void);

/**
 * @brief Changes mode and sets the streaming channel.
 */
void CM_startStreaming(uint32_t channel);

/**
 * @brief Changes mode and clears the streaming channel.
 */
void CM_stopStreaming(void);

/**
 * @brief Updates mode based on current ISP mode.
 */
void CM_modeSwitchDone(void);

/**
 * @brief Returns ISP mode.
 *
 * @see isp_mode
 */
uint32_t CM_getIspMode(void);
/** @} */ /* cm_core group */

/**
 * @defgroup cm_edma_api Context manager data transfer functions
 * @details These functions serve to transfer data between ISP and external
 * memory with help of DMA.
 * @{
 */
/**
 * @brief Triggers the corresponding DMA channel.
 * @details The function computes the channel based on the mu_channel, as this
 * identifies the DMA "slot" - 32 DMA channels are partitioned into slots of
 * 4 channels for each client, where each channel is dedicated to different
 * transfer type, e.g. transfer of registers from DDR into ISP.
 *
 * @param[in] edma_request_id Identifier of the transfer.
 * @param[in] mu_chan MU channel the request came from.
 *
 * @retval 0 on success, -1 on failure.
 *
 * @note The configuration of DMA source & destination addresses happens in
 * CM_setNextRequest().
 *
 * @see CM_setNextRequest()
 */
int CM_startDmaTransfer(enum edma_request_id request_id, uint8_t mu_chan);
/** @} */ /* cm_edma_api group */

/**
 * @defgroup cm_max_req_mechanism Context manager request limitter functions
 * @details To limit the total number of requests per camera client, the user
 * should invoke the CM_increaseRequestsFromISR() upon message reception. If
 * the function returns true, the message can be scheduled into a queue.
 * After a request has been processed, the CM_decreaseRequestsFromIRQ() must
 * be invoked to decrease the total request count for a given channel. In case
 * the count is not decreased, it may happen that the channel gets blocked and
 * all requests will be rejected by the CM.
 * @{
 */
/**
 * @brief Increase the total requests count for a camera client.
 *
 * @param[in] channel client channel 0-7
 *
 * @retval true if request can be scheduled in queue.
 * @retval false if the number of requests for given channel exceeds 2.
 */
bool CM_increaseRequestsFromISR(uint32_t channel);

/**
 * @brief Increase the total requests count for a camera client.
 *
 * @param[in] channel client channel 0-7
 *
 * @retval true if request can be scheduled in queue.
 * @retval false if the number of requests for given channel exceeds 2.
 */
bool CM_increaseRequests(uint32_t channel);

/**
 * @brief Decreases the total requests count for a camera client.
 *
 * @param[in] channel client channel 0-7
*/
void CM_decreaseRequests(uint32_t channel);
/** @} */ /* cm_max_req_mechanism */

/**
 * @defgroup cm_request Context manager request functions
 * @details Once a new request should be processed, store it's metadata using
 * CM_setNextRequest. The CM will use this metadata in other functions.
 * Invoke the CM_setActiveRequestFromISR() after the ISP generates PIPELINE1_FRAME_START
 * event.
 * @{
 */
/**
 * @brief Set the next context to be processed and update the DMA addresses.
 * @details The context manager has a buffer for two entries holding some
 * metadata about the processed context (e.g. the context address, the MU
 * channel). This stores the next request to be processed internally to keep
 * track, from where the request came from and where is the context stored.
 * The corresponding DMA channels source and destination addresses get updated
 * as well.
 *
 * @param[in] req Request to be set.
 *
 * @retval true if next request has been scheduled. False otherwise.
 *
 * @see CM_setActiveRequestFromISR()
 */
bool CM_setNextRequest(const struct request * req);

/**
 * @brief Set's the next request as active.
 * @details Once the ISP starts, the registers are being shadowed and the next
 * request becomes the active. The total requests per client count is decreased.
 *
 * @see CM_setNextRequest()
 */
void CM_setActiveRequestFromISR(void);
/** @} */ /* cm_request group */

/**
 * @defgroup cm_metadata API to retrieve request metadata
 * @{
 */
/**
 * @brief Returns the context address of given request.
 *
 * @param[in] req Next or active request to retrieve data from.
 *
 * @retval Context memory address of given request.
 */
uint64_t CM_getContextAddress(enum requestId req);

/**
 * @brief Returns the upper 32 bits of a 64 bit context address.
 *
 * @param[in] x 64 bit address

 * @retval Upper 32 bits of address.
 */
#define GET_ADDRESS_HI(addr)    ((uint32_t)((addr) >> 32U))

/**
 * @brief Returns the lower 32 bits of a 64 bit context address.
 *
 * @param[in] addr 64 bit address
 *
 * @retval Lower 32 bits of address.
 */
#define GET_ADDRESS_LO(addr)    (addr & UINT32_MAX)

/**
 * @brief Returns the MU channel of given request.
 *
 * @param[in] req Next or active request to retrieve data from.
 *
 * @retval MU channel of given request.
 */
uint8_t CM_getContextChannel(enum requestId req);

/**
 * @brief Sets an error for given request.
 *
 * @param[in] request Determines request (next or active).
 * @param[in] error Error flags to be set.
 *
 * @note The error flags are ored together.
 */
void CM_setErrors(enum requestId req, uint32_t error);

/**
 * @brief Returns any errors stored for given request (active/upcomming).
 * @details Returns the ored errors and clears the flags afterwards.
 *
 * @param[in] request Determines for which request the errors should be returned.
 *
 * @retval errors.
 */
uint32_t CM_getErrors(enum requestId req);

/**
 * @brief Clears errors for given request.
 *
 * @param[in] req Request for which all errors should be cleared.
 */
void CM_clearErrors(enum requestId req);
/** @} */ /* cm_metadata group */

/** @} */ /* ctx_mngr group */
/** @} */ /* drivers group */

void CM_enableStreaming(void);
void CM_disableStreaming(void);
uint32_t CM_getContextFrameIndex(enum requestId req);
void CM_initScatterGatherDescriptors(uint8_t channel);

void CM_reuseContext(void);
bool CM_isContextReused(void);
void CM_resetContextReuse(void);
// void CM_setFrameNumber(uint32_t measure_index);
void CM_resetISP(void);
bool CM_dmaTransferUsesDescriptors(void);

/**
 * @brief Handles EDMA errors and sets corresponding context.
 *
 * @returns true if an error was handled, false otherwise
 */
bool CM_handleEdmaError(void);

uint32_t CM_getFrameNumber(void);


#endif /* ISP_MW_GUARD */
