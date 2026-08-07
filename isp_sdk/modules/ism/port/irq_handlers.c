/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file irq.c
 *
 * Platform specific IRQ handlers required by ISP Subdomain Manager (ISP, DMA).
 */

#include "platform.h"
#include "ctx_mngr.h"
#include "ehandlers.h"
#include "evtbroker.h"
#include "imp_definition.h"
#include "irq_handlers.h"
#include "comm.h"
#include "timestamps.h"

int edma_error_handler(int id, void * regs)
{
    plt_assert(id >= 0 && id < EDMA_CHANNEL_COUNT);
    uint32_t channel = (uint32_t) id;
    int retval = 0;

    EDMA_TypeDef * dma = (EDMA_TypeDef *) regs;
    /* Handle errors */
    /* Based on the channel, we can determine, whether they belong to next request
     * or currently being processed request */
    enum requestId req;
    if ((channel % 4U) < 2U)
    {
        req = NEXT;
        CM_setErrors(NEXT, dma->ES);
    }
    else
    {
        req = ACTIVE;
        CM_setErrors(ACTIVE, dma->ES);
    }

    struct event ev = {0U, 0U, (uint32_t) req, (uint32_t) EI_DMA_ERROR};

    if (0 != EB_publishEventFromISR(&ev))
    {
        /* This should never happen if the synchronization is correct. If
        there is something wrong, it's bug and fatal error! */
        retval = (int) IMP_SYSTEM_EDMA_QUEUE_FULL;
    }

    return retval;
}

int edma_channel_handler(int id, void * regs)
{
    plt_assert(id >= 0 && id < EDMA_CHANNEL_COUNT);
    EDMA_TypeDef * edma = (EDMA_TypeDef *) regs;

    int retval       = 0;
    uint32_t channel = (uint32_t) id;
    struct event msg = {0U, 0U, channel, (uint32_t) RESERVED};

    plt_assert(channel < EDMA_CHANNEL_COUNT);

    /* Clear the flag. */
#if !defined (ISIM)
    edma->Channel[channel].INT |= EDMA_CHn_INT_INT_Msk;
#else
    edma->Channel[channel].INT &= (uint32_t) ~EDMA_CHn_INT_INT_Msk;
    /* Clearing INT_LOW is only for simulator, on real HW this is a read only
    bit which gets cleared, once no interrupt pending flags are present. */
    edma->INT_LOW &= (uint32_t) ~((uint32_t) 1U << channel);
#endif

    /* The EDMA may end-up in an error even after transfer, so we need to
    check the error status here. */
    uint32_t dma_channel_type = channel % 4U; /* determines, whether its channel 0/1 or 3 (2 is unused)*/
    if (true == CM_handleEdmaError())
    {
        msg.id = (uint32_t) EI_DMA_ERROR;
        msg.data0 = dma_channel_type < 2U ? (uint32_t) NEXT : (uint32_t) ACTIVE;
    }
    /* There is no distinction between DMA half transfer vs full transfer complete.
    The DMA is configured by context manager for full transfers only, so we expect
    this to be true without checking whether its true or not. */
    else
    {
        msg.id      = (uint32_t) EI_DMA_DONE;
        if ((channel % 4U) == 0U)
        {
            msg.data0 = (uint32_t) EI_DDR_TO_REGISTERS_TRANSFER_DONE;
        }
        else if ((channel % 4U) == 1U)
        {
            msg.data0 = (uint32_t) EI_DDR_TO_MEMORIES_TRANSFER_DONE;
        }
        else if ((channel % 4U) == 2U)
        {
            msg.data0 = (uint32_t) EI_REGISTERS_TO_DDR_TRANSFER_DONE;
        }
        else /* ((channel % 4U) == 3U) */
        {
            msg.data0 = (uint32_t) EI_MEMORIES_TO_DDR_TRANSFER_DONE;
        }
    }

    if (0 != EB_publishEventFromISR((struct event *) &msg))
    {
        /* This should never happen if the synchronization is correct. If
        there is something wrong, it's bug and fatal error! */
        retval = IMP_SYSTEM_EDMA_QUEUE_FULL;
    }

    return retval;
}

int isp_irq_handler(int id, void * regs)
{
    int retval = 0;
    (void) id;  /* We have single ISP on imx95, from this perspective we don't
    care about the ID. */

    ISP_TypeDef * isp = (ISP_TypeDef *) regs;

    /*
        Handle all ISP events, although there won't be much most probably.
        The ISP will generate interrupts related to statistics data availability
        and frame done. Potentially some errors.
    */
    /* ISP pipeline 1 start event */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_FS1_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_FS1_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_FS1_Msk;
#endif

        /*
        The ISP has started, this means, that the context has been copied into
        shadow registers and we can use it again for new frame. Notify about the
        context availability */
        struct event ev = {0U, 0U, 0U, EI_ISP_PIPELINE_STARTED};
        if (0 != EB_publishEventFromISR(&ev))
        {
            /* This should never happen, as the queue should have enough capacity
            to store all required events. If there are more,
            it's a bug and fatal error! */
            retval = IMP_SYSTEM_EVENT_QUEUE_FULL;
        }
    }

    /* ISP pipeline 2 start event */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_FS2_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_FS2_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_FS2_Msk;
#endif
    }

    /* ISP pipeline 1 done event */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_FD1_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_FD1_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_FD1_Msk;
#endif
    }

    /* Statistics and color temperature on pipeline 1 done */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_STATD_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_STATD_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_STATD_Msk;
#endif

        struct event ev = {0U, 0U, 0U, EI_ISP_STATISTICS_DONE};
        if (0 != EB_publishEventFromISR(&ev))
        {
            /* This should never happen, as the queue should have enough capacity
            to store all required requests for EDMA transfers.If there are more,
            it's a bug and fatal error! */
            retval = IMP_SYSTEM_EVENT_QUEUE_FULL;
        }
    }

    /* ISP pipeline 2 done event */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_FD2_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_FD2_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_FD2_Msk;
#endif
        struct event ev = {0U, 0U, 0U, EI_ISP_FRAME_DONE};
        if (0 != EB_publishEventFromISR(&ev))
        {
            /* This should never happen, as the queue should have enough capacity
            to store all required requests for EDMA transfers.If there are more,
            it's a bug and fatal error! */
            retval = IMP_SYSTEM_EVENT_QUEUE_FULL;
        }
    }

    /* DRC on pipeline 2 done */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_DRCD_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_DRCD_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_DRCD_Msk;
#endif
    }

    /* Bus error */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_BUS_ERR_Bit0))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_BUS_ERR_Bit0;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_BUS_ERR_Bit0;
#endif
    }

    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_BUS_ERR_Bit1))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_BUS_ERR_Bit1;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_BUS_ERR_Bit1;
#endif
    }

    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_BUS_ERR_Bit2))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_BUS_ERR_Bit2;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_BUS_ERR_Bit2;
#endif
    }

    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_BUS_ERR_Bit3))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_BUS_ERR_Bit3;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_BUS_ERR_Bit3;
#endif
    }

    /* CSI timing error */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_CSI_TERR_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_CSI_TERR_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_CSI_TERR_Msk;
#endif
    }

    /* Trigger error */
    if (0U != (isp->INT_STAT0 & ISP_INT_STAT0_S_TRIG_ERR_Msk))
    {
#if !defined (ISIM)
        isp->INT_STAT0 = ISP_INT_STAT0_S_TRIG_ERR_Msk;
#else
        isp->INT_STAT0 &= (uint32_t) ~ISP_INT_STAT0_S_TRIG_ERR_Msk;
#endif
    }

    return retval;
}
