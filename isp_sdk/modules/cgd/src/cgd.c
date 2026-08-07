/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cgd.c
 * @brief userspace i/f to access I2C devices.
 *******************************************************************************/
/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include <stdlib.h>

#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "cgd.h"
#include "cgd_internal.h"
#include "ilog.h"
#include "utils_i2c.h"
#include "cgd_hal.h"

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/**
 * @brief Default isi interruption handle - dont use
 */
static void IsiDefIrqHandle(cgd_IsiChannel_t aChannel);

/**
 * @brief Default Err interruption handle - dont use
 */
static void ErrDefIrqHandle(cgd_RegisterErrorTable_t* lastError);

/*==============================================================================
*                         LOCAL DATA
==============================================================================*/

/**< @brief local cache of the system info */
static cgd_System_t sgCgdSystemConfig = {CGD_STATE_OFF};
/**< @brief local cache of the config info */
static cgd_SystemExtCfg_t sgCgdExternalConfig = {0};
/*==============================================================================
*                         LOCAL FUNCTION DEFINITIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Default isi interruption handle - don't use
 */
static void IsiDefIrqHandle(cgd_IsiChannel_t aChannel)
{
    (void)aChannel;
    // ILOG(ILOG_MOD_ISI, ILOG_LEV_WRN, "CGD %d Isi Def Irq Handle warning!\n", aChannel);
    //  nothing to do
}

/******************************************************************************/
/**
 * @brief Default isi interruption handle - don't use
 */
static void ErrDefIrqHandle(cgd_RegisterErrorTable_t* lastError)
{
    (void)lastError;
    //  nothing to do
}

/******************************************************************************/
/**
 * @brief Call i2c_Open on selected port
 */
cgd_Result cgd_OpenI2CPort(cgd_CsiDeviceId_t acPortIdx)
{
    cgd_Result lRet     = CGD_LIB_SUCCESS;
    cgd_CsiPort_t* port = cgd_GetCsiPort(acPortIdx);

    if (port == NULL)
    {
        return lRet;
    }

    cgd_PortHead_t* head = port->mcHead;
    if (head != NULL)
    {
        if (i2c_Open(&head->mI2cPortDev) == I2C_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "i2c_Open failed to open port %d\n", acPortIdx);
            lRet = CGD_LIB_FAILURE;
        }
    }

    return lRet;
} // cgd_OpenI2CPort

/******************************************************************************/
cgd_Result cgd_Open(cgd_System_t* arcHardwareConfig)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    if (sgCgdSystemConfig.sgState != CGD_STATE_OFF)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "CGD error: driver already open\n");
        return CGD_LIB_FAILURE;
    }

    if (arcHardwareConfig == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD error: arcHardwareConfig is NULL\n");
        return CGD_LIB_FAILURE;
    }

    sgCgdSystemConfig         = *arcHardwareConfig;
    sgCgdSystemConfig.sgState = CGD_STATE_INIT;
    gpCgdFrameDoneEvtHandler = IsiDefIrqHandle; // clear frame done handler
    gpCgdFirstLineEvtHandler = IsiDefIrqHandle; // clear frame done handler
    gpCgdErrorEvtHandler     = ErrDefIrqHandle;

    ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD Opening driver...\n");

    // open CGD
    for (cgd_IsiChannel_t lIter = 0; lIter < CGD_ISI_CNT; lIter++)
    {
        cgd_IsiDevice_t* isiDev = sgCgdSystemConfig.mppIsiDevice[lIter];

        if (isiDev == NULL)
        {
            continue;
        }

        // call variable inits etc, from kernel
        if (cgd_IsiPrepare(isiDev) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "KernelIsiInit failed, port %d\n", lIter);
            lRet = CGD_LIB_FAILURE;
        }
    }

    for (cgd_CsiDeviceId_t lIter = CGD_MIPI_CSI_DEV; lIter < CGD_MIPI_CSI_CNT; lIter++)
    {
        cgd_CsiPort_t* csiPort = sgCgdSystemConfig.mppCsiPort[lIter];

        if (csiPort == NULL)
        {
            continue;
        }

        /* For GHS, we need to prepare CSI before opening I2C port, because I2C bus ID is read from DevTree using CSI2 IODevice */
        // call variable inits etc, from kernel
        if (cgd_CsiPrepare(lIter) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CsiInit failed, port %d\n", lIter);
            lRet = CGD_LIB_FAILURE;
        }
        // prepare I2C com port
        if (cgd_OpenI2CPort(lIter) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_OpenI2CPort failed, port %d\n", lIter);
            lRet = CGD_LIB_FAILURE;
        }

        csiPort->mPortState = CGD_STATE_OFF; // port is not yet configured
    }

    return lRet;
} // cgd_Open

