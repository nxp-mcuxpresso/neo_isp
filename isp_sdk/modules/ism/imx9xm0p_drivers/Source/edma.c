/*
 * Copyright 2022-2024, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file edma.c
 * @brief eDMA low level API implementation.
 */
#include "edma.h"
#include "_edma_trigger.h"

#if defined (ISIM)
EDMA_TypeDef edma_virtual;
#endif

/*
 *                      Public Functions
 */
void EDMA_setGlobalMasterIDReplication(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_GMRC_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_GMRC_Msk);
    }
}

void EDMA_setGlobalChannelLinking(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_GCLC_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_GCLC_Msk);
    }

}

void EDMA_setHalt(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_HALT_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_HALT_Msk);
    }
}

void EDMA_setHaltAfterError(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_HAE_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_HAE_Msk);
    }
}

void EDMA_setGlobalChannelPreemption(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_GCPC_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_GCPC_Msk);
    }
}

void EDMA_setRoundRobinChannelArbitration(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_ERCA_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_ERCA_Msk);
    }
}

void EDMA_setDebug(EDMA_TypeDef * edma, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    if (ENABLED == new_state)
    {
        edma->CSR |= EDMA_CSR_EDBG_Msk;
    }
    else
    {
        edma->CSR &= ((uint32_t) ~EDMA_CSR_EDBG_Msk);
    }
}

void EDMA_stopChannel(EDMA_TypeDef * edma, uint8_t channel, bool stop_with_error)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    uint32_t val =
        EDMA_MP_STOPCH_ERR(stop_with_error == true? 1U : 0U) |
        EDMA_MP_STOPCH_STOPCH(channel);
    edma->STOPCH = val;
}

void EDMA_setChannelVirtualAdressing(EDMA_TypeDef * edma, uint32_t channels)
{
    plt_assert(NULL != edma);
    edma->VPAS_LOW |= channels;
}

void EDMA_setChannelPhysicalAddressing(EDMA_TypeDef * edma, uint32_t channels)
{
    plt_assert(NULL != edma);
    edma->VPAS_LOW &= ((uint32_t) ~channels);
}

void EDMA_assignArbitrationGroupToChannel(EDMA_TypeDef * edma, uint8_t channel, uint8_t priority_group)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    edma->CHn_GRPRI[channel] = EDMA_MP_CHn_GRPRI_GRPRI(priority_group);
}

void EDMA_setChannelMasterIDReplication(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (ENABLED == new_state)
    {
        edma->Channel[channel].SBR |= EDMA_CHn_SBR_EMI_Msk;
    }
    else
    {
        edma->Channel[channel].SBR &= ((uint32_t) ~EDMA_CHn_SBR_EMI_Msk);
    }
}

void EDMA_setChannelPrivilegedAccessLevel(EDMA_TypeDef * edma, uint8_t channel, enum priviledged_access access_type)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (USER_PROTECTION_LEVEL == access_type)
    {
        edma->Channel[channel].SBR |= EDMA_CHn_SBR_PAL_Msk;
    }
    else
    {
        edma->Channel[channel].SBR &= ((uint32_t) ~EDMA_CHn_SBR_PAL_Msk);
    }
}

void EDMA_setChannelSecurityLevel(EDMA_TypeDef * edma, uint8_t channel, enum security_level level_type)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (NON_SECURE == level_type)
    {
        edma->Channel[channel].SBR |= EDMA_CHn_SBR_SEC_Msk;
    }
    else
    {
        edma->Channel[channel].SBR &= ((uint32_t) ~EDMA_CHn_SBR_SEC_Msk);
    }
}

void EDMA_setChannelTransactionType(EDMA_TypeDef * edma, uint8_t channel, enum transaction transaction_type)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (DATA_ACCESS == transaction_type)
    {
        edma->Channel[channel].SBR |= EDMA_CHn_SBR_INSTR_Msk;
    }
    else
    {
        edma->Channel[channel].SBR |= ((uint32_t) ~EDMA_CHn_SBR_INSTR_Msk);
    }
}

void EDMA_setChannelAttribute(EDMA_TypeDef * edma, uint8_t channel, uint8_t attribute)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    uint32_t reg = edma->Channel[channel].SBR & ((uint32_t) ~EDMA_CHn_SBR_ATTR_Msk);
    reg |= ((uint32_t) EDMA_CHn_SBR_ATTR(attribute));

    edma->Channel[channel].SBR = reg;
}

bool EDMA_isActive(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->CSR & EDMA_CSR_ACTIVE_Msk) != 0U;
}

bool EDMA_getErrorPresent(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_VLD_Msk) != 0U;
}

uint8_t EDMA_getErrorChannelNumber(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (uint8_t)((edma->ES & EDMA_ES_ERRCHN_Msk) >> EDMA_ES_ERRCHN_Pos);
}

