/*
 * Copyright 2022, 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file isp.c
 * @brief ISP low level API implementation.
 */

#include "platform.h"
#include "isp.h"
#include "_isp_trigger.h"

#if defined (ISIM)
ISP_TypeDef isp_virtual;
#endif

/*
 *                          Defines
 */
#define HARD_RESET_CYCLES   (100U)
#define SOFT_RESET_CYCLES   (HARD_RESET_CYCLES)
#define SUCCESS             (0U)
#define BUSY                (1U)
#define TIMEOUT             (1U)

/*
 *                          Public functions
 */
uint32_t ISP_softReset(ISP_TypeDef * isp)
{
    plt_assert(NULL != isp);

    uint32_t retval = SUCCESS;

    isp->SOFT_RESET |= ISP_SOFT_RESET_SOFT_RESET_Msk;
    /* After setting the soft reset bit, the reset takes 100 cycles to finish.
    Let's check whether the reset finished or not. To be sure, we add 50 cycles
    more. */
    uint32_t cycle_reserve = 50U;
    uint32_t i = SOFT_RESET_CYCLES + cycle_reserve;
    while (0U != i)
    {
        i--;
    }

    /* TODO for a simulated environment, this will always be true leading to
    timeout. Maybe the code should be reworked an a stub should be provided?? */
    if (0U != (isp->SOFT_RESET & ISP_SOFT_RESET_SOFT_RESET_Msk))
    {
        retval = TIMEOUT;
    }

    return retval;
}

uint32_t ISP_hardReset(ISP_TypeDef * isp)
{
    plt_assert(NULL != isp);

    uint32_t retval = SUCCESS;

    isp->SOFT_RESET |= ISP_SOFT_RESET_HARD_RESET_Msk;
    /* After setting the soft reset bit, the reset takes 100 cycles to finish.
    Let's check whether the reset finished or not. To be sure, we add 50 cycles
    more. */
    uint32_t cycle_reserve = 50U;
    uint32_t i = HARD_RESET_CYCLES + cycle_reserve;
    while (0U != i)
    {
        i--;
    }

    /* TODO for a simulated environment, this will always be true leading to
    timeout. Maybe the code should be reworked an a stub should be provided?? */
    if (0U != (isp->SOFT_RESET & ISP_SOFT_RESET_HARD_RESET_Msk))
    {
        retval = TIMEOUT;
    }

    return retval;
}

uint32_t ISP_initializePipeline(ISP_TypeDef * isp)
{
    plt_assert(NULL != isp);

    uint32_t retval = SUCCESS;
    retval = ISP_softReset(isp);
    if (SUCCESS == retval)
    {
        retval = ISP_hardReset(isp);
        if (SUCCESS == retval)
        {
            retval = 2U;
        }
    }

    return retval;
}

void ISP_setBurstSize(ISP_TypeDef * isp, enum burst_size size)
{
    plt_assert(NULL != isp);

    uint32_t reg = isp->BUS_TXPARAM;
    reg &= (uint32_t)~ISP_BUS_TXPARAM_BSIZE_Msk;
    reg |= ISP_BUS_TXPARAM_BSIZE(size);
    isp->BUS_TXPARAM = reg;
}

void ISP_setPostQoSEscalationThreshold(ISP_TypeDef * isp, uint16_t value)
{
    plt_assert(NULL != isp);

    uint32_t reg = isp->BUS_TXPARAM;
    reg &= (uint32_t)~ISP_BUS_TXPARAM_POSTQOS_Msk;
    reg |= ISP_BUS_TXPARAM_POSTQOS(value);
    isp->BUS_TXPARAM = reg;
}

void ISP_setOutstandingTransactionHighThreshold(ISP_TypeDef * isp, uint16_t threshold)
{
    plt_assert(NULL != isp);

    uint32_t reg = isp->BUS_TXPARAM;
    reg &= (uint32_t)~ISP_BUS_TXPARAM_OTHT_Msk;
    reg |= ISP_BUS_TXPARAM_OTHT(threshold);
    isp->BUS_TXPARAM = reg;
}

void ISP_setOustandingTransactionLowThreshold(ISP_TypeDef * isp, uint16_t threshold)
{
    plt_assert(NULL != isp);

    uint32_t reg = isp->BUS_TXPARAM;
    reg &= (uint32_t)~ISP_BUS_TXPARAM_OTLT_Msk;
    reg |= ISP_BUS_TXPARAM_OTLT(threshold);
    isp->BUS_TXPARAM = reg;
}

