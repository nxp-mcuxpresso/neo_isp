/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cgd_freertos_hal.c
 * @brief Freertos registry configuration class
 *******************************************************************************/
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ilog.h>
#include "cgd_internal.h"
#include "cgd_hal.h"
#include "cgd_types.h"
#include "utils_gpio.h"
#include "fsl_adapter_gpio.h"

#include "board.h"
#include "fsl_dwc_mipi_csi2rx.h"

#include "fsl_video_common.h"
#include "fsl_camera_receiver.h"
#include "fsl_isi_camera_adapter.h"
#include "fsl_isi.h"
#include "fsl_camera_device.h"
#include "fsl_ap1302.h"
#include "fsl_lpi2c.h"
#include "fsl_camera_csr.h"
#include "PERI_ISI.h"

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_pcal6408.h"
#include "fsl_irqsteer.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define BIT(nr) (((uint32_t)1U) << (nr))
#define upper_32_bits(n) ((uint32_t)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((uint32_t)((n) & 0xffffffff))
#define LOAD_BUF1_ADDR BIT(14)
#define LOAD_BUF2_ADDR BIT(15)

#define PHY_CFG_CLK_RANGE 0x1c /* (FMhz - 17) * 4 =>  (24 - 17) * 4 = 28*/

#define GPIO_PORT_STANDALONE_LINE 2

#define CGD_CAMERA_ISI_CHAN_BASE(id) ((ISI_Type *)&CAMERA__ISI->CHNL[id])
#define kISI_OutputRAW32 64U

#define CHNL_STS_BUF1_ACTIVE ((uint32_t)(1U << 8U))
#define CHNL_STS_BUF2_ACTIVE ((uint32_t)(1U << 9U))

#define CHNL_STS_FRM_STRD ((uint32_t)(1U << 29U))
#define CHNL_STS_LINE_STRD ((uint32_t)(1U << 30U))

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
/** @brief Frequency mapping for camera sensor PHY configuration */
typedef struct
{
    uint16_t mMbps;
    uint8_t mHsFreqRange;
    uint16_t mDdlFreq;
} cgd_DphyFreqMap_t;

/** @brief ISR thread states */
typedef enum
{
    BUFFER1    = 0,
    BUFFER2    = 1,
    BUFFER_CNT = 2,
    BUFFER_NA  = 3,
} isi_BuffersEnum_t;

/** @brief structure to define memmory region */
typedef struct
{
    uint32_t* mpData;
    uint64_t mHwAddr;
    uint32_t mIdx;
} isi_Buffer_t;

typedef struct
{
    /** @brief free buffer ready to use */
    isi_Buffer_t mFreeBuffer;
    /** @brief buffer1 assigned to ISI */
    isi_Buffer_t mBuffer1;
    /** @brief buffer2 assigned to ISI */
    isi_Buffer_t mBuffer2;
    /** @brief output buffer */
    isi_Buffer_t mFullBuffer;
    /** @brief  last buffer used to load frame data, expected values are only {1, 2}*/
    isi_BuffersEnum_t mLastBuffer;
} isi_Ctl_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/
/******************************************************************************/
/**
 * @brief        DPHY frequency mapping table for camera sensor configuration
 *******************************************************************************/