bool EDMA_getUncorrectableECCError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_UCE_Msk) != 0U;
}

bool EDMA_getTransferCanceledError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_ECX_Msk) != 0U;
}

bool EDMA_getSourceAddressError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_SAE_Msk) != 0U;
}

bool EDMA_getSourceOffsetError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_SOE_Msk) != 0U;
}

bool EDMA_getDestinationAddressError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_DAE_Msk) != 0U;
}

bool EDMA_getDestinationOffsetError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_DOE_Msk) != 0U;
}

bool EDMA_getNBytesCIterConfigurationError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_NCE_Msk) != 0U;
}

bool EDMA_getScatherGatherConfigurationError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_SGE_Msk) != 0U;
}

bool EDMA_getSourceBusError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_SBE_Msk) != 0U;
}

bool EDMA_getDestinationBusError(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return (edma->ES & EDMA_ES_DBE_Msk) != 0U;
}

uint32_t EDMA_getInterruptRequestStatusChannelRegisterContent(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return edma->INT_LOW;
}

uint32_t EDMA_getHardwareRequestStatusRegisterContent(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return edma->HRS_LOW;
}

uint32_t EDMA_getChannelStopStatusPendingRegisterContent(const EDMA_TypeDef * edma)
{
    plt_assert(NULL != edma);
    return edma->SSR_LOW;
}

void EDMA_setSignExtension(EDMA_TypeDef * edma, uint8_t channel, uint8_t sign_extension)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    uint32_t reg = edma->Channel[channel].CCSR & ((uint32_t) ~EDMA_CHn_CSR_SIGNEXT_Msk);
    reg |= EDMA_CHn_CSR_SIGNEXT(sign_extension);
    edma->Channel[channel].CCSR = reg;
}

void EDMA_setSwapSize(EDMA_TypeDef * edma, uint8_t channel, enum swap_size size)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    uint32_t reg = edma->Channel[channel].CCSR & ((uint32_t) ~EDMA_CHn_CSR_SWAP_Msk);
    reg |= EDMA_CHn_CSR_SWAP(size);
    edma->Channel[channel].CCSR = reg;
}

void EDMA_channelStopChannel(EDMA_TypeDef * edma, uint8_t channel, bool stop_with_error)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (true == stop_with_error)
    {
        edma->Channel[channel].CCSR |= EDMA_CHn_CSR_ECX_Msk;
    }
    else
    {
        edma->Channel[channel].CCSR |= EDMA_CHn_CSR_CX_Msk;
    }
}

void EDMA_setChannelErrorInterrupt(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (ENABLED == new_state)
    {
        edma->Channel[channel].CCSR |= EDMA_CHn_CSR_EEI_Msk;
    }
    else
    {
        edma->Channel[channel].CCSR &= ((uint32_t) ~EDMA_CHn_CSR_EEI_Msk);
    }
}

void EDMA_setAsynchronousDMARequestInStopMode(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (ENABLED == new_state)
    {
        edma->Channel[channel].CCSR |= EDMA_CHn_CSR_EARQ_Msk;
    }
    else
    {
        edma->Channel[channel].CCSR &= ((uint32_t) ~EDMA_CHn_CSR_EARQ_Msk);
    }
}

void EDMA_setDMAChannelHardwareRequestSignal(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    if (ENABLED == new_state)
    {
        edma->Channel[channel].CCSR |= EDMA_CHn_CSR_ERQ_Msk;
    }
    else
    {
        edma->Channel[channel].CCSR &= ((uint32_t) ~EDMA_CHn_CSR_ERQ_Msk);
    }
}

bool EDMA_isChannelActive(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].CCSR & EDMA_CHn_CSR_ACTIVE_Msk) != 0U;
}

bool EDMA_isChannelDone(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].CCSR & EDMA_CHn_CSR_DONE_Msk) != 0U;
}

bool EDMA_getChannelErrorPresent(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_ERR_Msk) != 0U;
}

bool EDMA_getChannelUncorrectableECCError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_UCE_Msk) != 0U;
}

bool EDMA_getChannelTransferCanceledError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_ECX_Msk) != 0U;
}

bool EDMA_getChannelSourceAddressError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_SAE_Msk) != 0U;
}

bool EDMA_getChannelSourceOffsetError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_SOE_Msk) != 0U;
}

bool EDMA_getChannelDestinationAddressError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_DAE_Msk) != 0U;
}

bool EDMA_getChannelDestinationOffsetError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_DOE_Msk) != 0U;
}

bool EDMA_getChannelNBytesCIterConfigurationError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_NCE_Msk) != 0U;
}

bool EDMA_getChannelScatherGatherConfigurationError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_SGE_Msk) != 0U;
}

