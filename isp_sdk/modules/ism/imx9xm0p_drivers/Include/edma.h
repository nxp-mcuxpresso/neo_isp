/*
 * Copyright 2022, 2024, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file edma.h
 * @brief eDMA low level API header file.
 */

#ifndef EDMA_GUARD
#define EDMA_GUARD

#include "platform.h"

/**
 * @addtogroup llapi
 * @{
 */

/**
 * @defgroup lledma eDMA low level API
 * @details As per the reference manual the eDMA programming model is split into
 * three parts:\n
 * - management page providing overall control functions\n
 * - channel control, status and configuration functions\n
 * - local transfer control descriptors
 *
 * From this perspective the @ref edma_global_control, @ref edma_global_channel_control,
 * @ref edma_global_status and @ref edma_global_channel_status groups belong to the
 * management page group.
 *
 * The @ref edma_channel_control and @ref edma_channel_status groups belong to
 * the channel control, status and configuration group.
 *
 * The @ref edma_tcd_control belongs to the local transfer control descriptors
 * group.
 *
 * The @ref edma_cfg_options group defines a set of types used in functions in
 * abovementioned groups.
 * @{
 */

/*
 *                              Enumerations
 */

/**
 * @defgroup edma_cfg_options eDMA configuration types and options
 * @{
 */
/**
 * @brief DMA protection level on system bus.
 *
 * @see EDMA_setChannelPrivilegedAccessLevel()
 */
enum priviledged_access
{
    USER_PROTECTION_LEVEL,
    PRIVILEDGED_PROTECTION_LEVEL
};

/**
 * @brief Channel's secruity options.
 *
 * @see EDMA_setChannelSecurityLevel()
 */
enum security_level
{
    NON_SECURE,
    SECURE
};

/**
 * @brief Transaction type.
 *
 * @see EDMA_setChannelTransactionType()
 */
enum transaction
{
    DATA_ACCESS,
    INTRUCTION_ACCESS
};

/**
 * @brief Swap size.

 * @see EDMA_setSwapSize()
 */
enum swap_size
{
    SWAP_DISABLED           = 0x0U,
    READ_WITH_8_BIT_SWAP    = 0x1U,
    READ_WITH_16_BIT_SWAP   = 0x2U,
    READ_WITH_32_BIT_SWAP   = 0x3U,
    WRITE_WITH_8_BIT_SWAP   = 0x9U,
    WRITE_WITH_16_BIT_SWAP  = 0xAU,
    WRITE_WITH_32_BIT_SWAP  = 0xBU,
};

/**
 * @brief Data transfer size.
 *
 * @see EDMA_setSourceDataTransferSize(), EDMA_setDestionationDataTransferSize()
*/
enum data_transfer_size
{
    SIZE_1_BYTES    = 0U,
    SIZE_2_BYTES    = 1U,
    SIZE_4_BYTES    = 2U,
    SIZE_8_BYTES    = 3U,
    SIZE_16_BYTES   = 4U,
    SIZE_32_BYTES   = 5U,
    SIZE_64_BYTES   = 6U,
    SIZE_128_BYTES  = 7U,
};

/**
 * @brief Source address feature options.
 *
 * @see EDMA_setSourceAddressFeature()
 */
enum source_address_feature
{
    LAST_SOURCE_ADDRESS_ADJUSTMENT_FEATURE          = 0U,
    STORE_DESTINATION_ADDRESS_FEATURE               = 1U,
};

/**
 * @brief Destination address feature options.
 *
 * @see EDMA_setDestinationAddressFeature()
 */
enum destination_address_feature
{
    FINAL_DESTINATION_ADDRESS_ADJUSTMENT_FEATURE    = 0U,
    SCATTER_GATHER_ADDRESS_FEATURE                  = 1U,
};

/**
 * @brief Transfer mode options.
 *
 * @see EDMA_setTransferMode()
 */
enum transfer_mode
{
    RW = 0U,
    RO = 1U,
    WO = 2U,
};
/** @} */ /* eDMA configuration options */

/**
 * @defgroup edma_global_control eDMA global control API
 * @details This API serves to control the eDMA global configuration which is
 * valid to the whole module. For channel configuration see either @ref edma_global_channel_control
 * or @ref edma_channel_control.
 * @{
 */
/**
 * @brief Enables/disables Global Master ID replication for all channels
 *
 * @param[in] edma eDMA peripheral to control
 * @param[in] new_state enables or disables Master ID replication for all channels
 *
 * @note If enabled, master ID replication is controlled by each channel
 * separately, otherwise this feature is disabled for all channels.
 */
void EDMA_setGlobalMasterIDReplication(EDMA_TypeDef * edma, enum feature_state new_state);