static cgd_DphyFreqMap_t scDphyFreqArr[] = {
    {  .mMbps = 80, .mHsFreqRange = 0x00, .mDdlFreq = 489},
    {  .mMbps = 90, .mHsFreqRange = 0x10, .mDdlFreq = 489},
    { .mMbps = 100, .mHsFreqRange = 0x20, .mDdlFreq = 489},
    { .mMbps = 110, .mHsFreqRange = 0x30, .mDdlFreq = 489},
    { .mMbps = 120, .mHsFreqRange = 0x01, .mDdlFreq = 489},
    { .mMbps = 130, .mHsFreqRange = 0x11, .mDdlFreq = 489},
    { .mMbps = 140, .mHsFreqRange = 0x21, .mDdlFreq = 489},
    { .mMbps = 150, .mHsFreqRange = 0x31, .mDdlFreq = 489},
    { .mMbps = 160, .mHsFreqRange = 0x02, .mDdlFreq = 489},
    { .mMbps = 170, .mHsFreqRange = 0x12, .mDdlFreq = 489},
    { .mMbps = 180, .mHsFreqRange = 0x22, .mDdlFreq = 489},
    { .mMbps = 190, .mHsFreqRange = 0x32, .mDdlFreq = 489},
    { .mMbps = 205, .mHsFreqRange = 0x03, .mDdlFreq = 489},
    { .mMbps = 220, .mHsFreqRange = 0x13, .mDdlFreq = 489},
    { .mMbps = 235, .mHsFreqRange = 0x23, .mDdlFreq = 489},
    { .mMbps = 250, .mHsFreqRange = 0x33, .mDdlFreq = 489},
    { .mMbps = 275, .mHsFreqRange = 0x04, .mDdlFreq = 489},
    { .mMbps = 300, .mHsFreqRange = 0x14, .mDdlFreq = 489},
    { .mMbps = 325, .mHsFreqRange = 0x25, .mDdlFreq = 489},
    { .mMbps = 350, .mHsFreqRange = 0x35, .mDdlFreq = 489},
    { .mMbps = 400, .mHsFreqRange = 0x05, .mDdlFreq = 489},
    { .mMbps = 450, .mHsFreqRange = 0x16, .mDdlFreq = 489},
    { .mMbps = 500, .mHsFreqRange = 0x26, .mDdlFreq = 489},
    { .mMbps = 550, .mHsFreqRange = 0x37, .mDdlFreq = 489},
    { .mMbps = 600, .mHsFreqRange = 0x07, .mDdlFreq = 489},
    { .mMbps = 650, .mHsFreqRange = 0x18, .mDdlFreq = 489},
    { .mMbps = 700, .mHsFreqRange = 0x28, .mDdlFreq = 489},
    { .mMbps = 750, .mHsFreqRange = 0x39, .mDdlFreq = 489},
    { .mMbps = 800, .mHsFreqRange = 0x09, .mDdlFreq = 489},
    { .mMbps = 850, .mHsFreqRange = 0x19, .mDdlFreq = 489},
    { .mMbps = 900, .mHsFreqRange = 0x29, .mDdlFreq = 489},
    { .mMbps = 950, .mHsFreqRange = 0x3a, .mDdlFreq = 489},
    {.mMbps = 1000, .mHsFreqRange = 0x0a, .mDdlFreq = 489},
    {.mMbps = 1050, .mHsFreqRange = 0x1a, .mDdlFreq = 489},
    {.mMbps = 1100, .mHsFreqRange = 0x2a, .mDdlFreq = 489},
    {.mMbps = 1150, .mHsFreqRange = 0x3b, .mDdlFreq = 489},
    {.mMbps = 1200, .mHsFreqRange = 0x0b, .mDdlFreq = 489},
    {.mMbps = 1250, .mHsFreqRange = 0x1b, .mDdlFreq = 489},
    {.mMbps = 1300, .mHsFreqRange = 0x2b, .mDdlFreq = 489},
    {.mMbps = 1350, .mHsFreqRange = 0x3c, .mDdlFreq = 489},
    {.mMbps = 1400, .mHsFreqRange = 0x0c, .mDdlFreq = 489},
    {.mMbps = 1450, .mHsFreqRange = 0x1c, .mDdlFreq = 489},
    {.mMbps = 1500, .mHsFreqRange = 0x2c, .mDdlFreq = 489},
    {.mMbps = 1550, .mHsFreqRange = 0x3d, .mDdlFreq = 303},
    {.mMbps = 1600, .mHsFreqRange = 0x0d, .mDdlFreq = 313},
    {.mMbps = 1650, .mHsFreqRange = 0x1d, .mDdlFreq = 323},
    {.mMbps = 1700, .mHsFreqRange = 0x2e, .mDdlFreq = 333},
    {.mMbps = 1750, .mHsFreqRange = 0x3e, .mDdlFreq = 342},
    {.mMbps = 1800, .mHsFreqRange = 0x0e, .mDdlFreq = 352},
    {.mMbps = 1850, .mHsFreqRange = 0x1e, .mDdlFreq = 362},
    {.mMbps = 1900, .mHsFreqRange = 0x1f, .mDdlFreq = 372},
    {.mMbps = 1950, .mHsFreqRange = 0x3f, .mDdlFreq = 381},
    {.mMbps = 2000, .mHsFreqRange = 0x0f, .mDdlFreq = 391},
    {.mMbps = 2050, .mHsFreqRange = 0x40, .mDdlFreq = 401},
    {.mMbps = 2100, .mHsFreqRange = 0x41, .mDdlFreq = 411},
    {.mMbps = 2150, .mHsFreqRange = 0x42, .mDdlFreq = 411},
    {.mMbps = 2200, .mHsFreqRange = 0x43, .mDdlFreq = 411},
    {.mMbps = 2250, .mHsFreqRange = 0x44, .mDdlFreq = 411},
    {.mMbps = 2300, .mHsFreqRange = 0x45, .mDdlFreq = 411},
    {.mMbps = 2350, .mHsFreqRange = 0x46, .mDdlFreq = 411},
    {.mMbps = 2400, .mHsFreqRange = 0x47, .mDdlFreq = 411},
    {.mMbps = 2450, .mHsFreqRange = 0x48, .mDdlFreq = 411},
    {.mMbps = 2500, .mHsFreqRange = 0x49, .mDdlFreq = 411},
    {   .mMbps = 0,    .mHsFreqRange = 0,   .mDdlFreq = 0},
};
/*=============================================================================
*                                  VARIABLES
==============================================================================*/

/******************************************************************************/
/**
 * @brief        Frame done event handler for CGD ISI channels
 *******************************************************************************/
cgd_IsiEventHandler_t gpCgdFrameDoneEvtHandler;

/******************************************************************************/
/**
 * @brief        First line event handler for CGD ISI channels
 *******************************************************************************/
cgd_IsiEventHandler_t gpCgdFirstLineEvtHandler;

/******************************************************************************/
/**
 * @brief        Error event handler for CGD ISI channels
 *******************************************************************************/
cgd_ErrorEventHandler_t gpCgdErrorEvtHandler;

/******************************************************************************/
/**
 * @brief        ISI control structure array for all CGD ISI channels
 *******************************************************************************/
