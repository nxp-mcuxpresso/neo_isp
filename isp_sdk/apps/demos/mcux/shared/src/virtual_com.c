/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
/*${standard_header_anchor}*/
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"

#include "usb_device_descriptor.h"
#include "virtual_com.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
// size of stack for main USB task (in stack indexes, NOT in bytes)
#define USB_DELIVERY_TASK_STACK_SIZE 4500L
// amount of transfers to schedule directly on USB hw
// we send this amount of transfers than need to handle IRQ and schedule more
// max for imx95 usb2 is 16
#define USB_TX_BULK_SCHEDULE_SIZE    (14U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
// functions defined in hardware_init.c, specific for each soc/cpu
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);

// mandatory callbacks for USB control lib. Defined in this file. Need to declare to init variables.
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Data structure of virtual com device - local control structure */
usb_cdc_vcom_struct_t s_cdcVcom;
static char const *s_appName = "USB Rx delivery task";
static StackType_t s_usbTaskStack[USB_DELIVERY_TASK_STACK_SIZE];
static StaticTask_t s_usbTaskTcb;

/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU,
                                                          (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {(COUNTRY_SETTING >> 0U) & 0x00FFU,
                                                        (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_cdc_acm_info_t s_usbCdcAcmInfo;
/* Data buffer for receiving and sending*/
DDR_NOINIT static uint8_t gUsbRxBuf[DATA_BUFF_CNT][DATA_BUFF_SIZE];
volatile static uint8_t gUsbRxActiveIdx = 0;
volatile static uint8_t gUsbRxReadyIdx = 0;
volatile static uint32_t gUsbRxSize[DATA_BUFF_CNT] = {0};
volatile static bool gUsbRxStall = false;
static const uint8_t *gUsbTxBufPtr = NULL;
volatile static size_t gUsbTxRemaining = 0;
SemaphoreHandle_t gUsbTxIdle;
StaticSemaphore_t gUsbTxIdleBuffer;
SemaphoreHandle_t gUsbTxDone;
StaticSemaphore_t gUsbTxDoneBuffer;

/* USB device class information */
static usb_device_class_config_struct_t s_cdcAcmConfig[1] = {{
    USB_DeviceCdcVcomCallback,
    0,
    &g_UsbDeviceCdcVcomConfig,
}};

/* USB device class configuration information */
static usb_device_class_config_list_struct_t s_cdcAcmConfigList = {
    s_cdcAcmConfig,
    USB_DeviceCallback,
    1,
};

#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
volatile static uint8_t s_waitForDataReceive = 0;
volatile static uint8_t s_comOpen            = 0;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle          The CDC ACM class handle.
 * @param event           The CDC ACM class event type.
 * @param param           The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
    uint32_t len;
#endif
    uint8_t *uartBitmap;
    usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
    usb_device_cdc_acm_request_param_struct_t *acmReqParam;
    usb_device_endpoint_callback_message_struct_t *epCbParam;
    acmReqParam = (usb_device_cdc_acm_request_param_struct_t *)param;
    epCbParam   = (usb_device_endpoint_callback_message_struct_t *)param;
    switch (event)
    {
        case kUSB_DeviceCdcEventSendResponse:
        {
            assert(s_cdcVcom.cdcAcmHandle == handle);

            gUsbTxBufPtr += epCbParam->length;
            gUsbTxRemaining -= epCbParam->length;

            if ((epCbParam->length != 0) &&
                (0U == (epCbParam->length % g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize)))
            {
                /* If the last packet is the size of endpoint, then send also zero-ended packet,
                 ** meaning that we want to inform the host that we do not have any additional
                 ** data, so it can flush the output.
                 */
                error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_BULK_IN_ENDPOINT, NULL, 0);
            }
            else
            {
                if (gUsbTxRemaining == 0)
                {
                    BaseType_t reschedule = pdFALSE;
                    if (pdTRUE != xSemaphoreGiveFromISR(gUsbTxDone, &reschedule))
                    {
                        PRINTF("Fail to give semaphore, USB tx will block from now. \n");
                    }
                    else
                    {
                        portYIELD_FROM_ISR(reschedule);
                    }
                    error = kStatus_USB_Success;
                }
                else
                {
                    size_t TxSize = gUsbTxRemaining;
                    if (TxSize >  g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize * USB_TX_BULK_SCHEDULE_SIZE)
                    {
                        TxSize = g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize * USB_TX_BULK_SCHEDULE_SIZE;
                    }

                    error = USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, (uint8_t *)gUsbTxBufPtr, TxSize);
                    if (error != kStatus_USB_Success)
                    {
                        /* Failure to send Data Handling code here */
                        PRINTF("Fail to send data over USB");
                        while(1) {}
                    }
                }
            }
        }
        break;
        case kUSB_DeviceCdcEventRecvResponse:
        {
            assert(s_cdcVcom.cdcAcmHandle == handle);
            bool MoreDataReceived = false;
            error = kStatus_USB_Success;
            if (epCbParam->length > 0)
            {
                assert(gUsbRxStall == false);
                // *** received normal packet ***
                // store received size
                gUsbRxSize[gUsbRxActiveIdx] = epCbParam->length;
                // move to next buffer
                gUsbRxActiveIdx = (gUsbRxActiveIdx+1) % DATA_BUFF_CNT;
                if (gUsbRxActiveIdx == gUsbRxReadyIdx)
                {
                    // all buffers are full
                    gUsbRxStall = true;
                }

                MoreDataReceived = true;
            }
            else
            {
                // just empty packet, USB notification that previous packet was aligned with endpoint size and that we can "flush" buffered data
                // I do not use this mechanism, we flush each USB packet immediately
            }

            if (!gUsbRxStall)
            {
                error = USB_DeviceCdcAcmRecv(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, gUsbRxBuf[gUsbRxActiveIdx], g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize);
                if (error != kStatus_USB_Success)
                {
                    PRINTF("Fail to prep receive of data over USB");
                    while(1) { }
                }
            }

            if (MoreDataReceived)
            {
                BaseType_t reschedule = pdFALSE;
                if (pdTRUE != xTaskNotifyFromISR(s_cdcVcom.mRxDeliveryTask, 0, eNoAction, &reschedule))
                {
                    PRINTF("fail to wake USB rx delivery task\n\r");
                }
                else
                {
                    portYIELD_FROM_ISR(reschedule);
                }
            }
        }
        break;
        case kUSB_DeviceCdcEventSerialStateNotif:
            ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
            error                                                 = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSendEncapsulatedCommand:
            break;
        case kUSB_DeviceCdcEventGetEncapsulatedResponse:
            break;
        case kUSB_DeviceCdcEventSetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                if (1U == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_abstractState;
                    *(acmReqParam->length) = sizeof(s_abstractState);
                }
                else
                {
                    /* no action, data phase, s_abstractState has been assigned */
                }
                error = kStatus_USB_Success;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                if (1U == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_countryCode;
                    *(acmReqParam->length) = sizeof(s_countryCode);
                }
                else
                {
                    /* no action, data phase, s_countryCode has been assigned */
                }
                error = kStatus_USB_Success;
            }
            else
            {
                /* no action, return kStatus_USB_InvalidRequest */
            }
            break;
        case kUSB_DeviceCdcEventGetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_abstractState;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
                error                  = kStatus_USB_Success;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_countryCode;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
                error                  = kStatus_USB_Success;
            }
            else
            {
                /* no action, return kStatus_USB_InvalidRequest */
            }
            break;
        case kUSB_DeviceCdcEventClearCommFeature:
            break;
        case kUSB_DeviceCdcEventGetLineCoding:
            *(acmReqParam->buffer) = s_lineCoding;
            *(acmReqParam->length) = LINE_CODING_SIZE;
            error                  = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetLineCoding:
        {
            if (1U == acmReqParam->isSetup)
            {
                *(acmReqParam->buffer) = s_lineCoding;
                *(acmReqParam->length) = sizeof(s_lineCoding);
            }
            else
            {
                /* no action, data phase, s_lineCoding has been assigned */
            }
            error = kStatus_USB_Success;
        }
        break;
        case kUSB_DeviceCdcEventSetControlLineState:
        {
            s_usbCdcAcmInfo.dteStatus = acmReqParam->setupValue;
            /* activate/deactivate Tx carrier */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }

            /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }
            /* Com port of terminal tool running on PC is closed now */
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }

            /* Indicates to DCE if DTE is present or not */
            acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

            /* Initialize the serial state buffer */
            acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                /* bmRequestType */
            acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
            acmInfo->serialStateBuf[2] = 0x00;                              /* wValue */
            acmInfo->serialStateBuf[3] = 0x00;
            acmInfo->serialStateBuf[4] = 0x00;                              /* wIndex */
            acmInfo->serialStateBuf[5] = 0x00;
            acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE;                  /* wLength */
            acmInfo->serialStateBuf[7] = 0x00;
            /* Notify to host the line state */
            acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
            /* Lower byte of UART BITMAP */
            uartBitmap    = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
            uartBitmap[0] = acmInfo->uartState & 0xFFu;
            uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
            len = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
            if (0U == ((usb_device_cdc_acm_struct_t *)handle)->hasSentState)
            {
                error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf, len);
                if (kStatus_USB_Success != error)
                {
                    usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
                }
                ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 1;
            }
