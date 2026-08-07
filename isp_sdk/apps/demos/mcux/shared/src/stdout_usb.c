/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/******************************************************************************/
/**
 * @file  stdout_usb.c
 * @brief redirects stdout to USB virtual COM port
 *******************************************************************************/

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"

#include "virtual_com.h"
#include "fsl_debug_console.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
// size of a single stdout buffer
#define STDOUT_BUF_SIZE 4096
// number of stdout buffers, now allocated on DDR
#define STDOUT_BUF_CNT  64
// size of USB transmit thread's stack
#define STDOUT_TX_STACK_SIZE 256
/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

/*==============================================================================
*                              LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/
// variables for thread that handles continual transmit of stdout data
static TaskHandle_t gTaskHandle;
static StaticTask_t gTaskBuffer;
static StackType_t gStack[STDOUT_TX_STACK_SIZE];

// stdout buffer variables
// USB transmit happens only if is attached (by cable) and opened (by any application on host).
// Up to this point we only buffer the data.
static DDR_NOINIT uint8_t StdOutBufs[STDOUT_BUF_CNT][STDOUT_BUF_SIZE];
static uint8_t StdoutPrepBufIdx = 0;
static uint8_t StdoutSendBufIdx = 0;
static uint32_t StdoutTxBufSize[STDOUT_BUF_CNT];

// mutex (handle and buffer) for protecting stdout buffer variables
// avoids race conditions when _write from threads access stdout buffers simultaneously also transmit thread accesses them
// not that this mutex is used along with ISR block and thread switch block
static SemaphoreHandle_t StdOutVarLock;
static StaticSemaphore_t StdOutVarLockData;

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/
#ifdef SEND_IMG_VIA_USB
// just a marker to indicate that next ready image should be sent via USB
// increased form PC host
// This is only temporary demonstration, until test team implements its own handling
uint32_t ShouldSendOutImg = 0;
#endif
/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/**
 * @brief A callback from USB subsystem that indicates new data arrived from PC host
 *
 * @param apcData pointer to data received from USB host
 * @param aSize size of received data in bytes
 */
void UsbEventMoreData(const uint8_t *apcData, size_t aSize)
{
#ifdef SEND_IMG_VIA_USB
    if (apcData == NULL || aSize == 0)
    {
        return;
    }

    if (strcmp((const char *)apcData, "SendOutImg") == 0)
    {
        // received request from PC host that we should send image via USB
        ShouldSendOutImg += CONFIG_ISP_SDK_CAMERA_COUNT;
    }
#endif
}

/**
 * @brief Overload of stdlib _write function to redirect stdout to USB virtual COM port
 *
 * @param fd file descriptor (unused, stdout)
 * @param ptr pointer to data to write
 * @param len number of bytes to write
 * @return positive number indicating amount of bytes written, or negative error
 *
 * @details Writes data to stdout buffer, handling buffer management and thread synchronization.
 * Actual USB transmit is done continually by dedicated thread.
 */
int _write(int fd, char *ptr, int len)
{
    (void)fd;

    // interrupts are blocked if transmit thread is running or if this function called from non ISR context =>
    // => if we arrive here on ISR context, we are sure that the mutex is free and need not to lock it as ISR cannot be preempted
    if (!xPortIsInsideInterrupt())
    {
        // block interrupts and acquire mutex for thread-safe access
        vPortEnterCritical();
        if (pdTRUE != xSemaphoreTake(StdOutVarLock, portMAX_DELAY))
        {
            return -1;
        }
    }

    // size of data can easily exceed buffer size, so we need to handle it in chunks
    uint32_t remaining = len;
    uint8_t *src = (uint8_t *)ptr;
    while (remaining)
    {
        // werify is there is space in current buffer
        if ((StdoutTxBufSize[StdoutPrepBufIdx] == STDOUT_BUF_SIZE) || (StdoutPrepBufIdx == StdoutSendBufIdx && StdoutTxBufSize[StdoutPrepBufIdx] > 0))
        {
            // prep buffer is full OR
            // buffer already have some data and therefore tx thread taken it for transmission

            // check if next buffer can be used
            uint8_t PrepBufIdx_next = (StdoutPrepBufIdx + 1) % STDOUT_BUF_CNT;

            if (StdoutTxBufSize[PrepBufIdx_next] != 0)
            {
                if (!xPortIsInsideInterrupt())
                {
                    // we just cycled through all buffers and next one is still being used =>
                    // => BUFFER OVERFLOW, data loss
                    if (pdTRUE != xSemaphoreGive(StdOutVarLock))
                    {
                        vPortExitCritical();
                        return -1;
                    }
                    vPortExitCritical();
                }
                return -2;
            }
            else
            {
                // next buffer is free, switch to it
                StdoutPrepBufIdx = PrepBufIdx_next;
                assert(StdoutTxBufSize[StdoutPrepBufIdx] == 0);
            }
        }

        // compute how much to coby based on remaining data size and remaining space in current buffer
        uint32_t copy_len = (remaining > (STDOUT_BUF_SIZE - StdoutTxBufSize[StdoutPrepBufIdx])) ? (STDOUT_BUF_SIZE - StdoutTxBufSize[StdoutPrepBufIdx]) : remaining;

        // copy data from source to our buffer for later transmit and update buffer and copy state
        memcpy(&StdOutBufs[StdoutPrepBufIdx][StdoutTxBufSize[StdoutPrepBufIdx]], src, copy_len);
        StdoutTxBufSize[StdoutPrepBufIdx] += copy_len;
        remaining -= copy_len;
        src += copy_len;
    }

    if (!xPortIsInsideInterrupt())
    {
        if (pdTRUE != xSemaphoreGive(StdOutVarLock))
        {
            vPortExitCritical();
            return -1;
        }
        vPortExitCritical();

        // notify transmit thread there are new data to send
        if (pdTRUE != xTaskNotify(gTaskHandle, 0, eNoAction))
        {
            return -1;
        }
    }
    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // notify transmit thread there are new data to send
        if (pdTRUE != xTaskNotifyFromISR(gTaskHandle, 0, eNoAction, &xHigherPriorityTaskWoken))
        {
            return -1;
        }

        // switch to USB transmit thread if it has higher priority than task that was running before this ISR
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }


    return len;
}

