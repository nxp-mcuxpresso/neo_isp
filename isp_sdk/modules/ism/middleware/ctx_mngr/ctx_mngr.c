/*
 * Copyright 2022-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file  ctx_mngr.c
 * @brief Middleware responsible for managing the ISP contexts.
 * @details The context manager will use the ISP driver and eDMA driver and
 * create a logic atop of those drivers to control the context switching.
 */

#include "platform.h"
#include "ctx_mngr.h"
#include "isp.h"
#include "edma.h"
#include "imp_definition.h"

/*
 *                 LOCAL TYPEDEFS
 */
#define REQUEST_COUNT   ((uint32_t) (2U))

/**
 * @brief Processing request internal metadata.
 */
struct request_metadata
{
    volatile uint32_t state;                    /**< @brief Request state. */
    volatile uint64_t context_address;          /**< @brief handle to context. */
    volatile uint8_t  channel;                  /**< @brief MU Channel the request came from. */
    volatile uint32_t errors;                   /**< @brief Holds any errors raised for given request. */
    volatile uint32_t frame_index;              /**< @brief Index of frame being processed. Used for measurements purposes. */
};

/**
 * @brief   Brief text description (one line).
 * @details Optionally a detailed text description of the code object.
 */
struct isp_manager
{
    isp_event_handler           isp_user_handler;
    edma_channel_event_handler  edma_channel_user_handler;
    edma_error_event_handler    edma_error_user_handler;

    /**
     * @brief Holds any errors from EDMA or ISP.
     * @details EDMA or ISP may generate any errors. These are stored into this
     * variable for frame being processed and are cleared upon reception of
     * a new frame.
     */
    volatile uint32_t errors;

    /**
     * @brief State variable indicating, whether ISP memories can be written.
     */
    bool memories_available;

    /**
     * @brief Tracks the number of requests in queue for each client.
     * @details We have 9 MU channels, where 8 are dedicated for clients.
     */
    volatile uint8_t requests_per_channel[MUP_CHANNEL_COUNT - 1U];

    /**
     * @brief ISP peripheral to control.
     */
    ISP_TypeDef * isp;

    /**
     * @brief eDMA peripheral assigned to ISP.
     */
    EDMA_TypeDef * edma;

    /**
     * @brief EDMA descriptors to control
     */
    EDMA_TCD_TypeDef * edma_descriptors;

    /**
     * @brief Array of requests metadata.
     * @details Holds some metadata about the requests so we can schedule two
     * requests in ISP and still be able to tell from which MU the request came
     * from and what was the context address in DDR.
     */
    struct request_metadata requests[REQUEST_COUNT];

    /**
     * @brief Indicates which request in reuquests array is currently active.
     */
    uint8_t active;

    /**
     * @brief Channel on which streaming has been requested
     */
    volatile uint32_t streaming_channel;

    /**
     * @brief Flag indicating reuse of configuration for next frame.
     * @details In case the host doesn't provide configuration for next frame
     * prior frame done in streaming mode, we must reuse the previous configuration.
     */
    volatile bool context_reused;

    /**
     * @brief ISP mode; streaming, memory to memory or transition between on to another.
     */
    volatile uint32_t mode;

    /**
     * @brief Status flag indicating whether ISP has been triggered in streaming mode.
     * @details In streaming mode, we can trigger the ISP only once, afterwards
     * the ISP shouldn't be triggered anymore.
     */
    bool isp_already_triggered;
};

/*
 *                              LOCAL CONSTANTS
 */
/*
According to discussion with Chanpreet Singh (ISP designer), the port sizes are:
- ISP context registers 64 bits
- DDR port 128 bits
*/
/**
 * @brief ISP context registers port size.
 */
#define ISP_CONTEXT_REGISTERS_PORT_SIZE         ((uint16_t) 64U / 8U)

/**
 * @brief DDR memory port size
 */
#define DDR_MEMORY_PORT_SIZE                    ((uint16_t) 128U / 8U)

/**
 * @brief eDMA direction for EDMA attribute register.
 */
enum edma_direction_attribute
{
    EDMA_DDR_TO_ISP_TRANSFER = 0x0U,
    EDMA_ISP_TO_DDR_TRANSFER = 0x8U,
};

/*
 *                              LOCAL VARIABLES
 */
#if !defined (ARM)
EDMA_TCD_TypeDef edma_descriptors[16];
#endif

static struct isp_manager g_isp_obj;

/*
 *                         LOCAL FUNCTION PROTOTYPES
 */
/**
 * @brief Returns the index of the other request buffer.
 *
 * This function toggles the given request index between 0 and 1
 * using an XOR operation. It is useful for double‑buffered request
 * handling where only two buffers exist.
 *
 * @param current_request The index of the current request (0 or 1).
 * @return The index of the other request (1 if input is 0, or 0 if input is 1).
 */
static inline uint8_t otherRequest(uint8_t current_request)
{
    return (uint8_t) (current_request ^ 1U);
}

/**
 * @brief Retrieves a pointer to the currently active request metadata.
 *
 * This function returns a pointer to the request metadata structure
 * corresponding to the active request index stored inside @ref g_isp_obj.
 *
 * @return Pointer to the active request's metadata structure.
 */
static inline struct request_metadata * activeRequest(void)
{
    plt_assert(g_isp_obj.active < REQUEST_COUNT);
    return &g_isp_obj.requests[g_isp_obj.active];
}

/**
 * @brief Retrieves a pointer to the next (inactive) request metadata.
 *
 * This function computes the index of the non‑active request using
 * @ref otherRequest() and returns a pointer to the corresponding
 * request metadata structure. This is typically used for preparing
 * the next request while the current one is being processed.
 *
 * @return Pointer to the next request's metadata structure.
 */
static inline struct request_metadata * nextRequest(void)
{
    return &g_isp_obj.requests[otherRequest(g_isp_obj.active)];
}

/**
 * @brief Initializes the ISP peripheral.
 * @details Enables the FS1 and FD2 interrupts, enables write into holes of the
 * ISP registers by setting the REG_XFR_DIS.
 *
 * @param[in] isp ISP peripheral to configure.
 */
