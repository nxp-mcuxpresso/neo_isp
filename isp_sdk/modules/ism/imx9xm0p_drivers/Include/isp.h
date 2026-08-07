/*
 * Copyright 2022, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ISP_GUARD
#define ISP_GUARD


#include "platform.h"

/**
 * @addtogroup llapi
 * @{
 */

/**
 * @defgroup isp_params ISP parameters
 * @{
 */
/**
 * @brief Number of pixels per second the ISP can process.
 */
#define ISP_COMPUTATION_POWER_PPS           (uint32_t) (500U * 1024U * 1024U)

/**
 * @brief ISP registers start address from eDMA view.
 *
 * @details The ISP dedicated eDMA has different address space compared to the
 * address space of M0+ core. Thus the registers are located at different
 * address.
 */
#define EDMA_VIEW_ISP_ADDRESS_SPACE_REGISTERS_START     ((uint32_t) 0x200000U)

/**
 * @brief ISP local memeories start address from eDMA view.

 * @details The ISP dedicated eDMA has different address space compared to the
 * address space of M0+ core. Thus the memories are located at different
 * address.
 */
#define EDMA_VIEW_ISP_ADDRESS_SPACE_MEMORIES_START      ((uint32_t) 0x3E0000U)
/** @} */ /* isp_params group */

/**
 * @defgroup llisp ISP low level API
 * @brief The ISP LLAPI is layed out across multiple logical groups.
 * @details The ISP peripheral processes the input images. The idea is to
 * preload the ISP registers (context) from an external memory (DDR RAM) as
 * a 1:1 copy. From this perspective the API is shortened and contains only
 * functions to manipulate the register addresses 0h - 20h, namely:\n
 * SOFT_RESET\n
 * BUS_TXPARAM\n
 * REG_XFR_DIS\n
 * CSI_CTRL\n
 * FRAME_NUM\n
 * REG_SHD_CTRL\n
 * REG_SHD_CMD\n
 * TRIG_CAM0\n
 * as these represent some control functionality.
 * The rest of registers are considered to be configured by copying data via eDMA.
 * @{
 */
/**
 * @defgroup isp_cfg_options ISP configuration types and options.
 * @{
 */
/**
 * @brief Bus transaction burst size.
 *
 * @see ISP_setBurstSize()
 */
enum burst_size
{
    BURST_SIZE_4_BEATS,
    BURST_SIZE_8_BEATS,
    BURST_SIZE_16_BEATS,
};
/** @} */   /* isp_cfg_options group */

/**
 * @defgroup isp_control ISP global configuration functions
 * @details Set of ISP configuration functions allowing to control the ISP global
 * functionality like quality of service, stopping the pipeline, triggering the
 * ISP pipeline etc. This set of functions doesn't contain funstions for
 * configuring the pipeline stages.
 * @{
 */
/**
 * @brief Pipeline Soft Reset.
 * @details The pipeline execution is stopped immediately. Any pending
 * transactions on the bus interface are completed.
 *
 * @param[in] isp peripheral to configure.
 *
 * @retval 0 on success
 * @retval 1 on timeout
 *
 * @note The reset should finish in 100 cycles of register configuration.
 * If this is not the case, a non-zero value is returned indicating timeout
 * error.
 *
 * @note Configures the SOFT_RESET[SOFT_RESET] register field.
 */
uint32_t ISP_softReset(ISP_TypeDef * isp);

/**
 * @brief Pipeline Hard Reset.
 * @details The pipeline execution is stopped immediately. The bus operations
 * are reset without compliance to the bus protocol.
 *
 * @param[in] isp peripheral to configure.
 *
 * @retval 0 on success
 * @retval 1 on timeout
 *
 * @note The reset should finish in 100 cycles of register configuration.
 * If this is not the case, a non-zero value is returned indicating timeout
 * error.
 *
 * @note Configures the SOFT_RESET[HARD_RESET] registe field.
 */
uint32_t ISP_hardReset(ISP_TypeDef * isp);

/**
 * @brief Initializes the pipeline.
 * @details A soft reset followed by a hard reset is performed to initialize the
 * pipeline.
 *
 * @param[in] isp peripheral to configure.
 *
 * @retval 0 on success
 * @retval 1 on soft reset timeout
 * @retval 2 on hard reset timeout
 *
 * @note Invokes the ISP_softReset and ISP_hardReset functions.
 */
uint32_t ISP_initializePipeline(ISP_TypeDef * isp);

/**
 * @brief Sets the burst size bus transaction parameter.
 * @details
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] size Number of beats within a single burst on read
 * transactions from ISP.
 *
 * @note Configures the BUS_TXPARAM[BSIZE] register field.
 */
void ISP_setBurstSize(ISP_TypeDef * isp, enum burst_size size);