void ISP_setTransferErrorDisable(ISP_TypeDef * isp, enum feature_state new_state)
{
    plt_assert(NULL != isp);

    if (ENABLED == new_state)
    {
        isp->REG_XFR_DIS |= ISP_REG_XFR_DIS_XFR_ERR_DIS_Msk;
    }
    else
    {
        isp->REG_XFR_DIS &= (uint32_t)~ISP_REG_XFR_DIS_XFR_ERR_DIS_Msk;
    }
}

void ISP_setStreamingMode(ISP_TypeDef * isp, enum feature_state new_state)
{
    plt_assert(NULL != isp);

    if (ENABLED == new_state)
    {
        isp->CSI_CTRL |= ISP_CSI_CTRL_CSI_EN_Msk;
    }
    else
    {
        isp->CSI_CTRL &= (uint32_t)~ISP_CSI_CTRL_CSI_EN_Msk;
    }
}

void ISP_setDualExposureVsyncLock(ISP_TypeDef * isp, enum feature_state new_state)
{
    plt_assert(NULL != isp);

    if (ENABLED == new_state)
    {
        isp->CSI_CTRL |= ISP_CSI_CTRL_DEVL_Msk;
    }
    else
    {
        isp->CSI_CTRL &= (uint32_t)~ISP_CSI_CTRL_DEVL_Msk;
    }
}

void ISP_setSingleStepMode(ISP_TypeDef * isp, enum feature_state new_state)
{
    plt_assert(NULL != isp);

    if (ENABLED == new_state)
    {
        isp->CSI_CTRL |= ISP_CSI_CTRL_SSEN_Msk;
    }
    else
    {
        isp->CSI_CTRL &= (uint32_t)~ISP_CSI_CTRL_SSEN_Msk;
    }
}

void ISP_setVirtualChannelID(ISP_TypeDef * isp, uint8_t linepath, uint8_t id)
{
    plt_assert(NULL != isp);
    plt_assert(linepath < 2U);

    uint32_t reg = isp->CSI_CTRL;

    reg &= (uint32_t) (ISP_CSI_CTRL_VID0_Msk << (ISP_CSI_CTRL_VID1_Pos * linepath));
    reg |= ISP_CSI_CTRL_VID0(id) << (ISP_CSI_CTRL_VID1_Pos * linepath);

    isp->CSI_CTRL |= reg;
}

void ISP_setRegisterShadowingControl(ISP_TypeDef * isp, enum feature_state new_state)
{
    plt_assert(NULL != isp);

    if (ENABLED == new_state)
    {
        isp->REG_SHD_CTRL |= ISP_REG_SHD_CTRL_CTRL_Msk;
    }
    else
    {
        isp->REG_SHD_CTRL &= (uint32_t)~ISP_REG_SHD_CTRL_CTRL_Msk;
    }
}

uint32_t ISP_setRegisterShadowingCommand(ISP_TypeDef * isp)
{
    plt_assert(NULL != isp);
    uint32_t retval = BUSY;

    if (0U == isp->REG_SHD_CMD)
    {
        isp->REG_SHD_CMD = 0x50505050U;
        retval = SUCCESS;
    }

    return retval;
}

uint32_t ISP_getFrameNumber(ISP_TypeDef * isp)
{
    return (uint32_t) (0x0000FFFFU & isp->FRAME_NUM);
}

void ISP_trigger(ISP_TypeDef * isp)
{
    isp_trigger_plt(isp);
}

void ISP_setInterrupts(ISP_TypeDef * isp, uint32_t interrupts, enum feature_state new_state)
{
    plt_assert(NULL != isp);

    if (ENABLED == new_state)
    {
        isp->INT_EN0 |= interrupts;
    }
    else
    {
        isp->INT_EN0 &= (uint32_t)~interrupts;
    }
}

bool ISP_isIspRunning(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_BUSY_Msk) != 0U;
}

bool ISP_isTriggerAvailable(const ISP_TypeDef * isp)
{
    return (isp->TRIG_CAM0 == 0U);
}

bool ISP_getTriggerErrorStatus(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_TRIG_ERR_Msk) != 0U;
}

bool ISP_getCsiTimingErrorStatus(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_CSI_TERR_Msk) != 0U;
}

uint8_t ISP_getBusError(const ISP_TypeDef * isp)
{
    return (uint8_t)((isp->INT_STAT0 & ISP_INT_STAT0_S_BUS_ERR_Msk) >> ISP_INT_STAT0_S_BUS_ERR_Pos);
}