static isi_Ctl_t gsIsiCtl[CGD_ISI_CNT] = {0};

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief        Fill Output buffer
 *******************************************************************************/
static void PrepareOutputBuffer(cgd_IsiChannel_t aChnId);

/******************************************************************************/
/**
 * @brief        Save new buffer to cache
 *******************************************************************************/
static int32_t PushNewBuffer(cgd_IsiChannel_t aChnId, isi_Buffer_t* apEmptyBuffer);

/******************************************************************************/
/**
 * @brief        Return a full buffer for a specific ISI channel
 *******************************************************************************/
static isi_Buffer_t GetFullBuffer(cgd_IsiChannel_t aChnId);

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/*******************************************************************************/
/******************************************************************************/
/**
 * @brief Get DPHY frequency mapping by Mbps value
 *
 * @param  aMbps  Target speed in Mbps
 *
 * @return Pointer to cgd_DphyFreqMap_t structure, NULL in case of error
 *
 ****************************************************************************/
static const cgd_DphyFreqMap_t* GetFreqRangeByMpbs(uint16_t aMbps)
{
    const cgd_DphyFreqMap_t* lpValue;
    const cgd_DphyFreqMap_t* plPrevValue = NULL;

    for (lpValue = scDphyFreqArr; lpValue->mMbps; lpValue++)
    {
        if (lpValue->mMbps >= aMbps)
            break;

        plPrevValue = lpValue;
    }

    if (plPrevValue && ((aMbps - plPrevValue->mMbps) <= (lpValue->mMbps - aMbps)))
        lpValue = plPrevValue;

    if (!lpValue->mMbps)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Unsupported PHY speed (%llu Mbps)", aMbps);
        return NULL;
    }

    return lpValue;
}

/******************************************************************************/
/**
 * @brief Set ISI device state, isi_MtxLock used
 *
 * @param  aIsiIdx  ISI index
 * @param  aNewState  desired ISI state
 *
 * @return cgd_State_t enum for correct ISI device, CGD_STATE_OFF in case of error
 *
 ****************************************************************************/
cgd_Result cgd_SetIsiState(cgd_IsiChannel_t aIsiIdx, cgd_State_t aNewState)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

    if (lpIsiDev != NULL)
    {
        lpIsiDev->mIsiState = aNewState;
    }

    return lRet;
}

/******************************************************************************/
/**
 * @brief Handle interruption from ISI channel and send him to next processing
 */
void IsiISRHandle(cgd_IsiChannel_t aChnId)
{
    if ((aChnId >= CGD_ISI_CNT))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Invalid ISI channel ID: %d", aChnId);
        return;
    }

    uint32_t interrupts;
    uint32_t chnlSts = CAMERA__ISI->CHNL[aChnId].STS;
    bool NeedTaskSwitch = false;

    if (CHNL_STS_FRM_STRD == (chnlSts & CHNL_STS_FRM_STRD))
    {
        /* Disable the interrupt to protect the index information in handle. */
        interrupts                       = ISI_DisableInterrupts(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)(kISI_FrameReceivedInterrupt | kISI_LineReceivedInterrupt));
        CAMERA__ISI->CHNL[aChnId].STS = CHNL_STS_FRM_STRD | CHNL_STS_LINE_STRD; // clear frame & line stored flag
        ISI_ClearInterruptStatus(CAMERA__ISI, (kISI_FrameReceivedInterrupt | kISI_LineReceivedInterrupt));

        PrepareOutputBuffer(aChnId); // take out the output buffer and prepare it for next frame
        gpCgdFrameDoneEvtHandler(aChnId);
        NeedTaskSwitch = true;

        if (0UL != (interrupts & (uint32_t)kISI_FrameReceivedInterrupt))
        {
            (void)ISI_EnableInterrupts(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)(kISI_FrameReceivedInterrupt | kISI_LineReceivedInterrupt));
        }
    }
    else if (CHNL_STS_LINE_STRD == (chnlSts & CHNL_STS_LINE_STRD))
    {
        interrupts = ISI_DisableInterrupts(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)(kISI_LineReceivedInterrupt));
        ISI_ClearInterruptStatus(CGD_CAMERA_ISI_CHAN_BASE(aChnId), kISI_LineReceivedInterrupt);

        gpCgdFirstLineEvtHandler(aChnId);
    }

    portYIELD_FROM_ISR(NeedTaskSwitch);
} // IsiISRHandle

/******************************************************************************/
/**
 * @brief Reset GPIO pin for the specified CSI device
 * @param aPortIdx CSI device identifier
 * @param aStart true to bring device out of reset, false to keep in reset
 * @return cgd_Result CGD_LIB_SUCCESS on success, CGD_LIB_FAILURE on error
 */
