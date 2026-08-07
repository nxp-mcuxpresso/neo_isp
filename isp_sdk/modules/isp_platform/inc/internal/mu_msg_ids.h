/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  mu_msg_ids.h
 * @brief Message IDs for isp_drv and fw communication
 *******************************************************************************/

#ifndef MU_MSG_IDS_H
#define MU_MSG_IDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Messages from ISP_DRV to FW.
 */
/** @brief Request internal restart of firmware
 * @details Response: FW_MSG_READY_AFTER_BOOT_NTF */
#define FW_MSG_REBOOT_REQ (0x0001U)
/** @brief Request the average idle percentage of the ISP (utilization)
 * @details Response: comes with the same ID */
#define FW_MSG_GET_IDLE_PERCENTAGE_REQ (0x0002U)
/** @brief Request firmware version
 * @details Response: comes with the same ID */
#define FW_MSG_GET_M0_SW_VERSION_REQ (0x0003U)
/** @brief Request soft- or hard-resetting of the ISP pipes
 * @details Response: comes with the same ID */
#define FW_MSG_RESET_STREAM_REQ (0x0004U)
/** @brief Request stream mode entry
 * @details Response: comes with the same ID */
#define FW_MSG_START_STREAM_MODE_REQ (0x0005U)
/** @brief Request stream mode exit (M2M mode entry)
 * @details Response: comes with the same ID */
#define FW_MSG_STOP_STREAM_MODE_REQ (0x0006U)
/** @brief Request processing of a task (schedule)
 * @details Response: FW_MSG_FRAME_DONE_NTF */
#define FW_MSG_SCHEDULE_TASK_REQ (0x000BU)

/**
 * @brief Messages from FW to ISP_DRV.
 */
/** @brief Notification - Firmware hit an error (different for user and control channel) */
#define FW_MSG_ERROR_NTF (0x0009U)
/** @brief Notification/Response -> Firmware is ready for operation */
#define FW_MSG_READY_AFTER_BOOT_NTF (0x000AU)
/** @brief Response to FW_MSG_SCHEDULE_TASK_REQ, informing task is done.
 * @details Error needs to be checked, as image data might be invalid */
#define FW_MSG_FRAME_DONE_NTF (0x000CU)

#ifdef __cplusplus
}
#endif

#endif // MU_MSG_IDS_H
