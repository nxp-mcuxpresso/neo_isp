/*******************************************************************************
 * Copyright 2017, 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/*******************************************************************************/
/**
 * @file           ilog_func.c
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#if defined(LINUX_EMB) || defined(LINUX_PC)
    #include <time.h>
#endif
#if defined(SDK_OS_FREE_RTOS) && defined(IMX95)
#include "fsl_debug_console.h"
#endif
#include "ilog.h"

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                    MACROS
==============================================================================*/

/*==============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/
/** @brief Array of log module names */
ilog_char_t* ilog_gpModNames[] =
{
    "GEN",          /*!< unspecified generic module*/
    "ISP",          /*!< ISP module */
    "CAM_LIB",      /*!< CAM_LIB module */
    "CCA",          /*!< CCA module */
    "ISP_FW",       /*!< ISP firmware simulator */
    "ISP_HW_SIM",   /*!< ISP hardware simulator (ISIM) */
    "ISI",          /*!< ISI module (ISIM) */
    "UTILS",        /*!< UTILS module */
    "CSI",          /*!< CSI module */
    "CGD",          /*!< CGD module */
    "CCF",          /*!< CCF module */
    "CCF_DRC",      /*!< CCF_DRC module */
    "CCF_WB",       /*!< CCF_WB module */
    "CCF_WBB",      /*!< CCF_WBB module */
    "CCF_WBR",      /*!< CCF_WBR module */
    "CCF_AEC",      /*!< CCF_AEC module */
    "CCF_CCTX",     /*!< CCF_CAM_CTX module */
    "LT",           /*!< Live tuning module */
    "CCF_TDS",      /*!< Tuning dataset module */
};

/** @brief Array of log level names */
ilog_char_t* ilog_gpLvlNames[] =
{
    "MSG",  /*!<  high prio messages */
    "ERR",  /*!<  errors */
    "WRN",  /*!<  warnings */
    "NFO",  /*!<  infos */
    "DBG"   /*!<  debug */
};

/*==============================================================================
*                         LOCAL VARIABLE DECLARATIONS
==============================================================================*/

uint32_t ilog_gMod              = ILOG_MOD_ALL; /**< default enabled modules flags*/
ilog_Level_t ilog_gLev          = ILOG_LEV_ERR; /**< default log level */
FILE* ilog_gpOstr               = NULL; /**< default log output stream */
ilog_OnAssert_t ilog_gpOnAssert = NULL; /**< default OnAssert function */
void* ilog_gpOnAssertParam      = NULL; /**< default OnAssert parameter */

/*==============================================================================
*                             FUNCTION IMPLEMENTATION
==============================================================================*/
void ilog_Add(ilog_Mod_t aModule,
              ilog_Level_t aLevel,
              const ilog_char_t* apcFile,
              int32_t aLine,
              const ilog_char_t* apcFunc,
              const ilog_char_t* apcFmt,
              ...)
{
    if (ilog_gpOstr == NULL)
    {
        ilog_gpOstr = stdout;
    }
    if ((aModule <= ILOG_MOD_LST) && (0 != (ilog_gMod & (1u << (aModule-1)))) && (aLevel <= ilog_gLev) && (aLevel <= ILOG_LEV_LST))
    {
        int32_t lBytesWritten;
        ilog_char_t lTmp[ilog_scMaxLogBytes];

#if defined(LINUX_EMB) || defined(LINUX_PC)
        clock_t lClk = clock() /*/CLOCKS_PER_SEC*/;
        // print initial fields
        lBytesWritten = snprintf(lTmp, ilog_scMaxLogBytes, "[%ld]:%s_%s: %s:%"PRIi32":%s():: ", lClk, ilog_gpModNames[aModule - 1], ilog_gpLvlNames[aLevel], apcFile, aLine, apcFunc);
#else
        // print initial fields
        lBytesWritten = snprintf(lTmp, ilog_scMaxLogBytes, "%s_%s: %s:%"PRIi32":%s():: ", ilog_gpModNames[aModule - 1], ilog_gpLvlNames[aLevel], apcFile, aLine, apcFunc);
#endif

        // check if space left on the message
        if ((lBytesWritten >= 0) && (lBytesWritten < ilog_scMaxLogBytes))
        {
            // log the message
            va_list lAp;
            va_start(lAp, apcFmt);
            vsnprintf(lTmp + lBytesWritten, ilog_scMaxLogBytes - lBytesWritten, apcFmt, lAp);
            va_end(lAp);
        } // if still some space

        //todo nxg10034 temporary for debugging
#if defined(SDK_OS_FREE_RTOS) && defined(IMX95)
        PRINTF("%s\r\n", lTmp);
#endif

        // output the log
        fprintf(ilog_gpOstr, "%s", lTmp);
    } // if module enabled and level fits
} // ilog_Add()