bool EDMA_getChannelSourceBusError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_SBE_Msk) != 0U;
}

bool EDMA_getChannelDestinationBusError(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return (edma->Channel[channel].ES & EDMA_CHn_ES_DBE_Msk) != 0U;
}

uint32_t EDMA_getChannelInterruptRequestStatusChannelRegisterContent(const EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);
    return edma->Channel[channel].INT & EDMA_CHn_INT_INT_Msk;
}

void EDMA_setSourceAddress(EDMA_TCD_TypeDef * tcd, uint64_t src_address)
{
    plt_assert(NULL != tcd);
    tcd->SADDR        = (uint32_t) (src_address & UINT32_MAX);
    tcd->SADDR_HIGH   = (uint32_t) (src_address >> 32U);
}

void EDMA_setSourceAddressModulo(EDMA_TCD_TypeDef * tcd, uint8_t modulo)
{
    plt_assert(NULL != tcd);
    uint16_t tmp = tcd->ATTR & (~(uint16_t)EDMA_TCDn_ATTR_SMOD_Msk);
    tmp |= (uint16_t) (((uint16_t) modulo) << EDMA_TCDn_ATTR_SMOD_Pos);
    tcd->ATTR = tmp;
}

void EDMA_setSourceDataTransferSize(EDMA_TCD_TypeDef * tcd, enum data_transfer_size size)
{
    plt_assert(NULL != tcd);
    uint16_t tmp = tcd->ATTR & (~(uint16_t) EDMA_TCDn_ATTR_SSIZE_Msk);
    tmp |= (uint16_t)(((uint16_t) size) << EDMA_TCDn_ATTR_SSIZE_Pos);
    tcd->ATTR = tmp;
}

void EDMA_setDestinationAddressModulo(EDMA_TCD_TypeDef * tcd, uint8_t modulo)
{
    plt_assert(NULL != tcd);
    uint16_t tmp = tcd->ATTR & (~(uint16_t) EDMA_TCDn_ATTR_DMOD_Msk);
    tmp |= (uint16_t) (((uint16_t) modulo) << EDMA_TCDn_ATTR_DMOD_Pos);
    tcd->ATTR = tmp;
}

void EDMA_setDestionationDataTransferSize(EDMA_TCD_TypeDef * tcd, enum data_transfer_size size)
{
    plt_assert(NULL != tcd);
    uint16_t tmp = tcd->ATTR & (~(uint16_t) EDMA_TCDn_ATTR_DSIZE_Msk);
    tmp |= (uint16_t)(((uint16_t) size) << EDMA_TCDn_ATTR_DSIZE_Pos);
    tcd->ATTR = tmp;
}

void EDMA_setTransferSize(
    EDMA_TCD_TypeDef * tcd,
    enum feature_state src_minor_loop_offset_state,
    enum feature_state dst_minor_loop_offset_state,
    uint32_t minor_loop_offset,
    uint32_t count
)
{
    plt_assert(NULL != tcd);
    uint32_t tmp = 0U;

    tmp |= EDMA_TCDn_NBYTES_SMLOE(src_minor_loop_offset_state);
    tmp |= EDMA_TCDn_NBYTES_DMLOE(dst_minor_loop_offset_state);

    if (0U != tmp)
    {
        tmp |= EDMA_TCDn_NBYTES_MLOFF(minor_loop_offset);
        tmp |= EDMA_TCDn_NBYTES_MLOFFYES_NBYTES(count);
    }
    else
    {
        tmp |= EDMA_TCDn_NBYTES_MLOFFNO_NBYTES(count);
    }

    tcd->NBYTES_MLOFF = tmp;
}

void EDMA_setSourceAddressFeature(EDMA_TCD_TypeDef * tcd, enum source_address_feature feature, uint64_t address)
{
    plt_assert(NULL != tcd);

    tcd->SLAST_SDA_HIGH   = EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA(address >> 32U);
    tcd->SLAST_SDA        = EDMA_TCDn_SLAST_SDA_SLAST_SDA(address & UINT32_MAX);

    if (LAST_SOURCE_ADDRESS_ADJUSTMENT_FEATURE == feature)
    {
        tcd->TCSR = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_ESDA_Msk);
    }
    else
    {
        tcd->TCSR |= EDMA_TCDn_CSR_ESDA_Msk;
    }
}

void EDMA_setDestinationAddress(EDMA_TCD_TypeDef * tcd, uint64_t dst_address)
{
    plt_assert(NULL != tcd);

    tcd->DADDR        = (uint32_t) (dst_address & UINT32_MAX);
    tcd->DADDR_HIGH   = (uint32_t) (dst_address >> 32U);
}