static void CM_initISP(ISP_TypeDef * isp);

/**
 * @brief Initializes the eDMA channels as needed by the design.
 *
 * @param[in] edma eDMA peripheral to configure.
 */
static void CM_initEdmaForIsp(EDMA_TypeDef * edma);

void CM_initScatterGatherDescriptors(uint8_t channel)
{
    if (NULL == g_isp_obj.edma_descriptors)
    {
        return;
    }

    uint64_t src_address;
    uint16_t src_port_size;
    uint64_t dst_address;
    uint16_t dst_port_size;
    uint32_t transfer_size_bytes;
    uint64_t src_address_adjustment;
    uint64_t dst_address_adjustment;
    enum feature_state      major_iteration_complete;
    enum data_transfer_size src_transfer_size;
    enum data_transfer_size dst_transfer_size;

    if ((channel % 2U) == 0U)
    {
        /* source address for CH2 + 4xN represents the address of aliased registers
        in ISP, which is fixed. */
        /* The aliased region is located at offset 0x2000, see memory map of
        ISP in RM for i.MX95. */
        src_address       = (uint64_t) ISP_getOutputRegistersAddress();
        src_port_size     = ISP_CONTEXT_REGISTERS_PORT_SIZE;
        src_transfer_size = SIZE_8_BYTES;
        /* destination address for CH2 + 4xN represents the address of aliased
        registers in DDR, will be updated upon ISP request reception on MU. */
        dst_address            = (uint64_t) 0U; /* address will be updated upon ISP request reception on MU */
        dst_port_size          = DDR_MEMORY_PORT_SIZE;
        dst_transfer_size      = SIZE_16_BYTES;
        transfer_size_bytes    = ISP_getOutputRegistersSize();
        src_address_adjustment = (~((uint64_t)transfer_size_bytes)) + 1U;
        dst_address_adjustment = ADDRESS_TO_UINT64(&g_isp_obj.edma_descriptors[channel + 1U]) - 0x20000000U + 0x394000U;
        major_iteration_complete = ENABLED; /* This is the second SCG channel in row, we want it to generate IRQ. */
        /* Channel 2, 6, 10... are responsible for transfering registers. */
        EDMA_setStartChannel(&g_isp_obj.edma_descriptors[channel]);
    }
    else
    {
        /* source address for CH3 represents the address of memory
        in ISP, which is fixed. */
        /* Unfortunately the memory of ISP is partiotioned into three
        consecutive parts. The first and third part holds data to be read,
        the second part is writable. To simplify the code and have as less
        as possible eDMA transfers accessing DDR, we read the whole memory. */
        src_address       = (uint64_t) ISP_getOutputMemoriesAddress();
        src_port_size     = ISP_CONTEXT_REGISTERS_PORT_SIZE;
        src_transfer_size = SIZE_8_BYTES;
        /* destination address for CH0 represents the address of context in
        DDR, will be updated upon ISP request reception on MU. */
        dst_address            = 0U; /* address will be updated upon ISP request reception on MU */
        dst_port_size          = DDR_MEMORY_PORT_SIZE;
        dst_transfer_size      = SIZE_16_BYTES;
        transfer_size_bytes    = ISP_getOutputMemoriesSize();
        src_address_adjustment = (~((uint64_t)transfer_size_bytes)) + 1U;
        dst_address_adjustment = ADDRESS_TO_UINT64(&g_isp_obj.edma_descriptors[channel - 1U]) - 0x20000000U + 0x394000U;
        #if !defined (ISIM)
        major_iteration_complete = DISABLED;    /* we don't want the first SCG channel to generate IRQ. */
        #else
        major_iteration_complete = ENABLED;
        #endif
    }
    EDMA_setSourceAddress(&g_isp_obj.edma_descriptors[channel], src_address);
    EDMA_setSignedSourceAddressOffset(&g_isp_obj.edma_descriptors[channel], src_port_size);

    /* Address wrapping is disabled -> modulo = 0U */
    EDMA_setSourceAddressModulo(&g_isp_obj.edma_descriptors[channel], 0U);
    EDMA_setSourceDataTransferSize(&g_isp_obj.edma_descriptors[channel], src_transfer_size);
    /* We are not going to adjust the address, as this will change with the
    next context (next ISP request) anyway. */
    EDMA_setSourceAddressFeature(&g_isp_obj.edma_descriptors[channel], LAST_SOURCE_ADDRESS_ADJUSTMENT_FEATURE, src_address_adjustment);

    /* Destination configurtion */
    EDMA_setDestinationAddress(&g_isp_obj.edma_descriptors[channel], dst_address);
    EDMA_setSignedDestinationAddressOffset(&g_isp_obj.edma_descriptors[channel], dst_port_size);
    /* Address wrapping is disabled -> modulo = 0U */
    EDMA_setDestinationAddressModulo(&g_isp_obj.edma_descriptors[channel], 0U);
    EDMA_setDestionationDataTransferSize(&g_isp_obj.edma_descriptors[channel], dst_transfer_size);
    /* We are not going to adjust the address, as this will change with the
    next context (next ISP request) anyway. */
    EDMA_setDestinationAddressFeature(&g_isp_obj.edma_descriptors[channel], SCATTER_GATHER_ADDRESS_FEATURE, dst_address_adjustment);

    EDMA_setTransferSize(&g_isp_obj.edma_descriptors[channel], DISABLED, DISABLED, 0U, transfer_size_bytes);
    /* We perform a single transfer, so we set the iteration count to 1
    with channel linking disabled. */
    EDMA_setMinorChannelLinking(&g_isp_obj.edma_descriptors[channel], DISABLED, 0U, 1U);
    /* No major channel linking. */
    EDMA_setMajorChannelLinking(&g_isp_obj.edma_descriptors[channel], DISABLED, 0U);

    EDMA_setTransferMode(&g_isp_obj.edma_descriptors[channel], RW);
    EDMA_setHalfCompleteInterrupt(&g_isp_obj.edma_descriptors[channel], DISABLED);
    EDMA_setMajorIterationCompletesInterrupt(&g_isp_obj.edma_descriptors[channel], major_iteration_complete);
}