cgd_Result cgd_ResetGpioPin(cgd_CsiDeviceId_t aPortIdx, bool aStart)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    pcal6408_handle_t handle;
    BOARD_InitPCAL6408_I2C3(&handle);

    status_t ret;

    ret = PCAL6408_SetDirection(&handle, (1 << GPIO_PORT_STANDALONE_LINE), kPCAL6408_Output); // set pin to output mode

    if (ret != kStatus_Success)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "PCAL6408_SetDirection failed.\r\n");
        lRet = CGD_LIB_FAILURE;
    }

    // reset is active in zero state
    if (PCAL6408_ClearPins(&handle, (1 << GPIO_PORT_STANDALONE_LINE)) != kStatus_Success)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "PCAL6408 chip failed to write zero.\n");
        lRet = CGD_LIB_FAILURE;
    }

    // bring up/down from reset
    if (aStart)
    {
        vTaskDelay(pdMS_TO_TICKS(50));

        if (PCAL6408_SetPins(&handle, (1 << GPIO_PORT_STANDALONE_LINE)) != kStatus_Success)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "PCAL6408 failed to write zero.\n");
            lRet = CGD_LIB_FAILURE;
        }
    }

    if (lRet == CGD_LIB_FAILURE)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_ResetGpioPin failed.\r\n");
    }

    return CGD_LIB_SUCCESS;
} // cgd_ResetGpioPin

/*==============================================================================
*                               GLOBAL FUNCTIONS
==============================================================================*/

/*******************************************************************************/
cgd_Result cgd_IsiChannelStreamOn(cgd_IsiChannel_t aChnId)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aChnId);

    if (lpIsiDev == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    lpIsiDev->mIsiState       = CGD_STATE_STRM; // isi is streaming now, change state

    ISI_ClearInterruptStatus(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)kISI_FrameReceivedInterrupt);
    (void)ISI_EnableInterrupts(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)kISI_FrameReceivedInterrupt);
    ISI_Start(CGD_CAMERA_ISI_CHAN_BASE(aChnId));

    switch (aChnId)
    {
        case 0:
        {
            EnableIRQ(ISI_IRQn);
            break;
        }
        case 1:
        {
            EnableIRQ(IRQSTEER_7_IRQn);
            IRQSTEER_EnableInterrupt(IRQSTEER, Reserved706_IRQn);
            break;
        }
        case 2:
        {
            EnableIRQ(IRQSTEER_7_IRQn);
            IRQSTEER_EnableInterrupt(IRQSTEER, Reserved707_IRQn);
            break;
        }
        case 3:
        {
            EnableIRQ(IRQSTEER_7_IRQn);
            IRQSTEER_EnableInterrupt(IRQSTEER, Reserved708_IRQn);
            break;
        }
        default:
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "ISI CHN %d Invalid IRQ enablement.\r\n", aChnId);
        }
    }

    return lRet;
}

/*******************************************************************************/
cgd_Result cgd_IsiChannelStreamOff(cgd_IsiChannel_t aChnId)
{
    cgd_Result lRet           = CGD_LIB_SUCCESS;
    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aChnId);

    if (lpIsiDev == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    ISI_Stop(CGD_CAMERA_ISI_CHAN_BASE(aChnId));
    (void)ISI_DisableInterrupts(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)kISI_FrameReceivedInterrupt);
    ISI_ClearInterruptStatus(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)kISI_FrameReceivedInterrupt);

    lpIsiDev->mIsiState = CGD_STATE_STRM_OFF;

    return lRet;
}

/*******************************************************************************/
cgd_Result cgd_IsiChannelConfigure(cgd_IsiChannel_t aChnId)
{
    cgd_Result lRet          = CGD_LIB_SUCCESS;
    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aChnId);
    cgd_IsiExtCfg_t* lpIsiExt = cgd_GetIsiDeviceConfig(aChnId);

    if (lpIsiDev == NULL || lpIsiExt == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    if ((lpIsiExt->mImageWidth * 2) > UINT16_MAX || (lpIsiExt->mImageHeight + lpIsiExt->mEmbdLinesBefore + lpIsiExt->mEmbdLinesAfter > UINT16_MAX))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "ISI CHN %d Invalid image dimensions.\r\n", aChnId);
        return CGD_LIB_FAILURE;
    }

    (void)ISI_DisableInterrupts(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)kISI_FrameReceivedInterrupt);
    ISI_ClearInterruptStatus(CGD_CAMERA_ISI_CHAN_BASE(aChnId), (uint32_t)kISI_FrameReceivedInterrupt);

    uint8_t sourcePort = 0;

    if (lpIsiDev->mCsiId == CGD_MIPI_CSI_DEV)
    {
        sourcePort = 0x02;
    }
    else
    {
        sourcePort = 0x03;
    }

    isi_config_t isiConfig;

    /* Initialize the ISI. */
    ISI_GetDefaultConfig(&isiConfig);
    isiConfig.isChannelBypassed    = true;
    isiConfig.isSourceMemory       = false;
    isiConfig.isYCbCr              = false;
    isiConfig.sourcePort           = sourcePort;
    isiConfig.mipiChannel          = lpIsiExt->mVC;
    isiConfig.inputHeight          = lpIsiExt->mImageHeight + lpIsiExt->mEmbdLinesBefore + lpIsiExt->mEmbdLinesAfter;
    isiConfig.inputWidth           = lpIsiExt->mImageWidth;
    isiConfig.outputFormat         = kISI_OutputRAW32;
    isiConfig.outputLinePitchBytes = (uint16_t)(lpIsiExt->mImageWidth * 2);

    ISI_Init(CGD_CAMERA_ISI_CHAN_BASE(aChnId));
    ISI_SetConfig(CGD_CAMERA_ISI_CHAN_BASE(aChnId), &isiConfig);
    ISI_SetScalerConfig(CGD_CAMERA_ISI_CHAN_BASE(aChnId), isiConfig.inputWidth, isiConfig.inputHeight, lpIsiExt->mImageWidth, lpIsiExt->mImageHeight);
    ISI_EnableColorSpaceConversion(CGD_CAMERA_ISI_CHAN_BASE(aChnId), false);

    /* configure RAW32 mode*/
    uint32_t reg;
    reg = CAMERA__ISI->CHNL[aChnId].IMG_CFG2;
    reg &= ~(ISI_IMG_CFG2_WIDTH_MASK);
    reg |= lpIsiExt->mImageWidth;
    CAMERA__ISI->CHNL[aChnId].IMG_CFG2 = reg;

    lpIsiDev->mIsiState = CGD_STATE_INIT;

    return lRet;
}