bool ISP_getDrcDoneStatus(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_DRCD_Msk) != 0U;
}

bool ISP_getStatisticsDoneStatus(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_STATD_Msk) != 0U;
}

bool ISP_getFrameDone2Status(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_FD2_Msk) != 0U;
}

bool ISP_getFrameDone1Status(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_FD1_Msk) != 0U;
}

bool ISP_getFrameStart2Status(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_FS2_Msk) != 0U;
}

bool ISP_getFrameStart1Status(const ISP_TypeDef * isp)
{
    return (isp->INT_STAT0 & ISP_INT_STAT0_S_FS1_Msk) != 0U;
}

uint32_t ISP_getImageSizeInPixels(const ISP_TypeDef * isp)
{
    uint32_t height = (uint32_t) (isp->IMG_SIZE_CAM0 >> 16U);
    uint32_t width = (uint32_t) (isp->IMG_SIZE_CAM0 & 0x0000FFFFU);

    /* As height and width is never going to be more then 16 bits, it's ok
    to multiply these two numbers, we should never overflow */
    return height * width;
}

uint32_t ISP_getInputRegistersSize(void)
{
    return sizeof(ISP_TypeDef) - offsetof(ISP_TypeDef, IMG_CONF_CAM0) - ISP_getOutputRegistersSize();
}

uint32_t ISP_getOutputRegistersSize(void)
{
    return sizeof(ISP_TypeDef) - offsetof(ISP_TypeDef, ALIAS);
}

uint32_t ISP_getInputMemoriesSize(void)
{
    #if defined (A0)
        return offsetof(ISP_MEMS_TypeDef, DRC_LOCAL_SUM) - offsetof(ISP_MEMS_TypeDef, VIGNETTING_TABLE);
    #elif defined (B0)
        return sizeof(ISP_MEMS_TypeDef) - offsetof(ISP_MEMS_TypeDef, VIGNETTING_TABLE);
    #else
     #error "Device revision not specified. Define one of [A0, B0]!"
    #endif

}

uint32_t ISP_getOutputMemoriesSize(void)
{
    #if defined (A0)
        return sizeof(ISP_MEMS_TypeDef);
    #elif defined (B0)
        return offsetof(ISP_MEMS_TypeDef, VIGNETTING_TABLE);
    #else
        #error "Device revision not specified! Define either A0 or B0"
    #endif
}

uint64_t ISP_getInputRegistersAddress(void)
{
    return EDMA_VIEW_ISP_ADDRESS_SPACE_REGISTERS_START + offsetof(ISP_TypeDef, IMG_CONF_CAM0);
}

uint64_t CTX_getInputRegistersAddress(uint64_t context)
{
    plt_assert(context < (UINT64_MAX - sizeof(ISP_TypeDef) - sizeof(ISP_MEMS_TypeDef)));
    return context + offsetof(ISP_TypeDef, IMG_CONF_CAM0);
}

uint64_t ISP_getOutputRegistersAddress(void)
{
    return EDMA_VIEW_ISP_ADDRESS_SPACE_REGISTERS_START + offsetof(ISP_TypeDef, ALIAS);
}

uint64_t CTX_getOutputRegistersAddress(uint64_t context)
{
    plt_assert(context < (UINT64_MAX - sizeof(ISP_TypeDef) - sizeof(ISP_MEMS_TypeDef)));
    return context + offsetof(ISP_TypeDef, ALIAS);
}

uint64_t ISP_getOutputMemoriesAddress(void)
{
    return EDMA_VIEW_ISP_ADDRESS_SPACE_MEMORIES_START;
}

uint64_t ISP_getInputMemoriesAddress(void)
{
    return EDMA_VIEW_ISP_ADDRESS_SPACE_MEMORIES_START + offsetof(ISP_MEMS_TypeDef, VIGNETTING_TABLE);
}

uint64_t CTX_getOutputMemoriesAddress(uint64_t context)
{
    plt_assert(context < (UINT64_MAX - sizeof(ISP_TypeDef) - sizeof(ISP_MEMS_TypeDef)));
    return context + sizeof(ISP_TypeDef);
}

uint64_t CTX_getInputMemoriesAddress(uint64_t context)
{
    plt_assert(context < (UINT64_MAX - sizeof(ISP_TypeDef) - sizeof(ISP_MEMS_TypeDef)));
    return context + sizeof(ISP_TypeDef) + offsetof(ISP_MEMS_TypeDef, VIGNETTING_TABLE);
}