/**
 * @brief Enables/disables Channel Linking for all channels
 *
 * @param[in] edma eDMA peripheral to control
 * @param[in] new_state enables or disables Channel Linking for all channels.
 *
 * @note If enabled, channel linking is controlled by each channel separately,
 * otherwise this feature is disabled for all channels.
 */
void EDMA_setGlobalChannelLinking(EDMA_TypeDef * edma, enum feature_state new_state);

/**
 * @brief Halts the eDMA execution or releases the eDMA from Halt
 * @details Stall the start of new channels. Executing channels complete it's
 * operation. The execution is resumed, if halt is disabled.
 *
 * @param[in] edma eDMA peripheral to control
 * @param[in] new_state enables or disables Halt
 */
void EDMA_setHalt(EDMA_TypeDef * edma, enum feature_state new_state);

/**
 * @brief Enables or disables whether the eDMA should halt after error
 * @details Any error causes the eDMA to halt if enabled. Subsequently all eDMA
 * requests are ignored until the peripheral is released from halt.
 *
 * @param[in] edma eDMA peripheral to control
 * @param[in] new_state enables or disables halting the eDMA on error
 *
 * @see EDMA_setHalt()
 */
void EDMA_setHaltAfterError(EDMA_TypeDef * edma, enum feature_state new_state);

/**
 * @brief Enables or disables Global Channel Preemption
 *
 * @param[in] edma eDMA peripheral to control
 * @param[in] new_state enables or disables channel preemption
 *
 * @note If disabled, preemption is disabled for all channels.
 */
void EDMA_setGlobalChannelPreemption(EDMA_TypeDef * edma, enum feature_state new_state);

/**
 * @brief Enables or disables Round Robin Channel Arbitration
 *
 * @param[in] edma EMDA peripheral to control
 * @param[in] new_state enables or disables channel arbitration
 *
 * @note If enabled, round robin arbitration is used for channel selection within
 * each group, otherwise, fixed arbitration is used.
 */
void EDMA_setRoundRobinChannelArbitration(EDMA_TypeDef * edma, enum feature_state new_state);

/**
 * @brief Enables or disables eDMA debug mode
 *
 * @param[in] edma eDMA peripheral to control
 * @param[in] new_state enables or disables debug mode
 *
 * @note If enabled, the DMA stalls the start of a new channel. Executing channels
 * complete its operation. Channel execution resumes when the system exits debug
 * mode or the debug mode is disabled.
 */
void EDMA_setDebug(EDMA_TypeDef * edma, enum feature_state new_state);
/** @} */ /* eDMA global control API */

/**
 * @defgroup edma_global_channel_control eDMA global channel control API
 * @{
 */
/**
 * @brief Stop eDMA channel with or without error reporting
 *
 * @param[in] edma eDMA peripheral to stop channel on
 * @param[in] channel channel to stop
 * @param[in] stop_with_error determines, whether and error should be generated on stop
 *
 * @note If the channel is stopped with error reporting, an optional error interrupt
 * is generated.
 */
void EDMA_stopChannel(EDMA_TypeDef * edma, uint8_t channel, bool stop_with_error);

/**
 * @brief Sets virtual addressing for each channel
 * @details Every bit in channels input variable represents one channel. Setting
 * this bit to 1 will enable virtual addressing for that channel. The rest of
 * channels stay untouched, i.e. if channel 0
 * & 8 should have virtual addressing enabled, set channels to 0x00000101.
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channels mask of channels for which virtual addressing should be enabled
 */
void EDMA_setChannelVirtualAdressing(EDMA_TypeDef * edma, uint32_t channels);

/**
 * @brief Sets physical addressing for each channel
 * @details Every bit in channels input variable represents one channel. Setting
 * this bit to 1 will enable physical addressing for that channel. The rest of
 * channels stay untouched, i.e. if channel 0
 * & 8 should have physical addressing enabled, set channels to 0x00000101.
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channels mask of channels for which virtual addressing should be enabled
 */
void EDMA_setChannelPhysicalAddressing(EDMA_TypeDef * edma, uint32_t channels);

/* TODO ask about the number of arbitration group, 6 bits are reserved, but RM
says 0 - 31 */
/**
 * @brief Asigns a channel to an arbitration group
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel channel which arbitration group should be set
 * @param[in] priority_group fixed priority arbitration group (0 - 31)
 */
void EDMA_assignArbitrationGroupToChannel(EDMA_TypeDef * edma, uint8_t channel, uint8_t priority_group);

/**
 * @brief Configures the channels master ID replication feature
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel channel to configure
 * @param[in] new_state enables or disables master ID replication for given channel
 */
void EDMA_setChannelMasterIDReplication(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state);

