/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DISPLAY_TASK_H
#define DISPLAY_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "display_support.h"
#include "FreeRTOS.h"
#include "semphr.h"

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/* RGB8888, 3 bytes per pixel. */
#define APP_BPP 3

#define APP_PANEL_FB_ADDR_ALIGN_BYTE   (32U)
#define APP_PANEL_FB_STRIDE_ALIGN_BYTE APP_BPP

#define APP_PANEL_FB_STRIDE_BYTE  (SDK_SIZEALIGN(APP_BPP * APP_PANEL_WIDTH, APP_PANEL_FB_STRIDE_ALIGN_BYTE))
#define APP_PANEL_FB_STRIDE_PIXEL (APP_PANEL_FB_STRIDE_BYTE / APP_BPP)

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

extern TaskHandle_t gDisplayTask;
extern uint32_t gDisplayFps;

void StartDisplayTask(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_TASK_H */
