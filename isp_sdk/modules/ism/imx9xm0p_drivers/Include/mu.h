/*
 * Copyright 2022, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MU_GUARD
#define MU_GUARD

/**
 * @file mu.h
 */

#include "platform.h"

/**
 * @addtogroup llapi
 * @{
 */

/**
 * @addtogroup llmu MU (Messaging Unit) low level API
 * @details The Messaging Unit is a very simple full-duplex peripheral consisting
 * of four 32-bit Rx & Tx buffers. To transfer a message, just simply write
 * data into Tx buffers 0 - 3. An interrupt can be enabled on any of the four
 * Rx/Tx buffers in any combination.\n
 * Once data were written to the Rx buffer by HW, an interrupt is raised. This
 * allows the user to have four separate buffers each four bytes wide or two
 * single buffer each eight bytes or any combination the users needs.\n The Tx
 * interrupt if enabled is raised once Rx data has been read. This represents
 * data transmission confirmation - acknowledge.\n
 * If there is no need to transfer data, just to signal the other side, the MU
 * has four generic signals - Generic Interrupt Request.\n
 * In case we need to transfer more than 16 bytes, an external memory must be
 * used. In such a case the 16 bytes should be used to transfer the address into
 * external memory where the data is stored and size.
 * @{
 */
/**
 * @defgroup mu_control MU control functions
 * @details These functions serve to configure the MU peripheral.
 * @{
 */
/**
 * @brief Enables Tx interrupt.
 * @details A Tx interrupt is raised once transmitted data have been read by the
 * Rx side. This mechanism works as an acknowledge.
 *
 * @param[in] mu Messaging unit to configure.
 * @param[in] mask Mask of Tx interrupts to enable.
 */
static inline void MU_enableTxInterrupts(MU_TypeDef * mu, uint32_t mask)
{
    mu->TCR |= (uint32_t) (mask & (MU_TCR_TIE3_Msk | MU_TCR_TIE2_Msk | MU_TCR_TIE1_Msk | MU_TCR_TIE0_Msk));
}

/**
 * @brief Disables Tx interrupts.
 *
 * @param[in] mu Messaging unit to configure.
 * @param[in] mask Mask of Tx interrupts to disable.
*/
static inline void MU_disableTxInterrupts(MU_TypeDef * mu, uint32_t mask)
{
    mu->TCR &= (uint32_t) ~(mask & (MU_TCR_TIE3_Msk | MU_TCR_TIE2_Msk | MU_TCR_TIE1_Msk | MU_TCR_TIE0_Msk));
}

/**
 * @brief Enables Rx interrupts.
 * @details An Rx interrupt is raised once data have written into Rx registers.
 *
 * @param[in] mu Messaging unit to configure.
 * @param[in] mask Mask of Rx interrupts to enable.
*/
static inline void MU_enableRxInterrupts(MU_TypeDef * mu, uint32_t mask)
{
    mu->RCR |= (uint32_t) (mask & (MU_RCR_RIE3_Msk | MU_RCR_RIE2_Msk | MU_RCR_RIE1_Msk | MU_RCR_RIE0_Msk));
}

/**
 * @brief Disables Rx interrupts.
 *
 * @param[in] mu Messaging unit to configure.
 * @param[in] mask Mask of Rx interrupts to disable.
 */
static inline void MU_disableRxInterrupts(MU_TypeDef * mu, uint32_t mask)
{
    mu->RCR &= (uint32_t) ~(mask & (MU_RCR_RIE3_Msk | MU_RCR_RIE2_Msk | MU_RCR_RIE1_Msk | MU_RCR_RIE0_Msk));
}

/**
 * @brief Enables Generic Interrupts for generic signaling.
 *
 * @param[in] mu Messaging unit to configure.
 * @param[in] mask Mask of GI to enable.
*/
static inline void MU_enableGeneralInterrupts(MU_TypeDef * mu, uint32_t mask)
{
    mu->GIER |= (uint32_t) (mask & (MU_GIER_GIE3_Msk | MU_GIER_GIE2_Msk | MU_GIER_GIE1_Msk | MU_GIER_GIE0_Msk));
}

/**
 * @brief Disables Generic Interrupts.
 *
 * @param[in] mu Messaging unit to configure.
 * @param[in] mask Mask of GI to disable.
*/
static inline void MU_disableGeneralInterrupts(MU_TypeDef * mu, uint32_t mask)
{
    mu->GIER &= (uint32_t) ~(mask & (MU_GIER_GIE3_Msk | MU_GIER_GIE2_Msk | MU_GIER_GIE1_Msk | MU_GIER_GIE0_Msk));
}
/** @} */ /* mu_control group */

/**
 * @defgroup mu_status MU status functions
 * @details These functions serve to get status of the MU.
 * @{
 */
/**
 * @brief Returns whether a given Generic Interrupt has been raised.
 *
 * @param[in] mu Messaging unit to get status from.
 * @param[in] mask Mask of GI to get status from.
 */
static inline bool MU_isGeneralInterruptReady(MU_TypeDef * mu, uint32_t mask)
{
    return 0U == (uint32_t) (mu->GSR & (uint32_t) (mask & (MU_GSR_GIP3_Msk | MU_GSR_GIP2_Msk | MU_GSR_GIP1_Msk | MU_GSR_GIP0_Msk)));
}
/** @} */

/**
 * @defgroup mu_comm MU communication interface.
 * @details This set of functions serves to communicate over the MU.
 * @{
 */
static inline void MU_triggerGeneralInterrupt(MU_TypeDef * mu, uint32_t mask)
{
    mu->GCR &= (uint32_t) (mask & (MU_GCR_GIR3_Msk | MU_GCR_GIR2_Msk | MU_GCR_GIR1_Msk | MU_GCR_GIR0_Msk));
}

void MU_writeBuffer(MU_TypeDef * mu, uint32_t buffer);

static inline void MU_writeTxBuffer(MU_TypeDef * mu, uint8_t buffer, uint32_t data)
{
    mu->TR[buffer] = data;
}

static inline uint32_t MU_readRxBuffer(MU_TypeDef * mu, uint8_t buffer)
{
    return mu->RR[buffer];
}
/** @} */ /* mu_comm group */

/** @} */ /* mu_driver group */
/** @} */ /* llapi group */

#endif /* _MU_GUARD_ header guard */
