/*
 * Copyright 2023-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 /* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
// #include "queue.h"
// #include "timers.h"

#include "fsl_dpu.h"
#include "display_support.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "display_task.h"
#include "camera_ctx.h"
/******************************************************************************/
/**
 * @file  display_task.c
 * @brief Display code startup that was originally part of DPU demo, but was adapted for FreeRtos
 *******************************************************************************/


/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
#define STACK_SIZE 512
#define SELECTED_CHANNEL 0

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/
/* Frame buffer to display. */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_displayFrameBuffer[APP_PANEL_HEIGHT][APP_PANEL_FB_STRIDE_PIXEL],
                              APP_PANEL_FB_ADDR_ALIGN_BYTE);

static volatile bool s_isContentStreamShadowPending = false;

TaskHandle_t gDisplayTask;
static StaticTask_t gTaskBuffer;
static StackType_t gStack[STACK_SIZE];
extern CameraCtx_t gAppCtx;
uint32_t gDisplayFps = 0;
/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/


static bool APP_IsContentStreamShadowPending(void)
{
    return s_isContentStreamShadowPending;
}

/******************************************************************************/

static void APP_TriggerContentStreamShadowLoad(void)
{
    s_isContentStreamShadowPending = true;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_CONTENT_STREAM_PIPELINE);
}

/******************************************************************************/

static void DisplayTask(void *apParameters)
{
    dpu_display_timing_config_t displayTimingConfig;
    dpu_const_frame_config_t cfConfig;
    dpu_layer_blend_config_t lbConfig;
    dpu_fetch_unit_config_t fetchConfig;
    dpu_src_buffer_config_t sbConfig;
    dpu_display_config_t displayConfig;

    /* Step 1: Configure the const stream */

    /* Pipeline Configuration */
    DPU_InitPipeline(APP_DPU, APP_CONTENT_STREAM_PIPELINE);

    /* ExtDst: set the dynamic source as layerblend 1 */
    DPU_InitExtDst(APP_DPU, APP_CONTENT_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend1));

    /* Layer blend 1 source: prim input constframe0, sec input FetchYUV0. */
     DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend1, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame0, kDPU_UnitSrcFetchYUV0));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend1, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend1, true);

    /* Init domain blend */
    DPU_InitDomainBlend(APP_DPU, APP_CONTENT_DOMAINBLEND);

    /* Const frame 0 configuration. */
    cfConfig.frameWidth  = APP_PANEL_WIDTH;
    cfConfig.frameHeight = APP_PANEL_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame0);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame0, &cfConfig);

    /*
     * Fetch unit.
     */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_PANEL_HEIGHT;
    fetchConfig.frameWidth  = APP_PANEL_WIDTH;
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchYuv0, &fetchConfig);

    /* Fetch unit source buffer configure. */
    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    sbConfig.bitsPerPixel = 24;
    sbConfig.pixelFormat  = kDPU_PixelFormatRGB888;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);

    /* FetchYUV0 configuration. */
    sbConfig.strideBytes  = APP_PANEL_FB_STRIDE_BYTE;
    sbConfig.bufferHeight = APP_PANEL_HEIGHT;
    sbConfig.bufferWidth  = APP_PANEL_WIDTH;
    sbConfig.baseAddr     = (uint32_t)s_displayFrameBuffer;
    uint32_t ptr = 0;
    uint8_t *base = (uint8_t *) &s_displayFrameBuffer;
    for (int y=0;y < APP_PANEL_HEIGHT; y++)
    {
        for (int x=0;x < APP_PANEL_WIDTH; x++)
        {
            assert(ptr < 0xFFFFFFF0);
            base[ptr++] = (x + y) & 0xFF; // blue
            base[ptr++] = y % 0xff; // green
            base[ptr++] = x % 0xff; // red
        }
    }
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchYuv0, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchYuv0, 0, 0, 0);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchYuv0, 0, true);

    /* Configuration complete, trigger the shadow load. */
    APP_TriggerContentStreamShadowLoad();

    DPU_EnableInterrupts(APP_DPU_DC, 0, APP_CONTENT_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU_DC, 1, APP_CONTENT_STREAM_INT_GROUP1);

    DPU_TriggerDisplayDbShadowLoad(APP_DPU, APP_CONTENT_DOMAINBLEND);

    /* Step 2: Configure the display stream. */
    DPU_DisplayTimingGetDefaultConfig(&displayTimingConfig);

    displayTimingConfig.flags  = APP_DPU_TIMING_FLAGS;
    displayTimingConfig.width  = APP_PANEL_WIDTH;
    displayTimingConfig.hsw    = APP_HSW;
    displayTimingConfig.hfp    = APP_HFP;
    displayTimingConfig.hbp    = APP_HBP;
    displayTimingConfig.height = APP_PANEL_HEIGHT;
    displayTimingConfig.vsw    = APP_VSW;
    displayTimingConfig.vfp    = APP_VFP - 1U;
    displayTimingConfig.vbp    = APP_VBP + 1U;

    DPU_InitDisplayTiming(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayTimingConfig);

    DPU_DisplayGetDefaultConfig(&displayConfig);

    /* Only show the content stream in normal mode. */
    displayConfig.displayMode = kDPU_DisplayOnlyPrim;

    displayConfig.primAreaStartX = 1;
    displayConfig.primAreaStartY = 1;

    DPU_SetDisplayConfig(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayConfig);

    DPU_TriggerDisplayShadowLoad(APP_DPU, APP_DPU_DISPLAY_INDEX);

    /*
     * Initialize workflow:
     * 1. Init DPU
     * 2. Configure pixel link.
     * 3. Init and start display interface.
     * 4. Start DPU
     */
    APP_InitPixelLink();

    BOARD_InitDisplayInterface();

    DPU_StartDisplay(APP_DPU, APP_DPU_DISPLAY_INDEX);

    // send test pattern to output so we know display is working
    {
        while (APP_IsContentStreamShadowPending())
        {
            vTaskDelay(1);
        }


        DPU_SetFetchUnitSrcBufferAddr(APP_DPU, kDPU_FetchYuv0, 0, (uint32_t)s_displayFrameBuffer);

        APP_TriggerContentStreamShadowLoad();
    }

    /* Loop displaying frames from CCL */
    for (;;)
    {
        if (pdTRUE != xTaskNotifyWait(0, 0, 0, pdMS_TO_TICKS(10000u)))
        {
            PRINTF("display task sleep error (possible timeout)\n\r");
            vTaskDelay(configTICK_RATE_HZ);
            continue;
        }

        while (APP_IsContentStreamShadowPending())
        {
            vTaskDelay(1);
        }

        StreamCtx_t *lpStr = &gAppCtx.mpStreamArr[SELECTED_CHANNEL];

        if (pdTRUE != xSemaphoreTake(lpStr->mLockHandle, portMAX_DELAY))
        {
            PRINTF("Failed to take semaphore for stream %lu\n", 0);
            gAppCtx.mpStop = true;
            break;
        }

        if (lpStr->mpFrameNew == NULL)
        {
            // no new frame ready, keep the current one on display

            if (pdTRUE != xSemaphoreGive(lpStr->mLockHandle))
            {
                PRINTF("Failed to release semaphore for stream %lu\n", 0);
                gAppCtx.mpStop = true;
                break;
            }
            continue;
        }

        ccl_Frame_t *lpOldFrame = lpStr->mpFrameOnDisplay;
        lpStr->mpFrameOnDisplay = lpStr->mpFrameNew;
        lpStr->mpFrameNew = NULL;

        if (pdTRUE != xSemaphoreGive(lpStr->mLockHandle))
        {
            PRINTF("Failed to release semaphore for stream %lu\n", 0);
            gAppCtx.mpStop = true;
            break;
        }

        // TODO remove (as we are working with static image now we need
        // to know that images are actually generated, so I paint a moving line ito output image)
        uint64_t Width = lpStr->mpFrameOnDisplay->mImg[CCA_IOBUFT_C012].mWidth;
        uint64_t LineOffset = lpStr->mpFrameOnDisplay->mMetaData.mFrameNo % APP_PANEL_HEIGHT;
        uint64_t Offset = LineOffset * Width * (lpStr->mpFrameOnDisplay->mImg[CCA_IOBUFT_C012].mBpp/8);
        memset(((uint8_t *)lpStr->mpFrameOnDisplay->mImg[CCA_IOBUFT_C012].mpData) + Offset, 0xFF, 64);

        if (gDisplayFps == UINT32_MAX)
        {
            PRINTF("Frame counter overflow.\n");
            gDisplayFps = 0;
        }
        gDisplayFps++;

        DPU_SetFetchUnitSrcBufferAddr(APP_DPU, kDPU_FetchYuv0, 0, (uint32_t)lpStr->mpFrameOnDisplay->mImg[CCA_IOBUFT_C012].mpData);

        APP_TriggerContentStreamShadowLoad();

        if (lpOldFrame != NULL)
        {
            if (CCL_ERR_SUCCESS != ccl_PushFrame(ISP_HWID_0, (ispdrv_CamPipeId_t)SELECTED_CHANNEL, lpOldFrame))
            {
                PRINTF("Failed to put frame back to frame pool\n");
                break;
            }
        }
    }
}