/******************************************************************************/
cgd_Result cgd_Config(cgd_SystemExtCfg_t* arcIspCfg, cgd_IsiChannel_t aIsiIdx)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    if ((sgCgdSystemConfig.sgState == CGD_STATE_OFF)) // chceck if driver is open
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "failed to configure system, CGD is closed.\n");
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    if (arcIspCfg == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD error: arcIspCfg is NULL\n");
        return CGD_LIB_FAILURE;
    }

    sgCgdExternalConfig = *arcIspCfg;

    cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice(aIsiIdx);

    if (isiDev == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_Config failed to get ISI, port %d\n", aIsiIdx);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    // reconfigure csi port and i2c devices
    cgd_CsiPort_t* csiPort      = cgd_GetCsiPort(isiDev->mCsiId);
    cgd_CsiExtCfg_t* csiPortCfg = cgd_GetCsiPortConfig(isiDev->mCsiId);

    if (csiPort == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_Config failed to get CSI, port %d\n", isiDev->mCsiId);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    if (csiPort->mPortState < CGD_STATE_INIT)
    {
        csiPort->mPortState = CGD_STATE_INIT; // need be first, because cgd_PrepareAndExecTable is checking it

        if (cgd_PrepareAndExecTable(isiDev->mCsiId, csiPortCfg->mpcInitArr) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "mpcInitArr i2c send failed, port %d\n", isiDev->mCsiId);
            lRet = CGD_LIB_FAILURE;
            return lRet;
        }

        if (cgd_CsiOpen(isiDev->mCsiId) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CsiPortConfigure failed, port %d\n", isiDev->mCsiId);
            lRet = CGD_LIB_FAILURE;
            return lRet;
        }
    }

    // reconfigure isi ports
    if (cgd_IsiChannelConfigure(aIsiIdx) == CGD_LIB_FAILURE)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_IsiInitSettings failed, ISI Chn %d\n", aIsiIdx);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    return lRet;
} // cgd_Config

/******************************************************************************/
cgd_Result cgd_StreamOn(cgd_IsiChannel_t aIsiIdx)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice(aIsiIdx);

    if (isiDev == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOn failed to get ISI, port %d\n", aIsiIdx);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    cgd_CsiPort_t* pCsiPort = cgd_GetCsiPort(isiDev->mCsiId);

    if (pCsiPort == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOn failed to get CSI, port %d\n", aIsiIdx);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    cgd_CsiExtCfg_t* csiPortCfg = cgd_GetCsiPortConfig(isiDev->mCsiId);

    if (csiPortCfg == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOn failed to get CSI port %d config!\n", aIsiIdx);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    if (pCsiPort->mPortState != CGD_STATE_OFF && pCsiPort->mPortState != CGD_STATE_STRM)
    {
        pCsiPort->mPortState = CGD_STATE_STRM;

        if (cgd_PrepareAndExecTable(isiDev->mCsiId, csiPortCfg->mpcStreamEn) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "mpcStreamEn failed, port %d\n", isiDev->mCsiId);
            lRet = CGD_LIB_FAILURE;
        }

        ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD Started streaming on CSI %d...\n", isiDev->mCsiId);
    }

    cgd_State_t lIsiState = cgd_GetIsiState(aIsiIdx);

    // start ISI channel
    if (lIsiState != CGD_STATE_OFF && lIsiState != CGD_STATE_STRM)
    {
        // call defined isi port init method
        if (cgd_IsiChannelStreamOn(isiDev->mIsiId) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOn failed, port %d\n", aIsiIdx);
            lRet = CGD_LIB_FAILURE;
        }

        ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD Started streaming on ISI CHN %d...\n", isiDev->mIsiId);
    }

    return lRet;
} // cgd_StreamOn

/******************************************************************************/
cgd_Result cgd_StreamOff(cgd_IsiChannel_t aIsiIdx)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    // stop ISI channel
    cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice(aIsiIdx);

    if (isiDev == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOff failed to get ISI, port %d\n", aIsiIdx);
        return CGD_LIB_FAILURE;
    }

    if (cgd_GetIsiState(aIsiIdx) != CGD_STATE_STRM)
    {
        return lRet;
    }

    cgd_CsiPort_t* csiPort = cgd_GetCsiPort(isiDev->mCsiId);

    if (csiPort == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOff failed cgd_GetCsiPort, dev %d\n", isiDev->mCsiId);
        return CGD_LIB_FAILURE;
    }

    cgd_CsiExtCfg_t* csiPortCfg = cgd_GetCsiPortConfig(isiDev->mCsiId);

    if (csiPortCfg == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOn failed to get CSI port %d config!\n", aIsiIdx);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    if (csiPort->mPortState != CGD_STATE_OFF && csiPort->mPortState != CGD_STATE_STRM_OFF)
    {
        if (cgd_PrepareAndExecTable(isiDev->mCsiId, csiPortCfg->mpcStreamDis) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CSI send Stream Disabling I2C commands failed, port %d\n", isiDev->mIsiId);
            lRet = CGD_LIB_FAILURE;
        }

        csiPort->mPortState = CGD_STATE_STRM_OFF;
        ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD Streaming off on CSI %d\n", isiDev->mCsiId);
    }

    if (cgd_IsiChannelStreamOff(isiDev->mIsiId) == CGD_LIB_FAILURE)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_IsiCleanupSettings failed, port %d\n", isiDev->mIsiId);
        lRet = CGD_LIB_FAILURE;
    }

    ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD Streaming off on ISI %d\n", aIsiIdx);

    return lRet;
} // cgd_StreamOff