/**
 * @brief Configures EDMA for ISP purposes
 * @details The configuration of eDMA peripheral is with respect to the designed
 * operation.
 * eDMA channels association:\n
 * Each camera channel/stream is going to have 4 eDMA channels associated, i.e.
 * CAM0 = eDMA_CH0-3, CAM1 = eDMA_CHA4-7 etc. The first channel in the group
 * serves to transfer data from DDR RAM into ISP registers, the second channel
 * transfers data from ISP registers into DDR RAM. The third and fourth channel
 * is reserved currently.\n\n

 * eDMA channel linking:\n
 * There seems to be no reason to use channel linking, as the operation of eDMA
 * channels have delays between themselves. Thus channel linking is completely
 * disabled.\n\n
 *
 * eDMA channel arbitration:\n
 * The eDMA transfers context into ISP registers. Once ISP starts processing
 * the image, the ISP registers get shadowed meaning a new context can be written
 * into the ISP registers. From this perspective there is no need for channel
 * arbitration etc.\n
 * After the ISP has done all processing, the ISP registers (shadowed) content
 * should be written back into DDR RAM. Again, we can't preempt the eDMA, as we
 * need to * finish, the transaction prior other usage. It is also expected,
 * that the ISP registers are preloaded. If not, it makes no sense to load them
 * first and afterwards wait on the shadowed registers to be written into DDR RAM.
 * Thus arbitration, channel preemption etc. is completely disabled.\n\n
 *
 * eDMA addressing:\n
 * Currently we have no clue how addressing is going to work and wether we will
 * receive physical DDR RAM addresses or virtual. In case virtual addressing is
 * going to be provided, some address translation would be needed most probably.
 *
 * eDMA security:\n
 * Currently we don't have many information regarding Master ID replication, it's
 * usage and security related stuff, so this will definitely change.\n\n
 *
 * eDMA transfer control descriptors:\n
 * First channel is preconfigured to transfer data from DDR RAM into ISP registers,
 * whereas the second channel is preconfigured to transfer data from ISP registers
 * into DDR RAM.\n
 * Each channel is configured the simplest way, only to transfer data without
 * channel linking or address modifications etc. Only the transfer finished
 * interrupt is enabled, the half transfer interrupt is not generated.
 */