/**
 * @brief Sets the QoS Escalation trigger threshold.
 * @details
 *
 * @param[in] isp ISP peripheral to configure
 * @param[in] value Value used to compute the threshold level -
 * level at which the QoS Escalation signal is set on the
 * output from ISP peripheral.
 *
 * @remark Threshold is calculated as 64 * value; value e <1;0xFE>.\n
 * Value = 0 => threshold = 10\n
 * Value = 0xFF => feature disabled.
 *
 * @note Configures the BUS_TXPARAM[POSTQOS] register field.
 */
void ISP_setPostQoSEscalationThreshold(ISP_TypeDef * isp, uint16_t value);

/**
 * @brief Outstanding Transaction High Threshold
 * @details TODO
 *
 * @param[in] isp IPS peripheral to configure.
 * @param[in] threshold
 *
 * @note Configures the BUS_TXPARAM[OTHT] register field.
 */
void ISP_setOutstandingTransactionHighThreshold(ISP_TypeDef * isp, uint16_t threshold);

/**
 * @brief Outstanding Transaction Low Threshold
 * @details TODO
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] threshold
 *
 * @note Configures the BUS_TXPARAM[OTLT] register field.
 */
void ISP_setOustandingTransactionLowThreshold(ISP_TypeDef * isp, uint16_t threshold);

/**
 * @brief Configures transfer error on register access behavior.
 * @details When enabled read/write access to address holes, write access to
 * read-only registers do not provide transfer error. Using this mechanism, the
 * single external DMA descriptor can save/restore context processing registers.
 * This feature does not effect transfer error response of partial writes i.e.
 * partial writes (less than 32-bit writes) always provide an error response.
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] new_state Enables or disables feature.
 *
 * @note Configures the REG_XFR_DIS[XFR_ERR_DIS] bit field.
 */
void ISP_setTransferErrorDisable(ISP_TypeDef * isp, enum feature_state new_state);

/**
 * @brief Sets the streaming mode feature.
 * @details If enabled, the ISP works in streaming mode and the ISI peripheral
 * directly feeds the ISP pipeline. If disabled, the memory-to-memory mode is
 * active and input image(s) are read from system memory.
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] new_state Enables or disables feature.
 *
 * @note Configures the CSI_CTL[CSI_EN] bit field.
 */
void ISP_setStreamingMode(ISP_TypeDef * isp, enum feature_state new_state);

/**
 * @brief Sets the dual exposure VSYNC lock feature.
 * @details Setting this feature enables the ISP to lock onto ongoing CSI
 * frame. This field should always be set to 1 if the CSI interface is active
 * when the Neo block is triggered. The locking incurs a latency of 1 frame.
 * If the CSI interface is inactive when the Neo is triggered and CSI is enabled
 * later then this bit can be set to 0 and this incurs no frame latency.
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] new_state Enables or disables feature.
 *
 * @note Configures the CSI_CTL[DEVL] bit field.
 */
void ISP_setDualExposureVsyncLock(ISP_TypeDef * isp, enum feature_state new_state);

/**
 * @brief Sets the single step frame mode.
 * @details Enabling this feature enables ISP to single step over the CSI frame.
 * When enabled only single CSI is outputted then CSI frame is stopped.
 * A re-trigger is required for restarting the CSI frame processing. If
 * disabled, then every CSI frame is processed.
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] new_state Enables or disables feature.
 *
 * @note Configures the CSI_CTL[SSEN] bit field.
 */
void ISP_setSingleStepMode(ISP_TypeDef * isp, enum feature_state new_state);

/**
 * @brief Sets the virtual ID for line path 0/1.
 * @details TODO
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] linepath for linepath 0 configures the VID0, for linepath 1
 * configures VID1
 * @param[in] id virtual channel ID
 *
 * @note Configure the CSI_CTL[VID1] or CSI_CTL[VID0] register fields.
 */
void ISP_setVirtualChannelID(ISP_TypeDef * isp, uint8_t linepath, uint8_t id);

/*
 * The shadowed frame number and current frame API is not provided as there is
 * currently no use of it.
 * FRAME_NUM register.
 */

/**
 * @brief Sets the register shadowing control.
 * @details If enabled, register shadowing must be done manually. If disabled,
 * register shadowing is done at every frame/context start.
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] new_state Enables or disables manual register shadowing.
 *
 * @note Configures the REG_SHD_CTRL[CTRL] bit field.
 * @see ISP_setRegisterShadowingCommand
 */
void ISP_setRegisterShadowingControl(ISP_TypeDef * isp, enum feature_state new_state);

/**
 * @brief Grants ISP register shadowing.
 * @details Once the ISP context has been written into ISP registers, invoking
 * this function grants to shadow the registers allowing to write new context
 * into the registers.
 *
 * @param[in] isp ISP peripheral to configure.
 *
 * @retval 0 if successful.
 * @retval 1 if register shadowing can't be started due to an ongoing shadowing
 * process.
 *
 * @note Writes the REG_SHD_CMD register.
 * @see ISP_setRegisterShadowingControl
 */
