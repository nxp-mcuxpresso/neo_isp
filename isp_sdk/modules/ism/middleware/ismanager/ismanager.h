/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ISP_SUBDOMAIN_MANAGER_
#define ISP_SUBDOMAIN_MANAGER_

#include "platform.h"

/**
 * ISP Subdomain Manager response handler.ACTIVE
 *
 * The data provided in response handler correspond to MU protocol.
 *
 * @param[in] channel Channel to which response is dedicated.
 * @param[in] data Response from ISM.
 * @param[in] size Number of bytes.
 *
 * @note The respone function is invoked from non-interrupt context.
 */
typedef int (* response_function)(uint32_t channel, const char * data, size_t size);

/**
 * Initializes the ISP Subdomain Manager.
 *
 * @param[in] isp ISP peripheral to be controlled.
 * @param[in] dma DMA peripheral to be used by isp.
 * @param[in] edma_descriptors Memory region where DMA descriptors can be stored.
 * @param[in] measurement_buffer Storage to hold measurement data. The minimal size
 * is 64 bytes. This will hold measurements for single image processing.
 * @param[in] buffer_size Size of buffer in bytes.
 * @param[in] responseHandler Provides response from ISM module.
 *
 * @note The DMA provided must be dedicated for provided ISP, as this is used to
 * transfer of configurations from DDR into ISP and vice versa. A generic DMA
 * might not be able to perform such transfers.
 *
 * @note The DMA descriptors represent a memory region, where configurations for
 * scatter gather transfers can be stored.
 *
 * @note See the imp_definition for details what responseHandler can return.
 */
int ISM_init(
    ISP_TypeDef *       isp,
    EDMA_TypeDef *      dma,
    EDMA_TCD_TypeDef *  edma_descriptors,
    uint8_t *           measurement_buffer,
    size_t              buffer_size,
    response_function   responseHandler
);

/**
 * @brief Possible options to for ISP modes.
 * @details The ISP can operate in two modes. Memory-to-memory, in which the
 * image from camera is stored into external memory (i.e. DDR) first and
 * afterwards the ISP reads this memory processes the image. Or in streaming mode
 * during which the output from camera is directly fed into ISP.
 */
enum isp_mode
{
    MEMORY_TO_MEMORY,
    STREAMING,
};

/**
 * Deinitializes the ISM module.
 *
 * @returns 0
 */
int ISM_deinit(void);

/**
 * @brief Initiates mode change.
 * @details Sends a request to change mode. During mode transition the ISM module
 * will ignore any requests for image processing until the mode transition has
 * not been finished. After the process is finished, the response handler
 * registered during ISM init will be invoked providing status about the
 * operation.\n
 * If the ISP is in memory-to-memory mode and a change to memory-to-memory is
 * requested, nothing happens and the reponse handler provides an error.\n
 * If the ISP is in streaming mode and a change to streaming mode happens, the
 * outcome depends on provided channel. If the streaming channel is the same
 * nothing happens and the reponse handler provides an error. However, if the
 * streaming channel differs, the ISP switches to stream on different channel.
 * This has impact only on processing requests, as requests for channel not in
 * streaming mode will be rejected, but the ISP will stay in streaming mode!
 * It's the responsibility of an upper layer controling the cameras which camera
 * is streaming into ISP.
 *
 * @param[in] channel Channel identifier. Indexed from 1.
 * @param[in] mode Requested mode.
 *
 * @retval 0x00000067 (IMP_SYSTEM_EVENT_QUEUE_FULL) this error reports overload
 * on the ISM module. Internal storage is not able to store additional data. This
 * is a fatal error and the functionality is not guaranteed. If this error happens,
 * the module might need to be reconfigured and the storage needs to be increased.
 *
 * @note This function shall not be invoked from interrupt context.
 */
int ISM_changeISPMode(uint8_t channel, enum isp_mode mode);

/**
 * @brief Initiates mode change.
 *
 * @returns 103 (IMP_SYSTEM_EVENT_QUEUE_FULL) on error, 0 otherwise.
 *
 * @note This function shall be invoked from interrupt context.
 *
 * @see ISM_changeISPMode for details.
 */
int ISM_changeISPModeFromISR(uint8_t channel, enum isp_mode mode);

/**
 * @brief Sends a request to process next image.
 *
 * @param[in] channel Camera channel for which an image shall be processed. Indexed from 1.
 * @param[in] address Address of configuration, known as ISP context.
 *
 * @retval 0 on success.
 * @retval 0x40000000 (IMP_FRAME_DONE_ERROR_STREAM) when a request for processing
 * has arrived, but the module is switching mode from memory to memory or vice versa.
 * @retval 0x80000000 (IMP_FRAME_DONE_ERROR_REJECT) on requesting to process image
 * while there are already N requests in the queue from given channel.
 * @retval 0x00000064 (IMP_SYSTEM_REQUEST_QUEUE_FULL) this error shall not be
 * reported and is for debug purpose. The ISM module holds processing requests
 * in it's internal storage and the number of requests is limited. However, due
 * to incorrect configuration or bug in code it might happen to store data into
 * full queue.
 * @retval 0x00000067 (IMP_SYSTEM_EVENT_QUEUE_FULL) this error reports overload
 * on the ISM module. Internal storage is not able to store additional data. This
 * is a fatal error and the functionality is not guaranteed. If this error happens,
 * the module might need to be reconfigured and the storage needs to be increased.
 *
 * @note This function shall not be invoked from interrupt context.
 *
 * @note The number of requests per channel is configurable and by default is
 * set to 2. The value has been selected as it makes no sense to store more
 * requests per channel.
 */
uint32_t ISM_requestISPProcessing(uint8_t channel, uint64_t address);

/**
 * @brief Sends a request to process next image.
*
 * @param[in] channel Camera channel for which an image shall be processed. Indexed from 1.
 * @param[in] address Address of configuration, known as ISP context.
 *
 * @note This function shall be invoked from interrupt context only.
 *
 * @see ISM_requestISPProcessing for details.
 */
uint32_t ISM_requestISPProcessingFromISR(uint8_t channel, uint64_t address);

/**
 * @brief Returns the address to table with measurements.
 * @details The ISM module captures times at which certain events happen like
 * the time when a processing request has been received, when DMA transactions
 * were initiated, when ISP events (start, frame done etc.) happen and when
 * a response has been send about frame done for each frame. There are 64 entries
 * and the table is constructed as a ring-buffer. Once the last entry has been
 * updated, the next will overwrite the first entry.\n
 * This function returns the address to this table.
 *
 * @param[in] address Address where the table is located.
 */
void ISM_getMeasurementsTableLocation(const void ** address);

#endif /* ISP_SUBDOMAIN_MANAGER_ */
