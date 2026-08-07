/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_TYPES_H
#define CGD_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * @file           cgd_types.h
 * @brief          Definitions of the Camera Generic Driver (CGD) public types.
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <stddef.h>
#include <pthread.h>
#include <stdint.h>
#include <utils_i2c.h>
#include <stdbool.h>
#include "utils_container.h"

#ifdef SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"
#endif

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define CGD_MAX_DEV_CNT 16U /** @brief Maximum number of I2C devices per port */
#define CGD_CSI_VC_CNT 8U /** @brief Number of CSI virtual channels */
#define GPIO_MAX_NAME_SIZE 32 /** @brief Maximum length of a GPIO controller name */
#define CGD_ISI_CNT 8U /** @brief Number of ISI channels */

/*==============================================================================
*                                    ENUMS
==============================================================================*/

/******************************************************************************
 * @brief          CGD system return values.
 *******************************************************************************/
typedef enum
{
    CGD_LIB_SUCCESS = 0, /** @brief Operation succeeded */
    CGD_LIB_FAILURE = 1 /** @brief Operation failed */
} cgd_Result;

/******************************************************************************
 * @brief          CSI device identification.
 *******************************************************************************/
typedef enum
{
    CGD_MIPI_CSI_DEV, /** @brief CAMERA.MIPI_CSI2_0, base address: 4AD3_0000h */
    CGD_MIPI_CSI_COMBO_DEV, /** @brief CAMERA.DSI_CSI_COMBO_COMPLEX_CSI1.CSI, base address: 4AD4_0000h */
    CGD_MIPI_CSI_CNT /** @brief Number of CSI devices */
} cgd_CsiDeviceId_t;

/******************************************************************************
 * @brief          Camera system states.
 *******************************************************************************/
typedef enum
{
    CGD_STATE_OFF      = 0, /** @brief Camera system uninitialized */
    CGD_STATE_INIT     = 1, /** @brief Camera system initialized */
    CGD_STATE_STRM     = 2, /** @brief Camera system streaming data */
    CGD_STATE_STRM_OFF = 3, /** @brief Camera system streaming stopped */
} cgd_State_t;

/******************************************************************************
 * @brief   Supported I2C command types used to build command tables.
 *
 * @details Each command table is an array of cgd_Cmd_t rows. Every row has the
 *          form { DEVIDX, CMD_ID, {mpData[0..3]} }, where DEVIDX selects the
 *          physical device address via cgd_CsiExtCfg_t::mpI2cAdr[DEVIDX].
 *
 *          A single row carries at most 4 payload bytes. When a command needs
 *          more bytes than fit in mpData, the extra bytes are supplied by one
 *          or more following CGD_CMD_CON rows (4 bytes each), appended in order.
 *
 *          Common fields used in the payload layouts below:
 *          - XFR_SIZE: number of data/register bytes handled by the command.
 *          - ADDR_H, ADDR_L: register address in the device. A WRITE command
 *            that sets the address must precede READ and MASK commands.
 *          - MSEC: time in milliseconds.
 *******************************************************************************/
typedef enum
{
    CGD_CMD_DELAY, /** @brief Sleep for a given time. Layout: {MSEC}. */
    CGD_CMD_WRITE, /** @brief Write bytes to the device (also used to set the register address). Layout: {XFR_SIZE, ADDR_H, ADDR_L, DATA...}. */
    CGD_CMD_MASK, /** @brief Read-modify-write: reg = (reg & ~MASK) | (DATA & MASK), per byte. CGD_CMD_CON is mandatory. Layout: {XFR_SIZE, DATA, MASK, (DATA, MASK, ...)}. */
    CGD_CMD_WAIT, /** @brief Poll a register until masked DATA matches, up to RCNT tries with DL ms between them. CGD_CMD_CON is mandatory. Layout: {XFR_SIZE, RCNT, DL, DATA, MASK, ...}. */
    CGD_CMD_READ, /** @brief Read XFR_SIZE bytes from the device into the table save buffer. Layout: {XFR_SIZE}. */
    CGD_CMD_CON, /** @brief Continuation row carrying 4 extra payload bytes for the previous command. Layout: {DATA, DATA, DATA, DATA}. */
    CGD_CMD_DBG, /** @brief Print a 4-character debug marker to the console. Layout: {CHAR, CHAR, CHAR, CHAR}. */
    CGD_CMD_CNT /** @brief Number of available command types. */
} cgd_CmdIds_t;

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

typedef uint8_t cgd_IsiChannel_t; /** @brief ISI channel identifier */

/******************************************************************************
 * @brief          Single register error definition.
 *
 * @details        Describes one error bit inside a device register, including
 *                 its translation and whether it is fatal.
 *******************************************************************************/