uint32_t ISP_setRegisterShadowingCommand(ISP_TypeDef * isp);

/**
 * @brief Returns the current frame number.
 *
 * @param[in] isp ISP peripheral to configure.
 *
 * @retval Current frame number.
 */
uint32_t ISP_getFrameNumber(ISP_TypeDef * isp);

/**
 * @brief Start the ISP pipeline processing.
 * @details If multiple trigger requests are received by the ISP during pipeline
 * operation than it can handle, an error is raised (see INT_STAT0[S_TRIG_ERR]
 * register bit field) and the corresponding write is ignored.
 *
 * @param[in] isp ISP peripheral to configure.
 *
 * @note Writes the TRIG_CAM0[TRIGGER] register.
 *
 * @see ISP_getTriggerErrorStatus
 */
void ISP_trigger(ISP_TypeDef * isp);

/**
 * @brief Configures ISP interrupts.
 * @details
 *
 * @param[in] isp ISP peripheral to configure.
 * @param[in] interrupts Mask of interrupts to configure.
 * @param[in] new_state Enable or disable provided interrupts.
 *
 * @note Writes the INT_EN0 register.
 * @note For interrupts, use or-ed ISP_INT_EN0_# masks.
 */
void ISP_setInterrupts(ISP_TypeDef * isp, uint32_t interrupts, enum feature_state new_state);
/** @} */ /* ISP configuation functions */

/**
 * @defgroup isp_status ISP status functions
 * @details This set of functions helps to find out whether a given event has
 * occured or not.
 * @{
 */
/**
 * @brief Returns if ISP is running or idle.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if ISP is runing, false otherwise.
 *
 * @note Returns the INT_STAT0[BUSY] flag state.
 */
bool ISP_isIspRunning(const ISP_TypeDef * isp);
/**
 * @brief Returns whether ISP can be triggered.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if ISP can be triggered, false otherwise.
 *
 * @note Checks the TRIG_CAM0 register.
 */
bool ISP_isTriggerAvailable(const ISP_TypeDef * isp);

/**
 * @brief Returns the trigger error status.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if error is present, false otherwise.
 *
 * @note Returns the INT_STAT0[S_TRIG_ERR] flag state.
 * @see ISP_trigger
 */
bool ISP_getTriggerErrorStatus(const ISP_TypeDef * isp);

/**
 * @brief Returns the CSI timing error status.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if error is present, false otherwise.
 *
 * @note Returns the INT_STAT0[S_CSI_TERR] flag state.
 */
bool ISP_getCsiTimingErrorStatus(const ISP_TypeDef * isp);

/**
 * @brief Returns the bus error status.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns 0 if no error; non-zero value otherwise.\n
 * Bit 0: read bus interface of linepath 0\n
 * Bit 1: read bus interface of linepath 1\n
 * Bit 2: write port 0 interface\n
 * Bit 3: write port 1 interface\n
 *
 * @note Returns the INT_STAT0[S_BUS_ERR] register field.
 */
uint8_t ISP_getBusError(const ISP_TypeDef * isp);

/**
 * @brief Returns the status of DRC frame processing on pipeline 2.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if processing finished, false otherwise.
 *
 * @note Returns the INT_STAT0[S_DRCD] flag state.
 */
bool ISP_getDrcDoneStatus(const ISP_TypeDef * isp);

/**
 * @brief Returns the status of statistics computation inside pipeline 1.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if statistics finished, false otherwise.
 *
 * @note Returns the INT_STAT0[S_STATD] flag state.
 */
bool ISP_getStatisticsDoneStatus(const ISP_TypeDef * isp);

/**
 * @brief Returns the status of frame done on pipeline 2.
 * @details The frame and all bus transactions are finished.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if frame done, false otherwise.
 *
 * @note Returns the INT_STAT0[S_FD2] flag state.
 */
bool ISP_getFrameDone2Status(const ISP_TypeDef * isp);

/**
 * @brief Returns the status of frame done on pipeline 1.
 * @details At this stage the color measurements are available.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if frame done, false otherwise.
 *
 * @note Returns the INT_STAT0[S_FD1] flag state.
 */
bool ISP_getFrameDone1Status(const ISP_TypeDef * isp);

/**
 * @brief Returns the status of frame start on pipeline 2.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if frame processing started, false otherwise.
 *
 * @note Returns the INT_STAT0[S_FS2] flag state.
 */
bool ISP_getFrameStart2Status(const ISP_TypeDef * isp);
/**
 * @brief Returns the status of frame start on pipeline 1.
 *
 * @param[in] isp ISP peripheral to get status from.
 *
 * @returns true if processing startd, false otherwise.
 *
 * @note Returns the INT_STAT0[S_FS1] flag state.
 */
