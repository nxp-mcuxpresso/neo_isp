/*******************************************************************************
 * Copyright 2017-2018, 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef ILOG_H
#define ILOG_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/** @file           ilog.h
 *  @brief          Vision logging public API declaration.
 ******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/
/** \todo should be controlled/defined outside */
#define ILOG_LOG_EN /**< @brief define to enable ILOG logging */
#define ILOG_ASSERT_EN /**< @brief define to enable ILOG asserting */
#define ILOG_SIMPLE_EN  /**< @brief define to enable simple ILOG logging */

static const int32_t ilog_scMaxLogBytes = 512U; /**< max allowed log message length in bytes */

/*==============================================================================
*                                   TYPEDEFS
==============================================================================*/
typedef char ilog_char_t;

/*==============================================================================
*                                    MACROS
==============================================================================*/

/******************************************************************************/
/** @brief      General logging macro to be used in all levels of implementation
 *  @param[in]  _mod: module identifier (see below)
 *  @param[in]  _lev: specifies level of logging (severity, see below)
 *  @param[in]  ...:  varargs like used with printf() functions
 ******************************************************************************/
#ifdef ILOG_LOG_EN
#define ILOG(_mod, _lev, ...) \
    ilog_Add(_mod, _lev, (const ilog_char_t*)__FILE__, __LINE__, (const ilog_char_t*)__func__, __VA_ARGS__);
#else
#define ILOG(_mod, _lev, ...)
#endif // ILOG_LOGGING

/******************************************************************************/
/** @brief      General assert macro, will log incident and aborts application
 *  @param[in]  _cond: condition required to be meet
 ******************************************************************************/