/*******************************************************************************/
cgd_Result cgd_IsiChannelClose(cgd_IsiChannel_t aChnId)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aChnId);

    if (lpIsiDev != NULL)
    {
        lpIsiDev->mIsiState = CGD_STATE_OFF;
    }

    return lRet;
}
/*******************************************************************************/

static cgd_Result CsiSetupPixelFormatter(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_Result lRet                = CGD_LIB_SUCCESS;
    cgd_CsiExtCfg_t* pCsiDevConfig = cgd_GetCsiPortConfig(aPortIdx);

    if (pCsiDevConfig == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    camera_csr_pixel_formatting_config_t cameracsr_config;

    for (int i = 0; i < CGD_CSI_VC_CNT; i++)
    {
        if (pCsiDevConfig->mDataType[i] == 0)
        {
            continue;
        }

        cameracsr_config.csiinterface            = (uint8_t)aPortIdx;
        cameracsr_config.mipiVc                  = i;
        cameracsr_config.PixelDataType           = pCsiDevConfig->mDataType[i];
        cameracsr_config.NonPixelDataNewVc       = i;
        cameracsr_config.enableRAW32             = true;
        cameracsr_config.enablePixelDataRoute    = false;
        cameracsr_config.enableNonPixelDataRoute = false;
        cameracsr_config.PixelDataNewVc          = i;
        cameracsr_config.NonPixelDataType        = pCsiDevConfig->mNPDataType[i];

        CAMERACSR_PixelFormatting(BLK_CTRL_CAMERAMIX, &cameracsr_config);
    }

    return lRet;
}

/*******************************************************************************/
cgd_Result cgd_CsiOpen(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    cgd_CsiExtCfg_t* pCsiDevConfig       = cgd_GetCsiPortConfig(aPortIdx);

    if (pCsiDevConfig == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    const cgd_DphyFreqMap_t* lpFreqValue = GetFreqRangeByMpbs(pCsiDevConfig->mMbps);
    if (lpFreqValue == NULL)
    {
        return CGD_LIB_FAILURE;
    }

    /* configure clock gating*/
    uint32_t reg;
    reg = BLK_CTRL_CAMERAMIX->ISP_CLOCK_GATING_CONTROL;
    reg &= 0x00000000U; // clear the clock gating for CSI2_pixel_formatting_0 & CSI2_pixel_formatting_1
    BLK_CTRL_CAMERAMIX->ISP_CLOCK_GATING_CONTROL = reg;

    /* configure raw32 mode */
    BLK_CTRL_CAMERAMIX->CSI0_RAW32_CTR = 0x000000FF; // enable RAW32 mode

    csi2rx_config_t csi2rxConfig = {0};
    csi2rxConfig.laneNum         = pCsiDevConfig->mLanes;
    csi2rxConfig.datatype        = 0;
    csi2rxConfig.pg_enable       = false;
    csi2rxConfig.vcnum           = 4;
    csi2rxConfig.hsfreqrange     = lpFreqValue->mHsFreqRange;
    csi2rxConfig.cfgclkfreqrange = PHY_CFG_CLK_RANGE;

    if (kStatus_Success != MIPI_CSI2RX_Init(MIPI_CSI2, CAMERA__DSI_OR_CSI_PHY_CSR, &csi2rxConfig))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "ISI CHN %d MIPI_CSI2RX_Init failed.\r\n", aPortIdx);
        lRet = CGD_LIB_FAILURE;
    }

    if (lRet == CGD_LIB_SUCCESS)
    {
        if (CGD_LIB_FAILURE == CsiSetupPixelFormatter(aPortIdx))
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "ISI CHN %d CsiSetupPixelFormatter failed.\r\n", aPortIdx);
            lRet = CGD_LIB_FAILURE;
        }
    }

    return lRet;
}

/*******************************************************************************/
cgd_Result cgd_CsiEnableInterruption(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    return lRet;
}