/**
 * @brief USB transmit thread
 *
 * @param apParameters task parameters (unused)
 */
static void StdoutTransmitTask(void *apParameters)
{
    // block interrupts and acquire mutex for thread-safe access
    vPortEnterCritical();
    if (pdTRUE != xSemaphoreTake(StdOutVarLock, portMAX_DELAY))
    {
        // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
        // coverity[infinite_loop:SUPPRESS]
        for(;;);
    }

    while (1)
    {
        // check state - if USB is attached and connected (opend by host)
        if (0U == s_cdcVcom.startTransactions)
        {
            // Other side of USB is not listening, just wait and try again
            if (pdTRUE != xSemaphoreGive(StdOutVarLock))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }
            vPortExitCritical();

            (void)vTaskDelay(pdMS_TO_TICKS(100));

            vPortEnterCritical();
            if (pdTRUE != xSemaphoreTake(StdOutVarLock, portMAX_DELAY))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }
        }
        else if (StdoutTxBufSize[StdoutSendBufIdx] < 0)
        {
            // no data to send, wait for notification
            if (pdTRUE != xSemaphoreGive(StdOutVarLock))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }
            vPortExitCritical();

            (void)xTaskNotifyWait(0, 0, 0, portMAX_DELAY);

            vPortEnterCritical();
            if (pdTRUE != xSemaphoreTake(StdOutVarLock, portMAX_DELAY))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }
        }
        else
        {
            // cache current buffer index and size for transmission
            uint8_t *ptr = (uint8_t *)StdOutBufs[StdoutSendBufIdx];
            uint32_t len = StdoutTxBufSize[StdoutSendBufIdx];

            if (pdTRUE != xSemaphoreGive(StdOutVarLock))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }
            vPortExitCritical();

            if (0 != UsbTransmit((uint8_t *)ptr, (uint32_t)len))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }

            vPortEnterCritical();
            if (pdTRUE != xSemaphoreTake(StdOutVarLock, portMAX_DELAY))
            {
                // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
                // coverity[infinite_loop:SUPPRESS]
                for(;;);
            }

            // mark the buffer as sent and move to the next buffer
            StdoutTxBufSize[StdoutSendBufIdx] = 0;
            StdoutSendBufIdx = (StdoutSendBufIdx + 1) % STDOUT_BUF_CNT;
        }
    }
}

void UsbStdOutInit(void)
{
    // initialize local variables
    memset(StdoutTxBufSize, 0, sizeof(StdoutTxBufSize));
    StdOutVarLock = xSemaphoreCreateMutexStatic(&StdOutVarLockData);

    // start transmit thread, note that the thread will probably not run immediately as this function should be called well before scheduler starts
    gTaskHandle = xTaskCreateStatic(StdoutTransmitTask, "StdoutTx", STDOUT_TX_STACK_SIZE, NULL, 1, gStack, &gTaskBuffer);
    if (NULL == gTaskHandle)
    {
        // NOTE: there is no warning message for failure as this is stdout and any message will fail to be send
        // coverity[infinite_loop:SUPPRESS]
        while (1)
            ;
    }
}