/******************************************************************************/
cgd_Result cgd_Close()
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    gpCgdFrameDoneEvtHandler = IsiDefIrqHandle; // clear frame done handler
    gpCgdFirstLineEvtHandler = IsiDefIrqHandle; // clear frame done handler
    gpCgdErrorEvtHandler     = ErrDefIrqHandle;

    if (sgCgdSystemConfig.sgState == CGD_STATE_OFF) // already closed
    {
        return lRet;
    }

    sgCgdSystemConfig.sgState = CGD_STATE_OFF;

    for (cgd_IsiChannel_t lIter = 0; lIter < CGD_ISI_CNT; lIter++)
    {
        cgd_IsiDevice_t* isiDev = cgd_GetIsiDevice(lIter);
        cgd_State_t lsIsiState  = cgd_GetIsiState(lIter);

        if (lsIsiState == CGD_STATE_OFF || isiDev == NULL)
        {
            continue;
        }

        if (lsIsiState == CGD_STATE_STRM)
        {
            if (cgd_StreamOff(isiDev->mIsiId) == CGD_LIB_FAILURE)
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_StreamOff failed, port %d\n", isiDev->mIsiId);
                lRet = CGD_LIB_FAILURE;
            }
        }

        if (cgd_IsiChannelClose(isiDev->mIsiId) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_IsiCleanupSettings failed, port %d\n", isiDev->mIsiId);
            lRet = CGD_LIB_FAILURE;
        }
    }

    for (cgd_CsiDeviceId_t lIter = CGD_MIPI_CSI_DEV; lIter < CGD_MIPI_CSI_CNT; lIter++)
    {
        cgd_CsiPort_t* csiPort = cgd_GetCsiPort(lIter);

        if (csiPort == NULL)
        {
            continue;
        }

        cgd_PortHead_t* cgdPortHead = csiPort->mcHead;
        cgd_CsiExtCfg_t* csiPortCfg = cgd_GetCsiPortConfig(lIter);

        if (csiPortCfg == NULL)
        {
            continue;
        }

        if (cgd_PrepareAndExecTable(lIter, csiPortCfg->mpcCloseArr) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "mpcCloseArr i2c send failed, port %d\n", lIter);
            lRet = CGD_LIB_FAILURE;
        }

        if (cgd_CsiClose(lIter) == CGD_LIB_FAILURE)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_CsiClose failed, port %d\n", lIter);
            lRet = CGD_LIB_FAILURE;
        }

        i2c_Close(&cgdPortHead->mI2cPortDev); // close file descriptor

        csiPort->mPortState = CGD_STATE_OFF;
        ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD closed CSI %d\n", lIter);
    }

    cgd_CleanUp();

    ILOG(ILOG_MOD_CGD, ILOG_LEV_DBG, "CGD driver closed\n");

    return lRet;
} // cgd_Close

/******************************************************************************/
cgd_CsiPort_t* cgd_GetCsiPort(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_CsiPort_t *lpRet;
    if (aPortIdx >= CGD_MIPI_CSI_CNT)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_GetCsiPort (%d) failed.\n", aPortIdx);
        lpRet = NULL;
    }
    else
    {
        lpRet = sgCgdSystemConfig.mppCsiPort[aPortIdx];
    }

    return lpRet;
} // cgd_GetCsiPort

/******************************************************************************/
cgd_CsiExtCfg_t* cgd_GetCsiPortConfig(cgd_CsiDeviceId_t aPortIdx)
{
    cgd_CsiExtCfg_t *lpRet;
    if (aPortIdx >= CGD_MIPI_CSI_CNT)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_GetCsiPortConfig (%d) failed.\n", aPortIdx);
        lpRet = NULL;
    }
    else
    {
        lpRet = sgCgdExternalConfig.mpCsi[aPortIdx];
    }

    return lpRet;
} // cgd_GetCsiPortConfig

