/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LOGGING_PROTOCOL_GUARD
#define LOGGING_PROTOCOL_GUARD

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define LOGGING_PROTOCOL_VERSION 0x00010000

#define FRAME_DONE_REJECT_STREAMING             0x00010000U
#define ON_PROCESSING_REQUEST_QUEUE_FULL        0x00020000U
#define REGISTERS_PRELOAD_REQUESTED             0x00030000U
#define MEMORY_PRELOAD_REQUESTED                0x00040000U
#define ISP_TRIGGERED                           0x00050000U
#define ISP_PIPELINE_STARTED                    0x00060000U
#define TRANSITION_TO_STREAMING_DONE            0x00070000U
#define TRANSITION_TO_M2M_DONE                  0x00080000U
#define MEMORY_READOUT_REQUEST                  0x00090000U
#define MEMORY_READOUT_REQUEST_FAILURE          0x000A0000U
#define FRAME_DONE_SEND_MSG                     0x000B0000U
#define FRAME_DONE_SET_STATE                    0x000C0000U
#define FRAME_DONE_HANDLE_NEXT_STATE            0x000D0000U
#define MEMORY_READOUT_DONE                     0x000E0000U
#define INVALID_STREAMING_CHANNEL               0x000F0000U
#define STREAMING_CHANNEL_ALREADY_ACTIVE        0x00100000U
#define SWITCH_TO_STREAMING_DELAYED             0x00110000U
#define MEMORIES_TO_DDR_FINISHED_SET_STATE      0x00120000U
#define NOTHING_TO_STOP                         0x00130000U
#define STREAMING_MODE_STOPPED                  0x00140000U
#define STREAMING_MODE_STOP_DELAYED             0x00150000U
#define PROCESS_REQUEST_EDMA_QUEUE_FULL         0x00160000U
#define DDR_TO_REGS_DONE_EDMA_QUEUE_FULL        0x00170000U
#define DMA_ERROR                               0x00180000U
#define ISP_ERROR                               0x00190000U
#define START_STOP_STREAMING_REQUEST_IGNORED    0x001A0000U
#define PROCESSING_REQ_IGNORED_MODE_TRANSITION  0x001B0000U
#define DDR_TO_ISP_REGS_TRIGGERED               0x001C0000U
#define DDR_TO_ISP_MEMS_TRIGGERED               0x001D0000U
#define ISP_TO_DDR_MEMS_TRIGGERED               0x001E0000U
#define TRANSITION_TO_STREAMING_DELAYED_DONE    0x001F0000U

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
/**
 * @brief Log decoding structure
 * @details Every entry in the log contains an ID. Every ID corresponds to an
 * event in the ISP FW. Thus every ID is unique. To better understand, what each
 * ID represents a text can be assigned so the log becomes human readable.
*/
struct DecodingTable_t {
    uint32_t    mId;        /**< Log event ID. */
    const char  * mText;    /**< Log event ID description*/
};

#if 0
static struct DecodingTable_t gTable[] __attribute__((unused)) =
{
    {0,           "not used."},
    {FRAME_DONE_REJECT_STREAMING,           "The ISP FW is in streaming mode, but request was received on non-streaming channel."},
    {ON_PROCESSING_REQUEST_QUEUE_FULL,      "During processing a request it was not possible to queue it into request queue." },
    {REGISTERS_PRELOAD_REQUESTED,           "Sending request to DMA task to preload ISP registers with new data."},
    {MEMORY_PRELOAD_REQUESTED,              "Sending request to DMA task to prelaod ISP memories with new data. "},
    {ISP_TRIGGERED,                         "Triggering ISP."},
    {ISP_PIPELINE_STARTED,                  "The ISP generated ISP_PIPELINE1_START trigger."},
    {TRANSITION_TO_STREAMING_DONE,          "ISP FW has switched mode from memory2memory to streaming mode."},
    {TRANSITION_TO_M2M_DONE,                "Streaming mode has been stopped with a delay as there was an image being processed by ISP."},
    {MEMORY_READOUT_REQUEST,                "Statistics done IRQ has been generated. Sending request to DMA task to read out ISP memories and registers."},
    {MEMORY_READOUT_REQUEST_FAILURE,        "FATAL ERROR: DMA task queue is full while requesting memory/registers readout. (this should never happen)"},
    {FRAME_DONE_SEND_MSG,                   "Frame done event: sending message to host during."},
    {FRAME_DONE_SET_STATE,                  "Frame done event: memories/registers not read yet, just updating context state."},
    {FRAME_DONE_HANDLE_NEXT_STATE,          ""},
    {MEMORY_READOUT_DONE,                   "Memory has been read out and frame done has been sent to host side."},
    {INVALID_STREAMING_CHANNEL,             "Streaming mode requested, but the streaming channel was invalid."},
    {STREAMING_CHANNEL_ALREADY_ACTIVE,      "Requested streaming on a channel which is already in streaming mode."},
    {SWITCH_TO_STREAMING_DELAYED,           "Switch to streaming has been posponed as an image is currently being processed."},
    {MEMORIES_TO_DDR_FINISHED_SET_STATE,    "ISP memories and aliased registers have been written into DDR, but the context is not ready yet to report frame done."},
    {NOTHING_TO_STOP,                       "Requested to stop streaming mode, but ISP FW is in memory2memory mode."},
    {STREAMING_MODE_STOPPED,                "Streaming mode has been stopped immediately as there was no image being processed."},
    {STREAMING_MODE_STOP_DELAYED,           "Stop streaming request has been delayed until the currently being processed image is ready."},
    {PROCESS_REQUEST_EDMA_QUEUE_FULL,       "FATAL ERROR: DMA task queue is full while requesting to preload registers. (this should never happen)"},
    {DDR_TO_REGS_DONE_EDMA_QUEUE_FULL,      "FATAL ERROR: DMA task queue is full while requesting to preload memories. (this should never happen)"},
    {DMA_ERROR,                             "DMA error, frame done reported to host side."},
    {ISP_ERROR,                             "ISP error."},
    {START_STOP_STREAMING_REQUEST_IGNORED,  "Request for for starting/stopping streaming mode received, but currently we are in a mode switch and we ignore these requests."},
    {PROCESSING_REQ_IGNORED_MODE_TRANSITION,"A processing request has been received, but we are in a mode switch state."},
    {DDR_TO_ISP_REGS_TRIGGERED,             "Transfer of registers data from DDR into ISP has been triggered."},
    {DDR_TO_ISP_MEMS_TRIGGERED,             "Transfer of memories data from DDR into ISP has been triggered."},
    {ISP_TO_DDR_MEMS_TRIGGERED,             "Transfer of registers/memories data from ISP into DDR has been triggered."},
    {TRANSITION_TO_STREAMING_DELAYED_DONE,  "ISP FW has switched mode from memory2memory to streaming mode after last image has been processed."},
};
#endif

#endif /* LOGGING_PROTOCOL_GUARD */