static void CM_initEdmaForIsp(EDMA_TypeDef * edma)
{
    /* Configure EDMA global settings */
    EDMA_setGlobalChannelLinking(edma, DISABLED);
    EDMA_setHaltAfterError(edma, DISABLED);
    EDMA_setGlobalChannelPreemption(edma, DISABLED);
    EDMA_setRoundRobinChannelArbitration(edma, DISABLED);
#if defined (NDEBUG)
    EDMA_setDebug(edma, ENABLED);
#else
    EDMA_setDebug(edma, DISABLED);
#endif
    EDMA_setChannelPhysicalAddressing(edma, 0xFFFFFFFFU);

    /* Configure EDMA channels */
    for (uint8_t ch = 0U; ch < EDMA_CHANNEL_COUNT; ch++)
    {
        /* TODO investigate arbitration groups */
        EDMA_assignArbitrationGroupToChannel(edma, ch, ch / 4U);
        /* TODO investigate ID replication */
        EDMA_setChannelMasterIDReplication(edma, ch, ENABLED);
        /* TODO investigate privileged access level */
        EDMA_setChannelPrivilegedAccessLevel(edma, ch, USER_PROTECTION_LEVEL);
        /* TODO investigate security level */
        EDMA_setChannelSecurityLevel(edma, ch, NON_SECURE);
        EDMA_setChannelTransactionType(edma, ch, DATA_ACCESS);
        /* The attibute content:
        ATTR[0:2] channel identifier
        ATTR[3] the direction
        Channels 0-1/4-5/8-9/... represent the transfers from DDR into ISP, thus the
        attribute is 0.
        Channels 2-3/6-7/10-11... represent the transfers from ISP into DDR, thus the
        attribute is 1.
        For ID's, we use the standard ID numbering, thus Ch0 = 0, Ch1 =1 etc.
        */
        if ((0U == (ch % 4U)) || (1U == (ch % 4U)))
        {
            // EDMA_setChannelAttribute(edma, ch, (uint8_t) EDMA_DDR_TO_ISP_TRANSFER | ch);
            EDMA_setChannelAttribute(edma, ch, (uint8_t) EDMA_DDR_TO_ISP_TRANSFER);
        }
        else
        {
            // EDMA_setChannelAttribute(edma, ch, (uint8_t) EDMA_ISP_TO_DDR_TRANSFER | ch);
            EDMA_setChannelAttribute(edma, ch, (uint8_t) EDMA_ISP_TO_DDR_TRANSFER);
        }

        EDMA_setSignExtension(edma, ch, 0);
        EDMA_setSwapSize(edma, ch, SWAP_DISABLED);
        EDMA_setChannelErrorInterrupt(edma, ch, ENABLED);
        EDMA_setAsynchronousDMARequestInStopMode(edma, ch, DISABLED);
        /* HW requests are not possible on imx95, disable them */
        EDMA_setDMAChannelHardwareRequestSignal(edma, ch, DISABLED);
    }

    /* Pre-configure EDMA TCDs, the only thing which is going to change afterwards
    is only the source and destination address
    Every Camera channel will have 4 eDMA channels assigned.
    Ch0 - transfers registers from DDR into ISP (write)
    Ch1 - transfers memory from DDR into ISP (write)
    Ch2 - transfers aliased registers from ISP into DDR (read)
    Ch3 - transfers memory from ISP into DDR (read)
    */
    uint64_t src_address;
    uint64_t dst_address;
    uint16_t src_port_size;
    uint16_t dst_port_size;
    uint32_t transfer_size_bytes;
    uint64_t src_address_adjustment;
    uint64_t dst_address_adjustment;

    enum data_transfer_size src_transfer_size;
    enum data_transfer_size dst_transfer_size;
    for (uint8_t ch = 0U; ch < EDMA_CHANNEL_COUNT; ch += 1U)
    {
        /* Source configuration */
        if ((ch % 4U) == 0U)
        {
            /* source address for CH0 + 4xN represents the address of context in DDR,
            will be updated upon ISP request reception on MU. */
            src_address         = 0U;
            src_port_size       = DDR_MEMORY_PORT_SIZE;
            src_transfer_size   = SIZE_16_BYTES;
            /* destination address for CH0 + 4xN represents the address of registers in
            ISP, which is fixed. */
            /* NOTE: Due to the design of ISP registers it's not possible to simply
            write the whole registers from beginning, as this would trigger the ISP.
            The trigger register TRIG_CAM0 is located at offset 0x20 and the next
            configuration register IMG_CONF_CAM0 starts at 0x30, thus we adjust
            the register start address by this offset. From eDMA view, the ISP registers
            start @0x20000 as per DOORs requirements for i.MX9x Camera AS.*/
            dst_address         = (uint64_t) ISP_getInputRegistersAddress();
            dst_port_size       = ISP_CONTEXT_REGISTERS_PORT_SIZE;
            dst_transfer_size   = SIZE_8_BYTES;
            /* Size of registers */
            transfer_size_bytes = ISP_getInputRegistersSize();
            DEBUG_PRINT("DMA CH0 ISP_getInputRegistersSize() %d, Input regs start address: 0x%lx, ISP_getOutputRegistersSize: %d\n",
                ISP_getInputRegistersSize(),
                ISP_getInputRegistersAddress(),
                ISP_getOutputRegistersSize()
            );
            DEBUG_PRINT("DMA CH0 transfer size is: %d [bytes]\n", transfer_size_bytes);
            src_address_adjustment = 0U;
            dst_address_adjustment = (~((uint64_t)transfer_size_bytes)) + 1U;
        }
        else if ((ch % 4U) == 1U)
        {
            /* source address for CH1 + 4xN represents the address of memories in DDR,
            will be updated upon ISP request reception on MU. */
            src_address         = 0U;
            src_port_size       = DDR_MEMORY_PORT_SIZE;
            src_transfer_size   = SIZE_16_BYTES;
            /* destination address for CH1 + 4xN represents the address of memory in
            ISP, which is fixed. */
            /* NOTE: Due to the design of ISP memories it's not possible to simply
            write the whole memory from beginning, as the write memory starts in
            the middle.
            The first writable memory starts with Vignetting Table located at
            offset 0x1000, thus we adjust the memory start address by that offset.
            From eDMA view, the ISP memories start @0x3E0000 as per DOORs
            requirements for i.MX9x Camera AS.*/
            dst_address         = (uint64_t) ISP_getInputMemoriesAddress();
            dst_port_size       = ISP_CONTEXT_REGISTERS_PORT_SIZE;
            dst_transfer_size   = SIZE_8_BYTES;
            transfer_size_bytes = ISP_getInputMemoriesSize();
            src_address_adjustment = 0U;
            dst_address_adjustment = (~((uint64_t)transfer_size_bytes)) + 1U;
        }
        else if ((ch % 4U) == 2U)
        {
            if (NULL != g_isp_obj.edma_descriptors)
            {
                /* Scatter / Gather descriptors are used, skip this channel. */
                continue;
            }

            /* Scatter / Gather descriptors are not used, initialize this channel */
            /* source address for CH2 + 4xN represents the address of aliased registers
            in ISP, which is fixed. */
            /* The aliased region is located at offset 0x2000, see memory map of
            ISP in RM for i.MX95. */
            src_address       = ISP_getOutputRegistersAddress();
            src_port_size     = ISP_CONTEXT_REGISTERS_PORT_SIZE;
            src_transfer_size = SIZE_8_BYTES;
            /* destination address for CH2 + 4xN represents the address of aliased
            registers in DDR, will be updated upon ISP request reception on MU. */
            dst_address            = 0U; /* address will be updated upon ISP request reception on MU */
            dst_port_size          = DDR_MEMORY_PORT_SIZE;
            dst_transfer_size      = SIZE_16_BYTES;
            transfer_size_bytes    = ISP_getOutputRegistersSize();
            src_address_adjustment = (~((uint64_t)transfer_size_bytes)) + 1U;
            dst_address_adjustment = 0U;
        }
        else
        {
            if (NULL != g_isp_obj.edma_descriptors)
            {
                /* Copy scatter gather descriptors for channel 4 into registers. */
                (void) memcpy((void *)(&g_isp_obj.edma->Channel[ch].TCD), (void *)(g_isp_obj.edma_descriptors + (ch / 2U)), sizeof(EDMA_TCD_TypeDef));
                continue;
            }
            /* Scatter / Gather descriptors are not used, initialize this channel */
            /* source address for CH3 represents the address of memory
            in ISP, which is fixed. */
            /* Unfortunately the memory of ISP is partiotioned into three
            consecutive parts. The first and third part holds data to be read,
            the second part is writable. To simplify the code and have as less
            as possible eDMA transfers accessing DDR, we read the whole memory. */
            src_address       = (uint64_t) ISP_getOutputMemoriesAddress();
            src_port_size     = ISP_CONTEXT_REGISTERS_PORT_SIZE;
            src_transfer_size = SIZE_8_BYTES;
            /* destination address for CH3 represents the address of context in
            DDR, will be updated upon ISP request reception on MU. */
            dst_address            = 0U; /* address will be updated upon ISP request reception on MU */
            dst_port_size          = DDR_MEMORY_PORT_SIZE;
            dst_transfer_size      = SIZE_16_BYTES;
            transfer_size_bytes    = ISP_getOutputMemoriesSize();
            DEBUG_PRINT("DMA CH3 (memories out) size: %d\n", transfer_size_bytes);
            src_address_adjustment =  (~((uint64_t)transfer_size_bytes)) + 1U;
            dst_address_adjustment = 0U;
        }

        EDMA_setSourceAddress(&edma->Channel[ch].TCD, src_address);
        EDMA_setSignedSourceAddressOffset(&edma->Channel[ch].TCD, src_port_size);

        /* Address wrapping is disabled -> modulo = 0U */
        EDMA_setSourceAddressModulo(&edma->Channel[ch].TCD, 0U);
        EDMA_setSourceDataTransferSize(&edma->Channel[ch].TCD, src_transfer_size);
        /* We are not going to adjust the address, as this will change with the
        next context (next ISP request) anyway. */
        EDMA_setSourceAddressFeature(&edma->Channel[ch].TCD, LAST_SOURCE_ADDRESS_ADJUSTMENT_FEATURE, src_address_adjustment);

        /* Destination configurtion */
        EDMA_setDestinationAddress(&edma->Channel[ch].TCD, dst_address);
        EDMA_setSignedDestinationAddressOffset(&edma->Channel[ch].TCD, dst_port_size);
        /* Address wrapping is disabled -> modulo = 0U */
        EDMA_setDestinationAddressModulo(&edma->Channel[ch].TCD, 0U);
        EDMA_setDestionationDataTransferSize(&edma->Channel[ch].TCD, dst_transfer_size);
        /* We are not going to adjust the address, as this will change with the
        next context (next ISP request) anyway. */
        EDMA_setDestinationAddressFeature(&edma->Channel[ch].TCD, FINAL_DESTINATION_ADDRESS_ADJUSTMENT_FEATURE, dst_address_adjustment);

        EDMA_setTransferSize(&edma->Channel[ch].TCD, DISABLED, DISABLED, 0U, transfer_size_bytes);
        /* We perform a single transfer, so we set the iteration count to 1
        with channel linking disabled. */
        EDMA_setMinorChannelLinking(&edma->Channel[ch].TCD, DISABLED, 0U, 1U);
        /* No major channel linking. */
        EDMA_setMajorChannelLinking(&edma->Channel[ch].TCD, DISABLED, 0U);

        EDMA_setTransferMode(&edma->Channel[ch].TCD, RW);
        EDMA_setHalfCompleteInterrupt(&edma->Channel[ch].TCD, DISABLED);
        EDMA_setMajorIterationCompletesInterrupt(&edma->Channel[ch].TCD, ENABLED);
    }

}

