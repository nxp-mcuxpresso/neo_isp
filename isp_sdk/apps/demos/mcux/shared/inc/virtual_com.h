/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_CDC_VCOM_H_
#define _USB_CDC_VCOM_H_ 1

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#define DATA_BUFF_SIZE HS_CDC_VCOM_BULK_OUT_PACKET_SIZE
#define DATA_BUFF_CNT 4U
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#define DATA_BUFF_SIZE FS_CDC_VCOM_BULK_OUT_PACKET_SIZE
#define DATA_BUFF_CNT 4U

#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#define DATA_BUFF_SIZE FS_CDC_VCOM_BULK_OUT_PACKET_SIZE
#define DATA_BUFF_CNT 4U

#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif
#define DATA_BUFF_SIZE HS_CDC_VCOM_BULK_OUT_PACKET_SIZE
#define DATA_BUFF_CNT 4U
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_DEVICE_INTERRUPT_PRIORITY (6U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif

/* Currently configured line coding */
#define LINE_CODING_SIZE (0x07)
#define LINE_CODING_DTERATE (115200)
#define LINE_CODING_CHARFORMAT (0x00)
#define LINE_CODING_PARITYTYPE (0x00)
#define LINE_CODING_DATABITS (0x08)

/* Communications feature */
#define COMM_FEATURE_DATA_SIZE (0x02)
#define STATUS_ABSTRACT_STATE (0x0000)
#define COUNTRY_SETTING (0x0000)

/* Notification of serial state */
#define NOTIF_PACKET_SIZE (0x08)
#define UART_BITMAP_SIZE (0x02)
#define NOTIF_REQUEST_TYPE (0xA1)

/* Define the types for application */
typedef struct _usb_cdc_vcom_struct
{
    usb_device_handle deviceHandle; /* USB device handle. */
    class_handle_t cdcAcmHandle; /* USB CDC ACM class handle.                                                         */
    volatile uint8_t attach;     /* A flag to indicate whether a usb device is attached. 1: attached, 0: not attached */
    TaskHandle_t deviceTaskHandle;      /* USB device task handle. */
    TaskHandle_t mRxDeliveryTask; /* Application task handle. */
    uint8_t speed; /* Speed of USB device. USB_SPEED_FULL/USB_SPEED_LOW/USB_SPEED_HIGH.                 */
    volatile uint8_t
        startTransactions; /* A flag to indicate whether a CDC device is ready to transmit and receive data.    */
    uint8_t currentConfiguration;                                           /* Current configuration value. */
    uint8_t currentInterfaceAlternateSetting[USB_CDC_VCOM_INTERFACE_COUNT]; /* Current alternate setting value for each
                                                                               interface. */
} usb_cdc_vcom_struct_t;

/* Define the information relates to abstract control model */
typedef struct _usb_cdc_acm_info
{
    uint8_t serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE]; /* Serial state buffer of the CDC device to notify the
                                                                     serial state to host. */
    bool dtePresent;          /* A flag to indicate whether DTE is present.         */
    uint16_t breakDuration;   /* Length of time in milliseconds of the break signal */
    uint8_t dteStatus;        /* Status of data terminal equipment                  */
    uint8_t currentInterface; /* Current interface index.                           */
    uint16_t uartState;       /* UART state of the CDC device.                      */
} usb_cdc_acm_info_t;

extern usb_cdc_vcom_struct_t s_cdcVcom;

/**
 * @brief This function need to be called before USB communication can be started.
 *
 */
void StartUsb(void);

/**
 * @brief Function that transmits data over USB virtual COM port.
 *
 * @param apcData Data to transmit.
 * @param aSize Number of bytes to transmit.
 * @return 0 on success, negative error code otherwise.
 *
 * @details This function should be called by client code when transmit is needed. Note that this is
 * blocking function. Upon return from this function, the data has been transmitted and "flushed" on the peer side.
 * For the duration of transmit apcData needs to be kept valid by the caller.
 */
int32_t UsbTransmit(const uint8_t *apcData, size_t aSize);

/**
 * @brief Function that transmits multiple data buffers over USB virtual COM port.
 *
 * @param apcData Array of pointers to data buffers to transmit.
 * @param aSize Array of sizes for each buffer in bytes.
 * @param Cnt Number of buffers to transmit.
 * @return 0 on success, negative error code otherwise.
 *
 * @details This function should be called by client code when uninterrupted sequence of transmit is needed over more buffers. Note that this is
 * blocking function. Upon return from this function, the data has been transmitted and "flushed" on the peer side.
 * For the duration of transmit apcData needs to be kept valid by the caller.
 */
int32_t UsbTransmitMultiple(uint8_t **apcData, size_t *aSize, uint8_t Cnt, TickType_t MaxDelay);

/**
 * @brief Function that receives data from USB virtual COM port.
 *
 * @param apcData Pointer to the data buffer with received data.
 * @param aSize Number of bytes received.
 *
 * @details This function needs to be implemented by client code. All data received over USB are
 *  passed to it. This function is called on dedicated FreeRtos task context (thread).
 */
void UsbEventMoreData(const uint8_t *apcData, size_t aSize);

#endif /* _USB_CDC_VCOM_H_ */
