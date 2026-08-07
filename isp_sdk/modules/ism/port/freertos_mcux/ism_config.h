/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ISM_CONFIG_
#define ISM_CONFIG_

/*
    The module captures timestamps for key events associated with each processed
    image. The total number of images for which timestamps are recorded is
    determined by the value of MEASUREMENT_COUNT. The buffer operates as a ring
    buffer—once MEASUREMENT_COUNT images have been processed, new measurements
    overwrite the oldest entries starting from the beginning.
*/
#define MEASUREMENTS_COUNT          ((uint32_t) 64U)

/*
    Defines the number of supported cameras.
*/
#define MAX_CAMERA_CHANNELS         ((uint32_t) 8U)

/*
    Defines the maximum number of requests per channel that the ISM module can
    hold. If more than MAX_REQUESTS_PER_CHANNEL requests are received, the
    module will reject further requests until previously stored ones are
    processed.
*/
#define MAX_REQUESTS_PER_CHANNEL    ((uint32_t) 2U)

/*
    Defines the size of the queue for DMA requests.
    This is an internal parameter and does not need to be modified by the user.
*/
#define DMA_REQUEST_QUEUE_SIZE      ((uint32_t) 4U)

#endif /* ISM_CONFIG_ */