/*******************************************************************************/
cgd_Result cgd_CsiClose(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    CSI2RX_Deinit(MIPI_CSI2);

    cgd_CsiPort_t* csiPort = cgd_GetCsiPort(aPortIdx);

    if (csiPort == NULL)
    {
        return CGD_LIB_SUCCESS;
    }

    // init mutex for i2c bus
    if (csiPort->mcHead != NULL)
    {
        vSemaphoreDelete(csiPort->mcHead->mI2CMtx);
    }

    if (cgd_ResetGpioPin(aPortIdx, false) == CGD_LIB_FAILURE)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "cgd_Close failed to clear GPIO pins for port %d.\n", aPortIdx);
    }

    return lRet;
}
/******************************************************************************/
cgd_Result cgd_IsiSubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apEmptyBuffer)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    isi_Buffer_t buff;

    buff.mHwAddr = apEmptyBuffer->mHwAddr;
    buff.mpData  = apEmptyBuffer->mpData;
    buff.mIdx    = apEmptyBuffer->mIdx;

    if (PushNewBuffer(aIsiIdx, &buff) == 0)
    {
        lRet = CGD_LIB_SUCCESS;
    }
    else
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "ISI CHN %d cgd_IsiSubmitEmptyBuffer failed.\n", aIsiIdx);
    }

    return lRet;
}

/******************************************************************************/

static inline isi_BuffersEnum_t GetActiveBuffer(cgd_IsiChannel_t aChnId)
{
    assert(aChnId < CGD_ISI_CNT);

    isi_BuffersEnum_t lRet = BUFFER_NA;
    uint32_t chnlSts       = CAMERA__ISI->CHNL[aChnId].STS;

    if (CHNL_STS_BUF1_ACTIVE == (chnlSts & CHNL_STS_BUF1_ACTIVE))
    {
        lRet = BUFFER1;
    }
    else if (CHNL_STS_BUF2_ACTIVE == (chnlSts & CHNL_STS_BUF2_ACTIVE))
    {
        lRet = BUFFER2;
    }

    return lRet;
} // GetActiveBuffer

/******************************************************************************/

static inline bool IsEmptyBuffer(isi_Buffer_t* apBuff)
{
    return apBuff->mHwAddr == 0 ? true : false;
} // IsEmptyBuffer

/******************************************************************************/
static inline void CleanBuffer(isi_Buffer_t* apBuff)
{
    apBuff->mHwAddr = 0;
    apBuff->mpData  = 0;
    apBuff->mIdx    = 0;
} // CleanBuffer

/******************************************************************************/

static inline void TransferAndCleanBuffer(isi_Buffer_t* apSource, isi_Buffer_t* apDestination)
{
    apDestination->mHwAddr = apSource->mHwAddr;
    apDestination->mpData  = apSource->mpData;
    apDestination->mIdx    = apSource->mIdx;
    CleanBuffer(apSource);
} // TransferAndCleanBuffer

/******************************************************************************/
bool cgd_IsiIsEmptyBufferPrepared(cgd_IsiChannel_t aIsiIdx)
{
    bool lRet = false;

    assert(aIsiIdx < CGD_ISI_CNT);

    isi_Ctl_t* lpIsi = &gsIsiCtl[aIsiIdx];

    if (lpIsi != NULL)
    {
        lRet = !IsEmptyBuffer(&lpIsi->mFreeBuffer);
    }

    return lRet;
} // cgd_IsiIsEmptyBufferPrepared

/******************************************************************************/
static bool CheckBufferUpdate(cgd_IsiChannel_t aChnId, isi_BuffersEnum_t aWorkingOn)
{
    assert(aChnId < CGD_ISI_CNT);

    bool lRet        = true;
    isi_Ctl_t* lpIsi = &gsIsiCtl[aChnId];

    if (lRet && aWorkingOn == BUFFER_NA) // ISI stopped
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "ISI CHN %d: No busy buffer available. The ISI is not currently running.\n", aChnId);
        lRet = false;
    }

    bool bFrmStored = CHNL_STS_FRM_STRD == (CAMERA__ISI->CHNL[aChnId].STS & CHNL_STS_FRM_STRD);

    if (lRet && bFrmStored) // new frame => buffer update late, previous frame missed
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "ISI CHN %d: Frame-done race hazard detected. A frame has been dropped.\n", aChnId);
        lRet = false;
    }

    if (lRet && IsEmptyBuffer(&lpIsi->mFreeBuffer)) // free buffer empty
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "ISI CHN %d: No free buffer. A frame has been dropped.\n", aChnId);
        lRet = false;
    }

    if (lRet && !IsEmptyBuffer(&lpIsi->mFullBuffer)) // output buffer is full
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "ISI CHN %d: Output buffer occupied. A frame has been dropped.\n", aChnId);
        lRet = false;
    }

    if (lRet && lpIsi->mLastBuffer != aWorkingOn)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "ISI CHN %d: Unexpected buffer write order. A frame might have been dropped. (%d -> %d)\n", aChnId, lpIsi->mLastBuffer, aWorkingOn);
    }

    return lRet;
} // CheckBufferUpdate

/******************************************************************************/