bool ISP_getFrameStart1Status(const ISP_TypeDef * isp);
/** @} */ /* ISP status functions */

/**
 * @defgroup isp_context ISP context functions.
 * @details This set of functions helps to get addresses of ISP registers and
 * memories.
 * @{
 */
/**
 * @brief Returns the image size in pixels.
 * @details The size is computed from width and height currently stored in
 * registers.
 *
 * @param[in] isp ISP to get image size from.
 *
 * @note As the registers are shadowed, the size is computed for an image not
 * necessarilly being currently processed.
 */
uint32_t ISP_getImageSizeInPixels(const ISP_TypeDef * isp);

/**
 * @brief Returns the input registers size.
 *
 * @retval Input registers size.
 */
uint32_t ISP_getInputRegistersSize(void);

/**
 * @brief Returns the output registers region size.
 *
 * @retval Output registers size.
 */
uint32_t ISP_getOutputRegistersSize(void);

/**
 * @brief Returns the size of input ISP memory in bytes.
 *
 * @retval ISP input memory size.

 * @note Input memory means memory which gets updated with values used by ISP.
 */
uint32_t ISP_getInputMemoriesSize(void);

/**
 * @brief Returns the size of output ISP memory in bytes.
 *
 * @retval ISP output memory size.
 *
 * @note Output memory means memory which gets updated by ISP and must be read.
 * Due to the design the output memory unfortunately represents the whole
 * memory space.
 */
uint32_t ISP_getOutputMemoriesSize(void);

/**
 * @brief Retrieves the base address of the input registers for the ISP.
 *
 * This function returns the memory-mapped address within the ISP's address
 * space where the pipeline configuration starts.
 *
 * @return The absolute address of the pipeline configuration register.
 *
 * @note Destination address for EDMA transfer from DDR to ISP.
 */
uint64_t ISP_getInputRegistersAddress(void);

/**
 * @brief Retrieves the base address of the output registers for the ISP.
 *
 * This function returns the memory-mapped address at the beginning of the `ALIAS`
 * block within the ISP's address space.
 *
 * @return The absolute address of the `ALIAS` register block.
 *
 * @note Source address for EDMA transfer from ISP to DDR.
 */
uint64_t ISP_getOutputRegistersAddress(void);

/**
 * @brief Retrieves the base address of the output memories for the ISP.
 *
 * This function returns the starting address of the ISP's memory-mapped output
 * memory region.
 *
 * @return The address of the ISP output memories.
 *
 * @note Source address for EDMA transfer from ISP to DDR.
 */
uint64_t ISP_getOutputMemoriesAddress(void);

/**
 * @brief Retrieves the base address of the input memories for the ISP.
 *
 * This function returns the starting address of the ISP's memory-mapped input
 * memory region.
 *
 * @return The address of the ISP input memories.
 *
 * @note Destination address for EDMA transfer from DDR to ISP.
 */
uint64_t ISP_getInputMemoriesAddress(void);

/**
 * @brief Computes the address of the input registers for a given ISP context.
 *
 * This function returns the memory-mapped address within the context address
 * space in DDR.
 *
 * @param context The address of the ISP context.
 * @return The absolute address of the `IMG_CONF_CAM0` register for the given context.
 *
 * @note Source address for EDMA transfer from DDR to ISP.
 */
uint64_t CTX_getInputRegistersAddress(uint64_t context);

/**
 * @brief Computes the address of the output registers for a given ISP context.
 *
 * This function returns the memory-mapped address within the context address
 * space in DDR.
 *
 * @param context The address of the ISP context.
 * @return The address of the `ALIAS` register for the given context.
 *
 * @note Destination address for EDMA transfer from ISP to DDR.
 */
uint64_t CTX_getOutputRegistersAddress(uint64_t context);

/**
 * @brief Computes the base address of the output memories within a given ISP context.
 *
 * This function returns the memory-mapped address within the context address
 * space in DDR.
 *
 * @param context The address of the ISP context.
 * @return The address of the output memories for the given context.
 *
 * @note Destination address for EDMA transfer from ISP to DDR.
 */
uint64_t CTX_getOutputMemoriesAddress(uint64_t context);

/**
 * @brief Computes the base address of the input memories for a given ISP context.
 *
 * This function retuns the memory-mapped address within the context address
 * space in DDR.
 *
 * @param context The address of the ISP context.
 * @return The address of the input memories for given context.
 *
 * @note Source address for EDMA transfer from DDR to ISP.
 */
uint64_t CTX_getInputMemoriesAddress(uint64_t context);
/** @} */ /* isp_addresses */

/** @} */ /* llisp group */
/** @} */ /* llapi group */

#endif  /* ISP_GUARD */