/******************************************************************************/
void ilog_Assert(const ilog_char_t* apcCond, const ilog_char_t* apcFile, int32_t aLine, const ilog_char_t* apcFunc)
{
#if defined(LINUX_EMB) || defined(LINUX_PC)
    clock_t lClk = clock() /*/CLOCKS_PER_SEC*/;

    fprintf(ilog_gpOstr,
            "[%ld]:ASSERT:%s:%s():%"PRIi32":: (%s)\n",
            lClk,
            apcFile,
            apcFunc,
            aLine,
            apcCond);
#else

    fprintf(ilog_gpOstr,
            "ASSERT:%s:%s():%"PRIi32":: (%s)\n",
            apcFile,
            apcFunc,
            aLine,
            apcCond);
#endif

    if (ilog_gpOnAssert != NULL)
    {
        ilog_gpOnAssert(ilog_gpOnAssertParam);
    } // if OnAssert defined
    else
    {
#ifdef __STANDALONE__
        while (1)
        {
            ;
        } // while(1)
#else // ifdef __STANDALONE__
        abort();
#endif // else from ifdef __STANDALONE__
    } // else from if OnAssert defined
} // ilog_Assert()

/******************************************************************************/
void ilog_EnableModule(ilog_Mod_t aMod)
{
    if (aMod == ILOG_MOD_ALL)
    {
        ilog_gMod = ILOG_MOD_ALL;
    }
    else
    {
        ilog_gMod |= (1u << (aMod-1));
    }
} // ilog_EnableModule()

/******************************************************************************/
void ilog_DisableModule(ilog_Mod_t aMod)
{
    if (aMod == ILOG_MOD_ALL)
    {
        ilog_gMod = 0;
    }
    else
    {
        ilog_gMod &= ~(1u << (aMod-1));
    }
} // ilog_DisableModule()

/******************************************************************************/
void ilog_SetLevel(ilog_Level_t aLvl)
{
    ilog_gLev = aLvl;
} // ilog_SetLevel()

/******************************************************************************/
void ilog_SetOstr(FILE* apOstr)
{
    ilog_gpOstr = apOstr;
} // ilog_SetOstr()

/******************************************************************************/
void ilog_SetOnAssert(ilog_OnAssert_t apOnAssert, void* apParam)
{
    ilog_gpOnAssert      = apOnAssert;
    ilog_gpOnAssertParam = apParam;
} // ilog_SetOnAssert()

/******************************************************************************/
void ilog_Simple(           ilog_Mod_t      aModule,
                            ilog_Level_t    aLevel,
                    const   ilog_char_t*    apcFmt,
                    ...)
{
    if (ilog_gpOstr == NULL)
    {
        ilog_gpOstr = stdout;
    }

    if ((aModule    <=  ILOG_MOD_LST) &&
        (0          !=  (ilog_gMod & (1u << (aModule-1)))) &&
        (aLevel     <=  ilog_gLev) &&
        (aLevel     <=  ILOG_LEV_LST))
    {
        ilog_char_t lTmp[ilog_scMaxLogBytes];

        // log the message
        va_list lAp;

        va_start(lAp, apcFmt);

        vsnprintf(  lTmp,
                    ilog_scMaxLogBytes,
                    apcFmt,
                    lAp);

        va_end(lAp);

        // output the log
        fprintf(ilog_gpOstr,
                "%s",
                lTmp);

    } // if module enabled and level fits

} // ccf_ilog_Simple()

/*EOF*/