/**
 * @brief Configures the channels privileged access level feature
 * @details DMA's protection level on the system bus when the channel is active.
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel channel to configure
 * @param[in] access_type priviledged access type
 */
void EDMA_setChannelPrivilegedAccessLevel(EDMA_TypeDef * edma, uint8_t channel, enum priviledged_access access_type);

/**
 * @brief Configures the channels security level feature
 * @details DMA's security level on the system bus when the channel is active
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel channel to configure
 * @param[in] level_type security level type
 */
void EDMA_setChannelSecurityLevel(EDMA_TypeDef * edma, uint8_t channel, enum security_level level_type);

/**
 * @brief Configures the eDMA's transaction type
 * @details EDMA's transaction type on the system bus when the channel is active
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel channel to configure
 * @param[in] transaction_type eDMA transaction type
 */
void EDMA_setChannelTransactionType(EDMA_TypeDef * edma, uint8_t channel, enum transaction transaction_type);

/**
 * @brief Configures the eDMA's channel attribute.
 * @details The attribute should identify the eDMA channel and it's direction.
 * ATTR[0:2] represent the channel identifier, ATTR[3] the direction (0: DDR2ISP,
 * 1: ISP2DDR).
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel channel to configure
 * TODO update doc
 * @param[in] attribute 4bit value representing the channel's attribute
 *
 * @note Configures the CHn_SBR[ATTR] register field.
 */
void EDMA_setChannelAttribute(EDMA_TypeDef * edma, uint8_t channel, uint8_t attribute);
/** @} */ /* eDMA global channel control API */

/**
 * @defgroup edma_global_status eDMA global status API
 * @{
 */
/**
 * @brief Returns the status of eDMA
 *
 * @param[in] edma eDMA to get status from
 *
 * @returns true if eDMA is active, false otherwise
 */