void EDMA_setDestinationAddressFeature(EDMA_TCD_TypeDef * tcd, enum destination_address_feature feature, uint64_t address)
{
    plt_assert(NULL != tcd);

    tcd->DLAST_SGA_HIGH   = EDMA_TCDn_DLAST_SGA_HIGH_DLAST_SGA(address >> 32U);
    tcd->DLAST_SGA        = EDMA_TCDn_DLAST_SGA_DLAST_SGA(address & UINT32_MAX);

    if (FINAL_DESTINATION_ADDRESS_ADJUSTMENT_FEATURE == feature)
    {
        tcd->TCSR = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_ESG_Msk);
    }
    else
    {
        tcd->TCSR |= EDMA_TCDn_CSR_ESG_Msk;
    }
}

void EDMA_setSignedSourceAddressOffset(EDMA_TCD_TypeDef * tcd, uint16_t offset)
{
    plt_assert(NULL != tcd);

    tcd->SOFF = offset;
}

void EDMA_setSignedDestinationAddressOffset(EDMA_TCD_TypeDef * tcd, uint16_t offset)
{
    plt_assert(NULL != tcd);

    tcd->DOFF = offset;
}

void EDMA_setMinorChannelLinking(
    EDMA_TCD_TypeDef * tcd,
    enum feature_state minor_channel_linking,
    uint8_t minor_loop_channel_link,
    uint16_t major_iteration_count
)
{
    plt_assert(NULL != tcd);

    uint16_t tmp;
    tmp = EDMA_TCDn_CITER_ELINK(minor_channel_linking);

    if (0U != tmp)
    {
        tmp |= EDMA_TCDn_CITER_LINKCH(minor_loop_channel_link);
        tmp |= EDMA_TCDn_CITER_ELINKYES_CITER(major_iteration_count);
    }
    else
    {
        tmp |= EDMA_TCDn_CITER_ELINKNO_CITER(major_iteration_count);
    }

    tcd->CITER_ELINK = tmp;
    /* BITER must equal to CITER register */
    tcd->BITER_ELINK = tmp;
}

void EDMA_setMajorChannelLinking(
    EDMA_TCD_TypeDef * tcd,
    enum feature_state major_channel_linking,
    uint8_t major_loop_channel_link
)
{
    plt_assert(NULL != tcd);

    uint16_t tmp;

    if (ENABLED == major_channel_linking)
    {
        tmp = tcd->TCSR;
        tmp |= EDMA_TCDn_CSR_MAJORELINK_Msk;
        tmp = tmp & (~(uint16_t) EDMA_TCDn_CSR_MAJORLINKCH_Msk);
        tmp |= EDMA_TCDn_CSR_MAJORLINKCH(major_loop_channel_link);
        tcd->TCSR = tmp;
    }
    else
    {
        tcd->TCSR = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_MAJORELINK_Msk);
    }
}

void EDMA_setTransferMode(EDMA_TCD_TypeDef * tcd, enum transfer_mode mode)
{
    plt_assert(NULL != tcd);

    uint16_t tmp = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_TMC_Msk);
    tmp |= EDMA_TCDn_CSR_TMC(mode);
    tcd->TCSR = tmp;
}

void EDMA_requestDisable(EDMA_TCD_TypeDef * tcd, enum feature_state new_state)
{
    plt_assert(NULL != tcd);

    if (ENABLED == new_state)
    {
        tcd->TCSR |= EDMA_TCDn_CSR_DREQ_Msk;
    }
    else
    {
        tcd->TCSR = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_DREQ_Msk);
    }
}

void EDMA_setHalfCompleteInterrupt(EDMA_TCD_TypeDef * tcd, enum feature_state new_state)
{
    plt_assert(NULL != tcd);

    if (ENABLED == new_state)
    {
        tcd->TCSR |= EDMA_TCDn_CSR_INTHALF_Msk;
    }
    else
    {
        tcd->TCSR = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_INTHALF_Msk);
    }
}

void EDMA_setMajorIterationCompletesInterrupt(EDMA_TCD_TypeDef * tcd, enum feature_state new_state)
{
    plt_assert(NULL != tcd);

    if (ENABLED == new_state)
    {
       tcd->TCSR |= EDMA_TCDn_CSR_INTMAJOR_Msk;
    }
    else
    {
        tcd->TCSR = tcd->TCSR & (~(uint16_t) EDMA_TCDn_CSR_INTMAJOR_Msk);
    }
}

void EDMA_setStartChannel(EDMA_TCD_TypeDef * tcd)
{
    plt_assert(NULL != tcd);
    tcd->TCSR |= EDMA_TCDn_CSR_START(1);
}

void EDMA_startChannel(EDMA_TypeDef * edma, uint8_t channel)
{
    plt_assert(NULL != edma);
    plt_assert(channel < EDMA_CHANNEL_COUNT);

    edma_trigger_plt(edma, channel);
}