int32_t AsignNewBuffer(cgd_IsiChannel_t aChnId, isi_BuffersEnum_t aBufferNo)
{
    assert(aChnId < CGD_ISI_CNT);

    isi_Ctl_t* lpIsi    = &gsIsiCtl[aChnId];

    uint32_t ctrlReg = CAMERA__ISI->CHNL[aChnId].OUT_BUF_CTRL;

    if (aBufferNo == BUFFER1)
    {
        CAMERA__ISI->CHNL[aChnId].OUT_BUF_CTRL &= ~LOAD_BUF1_ADDR; // clear load buffer1 registers
        // Channel RGB or Luma (Y) Output Buffer 1 Address
        CAMERA__ISI->CHNL[aChnId].OUT_BUF1_ADDR_Y  = lower_32_bits(lpIsi->mBuffer1.mHwAddr);
        CAMERA__ISI->CHNL[aChnId].Y_BUF1_XTND_ADDR = upper_32_bits(lpIsi->mBuffer1.mHwAddr);
        ctrlReg |= LOAD_BUF1_ADDR; // Load Buffer 1 Address, the new address values are shadowed in design
    }
    else
    {
        CAMERA__ISI->CHNL[aChnId].OUT_BUF_CTRL &= ~LOAD_BUF2_ADDR; // clear load buffer2 registers
        //  Channel RGB or Luma (Y) Output Buffer 2 Address
        CAMERA__ISI->CHNL[aChnId].OUT_BUF2_ADDR_Y  = lower_32_bits(lpIsi->mBuffer2.mHwAddr);
        CAMERA__ISI->CHNL[aChnId].Y_BUF2_XTND_ADDR = upper_32_bits(lpIsi->mBuffer2.mHwAddr);
        ctrlReg |= LOAD_BUF2_ADDR; // Load Buffer 2 Address, the new address values are shadowed in design
    }

    CAMERA__ISI->CHNL[aChnId].OUT_BUF_CTRL |= ctrlReg; // Load Buffer 2 Address, the new address values are shadowed in design
    lpIsi->mLastBuffer = aBufferNo;

    return 0;
} // AsignNewBuffer

/******************************************************************************/

int32_t PushNewBuffer(cgd_IsiChannel_t aChnId, isi_Buffer_t* apEmptyBuffer)
{
    assert(aChnId < CGD_ISI_CNT);

    int32_t lRet     = 1;
    isi_Ctl_t* lpIsi = &gsIsiCtl[aChnId];

    if (lpIsi->mBuffer1.mHwAddr == 0)
    {
        TransferAndCleanBuffer(apEmptyBuffer, &lpIsi->mBuffer1);
        lRet = AsignNewBuffer(aChnId, BUFFER1);
    }
    else if (lpIsi->mBuffer2.mHwAddr == 0)
    {
        TransferAndCleanBuffer(apEmptyBuffer, &lpIsi->mBuffer2);
        lRet = AsignNewBuffer(aChnId, BUFFER2);
    }
    else if (lpIsi->mFreeBuffer.mHwAddr == 0)
    {
        TransferAndCleanBuffer(apEmptyBuffer, &lpIsi->mFreeBuffer);
        lRet = 0;
    }
    else
    {
        lRet = 1; // no place for buffer, return error
    }

    return lRet;
} // PushNewBuffer

/******************************************************************************/
cgd_Result cgd_IsiGetFullBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer)
{
    assert(aIsiIdx < CGD_ISI_CNT);

    cgd_Result lRet           = CGD_LIB_FAILURE;
    cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

    if (lpIsiDev != NULL && apOutBuffer != NULL)
    {
        isi_Buffer_t pIsiBuff = GetFullBuffer(aIsiIdx);

        if (pIsiBuff.mHwAddr != 0)
        {
            apOutBuffer->mIdx    = pIsiBuff.mIdx;
            apOutBuffer->mHwAddr = pIsiBuff.mHwAddr;
            apOutBuffer->mpData  = pIsiBuff.mpData;
            lRet                 = CGD_LIB_SUCCESS;
        }
    }

    return lRet;
}

/******************************************************************************/
cgd_Result cgd_IsiGetFullBufferSubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer, utils_ImgBuf_t* apEmptyBuffer)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    isi_Buffer_t lFullBuffer = {0}; // Initialize return buffer to zero
    isi_Buffer_t lFreeBuff = {0};

    assert(aIsiIdx < CGD_ISI_CNT);

    isi_Ctl_t* lpIsi = &gsIsiCtl[aIsiIdx];

    if (!IsEmptyBuffer(&lFreeBuff) && IsEmptyBuffer(&lpIsi->mFreeBuffer) && IsEmptyBuffer(&lpIsi->mFullBuffer))
    {
        lFreeBuff.mHwAddr = apEmptyBuffer->mHwAddr;
        lFreeBuff.mpData  = apEmptyBuffer->mpData;
        lFreeBuff.mIdx    = apEmptyBuffer->mIdx;

        TransferAndCleanBuffer(&lFreeBuff, &lpIsi->mFreeBuffer);

        PrepareOutputBuffer(aIsiIdx); // prepare full buffer to output

        TransferAndCleanBuffer(&lpIsi->mFullBuffer, &lFullBuffer);

        apOutBuffer->mHwAddr = lFullBuffer.mHwAddr;
        apOutBuffer->mpData  = lFullBuffer.mpData;
        apOutBuffer->mIdx    = lFullBuffer.mIdx;
    }

    return lRet;
} // cgd_IsiGetFullBufferSubmitEmptyBuffer

/******************************************************************************/

static isi_Buffer_t GetFullBuffer(cgd_IsiChannel_t aChnId)
{
    assert(aChnId < CGD_ISI_CNT);

    isi_Buffer_t lRet = {0}; // Initialize return buffer to zero

    isi_Ctl_t* lIsi = &gsIsiCtl[aChnId];

    TransferAndCleanBuffer(&lIsi->mFullBuffer, &lRet);

    return lRet;
} // GetFullBuffer