#endif
            /* Update status */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                /*  To do: CARRIER_ACTIVATED */
            }
            else
            {
                /* To do: CARRIER_DEACTIVATED */
            }

            if (1U == s_cdcVcom.attach)
            {
                s_cdcVcom.startTransactions = 1;
            }
            error = kStatus_USB_Success;
        }
        break;
        case kUSB_DeviceCdcEventSendBreak:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint16_t *temp16   = (uint16_t *)param;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            s_cdcVcom.attach               = 0;
            s_cdcVcom.currentConfiguration = 0U;
            error                          = kStatus_USB_Success;

#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if !((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
            /* The work-around is used to fix the HS device Chirping issue.
             * Please refer to the implementation for the detail information.
             */
            USB_DeviceHsPhyChirpIssueWorkaround();
#endif
#endif

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_cdcVcom.speed))
            {
                USB_DeviceSetSpeed(handle, s_cdcVcom.speed);
            }
#endif
        }
        break;
#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))
        case kUSB_DeviceEventDetach:
        {
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if !((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
            USB_DeviceDisconnected();
#endif
#endif
            error = kStatus_USB_Success;
        }
        break;
#endif
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                s_cdcVcom.attach               = 0;
                s_cdcVcom.currentConfiguration = 0U;
                error                          = kStatus_USB_Success;
            }
            else if (USB_CDC_VCOM_CONFIGURE_INDEX == (*temp8))
            {
                s_cdcVcom.attach               = 1;
                s_cdcVcom.currentConfiguration = *temp8;
                error                          = kStatus_USB_Success;
                /* Schedule buffer for receive */
                error = USB_DeviceCdcAcmRecv(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, gUsbRxBuf[gUsbRxActiveIdx], g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize);
                if (error != kStatus_USB_Success)
                {
                    usb_echo("USB_DeviceCdcAcmRecv error!");
                    while (1) {}
                }
            }
            else
            {
                /* no action, return kStatus_USB_InvalidRequest */
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (0U != s_cdcVcom.attach)
            {
                uint8_t interface        = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (interface == USB_CDC_VCOM_COMM_INTERFACE_INDEX)
                {
                    if (alternateSetting < USB_CDC_VCOM_COMM_INTERFACE_ALTERNATE_COUNT)
                    {
                        s_cdcVcom.currentInterfaceAlternateSetting[interface] = alternateSetting;
                        error                                                 = kStatus_USB_Success;
                    }
                }
                else if (interface == USB_CDC_VCOM_DATA_INTERFACE_INDEX)
                {
                    if (alternateSetting < USB_CDC_VCOM_DATA_INTERFACE_ALTERNATE_COUNT)
                    {
                        s_cdcVcom.currentInterfaceAlternateSetting[interface] = alternateSetting;
                        error                                                 = kStatus_USB_Success;
                    }
                }
                else
                {
                    /* no action, return kStatus_USB_InvalidRequest */
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (NULL != param)
            {
                /* Get current configuration request */
                *temp8 = s_cdcVcom.currentConfiguration;
                error  = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (NULL != param)
            {
                /* Get current alternate setting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_CDC_VCOM_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | s_cdcVcom.currentInterfaceAlternateSetting[interface];
                    error   = kStatus_USB_Success;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (NULL != param)
            {
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (NULL != param)
            {
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (NULL != param)
            {
                /* Get device string descriptor request */
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            /* no action, return kStatus_USB_InvalidRequest */
            break;
    }

    return error;
}

static void CDC_VCOM_FreeRTOSEnterCritical(uint32_t *sr)
{
    *sr = DisableGlobalIRQ();
    __ASM("CPSID i");
}

static void CDC_VCOM_FreeRTOSExitCritical(uint32_t sr)
{
    EnableGlobalIRQ(sr);
}

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    s_cdcVcom.speed        = USB_SPEED_HIGH;
    s_cdcVcom.attach       = 0;
    s_cdcVcom.cdcAcmHandle = (class_handle_t)NULL;
    s_cdcVcom.deviceHandle = NULL;

    if (kStatus_USB_Success != USB_DeviceClassInit(CONTROLLER_ID, &s_cdcAcmConfigList, &s_cdcVcom.deviceHandle))
    {
        usb_echo("USB device init failed\r\n");
    }
    else
    {
        usb_echo("USB device init OK\r\n");
        s_cdcVcom.cdcAcmHandle = s_cdcAcmConfigList.config->classHandle;
    }

    USB_DeviceIsrEnable();

    /*Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection.*/
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(s_cdcVcom.deviceHandle);
}

int32_t UsbTransmit(const uint8_t *apcData, size_t aSize)
{
    int32_t ret = 0;

    if (1U != s_cdcVcom.startTransactions)
    {
        // USB not connected yet
        return -2;
    }
    if (apcData == NULL)
    {
        return -1;
    }
    if (aSize == 0)
    {
        return 0;
    }

    if (1U != s_cdcVcom.attach)
    {
        // USB not connected yet to anything
        return -2;
    }

    if (pdTRUE != xSemaphoreTakeRecursive(gUsbTxIdle, portMAX_DELAY))
    {
        return -1;
    }

    assert(gUsbTxBufPtr == NULL);
    assert(gUsbTxRemaining == 0);

    gUsbTxBufPtr = apcData;
    gUsbTxRemaining = aSize;

    size_t TxSize = gUsbTxRemaining;
    if (TxSize >  g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize * USB_TX_BULK_SCHEDULE_SIZE)
    {
        TxSize = g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize * USB_TX_BULK_SCHEDULE_SIZE;
    }

    usb_status_t error = USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, (uint8_t *)gUsbTxBufPtr, TxSize);
    if (error != kStatus_USB_Success)
    {
        /* Failure to send Data Handling code here */
        PRINTF("Fail to send data over USB");
        ret = -1;
    }

    if (pdTRUE != xSemaphoreTake(gUsbTxDone, portMAX_DELAY))
    {
        PRINTF("Fail to await USB tx done. \n");
        ret = -1;
    }

    gUsbTxBufPtr = NULL;
    gUsbTxRemaining = 0;

    if (pdTRUE != xSemaphoreGiveRecursive(gUsbTxIdle))
    {
        PRINTF("Fail to give semaphore, USB tx will block from now. \n");
    }

    return ret;
}

int32_t UsbTransmitMultiple(uint8_t **apcData, size_t *aSize, uint8_t Cnt, TickType_t MaxDelay)
{
    int32_t ret = 0;
    if (1U != s_cdcVcom.startTransactions)
    {
        // USB not connected yet
        return -2;
    }

    if (apcData == NULL || aSize == NULL)
    {
        return -1;
    }

    if (Cnt == 0)
    {
        return 0;
    }

    // here we recursively block USBtx mutex (UsbTransmit blocks it again) to avoid any insertions while switching to next buffer
    if (pdTRUE != xSemaphoreTakeRecursive(gUsbTxIdle, MaxDelay))
    {
        return -1;
    }

    // transmit buffers one by one
    for (uint8_t i = 0; i < Cnt; i++)
    {
        assert(gUsbTxBufPtr == NULL);
        assert(gUsbTxRemaining == 0);

        if (aSize[i] == 0)
        {
            continue;
        }
        if (apcData[i] == NULL)
        {
            continue;
        }

        int32_t ret = UsbTransmit(apcData[i], aSize[i]);
        if (ret != 0)
        {
            break;
        }
    }

    if (pdTRUE != xSemaphoreGiveRecursive(gUsbTxIdle))
    {
        PRINTF("Fail to give semaphore, USB tx will block from now. \n");
    }

    return 0;
}

/**
 * @brief USB Receive Delivery Task
 *
 * @param apParameters Pointer to task parameters (unused).
 */
void UsbRxDeliveryTask(void *apParameters)
{
    uint32_t usbOsaCurrentSr;

    CDC_VCOM_FreeRTOSEnterCritical(&usbOsaCurrentSr);

    while (1)
    {
        // if rx idx !=  of tx idx, we have data to deliver
        // if rx idx == tx idx we either completely emtpy or completely full
        // gUsbRxStall tells us which one - if true we are full => USB hw was not given next buffer and therefore it signals to peer that it can't transmit
        if ((gUsbRxReadyIdx == gUsbRxActiveIdx) && !gUsbRxStall)
        {
            // *** we are completely empty, wait for data ***

            CDC_VCOM_FreeRTOSExitCritical(usbOsaCurrentSr);

            if (pdTRUE != xTaskNotifyWait(0, 0, 0, pdMS_TO_TICKS(1000000u)))
            {
                PRINTF("UsbRxDeliveryTask (possible timeout)\n\r");
                vTaskDelay(configTICK_RATE_HZ);
                continue;
            }

            CDC_VCOM_FreeRTOSEnterCritical(&usbOsaCurrentSr);
        }
        else
        {
            // *** we have data to deliver ***

            CDC_VCOM_FreeRTOSExitCritical(usbOsaCurrentSr);
            // deliver data to client code
            UsbEventMoreData(gUsbRxBuf[gUsbRxReadyIdx], gUsbRxSize[gUsbRxReadyIdx]);

            CDC_VCOM_FreeRTOSEnterCritical(&usbOsaCurrentSr);

            gUsbRxSize[gUsbRxReadyIdx] = 0;
            // check if we have USB Rx blocked (for no space reason)
            if (gUsbRxStall)
            {
                assert(gUsbRxReadyIdx == gUsbRxActiveIdx);
                gUsbRxStall = false;
                gUsbRxReadyIdx = (gUsbRxReadyIdx+1) % DATA_BUFF_CNT;

                // active buffer was just freed, can start receiving more data
                usb_status_t error = USB_DeviceCdcAcmRecv(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, gUsbRxBuf[gUsbRxActiveIdx], g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize);
                if (error != kStatus_USB_Success)
                {
                    PRINTF("Fail to prep receive of data over USB");
                    // coverity[infinite_loop:SUPPRESS]
                    while(1)
                    {
                    }
                }
            }
            else
            {
                gUsbRxReadyIdx = (gUsbRxReadyIdx+1) % DATA_BUFF_CNT;
            }
        }
    }
}

void StartUsb(void)
{
    gUsbTxIdle = xSemaphoreCreateRecursiveMutexStatic(&gUsbTxIdleBuffer);
    gUsbTxDone = xSemaphoreCreateBinaryStatic(&gUsbTxDoneBuffer);
    if (gUsbTxIdle == NULL || gUsbTxDone == NULL)
    {
        PRINTF("USB semaphore creation failed!.\r\n");
        // coverity[infinite_loop:SUPPRESS]
        while (1) {}
    }

    USB_DeviceApplicationInit();

    s_cdcVcom.mRxDeliveryTask = xTaskCreateStatic(UsbRxDeliveryTask, s_appName, USB_DELIVERY_TASK_STACK_SIZE, NULL, 1, s_usbTaskStack, &s_usbTaskTcb);

    if (s_cdcVcom.mRxDeliveryTask == NULL)
    {
        PRINTF("USB Rx delivery task creation failed!.\r\n");
        // coverity[infinite_loop:SUPPRESS]
        while (1) {}
    }
}