#ifdef ILOG_ASSERT_EN
#define ILOG_ASSERT(_cond)                                                                         \
    if (!(_cond))                                                                                  \
    {                                                                                              \
        ilog_Assert(#_cond, (const ilog_char_t*)__FILE__, __LINE__, (const ilog_char_t*)__func__); \
    }
#else
#define ILOG_ASSERT(_cond) (void)(_cond)
#endif // ILOG_ASSERTING

/******************************************************************************/
/** @brief      Simple logging macro w/o info about time, file, line and function
 *  @param[in]  _mod: module identifier (see below)
 *  @param[in]  _lev: specifies level of logging (severity, see below)
 *  @param[in]  ...:  varargs like used with printf() functions
 ******************************************************************************/
#ifdef ILOG_SIMPLE_EN
#define ILOG_SIMPLE(_mod, _lev, ...) \
    ilog_Simple(_mod, _lev, __VA_ARGS__);
#else
#define ILOG_SIMPLE(_mod, _lev, ...)
#endif // ILOG_SIMPLE_EN

/*==============================================================================
*                                    ENUMS
==============================================================================*/
/******************************************************************************/
/** @brief          A brief text description (one line).
 *  @details        Optionally a detailed text description of the code object.
 ******************************************************************************/
typedef enum
{
    ILOG_MOD_GEN        = 0x00000001, /*!<  unspecified generic module*/
    ILOG_MOD_ISP        = 0x00000002, /*!<  ISP module */
    ILOG_MOD_CCL        = 0x00000003, /*!<  CCL (camlib) module */
    ILOG_MOD_CCA        = 0x00000004, /*!<  CCA module */
    ILOG_MOD_ISP_SIM    = 0x00000005, /*!<  ISP FW simulator layer */
    ILOG_MOD_ISP_HW_SIM = 0x00000006, /*!<  ISP HW simulator layer */
    ILOG_MOD_ISI        = 0x00000007, /*!<  ISI layer */
    ILOG_MOD_UTILS      = 0x00000008, /*!<  utils */
    ILOG_MOD_CSI        = 0x00000009, /*!<  CSI layer */
    ILOG_MOD_CGD        = 0x0000000A, /*!<  Camera generic driver */
    ILOG_MOD_CCF        = 0x0000000B, /*!<  CCF module */
    ILOG_MOD_CCF_DRC    = 0x0000000C, /*!<  DRC part of CCF module */
    ILOG_MOD_CCF_WB     = 0x0000000D, /*!<  WB part of CCF module */
    ILOG_MOD_CCF_WBB    = 0x0000000E, /*!<  WBB part of CCF module */
    ILOG_MOD_CCF_WBR    = 0x0000000F, /*!<  WBR part of CCF module */
    ILOG_MOD_CCF_AEC    = 0x00000010, /*!<  AEC part of CCF module */
    ILOG_MOD_CCF_CCTX   = 0x00000011, /*!<  CAMCTX part of CCF module */
    ILOG_MOD_LT         = 0x00000012, /*!<  Live tuning module */
    ILOG_MOD_TDS        = 0x00000013, /*!<  Tuning dataset module */
    ILOG_MOD_LST        = 0x00000013, /*!<  last defined module */
    ILOG_MOD_ALL        = (1 << ILOG_MOD_LST) - 1  /*!< bit flag value of all mods, enables all, must be in range of int */
} ilog_Mod_t;

/******************************************************************************/
/** @brief          A brief text description (one line).
 *  @details        Optionally a detailed text description of the code object.
 ******************************************************************************/
typedef enum
{
    ILOG_LEV_MSG = 0, /*!<  log only high prio messages */
    ILOG_LEV_ERR = 1, /*!<  log errors and above */
    ILOG_LEV_WRN = 2, /*!<  log warnings and above */
    ILOG_LEV_NFO = 3, /*!<  log infos and above */
    ILOG_LEV_DBG = 4, /*!<  log debug logs and above */

    ILOG_LEV_LST = 4   /*!<  log everything, MUST be last in list */
} ilog_Level_t;

/******************************************************************************/
/** @brief  Function to call if assertion hits
 ******************************************************************************/
typedef void (*ilog_OnAssert_t)(void*);

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/
/******************************************************************************/
/** @brief          Add log entry
 *  @param[in]      aModule  Module ID
 *  @param[in]      aLevel   Level of log
 *  @param[in]      apcFile  File posting log
 *  @param[in]      aLine    Line posting log
 *  @param[in]      apcFunc  Function posting log
 *  @param[in]      apcFmt   Format string of the log message
 *  @param[in]      ...      Var args of log
 ******************************************************************************/
extern void ilog_Add(ilog_Mod_t aModule,
                     ilog_Level_t aLevel,
                     const ilog_char_t* apcFile,
                     int32_t aLine,
                     const ilog_char_t* apcFunc,
                     const ilog_char_t* apcFmt,
                     ...);

/******************************************************************************/
/** @brief          Assert log
 *  @param[in]      apcCond  Assert condition
 *  @param[in]      apcFile  File issuing Assert
 *  @param[in]      aLine    Line issuing Assert
 *  @param[in]      apcFunc  Function issuing Assert
 ******************************************************************************/
extern void ilog_Assert(const ilog_char_t* apcCond,
                        const ilog_char_t* apcFile,
                        int32_t aLine,
                        const ilog_char_t* apcFunc);

/******************************************************************************/
/** @brief          Enable module for logging
 *  @param[in]      aMod   Module ID
 ******************************************************************************/
extern void ilog_EnableModule(ilog_Mod_t aMod);

/******************************************************************************/
/** @brief          Disable module for logging
 *  @param[in]      aMod   Module ID
 ******************************************************************************/
extern void ilog_DisableModule(ilog_Mod_t aMod);

/******************************************************************************/
/** @brief          Set logging level
 *  @param[in]      aLvl level
 ******************************************************************************/
extern void ilog_SetLevel(ilog_Level_t aLvl);

/******************************************************************************/
/** @brief          Set logging output stream
 *  @param[in]      apOstr output FILE stream pointer
 ******************************************************************************/
extern void ilog_SetOstr(FILE* apOstr);

/******************************************************************************/
/** @brief          Set function to be called if assertion hits
 *  @param[in]      apOnAssert function pointer
 *  @param[in]      apParam parameter to pass to the function
 ******************************************************************************/
extern void ilog_SetOnAssert(ilog_OnAssert_t apOnAssert, void* apParam);

/******************************************************************************/
/** @brief          Simple log entry
 *  @param[in]      aModule  Module ID
 *  @param[in]      aLevel   Level of log
 *  @param[in]      apcFmt   Format string of the log message
 *  @param[in]      ...      Var args of log
 ******************************************************************************/
extern void ilog_Simple(            ilog_Mod_t      aModule,
                                    ilog_Level_t    aLevel,
                            const   ilog_char_t*    apcFmt,
                                    ...);

#ifdef __cplusplus
}
#endif

#endif // #ifndef ILOG_H
