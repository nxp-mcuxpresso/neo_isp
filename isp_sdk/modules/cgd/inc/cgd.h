/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CGD_H
#define CGD_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  cgd.h
 * @brief Camera Generic Driver accessibility from user space
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <stddef.h>
#include <stdint.h>
#include <utils_i2c.h>
#include "utils_container.h"
#include "cgd_types.h"
#include "cgd_i2c.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
/*==============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/**
 * @brief definition of event handler data type
 */
typedef void (*cgd_IsiEventHandler_t)(cgd_IsiChannel_t);

/**
 * @brief definition of event handler data type
 */
typedef void (*cgd_ErrorEventHandler_t)(cgd_RegisterErrorTable_t*);

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief          Initialize the camera subsystem from hardware config.
 *
 * @param[in]      arcHardwareConfig Reference to cgd system config.
 *
 * @return         cgd_Result
 *
 * @note
 ****************************************************************************/
cgd_Result cgd_Open(cgd_System_t* arcHardwareConfig);

/******************************************************************************/
/**
 * @brief          Change configuration of ISP system.
 *
 * @param[in]      arcIspCfg Reference to config
 * @param[in]      aIsiIdx  ISI index
 *
 * @return         cgd_Result
 *
 * @note
 ****************************************************************************/
cgd_Result cgd_Config(cgd_SystemExtCfg_t* arcIspCfg, cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief          Execute defined arCmdTable to I2C bus
 *
 * @param[in]      arCmdTable Reference to the table command
 *
 * @return         cgd_Result
 *
 * @note
 ****************************************************************************/
cgd_Result cgd_ExecTable(cgd_CmdTable_t* arCmdTable);

/******************************************************************************/
/**
 * @brief          Disable and disconnect current camera subsystem.
 *
 * @param[in]      void
 *
 * @return         cgd_Result
 *
 * @note
 *****************************************************************************/
cgd_Result cgd_Close(void);

/******************************************************************************/
/**
 * @brief Start to stream of port index, ISI channel buffers must be assigned before start streaming
 *
 * @param  aIsiIdx  ISI index
 *
 * @return CGD_LIB_SUCCESS If all ok
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
cgd_Result cgd_StreamOn(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief Stop to stream of port index
 *
 * @param  aIsiIdx  ISI index
 *
 * @return CGD_LIB_SUCCESS If all ok
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
cgd_Result cgd_StreamOff(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief Return CSI device
 *
 * @param  aCSIIdx  device IDX
 *
 * @return cgd_PortHead_t structure, NULL in case of an error
 *
 ****************************************************************************/
cgd_CsiPort_t* cgd_GetCsiPort(cgd_CsiDeviceId_t aCSIIdx);

/******************************************************************************/
/**
 * @brief Return CSI device config
 *
 * @param  aCSIIdx  device IDX
 *
 * @return cgd_CsiExtCfg_t structure, NULL in case of an error
 *
 ****************************************************************************/
cgd_CsiExtCfg_t* cgd_GetCsiPortConfig(cgd_CsiDeviceId_t aCSIIdx);

/******************************************************************************/
/**
 * @brief Return ISI device
 *
 * @param  aIsiIdx  ISI index
 *
 * @return cgd_IsiDevice_t* structure, NULL in case of an error
 *
 ****************************************************************************/
cgd_IsiDevice_t* cgd_GetIsiDevice(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief Return ISI device config
 *
 * @param  aIsiIdx  ISI index
 *
 * @return cgd_IsiExtCfg_t* structure, NULL in case of an error
 *
 ****************************************************************************/
cgd_IsiExtCfg_t* cgd_GetIsiDeviceConfig(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief Return current ISI device state, isi_MtxLock used
 *
 * @param  aIsiIdx  ISI index
 *
 * @return cgd_State_t enum
 *
 ****************************************************************************/
cgd_State_t cgd_GetIsiState(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief Check if empty buffer is available for use.
 *
 * @param  aIsiIdx  ISI index
 *
 * @return false - no empty buffer available, true - empty buffer is available
 *
 ****************************************************************************/
bool cgd_IsEmptyBufferPrepared(cgd_IsiChannel_t aIsiIdx);

/******************************************************************************/
/**
 * @brief Submit new free buffer to ISI device queue, should be used before cgd_GetFullBuffer
 *
 * @param  aIsiIdx  ISI index
 * @param  apEmptyBuffer  free buffer
 *
 * @return cgd_State_t enum
 *
 ****************************************************************************/
cgd_Result cgd_SubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apEmptyBuffer);

/******************************************************************************/
/**
 * @brief Return full frame buffer. NULL in case of error, also NULL when empty buffer queue is empty. Is recommended to use cgd_SubmitEmptyBuffer first.
 *
 * @param  aIsiIdx  ISI index
 *
 * @param  apOutBuffer  (out) full buffer output
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_GetFullBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer);

/******************************************************************************/
/**
 * @brief Get full frame buffer and submit empty buffer in single operation.
 *
 * @param  aIsiIdx  ISI index
 *
 * @param  apOutBuffer  (out) full buffer output
 * @param  apEmptyBuffer (in)  free buffer to be used
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_GetFullBufferSubmitEmptyBuffer(cgd_IsiChannel_t aIsiIdx, utils_ImgBuf_t* apOutBuffer, utils_ImgBuf_t* apEmptyBuffer);

/******************************************************************************/
/**
 * @brief Register frame done event handler, mandatory before start streaming.
 *
 * @param  apEventHandler event handler
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_RegisterFrameDoneEvtHandler(cgd_IsiEventHandler_t apEventHandler);

/******************************************************************************/
/**
 * @brief Register frame done event handler, optional before start streaming.
 *
 * @param  apEventHandler event handler
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_RegisterFirstLineStoredEvtHandler(cgd_IsiEventHandler_t apEventHandler);

/******************************************************************************/
/**
 * @brief Register frame done event handler, optional before start streaming.
 *
 * @param  apEventHandler event handler
 *
 * @return CGD_LIB_SUCCESS If all ok, otherwise CGD_LIB_FAILURE
 *
 ****************************************************************************/
cgd_Result cgd_RegisterErrorEvtHandler(cgd_ErrorEventHandler_t apEventHandler);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CGD_H */