/******************************************************************************/
static void PrepareOutputBuffer(cgd_IsiChannel_t aChnId)
{
    assert(aChnId < CGD_ISI_CNT);

    isi_Ctl_t* lIsi = &gsIsiCtl[aChnId];

    isi_BuffersEnum_t workingOn = GetActiveBuffer(aChnId);

    if (CheckBufferUpdate(aChnId, workingOn)) // free buffer is available
    {
        if (workingOn == BUFFER1) // buff1 is used to store new data => read from buffer2
        {
            TransferAndCleanBuffer(&lIsi->mBuffer2, &lIsi->mFullBuffer); // move full buffer to output buffer
            TransferAndCleanBuffer(&lIsi->mFreeBuffer, &lIsi->mBuffer2); // move free buffer to ISI, clean free buffer
            AsignNewBuffer(aChnId, BUFFER2);
        }
        else // BUFF2 is busy => read from BUFF1
        {
            TransferAndCleanBuffer(&lIsi->mBuffer1, &lIsi->mFullBuffer); // move full buffer to output buffer
            TransferAndCleanBuffer(&lIsi->mFreeBuffer, &lIsi->mBuffer1); // move free buffer to ISI, clean free buffer
            AsignNewBuffer(aChnId, BUFFER1);
        }
    }

    return;
} // PrepareOutputBuffer

/******************************************************************************/
void cgd_IsIChannelErrorCheck(cgd_IsiChannel_t aChnId)
{
    return;
} // cgd_IsIChannelErrorCheck

/******************************************************************************/
void cgd_CsiChannelErrorCheck(cgd_CsiDeviceId_t aDevId)
{
    return;
} // cgd_CsiChannelErrorCheck

/******************************************************************************/
cgd_Result cgd_IsiPrepare(cgd_IsiDevice_t* apIsiDevice)
{
    cgd_SetIsiState(apIsiDevice->mIsiId, CGD_STATE_INIT);

    return CGD_LIB_SUCCESS;
}
/******************************************************************************/
/**
 * @brief Call csi_Init -> CamCsr, PhyCsr, Csi registry mapping from kernel space to CGD one
 */
cgd_Result cgd_CsiPrepare(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_CsiPort_t* csiPort = cgd_GetCsiPort(aPortIdx);

    if (csiPort == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "csi_Init failed - no CSI ports.\n");
        return CGD_LIB_FAILURE;
    }

    if (csiPort->mPortState != CGD_STATE_OFF)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "csi_Init failed - CSI port started already.\n");
        return CGD_LIB_FAILURE;
    }

    // init mutex for i2c bus
    if (csiPort->mcHead != NULL)
    {
        csiPort->mcHead->mI2CMtx = xSemaphoreCreateBinaryStatic(&csiPort->mcHead->mI2CMtxData);
        if (csiPort->mcHead->mI2CMtx == NULL)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_CsiPrepare failed I2C mutex init for port %d.\n", aPortIdx);
            return CGD_LIB_FAILURE;
        }
        (void)xSemaphoreGive(csiPort->mcHead->mI2CMtx);
    }

    if (cgd_ResetGpioPin(aPortIdx, true) == CGD_LIB_FAILURE)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "CsiInit failed to reset GPIO pins for port %d. OVX3C should work, other sensors not.\n", aPortIdx);
    }

    return CGD_LIB_SUCCESS;
} // CsiInit
/******************************************************************************/
cgd_Result cgd_CleanUp()
{
    return CGD_LIB_SUCCESS;
} // cgd_CleanUp

/******************************************************************************/
/**
 * @brief ISI interrupt handler for channel 0
 *
 * Handles interrupts for ISI channel 0 and calls the appropriate frame done handler.
 */
void ISI_IRQHandler(void)
{
    IsiISRHandle(0);
}

/******************************************************************************/
/**
 * @brief IRQSTEER 7 interrupt handler for ISI channels 1-3
 *
 * Handles interrupts for ISI channels 1, 2, and 3 through IRQSTEER.
 * Temporarily disables master interrupt, checks for specific reserved IRQs,
 * and calls appropriate frame done handlers for each ISI channel.
 */
void IRQSTEER_7_IRQHandler(void)
{
    IRQSTEER_DisableMasterInterrupt(IRQSTEER, IRQSTEER_7_IRQn);

    if (IRQSTEER_IsInterruptSet(IRQSTEER, Reserved706_IRQn))
    {
        // IRQSTEER_SetInterrupt(IRQSTEER, Reserved706_IRQn, false);
        IsiISRHandle(1u);
    }
    if (IRQSTEER_IsInterruptSet(IRQSTEER, Reserved707_IRQn))
    {
        // IRQSTEER_SetInterrupt(IRQSTEER, Reserved707_IRQn, false);
        IsiISRHandle(2u);
    }
    if (IRQSTEER_IsInterruptSet(IRQSTEER, Reserved708_IRQn))
    {
        // IRQSTEER_SetInterrupt(IRQSTEER, Reserved708_IRQn, false);
        IsiISRHandle(3u);
    }

    IRQSTEER_EnableMasterInterrupt(IRQSTEER, IRQSTEER_7_IRQn);
}
/******************************************************************************/