typedef struct
{
    uint8_t mRegOrder; /** @brief Register order in the array (used for CSI) */
    uint32_t mOffset; /** @brief Offset in the register struct, computed with offsetof() */
    uint32_t mBitMask; /** @brief Mask selecting the error bit(s) in the register value */
    bool mErrorValueHi; /** @brief Which level signals an error: true means 1, false means 0 */
    char mErrorMsg[100]; /** @brief Error message taken from the documentation */
    bool mThrowError; /** @brief If true, report the error to the app via gpCgdErrorEvtHandler */
    bool mTrowed; /** @brief Already reported to gpCgdErrorEvtHandler, do not report again */
} cgd_RegisterErrorTable_t;

/******************************************************************************
 * @brief          Register error table for a device.
 *
 * @details        Groups all error definitions (cgd_RegisterErrorTable_t)
 *                 checked for a device.
 *******************************************************************************/
typedef struct
{
    cgd_RegisterErrorTable_t* mpErrorTable; /** @brief Table of all defined error bits */
    uint32_t mErrorTableSz; /** @brief Number of entries in the table */
} cgd_RegisterError_t;

/******************************************************************************
 * @brief          GPIO pin configuration for a single port.
 *******************************************************************************/
typedef struct
{
    char mLabel[GPIO_MAX_NAME_SIZE]; /** @brief GPIO controller name */
    int32_t mPinLine; /** @brief Line index within the GPIO controller */
    bool mActiveLow; /** @brief True if the GPIO is active-low */
} cgd_PinCfg_t;

/******************************************************************************
 * @brief   Single I2C command-table row.
 *
 * @details Payload bytes in mpData are interpreted according to mCmdId; see
 *          cgd_CmdIds_t for the per-command layout.
 *******************************************************************************/
typedef struct
{
    uint8_t mDevId; /** @brief Device index into cgd_CsiExtCfg_t::mpI2cAdr[] selecting the target I2C address */
    cgd_CmdIds_t mCmdId; /** @brief Command type, see cgd_CmdIds_t */
    uint8_t mpData[4]; /** @brief Up to 4 payload bytes, layout depends on mCmdId (extended by CGD_CMD_CON rows) */
} cgd_Cmd_t;

/*****************************************************************************
 * @brief   Command table passed to the execute-table API.
 *
 * @details Holds an array of cgd_Cmd_t rows executed in order, plus an
 *          optional buffer that collects the bytes returned by CGD_CMD_READ.
 *******************************************************************************/
typedef struct cgd_CmdTable
{
    cgd_CsiDeviceId_t mPortIdx; /** @brief CSI port index on which the table is executed */
    cgd_Cmd_t* ympI2cTable; /** @brief Array of command rows */
    uint32_t mI2cTableSz; /** @brief Table size in bytes (row count = mI2cTableSz / sizeof(cgd_Cmd_t)) */
    uint8_t* mpSaveBuff; /** @brief Optional buffer receiving data from CGD_CMD_READ, may be NULL */
    uint32_t mSaveBuffSz; /** @brief Size of mpSaveBuff in bytes */
    bool mIsDebug; /** @brief When true, every executed command is printed to the console */
} cgd_CmdTable_t;

/******************************************************************************
 * @brief          Common properties shared by all port types.
 *******************************************************************************/
typedef struct
{
    // I2C device configuration
    i2c_Dev_t mI2cPortDev; /** @brief I2C device currently in use */
#ifdef LINUX_EMB
    pthread_mutex_t mI2CMtx; /** @brief I2C bus mutex */
    // GPIO configuration
    cgd_PinCfg_t mGpioPinCfg; /** @brief GPIO configuration */
#endif
#ifdef SDK_OS_FREE_RTOS
    SemaphoreHandle_t mI2CMtx; /** @brief Mutex protecting the I2C bus during a transfer */
    StaticSemaphore_t mI2CMtxData; /** @brief Storage backing the static I2C mutex */
#endif
#ifdef GHS
    LocalMutex mI2CMtx; /** @brief I2C bus mutex */
#endif
} cgd_PortHead_t;

/******************************************************************************
 * @brief          Fixed CSI port configuration.
 *
 * @details        These parameters must be predefined and cannot be changed.
 *******************************************************************************/
typedef struct
{
    cgd_State_t mPortState; /** @brief State of the CSI port */
    cgd_PortHead_t* mcHead; /** @brief Data common to all port types */

#ifdef LINUX_EMB
    cgd_RegisterError_t* mpErrorTable; /** @brief Register error table for this port */
#endif
} cgd_CsiPort_t;

/******************************************************************************
 * @brief          Fixed ISI device configuration.
 *
 * @details        These parameters must be predefined and cannot be changed.
 *******************************************************************************/
typedef struct
{
    cgd_IsiChannel_t mIsiId; /** @brief ISI channel index */
    cgd_CsiDeviceId_t mCsiId; /** @brief CSI device this ISI channel is assigned to */
    cgd_State_t mIsiState; /** @brief State of the ISI channel */
#if defined(LINUX_EMB) || defined(GHS)
    cgd_RegisterError_t* mpErrorTable; /** @brief Register error table for this channel */
#endif
} cgd_IsiDevice_t;

