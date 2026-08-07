/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCA_NEO_ISP_CONTROL_H
#define CCA_NEO_ISP_CONTROL_H

/******************************************************************************/
/**
 * @file cca_neo_isp_control.h
 * @brief Camera abstraction library integration of control functions
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "cca.h"
#include "cca_internal.h"
#include "cca_ovx3c.h"
#include "utils_load_bin.h"


#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define SEN_OVX3C_ID        (2010U)
/*============================ ==================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/
/******************************************************************************/
/**
 * @brief   Defines correspondance between sensor ID known at application level
 * and sensor IDs used by tunning parameters database.
 *******************************************************************************/
typedef struct
{
    uint32_t mSensorId;     /**< sensor ID */
    // uint32_t mDtpSensorId;  /**< tunning parameters sensor ID */ // TODO enable when used or remove if not used at all
} cca_NeocontrolSensorIdPair_t;

/******************************************************************************/
/**
 * @brief   Defines mapping between sensor IDs to sensor DTP IDs.
 *******************************************************************************/
typedef struct
{
    cca_NeocontrolSensorIdPair_t *mpMap;      /**< array of Sensor ID pairs */
    uint32_t                mLength;    /**< number of pairs in the Map */
} cca_NeocontrolSensorIdMap_t;

/******************************************************************************/
/**
 * @brief   Control function specific parameters to be passed to ccf at init.
 *******************************************************************************/
typedef struct
{
    void*                   mpCcfAlgoConfig;                        /**< CCF algorithm configurations */
    cca_NeocontrolSensorIdMap_t   mSensorIdMap;                           /**< sensor ID map */ // TODO not used on CCF side now
    uint32_t                mpIspActive[ISPDRV_CAM_PIPE_ID_CNT];    /**< set 1 if ISP settings to be generated, 0 if not */
    uint8_t                 mTuningToolCon;                         /**< if != 0, tuning tool connection shell be established */
    const char*             mpcTuningServerIp;                      /**< IPv4 address of the tuning server */
    uint16_t                mTuningServerPort;                      /**< port the tuning server shall listen on */
} cca_NeocontrolParams_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Gets public API functions of the control functions integration
 *
 * @param[in]  acpOps  control functions operations container
 *******************************************************************************/
void cca_ccf_GetOps(cca_CcfOps_t* const acpOps);

/******************************************************************************/
/**
 * @brief Applies specific configuration settings
 *
 * @param[in]  aChnlIdx  index of the channel to configure
 * @param[in]  aCmdId  command identifier, use enums
 * @param[in]  aVal  value to set
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_ccf_Config(uint32_t aChnlIdx, uint32_t aCmdId, int32_t aVal);

#ifdef __cplusplus
}
#endif

/** @} */ // addtogroup cca_ug_int
#endif /* #ifndef CCA_NEO_ISP_CONTROL_H */