static void CM_initISP(ISP_TypeDef * isp)
{
    /* Disable transfer error while writing into gaps in the ISP register address space */
    isp->REG_XFR_DIS = ISP_REG_XFR_DIS_XFR_ERR_DIS(1);

    /* Enable ISP interrupts */
    ISP_setInterrupts(
        isp,
        ISP_INT_EN0_EN_FS1_Msk      |   /* Enable Frame Start 1 interrupt */
        ISP_INT_EN0_EN_FD2_Msk      |   /* Enable Frame Done 2 Interrupt */
        ISP_INT_EN0_EN_STATD_Msk    |
        ISP_INT_EN0_EN_BUS_ERR0_Msk |   /* Enable read bus interface on linepath 0 error interrupt */
        ISP_INT_EN0_EN_BUS_ERR1_Msk |   /* Enable read bus interface on linepath 1 error interrupt */
        ISP_INT_EN0_EN_BUS_ERR2_Msk |   /* Enable write port 0 error interrupt */
        ISP_INT_EN0_EN_BUS_ERR3_Msk,    /* Enable write port 1 error interrupt */
        ENABLED
    );
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/
int CM_startDmaTransfer(enum edma_request_id request_id, uint8_t mu_chan)
{
    plt_assert(mu_chan > 0U);
    plt_assert(mu_chan < 9U);

    if (true == EDMA_isActive(g_isp_obj.edma))
    {
        DEBUG_PRINT("Cant trigger EDMA, it is active.");
        return -1;
    }

    /* Remap the mu_channel to DMA "slot" by subtracting 1 (MU client channels
    requiring DMA transfers are indexed from 1 - 8) and multiply the value by
    4 - each client has 4 dedicated DMA channels. Afterwards add the request ID,
    which is equal to the DMA channel number in the slot. */
    uint8_t dma_channel = 4U * (mu_chan - 1U) + (uint8_t) request_id;

    /* If single stream receives two requests one after another, we will have
    ACTIVE and NEXT configuration for the same channel. As these share the DMA
    channels, the DMA channels configured for first request will be overwriten,
    by the second request. Thus we need to configure the channel 4 here. */
    if (3U == (dma_channel % 4U))
    {
    #if defined (ISIM)
        uint64_t aliased_reg_address    = activeRequest()->context_address;
        uint64_t memory_out_address     = activeRequest()->context_address;
    #else
        uint64_t aliased_reg_address    = CTX_getOutputRegistersAddress(activeRequest()->context_address);
        uint64_t memory_out_address     = CTX_getOutputMemoriesAddress(activeRequest()->context_address);
    #endif
        DEBUG_PRINT("Aliased address: 0x%08llx\n", aliased_reg_address);
        DEBUG_PRINT("Memory out address: 0x%08llx\n", memory_out_address);
        if (NULL == g_isp_obj.edma_descriptors)
        {
            EDMA_setDestinationAddress(&g_isp_obj.edma->Channel[dma_channel - 1U].TCD, aliased_reg_address);
            EDMA_setDestinationAddress(&g_isp_obj.edma->Channel[dma_channel].TCD, memory_out_address);
        }
        else
        {
            EDMA_setDestinationAddress(&g_isp_obj.edma_descriptors[(dma_channel - 3U) / 2U], aliased_reg_address);
            EDMA_setDestinationAddress(&g_isp_obj.edma->Channel[dma_channel].TCD, memory_out_address);
        }
    }

    EDMA_startChannel(g_isp_obj.edma, dma_channel);

    return 0;
}

bool CM_dmaTransferUsesDescriptors(void)
{
    return (g_isp_obj.edma_descriptors != NULL);
}

bool CM_increaseRequestsFromISR(uint32_t channel)
{
    plt_assert(channel < 8U);
    plt_assert(channel > 0);

    bool retval = false;
    uint32_t isr_mask = taskENTER_CRITICAL_FROM_ISR();
    if (MAX_REQUESTS_PER_CHANNEL > g_isp_obj.requests_per_channel[channel])
    {
        g_isp_obj.requests_per_channel[channel]++;
        retval = true;
    }
    taskEXIT_CRITICAL_FROM_ISR(isr_mask);

    return retval;
}

bool CM_increaseRequests(uint32_t channel)
{
    plt_assert(channel < 8U);

    bool retval = false;
    taskENTER_CRITICAL();
    if (MAX_REQUESTS_PER_CHANNEL > g_isp_obj.requests_per_channel[channel])
    {
        g_isp_obj.requests_per_channel[channel]++;
        retval = true;
    }
    taskEXIT_CRITICAL();

    return retval;
}

void CM_decreaseRequests(uint32_t channel)
{
    plt_assert(channel < MAX_CAMERA_CHANNELS);

    if (channel < MAX_CAMERA_CHANNELS)
    {
        taskENTER_CRITICAL();
        if (0U < g_isp_obj.requests_per_channel[channel])
        {
            g_isp_obj.requests_per_channel[channel]--;
        }
        else
        {
            DEBUG_PRINT("Warning - CM_decreaseRequests() decrease requests_per_channel when already 0.\n");
        }
        taskEXIT_CRITICAL();
    }
}

int CM_init(ISP_TypeDef * isp, EDMA_TypeDef * edma, EDMA_TCD_TypeDef * edma_descriptors)
{
    if ((NULL == isp) || (NULL == edma))
    {
        return -EINVAL;
    }

    for (uint8_t i = 0U; i < 8U; i++)
    {
        g_isp_obj.requests_per_channel[i] = 0U;
    }

    g_isp_obj.isp                   = isp;
    g_isp_obj.edma                  = edma;
    g_isp_obj.edma_descriptors      = edma_descriptors;

    /* Channels 4xN are configured for scatter/gather. This means 16 configs in
    total -> EDMA_CHANNEL_COUNT / 2U */
    /* We need to initialize the scatter/gather registers first, as it's
    configuration is used in the initialization of EDMA. */
    if (NULL != g_isp_obj.edma_descriptors)
    {
        for (uint8_t ch = 0U; ch < EDMA_CHANNEL_COUNT / 2U; ch++)
        {
            CM_initScatterGatherDescriptors(ch);
        }
    }
    CM_initEdmaForIsp(g_isp_obj.edma);
    CM_initISP(g_isp_obj.isp);

    g_isp_obj.active = 1U;

    g_isp_obj.memories_available = true;
    for (uint8_t i = 0U; i < sizeof(g_isp_obj.requests)/sizeof(g_isp_obj.requests[0U]); i++)
    {
        g_isp_obj.requests[i].channel                   = 1U;
        g_isp_obj.requests[i].context_address           = 0U;
    }

    /* ISP mode initial value */
    g_isp_obj.mode                  = MEMORY_TO_MEMORY_MODE;

    /* Streaming mode related parameters */
    g_isp_obj.streaming_channel     = 0U;
    g_isp_obj.isp_already_triggered = false;
    g_isp_obj.context_reused        = false;

    return 0;
}

void CM_deinit(void)
{
    (void) ISP_hardReset(g_isp_obj.isp);
    g_isp_obj.isp   = NULL;
    /* EDMA has no reset mechanism, so leave it as is and just set it to NULL */
    /* TODO We might want to stop EDMA transfers?? */
    /* Maybe by disabling IRQs, stopping the DMA transfers if any and clearing
    all pending flags, if any. */
    g_isp_obj.edma  = NULL;
    /* Descriptors are stored in memory. Leave the memory as is, just set the
    descriptors to NULL. */
    g_isp_obj.edma_descriptors = NULL;

    for (uint8_t i = 0U; i < 8U; i++)
    {
        g_isp_obj.requests_per_channel[i] = 0U;
    }

    CM_deinitContext(NEXT);
    CM_deinitContext(ACTIVE);

    g_isp_obj.mode  = MEMORY_TO_MEMORY_MODE;

    g_isp_obj.streaming_channel     = 0U;
    g_isp_obj.isp_already_triggered = false;
    g_isp_obj.context_reused        = false;
}

bool CM_isIspMemoriesAvailable(void)
{
    return g_isp_obj.memories_available;
}

void CM_dmaDone(uint8_t dma_channel)
{
    dma_channel %= 4U;

    if (0U == dma_channel)
    {
        DEBUG_PRINT("Checking content of ISP registers...\n");
        DEBUG_PRINT("INT_EN0          : %#0x\n", g_isp_obj.isp->INT_EN0);
        DEBUG_PRINT("INT_EN0          : %#0x\n", g_isp_obj.isp->INT_EN0);
        DEBUG_PRINT("INT_STAT0        : %#0x\n", g_isp_obj.isp->INT_STAT0);
        DEBUG_PRINT("IMG0_IN_ADDR_CAM0: %#0x\n", g_isp_obj.isp->IMG0_IN_ADDR_CAM0);
        DEBUG_PRINT("IMG0_IN_ADDR_CAM1: %#0x\n", g_isp_obj.isp->IMG1_IN_ADDR_CAM0);
        DEBUG_PRINT("OUTCH0_ADDR_CAM0 : %#0x\n", g_isp_obj.isp->OUTCH0_ADDR_CAM0);
        DEBUG_PRINT("OUTCH1_ADDR_CAM0 : %#0x\n", g_isp_obj.isp->OUTCH1_ADDR_CAM0);
        nextRequest()->state |= REGS_PRELOADED;
    }
    else if (1U == dma_channel)
    {
        nextRequest()->state |= MEMS_PRELOADED;
    }
    else if (2U == dma_channel)
    {
        activeRequest()->state |= REGS_READ;
    }
    else
    {
        activeRequest()->state |= MEMS_READ;
    }
}

bool CM_setNextRequest(const struct request * req)
{
    bool retval = false;

    struct request_metadata * next_request = nextRequest();
    if (UNINITIALIZED == next_request->state)
    {
        retval                        = true;
        next_request->channel         = req->channel;
        next_request->context_address = req->context_address;
        next_request->frame_index     = req->frame_index;
        /* The request is being processed, so the queue can accept additional
        request from this client. */
        next_request->state       = INITIALIZED;
        next_request->errors      = 0U;

        uint8_t edma_channel    = next_request->channel;
        /* The request channel corresponds to the MU channel, which is indexed from
        1 - 8. So we need to remap them to DMA channel by subtracting 1 and multiplying
        by 4. */
        plt_assert(0 < edma_channel && edma_channel < MUP_CHANNEL_COUNT);
        edma_channel             = 4U * (edma_channel - (uint8_t)1U);

        plt_assert(edma_channel < EDMA_CHANNEL_COUNT);

        uint64_t context_address = next_request->context_address;

        /* We need to update the src/dst addresses of DMAs following way:
            Ch0: src: DDR registers address
            Ch1: src: DDR memory address
            Ch2: dst: DDR aliased address
            Ch3: dst: DDR memory address
        */
#if defined (ISIM)
        /* On ISIM we don't have access to the whole address range, only to single
        "virtual" address */
        uint64_t reg_address        = context_address;
        uint64_t memory_in_address  = context_address;
#else
        uint64_t reg_address        = CTX_getInputRegistersAddress(context_address);
        uint64_t memory_in_address  = CTX_getInputMemoriesAddress(context_address);
#endif
        DEBUG_PRINT("DDR regs copy start address: 0x%08llx\n", reg_address);
        DEBUG_PRINT("DDR mems copy start address: 0x%08llx\n", memory_in_address);
        EDMA_setSourceAddress(&g_isp_obj.edma->Channel[edma_channel].TCD, reg_address);
        edma_channel++;
        plt_assert(edma_channel < EDMA_CHANNEL_COUNT);
        EDMA_setSourceAddress(&g_isp_obj.edma->Channel[edma_channel].TCD, memory_in_address);

        if (NULL == g_isp_obj.edma_descriptors)
        {
            uint64_t alias_region_address   = CTX_getOutputRegistersAddress(context_address);
            uint64_t memory_out_address     = CTX_getOutputMemoriesAddress(context_address);
            /* Scatter / Gather descriptors are not used, we need to initialize
            the destination addresses as well. */
            edma_channel++;
            plt_assert(edma_channel < EDMA_CHANNEL_COUNT);
            EDMA_setDestinationAddress(&g_isp_obj.edma->Channel[edma_channel].TCD, alias_region_address);
            edma_channel++;
            plt_assert(edma_channel < EDMA_CHANNEL_COUNT);
            EDMA_setDestinationAddress(&g_isp_obj.edma->Channel[edma_channel].TCD, memory_out_address);
        }
    }

    return retval;
}

void CM_triggerISP(void)
{
    g_isp_obj.memories_available = false;
    /* The ISP can be triggered only once in Streaming mode, any other triggers
    may cause an error during streaming. */
    /* First check that ISP was triggered. This is relevant for streaming mode.
    Otherwise we pass this check always and continue (the variable should be
    false for memory-to-memory mode). */
#if defined (ISP_AUTOMATIC_MODE)
    /* Due to bug in HW we disable the automatic ISP triggering
    and trigger the ISP over and over again as in memory2memory mode. */
    if (!(true == g_isp_obj.isp_already_triggered))
    {
#endif
        if (true == ISP_isTriggerAvailable(g_isp_obj.isp))
        {
            ISP_trigger(g_isp_obj.isp);

#if defined (ISP_AUTOMATIC_MODE)
            if ((STREAMING_MODE == CM_getIspMode()) && (false == g_isp_obj.isp_already_triggered))
            {
                /* We were in streaming mode and we triggered the ISP. Let's
                set a flag. */
                g_isp_obj.isp_already_triggered = true;
            }
#endif
        }
#if defined (ISP_AUTOMATIC_MODE)
    }
#endif
}

void CM_setActiveRequestFromISR(void)
{
    /* Set active request */
    g_isp_obj.active = otherRequest(g_isp_obj.active);
    activeRequest()->state |= PROCESSING;
    taskENTER_CRITICAL();
    if ( 0 >= g_isp_obj.requests_per_channel[activeRequest()->channel - 1U])
    {
        DEBUG_PRINT("Warning - CM_setActiveRequestFromISR() decrease requests_per_channel when already 0.\n");
    }
    else
    {
        g_isp_obj.requests_per_channel[activeRequest()->channel - 1U]--;
    }
    taskEXIT_CRITICAL();
}

uint64_t CM_getContextAddress(enum requestId req)
{
    uint64_t retval = 0U;
    if (NEXT == req)
    {
        retval = nextRequest()->context_address;
    }
    else
    {
        retval = activeRequest()->context_address;
    }

    return retval;
}

uint32_t CM_getContextFrameIndex(enum requestId req)
{
    uint32_t retval;
    if (NEXT == req)
    {
        retval = nextRequest()->frame_index;
    }
    else
    {
        retval = activeRequest()->frame_index;
    }
    return retval;
}

uint8_t CM_getContextChannel(enum requestId req)
{
    uint8_t retval;
    if (NEXT == req)
    {
        retval = nextRequest()->channel;
    }
    else
    {
        retval = activeRequest()->channel;
    }
    return retval;
}

void CM_setErrors(enum requestId req, uint32_t error)
{
    if (NEXT == req)
    {
        nextRequest()->errors |= error;
        DEBUG_PRINT("NEXT context DMA error: %x", nextRequest()->errors);
    }
    else
    {
        activeRequest()->errors |= error;
        DEBUG_PRINT("ACTIVE context DMA error: %x", activeRequest()->errors);
    }
}

uint32_t CM_getErrors(enum requestId req)
{
    uint32_t retval;
    if (NEXT == req)
    {
        retval = nextRequest()->errors;
    }
    else
    {
        retval = activeRequest()->errors;
    }
    return retval;
}

void CM_clearErrors(enum requestId req)
{
    if (NEXT == req)
    {
        nextRequest()->errors = 0U;
    }
    else
    {
        activeRequest()->errors = 0U;
    }
}

void CM_deinitContext(enum requestId req)
{
    if (NEXT == req)
    {
        /* Defensive programing, if somehow the count has been cleared, don't decrease it. */
        taskENTER_CRITICAL();
        if (0U < g_isp_obj.requests_per_channel[nextRequest()->channel - 1U])
        {
            g_isp_obj.requests_per_channel[nextRequest()->channel - 1U]--;
        }
        else
        {
            DEBUG_PRINT("Warning - CM_deinitContext() decrease requests_per_channel when already 0.\n");
        }
        taskEXIT_CRITICAL();
        nextRequest()->channel          = 1U;
        nextRequest()->context_address  = 0U;
        nextRequest()->errors           = 0U;
        nextRequest()->state            = UNINITIALIZED;
    }
    else if (ACTIVE == req)
    {
        activeRequest()->channel          = 1U;
        activeRequest()->context_address  = 0U;
        activeRequest()->errors           = 0U;
        activeRequest()->state            = UNINITIALIZED;

        /* The request has been deinitialized, this means, that we can use the
        memories again. */
        g_isp_obj.memories_available = true;
    }
    else
    {
        /* Unhandled request!!! Report sys error! */
    }
}

void CM_resetRequestCounts(void)
{
    for (uint32_t i = 0; i < REQUEST_COUNT; i++)
    {
        g_isp_obj.requests_per_channel[i] = 0U;
    }
}

void CM_resetISP(void)
{
    (void) ISP_hardReset(g_isp_obj.isp);
    CM_initISP(g_isp_obj.isp);
    g_isp_obj.memories_available = true;
}

uint32_t CM_getContextState(enum requestId req)
{
    uint32_t retval;
    if (NEXT == req)
    {
        retval = nextRequest()->state;
    }
    else if (ACTIVE == req)
    {
        retval = activeRequest()->state;
    }
    else
    {
        retval = UNINITIALIZED;
    }

    return retval;
}

uint32_t CM_getStreamingChannel(void)
{
    return g_isp_obj.streaming_channel;
}

void CM_ispFinished(void)
{
    activeRequest()->state |= PROCESSED;
}

void CM_statisticsDone(void)
{
    activeRequest()->state |= STATISTICS_DONE;
}

bool CM_isModeTransitonActive(void)
{
    uint32_t mode = g_isp_obj.mode;
    return (TRANSITION_TO_STREAMING == mode) || (TRANSITION_TO_MEMORY_TO_MEMORY == mode);
}

void CM_startStreaming(uint32_t channel)
{
    g_isp_obj.mode = TRANSITION_TO_STREAMING;
    g_isp_obj.streaming_channel = channel;
}

void CM_stopStreaming(void)
{
    g_isp_obj.mode = TRANSITION_TO_MEMORY_TO_MEMORY;
    g_isp_obj.streaming_channel = 1U;
}

void CM_modeSwitchDone(void)
{
    if (TRANSITION_TO_MEMORY_TO_MEMORY == g_isp_obj.mode)
    {
        g_isp_obj.isp_already_triggered = false;
        g_isp_obj.mode = MEMORY_TO_MEMORY_MODE;
    }

    if (TRANSITION_TO_STREAMING == g_isp_obj.mode)
    {
        g_isp_obj.mode = STREAMING_MODE;
    }
}

uint32_t CM_getIspMode(void)
{
    return g_isp_obj.mode;
}

bool CM_handleEdmaError(void)
{
    uint8_t error_channel;
    bool retval = false;
    if (true == EDMA_getErrorPresent(g_isp_obj.edma))
    {
        error_channel = EDMA_getErrorChannelNumber(g_isp_obj.edma);

        if ((error_channel % 4U) < 2U)
        {
            CM_setErrors(NEXT, g_isp_obj.edma->ES);
        }
        else
        {
            CM_setErrors(ACTIVE, g_isp_obj.edma->ES);
        }

        g_isp_obj.edma->Channel[error_channel].INT = EDMA_CHn_INT_INT_Msk;
        g_isp_obj.edma->Channel[error_channel].ES = EDMA_CHn_ES_ERR_Msk;

        retval = true;
    }

    return retval;
}

void CM_reuseContext(void)
{
    g_isp_obj.context_reused = true;
    activeRequest()->state &= ((uint32_t) ~(MEMS_READ | PROCESSED));
}

bool CM_isContextReused(void)
{
    return g_isp_obj.context_reused;
}

void CM_resetContextReuse(void)
{
    g_isp_obj.context_reused = false;
}

uint32_t CM_getFrameNumber(void)
{
    return ISP_getFrameNumber(g_isp_obj.isp);
}

void CM_enableStreaming(void)
{
    ISP_setStreamingMode(g_isp_obj.isp, DISABLED);
    // ISP_setDualExposureVsyncLock(ISP, ENABLED);
}

void CM_disableStreaming(void)
{
    /* start streaming on new channel */
    // ISP_setDualExposureVsyncLock(ISP, ENABLED);
    ISP_setStreamingMode(g_isp_obj.isp, ENABLED);
    ISP_setSingleStepMode(g_isp_obj.isp, ENABLED);
    uint32_t channel = CM_getStreamingChannel();
    plt_assert(0 < channel && channel < UINT8_MAX);
    uint8_t virtual_channel = (uint8_t) ((channel - 1U) | 0x8U);
    ISP_setVirtualChannelID(g_isp_obj.isp, 0, virtual_channel);   /* VC0 = MU1..., 0x8 == PED bit */
    ISP_setVirtualChannelID(g_isp_obj.isp, 1, virtual_channel);   /* VC1 = MU1..., 0x8 == PED bit*/
}
