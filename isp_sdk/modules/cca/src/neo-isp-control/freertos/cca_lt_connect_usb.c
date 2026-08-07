/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cca_lt_connect_usb.c
 * @brief Implements connection with live tuning module
 *******************************************************************************/

#include "stdint.h"
#include "cca.h"
#include "virtual_com.h"
#include "lt/live_tuning.h"
#include "cca.h"
#include "ilog.h"

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
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

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

cca_Result_t cca_LtConnect()
{
    StartUsb();

    // Initialize the LT communication module and pass the write function to LT
    if (0U != lt_InitLtModule(UsbTransmit))
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_ERR, "Failed to init communication live tuning module.\n");
        return CCA_ERROR;
    }

    return CCA_SUCCESS;
}

/******************************************************************************/

cca_Result_t cca_LiveTune()
{
    // called after CCA process finished - not used for USB

    return CCA_SUCCESS;
}

/******************************************************************************/

//#define USB_SPEED_TEST
#ifndef USB_SPEED_TEST
void UsbEventMoreData(const uint8_t *apcData, size_t aSize)
{
    assert(apcData != NULL);
    assert(aSize < DATA_BUFF_SIZE);

    // Data available -> send them to Live tuning
    if (0 != lt_ProcessRawPacket(apcData, aSize))
    {
        ILOG(ILOG_MOD_LT, ILOG_LEV_ERR, "Failed to process TT message.\n");
    }
}
#else
void UsbEventMoreData(const uint8_t *apcData, size_t aSize)
{
    static uint32_t s_usbRxTotalSize = 0;
    s_usbRxTotalSize += aSize;
    if (s_usbRxTotalSize > 100*1024*1024)
    {
        // loop back random data for speed test
        UsbTransmit(apcData, 80*1024*1024);
        ILOG(ILOG_MOD_LT, ILOG_LEV_NFO, "USB Rx total size: %u bytes\r\n", s_usbRxTotalSize);
        s_usbRxTotalSize = 0;
    }
}
#endif

/******************************************************************************/