/******************************************************************************/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

void DPU_IRQHandler(void)
{
    uint32_t intGroup0Flags, intGroup1Flags;
    intGroup0Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 0);
    intGroup1Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 1);

    /* Check the ExtDstxShadowLoadInterrupt in status reg for Display engine */
    if ((intGroup0Flags & APP_CONTENT_STREAM_INT_GROUP0) || (intGroup1Flags & APP_CONTENT_STREAM_INT_GROUP0))
    {
        s_isContentStreamShadowPending = false;
        DPU_ClearInterruptsPendingFlags(APP_DPU_DC, 0, intGroup0Flags);
        DPU_ClearInterruptsPendingFlags(APP_DPU_DC, 1, intGroup1Flags);
    }

    DPU_ClearInterruptsPendingFlags(APP_DPU, 0, intGroup0Flags);
    DPU_ClearInterruptsPendingFlags(APP_DPU, 1, intGroup1Flags);
    SDK_ISR_EXIT_BARRIER;
}

/******************************************************************************/

void StartDisplayTask(void)
{
    gDisplayTask = xTaskCreateStatic(DisplayTask, "Display task", STACK_SIZE, NULL, configMAX_PRIORITIES/2, gStack, &gTaskBuffer);

    if (NULL == gDisplayTask)
    {
        PRINTF("Display task creation failed!.\r\n");
        while (1)
            ;
    }
}
