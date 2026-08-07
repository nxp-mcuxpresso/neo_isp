/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ispdrv_irq.c
 *
 * @brief Irq callbacks for Isp driver for FreeRtos (no firmware)
 *
 *******************************************************************************/

#include "FreeRTOS.h"
#include "ismanager.h"
#include "irq_handlers.h"

static void EDMA_IRQ_Handler(uint8_t aIsr)
{
    uint32_t channel = aIsr;
    /* The channel we get is in range 0 - 15, because two channels share one IRQ
    signal. However, every bit in INT_LOW register represents one channel. We
    know, if we get channel 7, that this can be eDMA channel 14 or 15. If we
    shift the INT_LOW register by channel * 2 (i.e. 14), we get 1 if the channel
    is in reality 14 or 2, if the IRQ corresponds to channel 15. So if we shift
    the INT_LOW by channel * 2 + 1, we get 0/1 based on the fact, if the channel
    is 14 or 15. We add this to the channel * 2 and we have the real channel.
    This implementation is used to speed-up to computation instead of a while
    loop. */
    assert(channel < 16);
    channel = 2U * channel;
    channel = (EDMA->INT_LOW >> (channel + 1U)) + channel;

        /* There is no status to distinguish between half-complete and complete
    interrupt. TODO handle this, for now we expect that the interrupt is only
    full complete, but that's wrong and dangerous. */
    assert(channel < EDMA_CHANNEL_COUNT);
    edma_channel_handler(channel, EDMA);

    portYIELD_FROM_ISR(pdTRUE);
}

/* ISP Processing Interrupt - Context 0*/
void ISP_IRQHandler(void)
{
    (void)isp_irq_handler(0, (void *) ISP);
    portYIELD_FROM_ISR(pdTRUE);
}

/* CAMERAMIX EDMA error interrupt*/
void DMA5_4_ERROR_IRQHandler(void)
{
    edma_error_handler(0, (void *) EDMA);
    portYIELD_FROM_ISR(pdTRUE);
}

/* CAMERAMIX EDMA channel 0 interrupt*/
void DMA5_4_0_1_IRQHandler(void)
{
    EDMA_IRQ_Handler(0);
}

/* CAMERAMIX EDMA channel 2 interrupt*/
void DMA5_4_2_3_IRQHandler(void)
{
    EDMA_IRQ_Handler(1);
}

/* CAMERAMIX EDMA channel 4 interrupt*/
void DMA5_4_4_5_IRQHandler(void)
{
    EDMA_IRQ_Handler(2);
}

/* CAMERAMIX EDMA channel 6 interrupt*/
void DMA5_4_6_7_IRQHandler(void)
{
    EDMA_IRQ_Handler(3);
}

/* CAMERAMIX EDMA channel 8 interrupt*/
void DMA5_4_8_9_IRQHandler(void)
{
    EDMA_IRQ_Handler(4);
}

/* CAMERAMIX EDMA channel 10 interrupt*/
void DMA5_4_10_11_IRQHandler(void)
{
    EDMA_IRQ_Handler(5);
}

/* CAMERAMIX EDMA channel 12 interrupt*/
void DMA5_4_12_13_IRQHandler(void)
{
    EDMA_IRQ_Handler(6);
}

/* CAMERAMIX EDMA channel 14 interrupt*/
void DMA5_4_14_15_IRQHandler(void)
{
    EDMA_IRQ_Handler(7);
}

/* CAMERAMIX EDMA channel 16 interrupt*/
void DMA5_4_16_17_IRQHandler(void)
{
    EDMA_IRQ_Handler(8);
}

/* CAMERAMIX EDMA channel 18 interrupt*/
void DMA5_4_18_19_IRQHandler(void)
{
    EDMA_IRQ_Handler(9);
}

/* CAMERAMIX EDMA channel 20 interrupt*/
void DMA5_4_20_21_IRQHandler(void)
{
    EDMA_IRQ_Handler(10);
}

/* CAMERAMIX EDMA channel 22 interrupt*/
void DMA5_4_22_23_IRQHandler(void)
{
    EDMA_IRQ_Handler(11);
}

/* CAMERAMIX EDMA channel 24 interrupt*/
void DMA5_4_24_25_IRQHandler(void)
{
    EDMA_IRQ_Handler(12);
}

/* CAMERAMIX EDMA channel 26 interrupt*/
void DMA5_4_26_27_IRQHandler(void)
{
    EDMA_IRQ_Handler(13);
}

/* CAMERAMIX EDMA channel 28 interrupt*/
void DMA5_4_28_29_IRQHandler(void)
{
    EDMA_IRQ_Handler(14);
}

/* CAMERAMIX EDMA channel 30 interrupt*/
void DMA5_4_30_31_IRQHandler(void)
{
    EDMA_IRQ_Handler(15);
}