bool EDMA_isActive(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether an error occured on eDMA
 *
 * @param[in] edma eDMA peripheral to get status from
 *
 * @returns true on error, false otherwise
 *
 * @note The function returns true if at least one error bit is set.
 */
bool EDMA_getErrorPresent(const EDMA_TypeDef * edma);

/**
 * @brief Returns the eDMA channel where an error happened.
 *
 * @param[in] edma eDMA peripheral the channel is bound
 *
 * @returns channel number of the last recorded error or last recorded error canceled transfer.
 */
uint8_t EDMA_getErrorChannelNumber(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether the uncorrectable TCD error during channel execution happened.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[UCE] bit.
 */
bool EDMA_getUncorrectableECCError(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether a transfer has been canceled.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[ECX] bit.
 */
bool EDMA_getTransferCanceledError(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether a source address error happened.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[SAE] bit. The error is due to configuration
 * error detected in TCDn_SADDR field. TCDn_SADDR is inconsistent with TCDn_ATTR[SSIZE].
 */
bool EDMA_getSourceAddressError(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether a source offset error happened.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[SOE] bit. The error is due to configuration
 * error detected in TCDn_SOFF field. TCDn_SOFF is inconsistent with TCDn_ATTR[SSIZE].
 */
bool EDMA_getSourceOffsetError(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether a destination adress error happened.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[DAE] bit. The error is due to configuration
 * error detected in TCDn_DADDR field. TCDn_DADDR is inconsistent with TCDn_ATTR[DSIZE].
 */
bool EDMA_getDestinationAddressError(const EDMA_TypeDef * edma);

/**
 * @brief Reports whether a destination offset error happened.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[DOE] bit. The error is due to configuration
 * error detected in TCDn_DOFF field. TCDn_DOFF is inconsistent with TCDn_ATTR[DSIZE].
 */
bool EDMA_getDestinationOffsetError(const EDMA_TypeDef * edma);

/**
 * @brief Number of bytes or minor loop counter configuration error.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[NCE] bit. The error is due to configuration
 * error detected in TCDn_NBYTES or TCDn_CITER field. The flag is raised in case the
 * NBYTES is equal to zero or CITER is not equal to BITER. (TCDn_NBYTES is not
 * a multiple of of TCDn_ATTR[SSIZE] and TCDn_ATTR[DSIZE], or TCDn_CITER[CITER]
 * is equal to zero, or TCDn_CITER[ELINK] is not equal to TCDn_BITER[ELINK]).
 */
bool EDMA_getNBytesCIterConfigurationError(const EDMA_TypeDef * edma);

/**
 * @brief Scatter/gather configuration error.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[SGE] bit. The error is due to configuration
 * error detected in TCDn_DLASTSGA field. This field is checked at the beginning
 * of a scatter/gather operation after major loop completition if TCDn_CSR[ESG]
 * is enabled. TCDn_DLASTSGA is not a 32 byte boundary.
 */
bool EDMA_getScatherGatherConfigurationError(const EDMA_TypeDef * edma);

/**
 * @brief Reports source bus error.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[SBE] bit. The error is a bus error on
 * a source read.
 */
bool EDMA_getSourceBusError(const EDMA_TypeDef * edma);

/**
 * @brief Reports destination bus error.
 *
 * @param[in] edma eDMA peripheral to check error status
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the MP_ES[DBE] bit. The error is a bus error on
 * a destination write.
 */
bool EDMA_getDestinationBusError(const EDMA_TypeDef * edma);

/* TODO is there a single interrupt handler for all channels or each channel
has it's own IRQ?? */
/**
 * @brief Returns the eDMA Interrupt Request Status register content.
 *
 * @param[in] edma eDMA peripheral to get status from
 *
 * @returns the interrupt request status for each channel in one word
 */
uint32_t EDMA_getInterruptRequestStatusChannelRegisterContent(const EDMA_TypeDef * edma);
/* TODO ask about Hardware Request Status register */
/**
 * @brief Returns the eDMA Hardware Request Status register contet.
 *
 * @param[in] edma eDMA peripheral to get status from
 *
 * @returns the hardware request status for each channel in one word
 */
uint32_t EDMA_getHardwareRequestStatusRegisterContent(const EDMA_TypeDef * edma);

/**
 * @brief Returns the eDMA channel IRQ number.
 * @details As one interrupt is shader by two channels, the number
 * returned corresponds to the channel pair, i.e. 0/1, 2/3, 4/5 etc.
 *
 * @retval eDMA IRQ channel pair number.
 */
static inline uint32_t EDMA_getEDMAChannelInIrq(void)
{
    /* TODO maybe create stubs instead of ifdefs?? */
#if defined (ARM)
    /* TODO double check that the computation is ok */
    return __get_IPSR() - (uint32_t) EDMA_CH0_1_IRQn - 16U;
#else
    return 0;
#endif
}
/** @} */ /* eDMA global status API */

/**
 * @defgroup edma_global_channel_status eDMA global channel status API
 * @{
 */
/**
 * @brief Returns the eDMA Stop Status Register content
 * @param[in] edma eDMA peripheral to get status from
 *
 * @returns the stop status pending for each channel in one word
 */
uint32_t EDMA_getChannelStopStatusPendingRegisterContent(const EDMA_TypeDef * edma);
/** @} */ /* eDMA global channel status API */

/**
 * @defgroup edma_channel_control eDMA channel control API
 * @{
 */
/**
 * @brief Sets the sign extension
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel eDMA channel to configure
 * @param[in] sign_extension TODO
 *
 * @note Configures the CHn_CSR[SIGNEXT] register field.
 */
void EDMA_setSignExtension(EDMA_TypeDef * edma, uint8_t channel, uint8_t sign_extension);

/**
 * @brief Set the swap size
 * @details Swap data elements within the specified transfer. Swap occurs with
 * respect to the specified transfer size (SSIZE or DSIZE).
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel eDMA channel to configure
 * @param[in] size Swap size
 *
 * @note Configures the CHn_CSR[SWAP] register field.
 */
void EDMA_setSwapSize(EDMA_TypeDef * edma, uint8_t channel, enum swap_size size);

/**
 * @brief Stop channel transfer
 * @details For an active channel, stop any new transactions. Any transactions
 * that have already been issued are allow to complete. If the channel is
 * inactive, the stop reset will negate a subsequent service request.  After
 * stopping the channel, the ECX error flag will be set and an optional
 * error interrupt generated if stop_with_error is true, otherwise the CX flag
 * is set. The ECX/CX flag is automatically cleared after the stop request is
 * honored. Once set, it cannot be cleared by the programmer's model.
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel eDMA channel to configure
 * @param[in] stop_with_error determines, whether and error should be generated on stop
 */
void EDMA_channelStopChannel(EDMA_TypeDef * edma, uint8_t channel, bool stop_with_error);

/**
 * @brief Enables/disables channel error interrupt generation
 *
 * @param[in] edma eDMA peripheral to configure channel
 * @param[in] channel eDMA channel to configure
 * @param[in] new_state enables or disables feature
 *
 * @note Configures the CHn_CSR[EEI] register field.
 */
void EDMA_setChannelErrorInterrupt(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state);

/**
 * @brief Enables/disables asynchronous DMA request in stop mode for a channel
 * @details The enable asynchronous DMA request bit (EARQ) does not affect DMA
 * operations. When set, this bit allows the hardware service request enable bit
 * (ERQ) to propagate out of the DMA to the power controller. When cleared,
 * this bit masks the hardware service request enable bit to the power controller.
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel eDMA channel
 * @param[in] new_state enables or disables feature
 *
 * @note Configures the CHn_CSR[EARQ] register field.
 */
void EDMA_setAsynchronousDMARequestInStopMode(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state);

/**
 * @brief Enables/disables DMA channel hardware request
 * @details The DMA hardware request input signal and the enable request bit (ERQ)
 * must be asserted before a channel's hardware service request is accepted.
 * The state of the eDMA enable request bit does not affect a channel service
 * request made explicitly through software or channel linking. The state of the
 * ERQ bit does not affect the channel's START bit.
 *
 * @param[in] edma eDMA peripheral to configure
 * @param[in] channel eDMA channel to configure
 * @param[in] new_state enables or disables feature
 *
 * @note Configures the CHn_CSR[ERQ] register field.
 */
void EDMA_setDMAChannelHardwareRequestSignal(EDMA_TypeDef * edma, uint8_t channel, enum feature_state new_state);
/** @} */ /* eDMA channel control API */

/**
 * @defgroup edma_channel_status eDMA channel status API
 * @{
 */
/**
 * @brief Indicates whether the channel is active
 * @details Channel in execution is considered active.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if channel is active, false otherwise
 */
bool EDMA_isChannelActive(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Indicates whether the channel has completed (done) the major loop
 * @details Internally a Done flag indicates the eDMA has completed the major
 * loop. The eDMA engine sets the flag as CITER count reaches zero. The software
 * clears it, or the hardware when the channel is activated.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if channel has finished execution, false otherwise
 *
 * @note The Done flag must be cleared to write a 1 to the MAJORELINK bit.
 */
bool EDMA_isChannelDone(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether an error occured on eDMA channel
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true on error, false otherwise
 *
 * @note The function returns true if at least one error bit is set.
 */
bool EDMA_getChannelErrorPresent(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether the uncorrectable TCD error during channel execution happened.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[UCE] bit.
 */
bool EDMA_getChannelUncorrectableECCError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether a transfer has been canceled.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[ECX] bit.
 */
bool EDMA_getChannelTransferCanceledError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether a source address error happened.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[SAE] bit. The error is due to configuration
 * error detected in TCDn_SADDR field. TCDn_SADDR is inconsistent with TCDn_ATTR[SSIZE].
 */
bool EDMA_getChannelSourceAddressError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether a source offset error happened.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[SOE] bit. The error is due to configuration
 * error detected in TCDn_SOFF field. TCDn_SOFF is inconsistent with TCDn_ATTR[SSIZE].
 */
bool EDMA_getChannelSourceOffsetError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether a destination adress error happened.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[DAE] bit. The error is due to configuration
 * error detected in TCDn_DADDR field. TCDn_DADDR is inconsistent with TCDn_ATTR[DSIZE].
 */
bool EDMA_getChannelDestinationAddressError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports whether a destination offset error happened.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[DOE] bit. The error is due to configuration
 * error detected in TCDn_DOFF field. TCDn_DOFF is inconsistent with TCDn_ATTR[DSIZE].
 */
bool EDMA_getChannelDestinationOffsetError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Number of bytes or minor loop counter configuration error.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[NCE] bit. The error is due to configuration
 * error detected in TCDn_NBYTES or TCDn_CITER field. The flag is raised in case the
 * NBYTES is equal to zero or CITER is not equal to BITER. (TCDn_NBYTES is not
 * a multiple of of TCDn_ATTR[SSIZE] and TCDn_ATTR[DSIZE], or TCDn_CITER[CITER]
 * is equal to zero, or TCDn_CITER[ELINK] is not equal to TCDn_BITER[ELINK]).
 */
bool EDMA_getChannelNBytesCIterConfigurationError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Scatter/gather configuration error.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[SGE] bit. The error is due to configuration
 * error detected in TCDn_DLASTSGA field. This field is checked at the beginning
 * of a scatter/gather operation after major loop completition if TCDn_CSR[ESG]
 * is enabled. TCDn_DLASTSGA is not a 32 byte boundary.
 */
bool EDMA_getChannelScatherGatherConfigurationError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports source bus error.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[SBE] bit. The error is a bus error on
 * a source read.
 */
bool EDMA_getChannelSourceBusError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Reports destination bus error.
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns true if flag is set, false otherwise
 *
 * @note The function checks the CHn_ES[DBE] bit. The error is a bus error on
 * a destination write.
 */
bool EDMA_getChannelDestinationBusError(const EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Returns the eDMA channel Interrupt Status register content
 *
 * @param[in] edma eDMA peripheral to get status from
 * @param[in] channel eDMA channel
 *
 * @returns 1 if interrupt request for the channel is present, 0 otherwise
 *
 * @note The function checks the CHn_INT[INT] register field.
 */
uint32_t EDMA_getChannelInterruptRequestStatusChannelRegisterContent(const EDMA_TypeDef * edma, uint8_t channel);


/** @} */ /* eDMA channel status API */

/**
 * @defgroup edma_tcd_control eDMA transfer control descriptor API
 * @{
 */
/**
 * @brief Sets the data source address of given channel.
 * @details Source address of eDMA transfer.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] src_address memory address pointing to source data
 *
 * @note Sets the SADDR and SADDR_HIGH registers.
 */
void EDMA_setSourceAddress(EDMA_TCD_TypeDef * tcd, uint64_t src_address);
/**
 * @brief Sets the source address increment value.
 * @details The eDMA source address is updated after each subtransfer as
 * SADDR + SOFF.\n
 * e.g. the source memory port is 1 byte wide, thus SOFF = 1, for a 4 byte port
 * SOFF = 4.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] offset source address increment value
 *
 * @note One transfer represents the total number of bytes to be transferred.
 * A subtransfer should be understood as a transfer of a minimum allowed unit.
 *
 * @note Configures the SOFF register.
 */
void EDMA_setSignedSourceAddressOffset(EDMA_TCD_TypeDef * tcd, uint16_t offset);
/**
 * @brief Sets the source address modulo.
 * @details The SMOD (5-bit) value specifies, which lower address bits are
 * incremented, whereas the upper bits are left unchanged.\n
 * e.g.: SMOD = 4, 2^4 = 16\n
 * bits [0:4] are updated, bits [31:5] are left unchanged.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] modulo value (2^modulo) defining the lower bits to change
 *
 * @note Configures the TCDn_ATTR[SMOD] register field. A value of 0 disables this feature.
 *
 * @note Modulo addressing applies to cases, where the minor loop offset is enabled.
 */
void EDMA_setSourceAddressModulo(EDMA_TCD_TypeDef * tcd, uint8_t modulo);
/**
 * @brief Sets the source data transfer size.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] size transfer size
 *
 * @note Configures the TCDn_ATTR[SSIZE] register field.
 */
void EDMA_setSourceDataTransferSize(EDMA_TCD_TypeDef * tcd, enum data_transfer_size size);

/**
 * @brief Sets the destination address modulo.
 * @details The DMOD (5-bit) value specifies, which lower address bits are
 * incremented, whereas the upper bits are left unchanged.\n
 * e.g.: DMOD = 4, 2^4 = 16\n
 * bits [0:4] are updated, bits [31:5] are left unchanged.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] modulo value (2^modulo) defining the lower bits to change
 *
 * @note Configures the TCDn_ATTR[DMOD] register field. A value of 0 disables this feature.
 *
 * @note Modulo addressing applies to cases, where the minor loop offset is enabled.
 */
void EDMA_setDestinationAddressModulo(EDMA_TCD_TypeDef * tcd, uint8_t modulo);

/**
 * @brief Sets the destination data transfer size.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] size transfer size
 *
 * @note Configures the TCDn_ATTR[DSIZE] register field.
 */
void EDMA_setDestionationDataTransferSize(EDMA_TCD_TypeDef * tcd, enum data_transfer_size size);

/**
 * @brief Sets transfer size and minor loop offset.
 * @details Minor loop offset is an address offset value added to the final
 * source address (TCDn_SADDR) or destination address (TCDn_DADDR) upon minor
 * loop completion. Minor loop completion is when the channel has finished the
 * service request and has transferred NBYTES. The source enable bit (SMLOE)
 * specifies the minor loop offset value (MLOFF) to be applied to the source
 * address (TCDn_SADDR) upon minor loop completion. The destination enable bit
 * (DMLOE) specifies the minor loop offset (MLOFF) to be applied to the
 * destination address (TCDn_DADDR) upon minor loop completion. If the major
 * loop is complete, the minor loop offsets are ignored and the major loop
 * address offsets (TCDn_SLAST_SDA and TCDn_DLAST_SGA) are used to compute the
 * next TCDn_SADDR and TCDn_DADDR values.\n
 * The same offset value (MLOFF) is used for both source and destination minor
 * loop offsets. When enabled, the minor loop offset must be aligned to the
 * transfer size of the source or destination it is associated with. When either
 * minor loop offset is enabled (SMLOE set or DMLOE set), the NBYTES field is
 * reduced to 10 bits. When both minor loop offsets are disabled, the NBYTES
 * field is a 30-bits.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] src_minor_loop_offset_state enables/disables source minor loop offset (SMLOE)
 * @param[in] dst_minor_loop_offset_state enables/disables destination minor loop offset (DMLOE)
 * @param[in] minor_loop_offset sign-extended offset (MLOFF) applied to the
 * source or destination address to form the next-state value after the minor
 * loop completes. This parameter is unused if src/dst_minor_loop_offset_state
 * is disabled.
 * @param[in] count number of bytes (NBYTES) to transfer per service request. If
 * srd/dst_minor_loop_offset_state are both disabled, count is a 30-bit value,
 * 10-bit otherwise, so it may happen the value is truncated.
 *
 * @note Sets the TCDn_NBYTES_MLOFF register.
 */
void EDMA_setTransferSize(
    EDMA_TCD_TypeDef * tcd,
    enum feature_state src_minor_loop_offset_state,
    enum feature_state dst_minor_loop_offset_state,
    uint32_t minor_loop_offset,
    uint32_t count
);

/**
 * @brief Sets address and enables appropriate feature (SLAST/SDA).
 * @details Enables one of Last Source Address Adjustment (SLAST) or Store
 * Destination Address (SDA) feature and sets the address into register.\n
 * In case the of SLAST the address is added to the source address at the
 * completion of the major iteration count. This value can be used to restore
 * the source address to the initial value or adjust the address to reference
 * the next data structure.\n
 • This field uses two's complement notation for the final source address
 * adjustment.\n
 * In case of SDA the address points to the 32-bit-aligned memory location where
 * the destination address (DADDR) is to be stored in system memory. By saving
 * the final destination address in system memory via the SDA feature, you are
 * able to compute the size of a variable destination data buffer by simply
 * subtracting the beginning DADDR from the final, saved DADDR. This feature is
 * used together with the scatter/gather operation to prevent the loss of the
 * final DADDR, which is overwritten during the scatter/gather operation.\n
 * The "Store Destination Address" (SDA) value must be a 32-bit-aligned location
 * because the eDMA forces the lower two address bits of the SLAST_SDA field to
 * zero when SDA is enabled. The module performs this write operation when the
 * major loop is done; that is, when the major iteration count (CITER)
 * decrements to zero.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] feature feature selector
 * @param[in] address SLAST/SDA address
 *
 * @note Configures the TCDn_SLAST_SDA and TCDn_SLAST_SDA_HIGH registers.
 */
void EDMA_setSourceAddressFeature(EDMA_TCD_TypeDef * tcd, enum source_address_feature feature, uint64_t address);

/**
 * @brief Sets the data destination address of given channel.
 * @details Destination address of eDMA transfer.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] dst_address memory address pointing to destination data
 *
 * @note Configures the DADDR and DADDR_HIGH registers.
 */
void EDMA_setDestinationAddress(EDMA_TCD_TypeDef * tcd, uint64_t dst_address);

/**
 * @brief Sets Last Destination Address / Scatter Gather Address.
 * @details Enables one of Last Destination Address Adjustment (DLAST) or Scatter
 * Gather Address (SDA) feature and sets the address into register.\n
 * In case of DLAST the address is added to the destination address at the
 * completion of the major iteration count. This value can be used to restore
 * the destination address to the initial value or adjust the address to reference
 * the next data structure.\n
 • This field uses two's complement notation for the final source address
 * adjustment.\n
 * In case of SDA the address points to the beginning of a 0-modulo-32-byte
 * region containing the next transfer control descriptor to be loaded into this
 * channel. This channel reload is performed as the major iteration count
 * completes. The scatter/gather address must be 0-modulo-32-byte, otherwise
 * a configuration error is reported.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] feature feature selector
 * @param[in] address DLAST/SGA address
 * @note Configures the TCDn_DLAST_SGA & TCDn_DLAST_SGA_HIGH registers.
 */
void EDMA_setDestinationAddressFeature(EDMA_TCD_TypeDef * tcd, enum destination_address_feature feature, uint64_t address);

/**
 * @brief Sets the destination address increment value.
 * @details The eDMA destination address is updated after each subtransfer as
 * DADDR + DOFF.\n
 * e.g. the destination memory port is 1 byte wide, thus DOFF = 1, for a 4 byte port
 * DOFF = 4.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] offset destination address increment value
 *
 * @note One transfer represents the total number of bytes to be transferred.
 * A subtransfer should be understood as a transfer of a minimum allowed unit.
 *
 * @note Configures the DOFF register.
 */
void EDMA_setSignedDestinationAddressOffset(EDMA_TCD_TypeDef * tcd, uint16_t offset);

/**
 * @brief Sets minor loop channel linking.
 * @details Enables/disables channel-to-channel linking on minor/major-loop complete
 * and sets the channel to link to.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] minor_channel_linking enables/disables channel linking on minor-loop complete (ELINK)
 * @param[in] minor_loop_channel_link defines which channel should be started after the minor loop counter is exhausted
 * @param[in] major_iteration_count sets the current major loop count for the channel (CITER).
 * For ELINK enabled, the value is 9-bit otherwise 15-bit.
 *
 * @note Configures the TCDn_CITER_ELINK & TCDn_CSR registers.
 */
void EDMA_setMinorChannelLinking(
    EDMA_TCD_TypeDef * tcd,
    enum feature_state minor_channel_linking,
    uint8_t minor_loop_channel_link,
    uint16_t major_iteration_count
);

/**
 * @brief Sets major loop channel linking.
 * @details Enables/disables channel-to-channel linking on major-loop complete
 * and sets the channel to link to.
 *
 * @param[in] tcd tansfer control descriptor to configure
 * @param[in] major_channel_linking enables/disables channel linking on minor-loop complete (ELINK)
 * @param[in] major_loop_channel_link defines which channel should be started after the minor loop counter is exhausted
 *
 * @note Configures the TCDn_CSR[MAJORLINKCH] & TCDn_CSR[MAJORELINK] register fields.
 *
 * @note the MAJORLINKCH and MAJORELINK fields are updated simultaneously to
 * support dynamic channel link.
 */
void EDMA_setMajorChannelLinking(
    EDMA_TCD_TypeDef * tcd,
    enum feature_state major_channel_linking,
    uint8_t major_loop_channel_link
);

/**
 * @brief Sets the Transfer Control Mode.
 *
 * @param[in] tcd tansfer control descriptor to configure
 * @param[in] mode mode selector
 *
 * @note Configures the TCDn_CSR[TMC] register field.
 */
void EDMA_setTransferMode(EDMA_TCD_TypeDef * tcd, enum transfer_mode mode);

/**
 * @brief Sets the disable request.
 * @details If enabled, the eDMA hardware automatically clears the corresponding
 * ERQ bit when the current major iteration count reaches zero. The ERQ bit is
 * unaffected if disabled.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] new_state enables/disables the disable request (DREQ).
 *
 * @note Configures the TCDn_CSR[DREG] register field.
 */
void EDMA_requestDisable(EDMA_TCD_TypeDef * tcd, enum feature_state new_state);

/**
 * @brief Sets the generation of interrupt when major counter is half completed.
 * @details If enabled, the channel generates an interrupt request by setting
 * the appropriate bit in the INT register when the current major iteration
 * count reaches the halfway point. Specifically, the comparison performed by
 * the eDMA engine is (CITER == (BITER/2)). This halfway point interrupt request
 * is provided to support double-buffered, also known as ping-pong, schemes or
 * other types of data movement where the processor needs an early indication of
 * the transfer’s progress.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] new_state enables/disables the interrupt generation
 *
 * @note Configures the TCDn_CSR[INTHALF] register field.
 *
 * @warning If BITER = 1, do not use INTHALF. Use INTMAJOR instead.
 *
 * @see EDMA_setMajorIterationCompletesInterrupt
 */
void EDMA_setHalfCompleteInterrupt(EDMA_TCD_TypeDef * tcd, enum feature_state new_state);

/**
 * @brief Sets the interrupt generation when current major iteration count completes.
 * @details If enabled, the channel generates an interrupt request by setting
 * the appropriate bit in the INT when the current major iteration count (CITER)
 * reaches zero.
 *
 * @param[in] tcd transfer control descriptor to configure
 * @param[in] new_state enables/disables the interrupt generation
 *
 * @note Configures the TCDn_CSR[INTMAJOR] register field.
 */
void EDMA_setMajorIterationCompletesInterrupt(EDMA_TCD_TypeDef * tcd, enum feature_state new_state);

/**
 * @brief Channel transfer request.
 * @details Software initiated service request. The start bit is automatically
 * cleared upon channel execution.
 *
 * @param[in] edma eDMA to configure
 * @param[in] channel eDMA channel
 *
 * @note Configures the TCDn_CSR[START] register field.
 * @warning This function configures the EDMA registers, which may lead to triggering
 * the DMA channel to start a transfer.
 */
void EDMA_startChannel(EDMA_TypeDef * edma, uint8_t channel);

/**
 * @brief Channel transfer request bit configuration.
 * @details Sets the start channel bit of a TCD.
 *
 * @param[in] tcd transfer control descriptor to configure
 *
 * @note Configures the TCDn_CSR[START] register field.
 * @warning Use this function only to configure the TCD descriptors.
 *
 * @see EDMA_startChannel()
 */
void EDMA_setStartChannel(EDMA_TCD_TypeDef * tcd);
/** @} */ /* eDMA transfer control descriptor API block */

/** @} */ /* edma group */
/** @} */ /* llapi group */


#endif /* EDMA_GUARD */
