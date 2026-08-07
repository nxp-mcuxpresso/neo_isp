/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef USB_STDOUT_H
#define USB_STDOUT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Prepares buffers, mutexes and initiates USB sending thread.
 * @details Stdout printing is enabled immediately, USB sending takes a bit more time, but once ready prints all the buffer content.
 * This function should be called as early as possible. USB starts sending only after FreeRtos is running threads.
 */
void UsbStdOutInit(void);

#ifdef __cplusplus
}
#endif

#endif /* USB_STDOUT_H */