/******************************************************************************
 * @brief          Fixed camera hardware configuration.
 *
 * @details        These values are part of the fixed camera configuration.
 *
 * @note           Uses arrays of pointers so instances can be swapped easily.
 *******************************************************************************/
typedef struct
{
    cgd_State_t sgState; /** @brief System state, uninitialized by default */
    cgd_CsiPort_t* mppCsiPort[CGD_MIPI_CSI_CNT]; /** @brief CSI ports, if any */
    cgd_IsiDevice_t* mppIsiDevice[CGD_ISI_CNT]; /** @brief ISI devices, if any */
} cgd_System_t;

/******************************************************************************
 * @brief          Externally configurable parameters of the CSI port.
 *
 * @details        These values come from outside the camera specification,
 *                 for example from the ISP graph, SDI or the user application.
 *******************************************************************************/
typedef struct
{
    uint8_t mLanes; /** @brief Number of lanes used on the CSI device (N_LANES) */
    uint32_t mDataType[CGD_CSI_VC_CNT]; /** @brief Pixel data type used for each pixel link */
    uint32_t mNPDataType[CGD_CSI_VC_CNT]; /** @brief Non-pixel data type used for each pixel link */
    uint16_t mMbps; /** @brief Desired DPHY bit rate in Mbps */
    bool mRerouteEnable[CGD_CSI_VC_CNT]; /** @brief Enable VC routing (CSI*_Pixel_data_ctrl_VC*[Reroute_VC]); requires both mPixelDataRouteVC and mNonPixelDataRouteVC */
    cgd_IsiChannel_t mPixelDataRouteVC[CGD_CSI_VC_CNT]; /** @brief New VC for pixel data (enabled by mRerouteEnable) */
    cgd_IsiChannel_t mNonPixelDataRouteVC[CGD_CSI_VC_CNT]; /** @brief New VC for non-pixel data (enabled by mRerouteEnable) */

    // I2C address table for this CSI port
    uint8_t mI2cAdrCnt; /** @brief Number of I2C devices on this port */
    const uint8_t* mpI2cAdr; /** @brief Array of I2C device addresses, indexed by mDevId in command tables */

    // I2C command tables
    cgd_CmdTable_t* mpcInitArr; /** @brief Command table used by cgd_Config() for the initial configuration */
    cgd_CmdTable_t* mpcCloseArr; /** @brief Command table used by cgd_Close() to close the system */
    cgd_CmdTable_t* mpcStreamEn; /** @brief Command table used by cgd_StreamOn() to enable streaming */
    cgd_CmdTable_t* mpcStreamDis; /** @brief Command table used by cgd_StreamOff() to disable streaming */
} cgd_CsiExtCfg_t;

/******************************************************************************
 * @brief          Externally configurable parameters of the ISI channel.
 *
 * @details        These values come from outside the camera specification,
 *                 for example from the ISP graph, SDI or the user application.
 *******************************************************************************/
typedef struct
{
    uint8_t mVC; /** @brief Virtual channel enabled on the ISI: CHNL_CTRL[VC_ID_0] and CHNL_CTRL[VC_ID_1] */
    uint8_t mHwId; /** @brief Hardware ID for the CCL system */
    uint32_t mPPErrIrqMask; /** @brief Channel interrupt enable mask, CHNL_IER */
    uint32_t mImageWidth; /** @brief ISI channel active image width, used in CHNL_IMG_CFG and CHNL_IMG_CFG2 */
    uint32_t mImageHeight; /** @brief ISI channel active image height without embedded lines, used in CHNL_IMG_CFG */
    bool mVerticalFlip; /** @brief Enable vertical image flipping, CHNL_IMG_CTRL[VFLIP_EN] */
    bool mHorizontalFlip; /** @brief Enable horizontal image flipping, CHNL_IMG_CTRL[HFLIP_EN] */
    uint32_t mEmbdLinesBefore; /** @brief Number of embedded-data lines at the start of the captured image */
    uint32_t mEmbdLinesAfter; /** @brief Number of embedded-data lines at the end of the captured image */
    uint32_t mBPP; /** @brief Bytes per pixel for the image format */
} cgd_IsiExtCfg_t;

/******************************************************************************
 * @brief          Externally configurable parameters of the camera system.
 *
 * @details        These values come from outside the camera specification,
 *                 for example from the ISP graph, SDI or the user application.
 *                 The configuration must match the camera system definition.
 *******************************************************************************/
typedef struct
{
    cgd_CsiExtCfg_t* mpCsi[CGD_MIPI_CSI_CNT]; /** @brief External configuration for each CSI port */
    cgd_IsiExtCfg_t* mpIsi[CGD_ISI_CNT]; /** @brief External configuration for each ISI channel */
} cgd_SystemExtCfg_t;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_TYPES_H */
