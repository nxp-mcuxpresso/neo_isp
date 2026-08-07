/*******************************************************************************
 * Copyright 2023-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ISPDRV_USR_H
#define ISPDRV_USR_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  ispdrv_usr.h
 * @brief Isp driver -> User API (SW)
 * @details Main (and only) header file that should be included by client code. Contains
 *      all definitions needed to translate sensor data into image.
 *******************************************************************************/

#include <stdint.h>

#include "isp_platform.h"
#include "ispdrv_events.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/**
 * @brief A prototype for user callback. It will be called each time an event happens.
 *
 * @param aHwId - HW block the event happened on
 * @param aCamId - Pipe within the HW block the event happened on
 * @param aEventId - Identifier of the event that happened
 * @param apParam - Extended information about the event. Each event have its own format
 * @param apClientPtr - Client pointer passed during a call to ispdrv_Open()
 *
 */
typedef void (*ispdrv_EventCback_t)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_EventId_t aEventId, ispdrv_EventParam_t* apParam, void* apClientPtr);

typedef struct
{
    /** Client event callback function - all events for client will arrive there
     * see ispdrv_EventId_t and ispdrv_EventParam_t for tedails about events */
    ispdrv_EventCback_t mEventCback;

    /** application defined parameter that will be passed as apClientPtr in all mEventCback events*/
    void* mpClientPtr;

} ispdrv_OpenCfg_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Open a pipe so tasks can be send to it.
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in]    aCamId     - Id of a pipe within the HW block
 * @param[in]    acpConfig  - parameters to use while opening the pipe
 *
 * @return  ISPDRV_RES_SUCCESS if pipe was opened, numbered error otherwise.
 *
 * @details Checks if pipe is available (optionally communicates with HW control driver).
 *          Initializes any needed HW specific to this pipe.
 *******************************************************************************/
ispdrv_Result_t ispdrv_Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_OpenCfg_t* const acpConfig);

/******************************************************************************/
/**
 * @brief Closes a pipe
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in]    aCamId     - Id of a pipe within the HW block
 *
 * @return  ISPDRV_RES_SUCCESS if all OK, numbered error otherwise.
 *******************************************************************************/
ispdrv_Result_t ispdrv_Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/******************************************************************************/
/**
 * @brief Closes a pipe, by performing local and server cleanup, regardless of the current state.
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in]    aCamId     - Id of a pipe within the HW block
 *
 * @details Note that by calling this fcn, you may close pipe owned by another process.
 *
 * @return  ISPDRV_RES_SUCCESS if all OK, numbered error otherwise.
 *******************************************************************************/
ispdrv_Result_t ispdrv_CloseForced(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/******************************************************************************/
/**
 * @brief Schedules a task (frame processing)
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in]    aCamId     - Id of a pipe within the HW block
 * @param[in]    aTaskAddr  - Shared address of the task that should be done
 *
 * @return  ISPDRV_RES_SUCCESS if task was successfully scheduled, numbered error otherwise.
 *
 * @details As this is not a blocking call, it returns immediately once firmware accepts it to queue.
 *          Completion of the task is signalled via client callback (passed to ispdrv_Open).
 *          Any potential error during processing is also delivered cia client callback.
 *******************************************************************************/
ispdrv_Result_t ispdrv_TaskSchedule(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint64_t aTaskAddr);

/******************************************************************************/
/**
 * @brief Schedules a task (frame processing) and waits for its completion
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in]    aCamId     - Id of a pipe within the HW block
 * @param[in]    acpTask    - Shared address of the task that should be done
 *
 * @return  ISPDRV_RES_SUCCESS if task was successfully scheduled, numbered error otherwise.
 *
 *******************************************************************************/
ispdrv_Result_t ispdrv_TaskScheduleBlocking(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, isp_Task_t* const acpTask);

/******************************************************************************/
/**
 * @brief Sends request to control entity to change mode.
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in]    aCamId     - Id of a pipe within the HW block (see details below)
 * @param[in]    aNewMode    - New mode to be entered
 *
 * @return  ISPDRV_RES_SUCCESS if new mode was successfully entered, numbered error otherwise.
 *
 * @details aStreamingCamId is control id for streaming mode. This will be the only pipe
 * accepting TaskSchedule() calls. For M2M mode this parameter serves only as
 * permission identifier - should control entity implement permissions.
 *******************************************************************************/
ispdrv_Result_t ispdrv_ChangeMode(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ispdrv_Mode_t aNewMode);

/**
 * @brief Fills additional metadata for finished ISP task. (read from fw's OCRAM)
 *
 * @param[in]    aHwId      - Id of the Hw block
 * @param[in,out] apIspCfg
 * @param[in] aTableIdx
 *
 * @return  ISPDRV_RES_SUCCESS if update was successful. error on failure. For error
 * the contents of apIspCfg are not modified.
 *
 */
ispdrv_Result_t ispdrv_LoadFwMeta(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, isp_Task_t* apIspCfg, uint8_t aTableIdx);

/**
 * @brief Updates ISI to work with streaming mode changes (output format for ISP is different so ISI needs to match)
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @return 0 on success, -1 otherwise.
 */
ispdrv_Result_t ispdrv_isifix(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, bool aStreamActive);

/**
 * @brief Reads utilization values from fw.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apIspIdle - variable that receives ISP idle value (NULL acceptable)
 * @param apCpuIdle - variable that receives M0 CPU core idle value (NULL acceptable)
 *
 * @return  ISPDRV_RES_SUCCESS on successful read, numbered error otherwise.
 *
 * @details The returned values contains two decimal places; e.g. 5524 represents 55.24%, 8 represents 0.08%.
  * Fw updates its utilization values with period of 1 sec. The update event is not synchronized.
 */
ispdrv_Result_t ispdrv_GetFwIdleStats(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint32_t *apIspIdle, uint32_t *apCpuIdle);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ISPDRV_USR_H */
