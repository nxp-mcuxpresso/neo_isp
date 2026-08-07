/*******************************************************************************
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCF_ALGORITHM_H
#define CCF_ALGORITHM_H

/**
 * @file    algorithm.h
 * @brief   generic algorithm descriptor and its operations
 */

#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif
/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define CCF_ALGO_STATE_DISABLED 0x0000U   /**< Update neither control params nor ISP regs/sensor */
#define CCF_ALGO_STATE_ENABLED  0x0001U   /**< Normal operation - update control params & ISP regs/sensor */
#define CCF_ALGO_STATE_IGNORE   0x0002U   /**< Update control params but NOT ISP regs/sensor */
#define CCF_ALGO_STATE_ERROR    0xFFFFU   /**< Algorithm error state */

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/
/******************************************************************************/
/**
 * @brief   CCF generic algorithm operations
 ******************************************************************************/
typedef struct {
    int         (*Open) (void *     apCtxHwIn,
                         void *     apCtxHwOut,
                         uint32_t   aChnIdx,
                         uint32_t   aSensID);

    int         (*Process) (void *      apCtxHwIn,
                            void *      apCtxHwOut,
                            void *      apSensData,
                            void *      apCtrlParams,
                            uint32_t    aChnIdx);

    int         (*UpdateContext) (uint32_t aChnIdx,
                                  void* apData);

    int         (*InitToDefault) (uint32_t aChnIdx);

    void*       (*GetConfig)  (uint32_t aChnIdx);

    uint32_t    (*GetContextSize)(void);
    uint32_t    (*GetConfigSize) (void);

    uint32_t  (*GetState) (const uint32_t aChnIdx);
    void      (*SetState) (const uint32_t aState,
                           const uint32_t aChnIdx);
} ccf_AlgorithmOps_t;

/******************************************************************************/
/**
 * @brief   CCF generic algorithm
 *******************************************************************************/
typedef struct {
    const char*         mName;
    uint32_t            mId;
    ccf_AlgorithmOps_t  mOps;
} ccf_Algorithm_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCF_ALGORITHM_H */
