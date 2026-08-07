/*
 * Copyright 2022-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IRQ_HEADER_GUARD_
#define IRQ_HEADER_GUARD_

/**
 * ISP IRQ handler.
 *
 * @param[in] id Not used.
 * @param[in] regs ISP registers address location
 */
int isp_irq_handler(int id, void * regs);

/**
 * EDMA error IRQ handler.
 *
 * @param[in] id DMA channel.
 * @param[in] regs EDMA registers.
 */
int edma_error_handler(int id, void * regs);

/**
 * DMA IRQ handler.
 *
 * @param[in] id DMA channel.
 * @param[in] regs EDMA registers.
 */
int edma_channel_handler(int id, void * regs);

#endif /* IRQ_HEADER_GUARD_ */