/******************************************************************************/
cgd_IsiDevice_t* cgd_GetIsiDevice(cgd_IsiChannel_t aIsiIdx)
{
    cgd_IsiDevice_t *lpRet;
    if (aIsiIdx >= CGD_ISI_CNT)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_GetIsiDevice (%d) failed.\n", aIsiIdx);
        lpRet = NULL;
    }
    else
    {
        lpRet = sgCgdSystemConfig.mppIsiDevice[aIsiIdx];
    }

    return lpRet;
} // cgd_GetIsiDevice

/******************************************************************************/
cgd_IsiExtCfg_t* cgd_GetIsiDeviceConfig(cgd_IsiChannel_t aIsiIdx)
{
    cgd_IsiExtCfg_t *lpRet;
    if (aIsiIdx >= CGD_ISI_CNT)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "get ISI channel config (%d) failed.\n", aIsiIdx);
        lpRet = NULL;
    }
    else
    {
        lpRet = sgCgdExternalConfig.mpIsi[aIsiIdx];
    }

    return lpRet;
} // cgd_GetIsiDeviceConfig

/******************************************************************************/
cgd_State_t cgd_GetIsiState(cgd_IsiChannel_t aIsiIdx)
{
    cgd_State_t lsReturn      = CGD_STATE_OFF;
    if (aIsiIdx < CGD_ISI_CNT)
    {
        cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

        if (lpIsiDev != NULL)
        {
            lsReturn = lpIsiDev->mIsiState;
        }
    }

    return lsReturn;
}

/******************************************************************************/

bool cgd_IsEmptyBufferPrepared(cgd_IsiChannel_t aIsiIdx)
{
    bool lRet = false;

    if (aIsiIdx < CGD_ISI_CNT)
    {
        cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

        if (lpIsiDev != NULL)
        {
            lRet = cgd_IsiIsEmptyBufferPrepared(aIsiIdx);
        }
    }

    return lRet;
}

/******************************************************************************/

cgd_Result cgd_SubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apEmptyBuffer)
{
    cgd_Result lRet = CGD_LIB_FAILURE;
    if (aIsiIdx < CGD_ISI_CNT)
    {
        cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

        if (lpIsiDev != NULL)
        {
            lRet = cgd_IsiSubmitEmptyBuffer(aIsiIdx, apEmptyBuffer);
        }
    }

    return lRet;
}

/******************************************************************************/
cgd_Result cgd_GetFullBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer)
{
    cgd_Result lRet = CGD_LIB_FAILURE;
    if (aIsiIdx < CGD_ISI_CNT)
    {
        cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

        if (lpIsiDev != NULL)
        {
            lRet = cgd_IsiGetFullBuffer(aIsiIdx, apOutBuffer);
        }
    }

    return lRet;
}

/******************************************************************************/
cgd_Result cgd_GetFullBufferSubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer, utils_ImgBuf_t* apEmptyBuffer)
{
    cgd_Result lRet = CGD_LIB_FAILURE;
    if (aIsiIdx < CGD_ISI_CNT)
    {
        cgd_IsiDevice_t* lpIsiDev = cgd_GetIsiDevice(aIsiIdx);

        if (lpIsiDev != NULL)
        {
            lRet = cgd_IsiGetFullBufferSubmitEmptyBuffer(aIsiIdx, apOutBuffer, apEmptyBuffer);
        }
    }

    return lRet;
}

/******************************************************************************/
cgd_Result cgd_RegisterFrameDoneEvtHandler(cgd_IsiEventHandler_t apEventHandler)
{
    cgd_Result lRet = CGD_LIB_FAILURE;

    if (apEventHandler != NULL)
    {
        gpCgdFrameDoneEvtHandler = apEventHandler;
        lRet                     = CGD_LIB_SUCCESS;
    }
    else
    {
        gpCgdFrameDoneEvtHandler = IsiDefIrqHandle;
    }

    return lRet;
}

/******************************************************************************/
cgd_Result cgd_RegisterFirstLineStoredEvtHandler(cgd_IsiEventHandler_t apEventHandler)
{
    cgd_Result lRet = CGD_LIB_FAILURE;

    if (apEventHandler != NULL)
    {
        gpCgdFirstLineEvtHandler = apEventHandler;
        lRet                     = CGD_LIB_SUCCESS;
    }
    else
    {
        gpCgdFirstLineEvtHandler = IsiDefIrqHandle;
    }

    return lRet;
}

/******************************************************************************/
cgd_Result cgd_RegisterErrorEvtHandler(cgd_ErrorEventHandler_t apEventHandler)
{
    cgd_Result lRet = CGD_LIB_FAILURE;

    if (apEventHandler != NULL)
    {
        gpCgdErrorEvtHandler = apEventHandler;
        lRet                 = CGD_LIB_SUCCESS;
    }
    else
    {
        gpCgdErrorEvtHandler = ErrDefIrqHandle;
    }

    return lRet;
}

/******************************************************************************/
