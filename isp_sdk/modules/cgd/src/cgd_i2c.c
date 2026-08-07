/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cgd_i2c.c
 * @brief userspace i/f to access I2C devices.
 *******************************************************************************/
#ifdef LINUX_EMB
#define _DEFAULT_SOURCE /* remove warning from usleep() */
#endif

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#ifdef LINUX_EMB
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <assert.h>
#elif SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#if defined(GHS)
#include <INTEGRITY.h>
#include <INTEGRITY_assert.h>
#define assert(x)   Assert(x)
#endif

#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "ilog.h"
#include "cgd.h"
#include "cgd_internal.h"
#include "utils_i2c.h"

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/
// max command size XFR_SIZE - uint8_t
#define CGD_CMD_MAX_SIZE 255U
#define CGD_I2C_MAX_SEND_ATTEMPS 3
/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

typedef struct
{
    i2c_Dev_t* mI2cDevice; /** @brief device used for I2C comuniccation */
    cgd_Cmd_t* mpcI2cTable; /** @brief I2c command data table */
    uint32_t mCmdCnt; /** @brief Size of command table */
    uint32_t mCmdIdx; /** @brief Current index in command table */
    uint32_t mSaveIdx; /** @brief Current index of save buffer*/
    uint8_t* mpSaveBuff; /** @brief the save data buff*/
    uint32_t mSaveBuffSz; /** @brief Size of save data buff*/

    uint32_t mByteCnt; /** @brief Bytes count for actual command, XFR_SIZE */
    uint8_t mpData[CGD_CMD_MAX_SIZE]; /** @brief Contain read and mask data */
    uint32_t mWriteDataByteCnt; /** @brief Bytes count for write command, XFR_SIZE */
    uint8_t mpWriteData[CGD_CMD_MAX_SIZE]; /** @brief Contain write data */
    uint32_t mDelay; /** @brief Delay data */
    bool mIsMask; /** @brief Mark WAIT and MASK command */
    bool mIsDebug; /** @brief Mark as debug table - all commands are printed to console */
} TableStatus_t;

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/****************************************************************************/
/** Prepare mpDataWrite, mpDataRead, mpDataMask in TableStatus_t type
 *  also include CGD_CMD_CON
 *
 * @param  pTableStatus  comand table
 *
 * @return CGD_LIB_SUCCESS If all ok
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result PrepareCommandData(TableStatus_t* pTableStatus);

/****************************************************************************/
/** Execute single command from pTableStatus
 *
 * @param  pTableStatus  comand table
 * @param  mI2cDevice  connected i2c device
 *
 * @return CGD_LIB_SUCCESS If all ok
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result ExecCmdTable(TableStatus_t* pTableStatus);

/****************************************************************************/
/**
 * @brief Read and check readed data with values from table, data are masked with mask value
 *
 * @param  pTableStatus  comand table
 *
 * @return CGD_LIB_SUCCESS if data are same
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result CheckWrittenData(TableStatus_t* pTableStatus);

/****************************************************************************/
/**
 * @brief Read and mask readed data with values from table, at the end write data back to I2C
 *
 * @param  pTableStatus  comand table
 *
 * @return CGD_LIB_SUCCESS if data are same
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result MaskData(TableStatus_t* pTableStatus);

/****************************************************************************/
/**
 * @brief Read data from I2C
 *
 * @param  pTableStatus  comand table
 *
 * @return CGD_LIB_SUCCESS if data are same
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result ReadData(TableStatus_t* pTableStatus);

/****************************************************************************/
/**
 * @brief Write data to I2C
 *
 * @param  pTableStatus  comand table
 *
 * @return CGD_LIB_SUCCESS if data are same
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result WriteData(TableStatus_t* pTableStatus);

/****************************************************************************/
/**
 * @brief Write data to I2C - internal call, repeating in case of failure
 *
 * @param  pTableStatus  comand table
 *
 * @return CGD_LIB_SUCCESS if data are same
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result WriteDataInternal(const i2c_Dev_t* apcDev, const uint8_t* apcBuf, const ssize_t acByteCnt);

/****************************************************************************/
/**
 * @brief Get continued command data from table
 *
 * @param  arTabStatus  comand table
 * @param  aReadBytes  already read bytes
 * @param  aTotalBytes  size of command (defined in table on start)
 * @param  destArray  temp table to save result
 *
 * @return CGD_LIB_SUCCESS if load succeded
 *         CGD_LIB_FAILURE If failed
 *
 ****************************************************************************/
static cgd_Result GetConCmdData(TableStatus_t* arTabStatus, uint16_t aReadBytes, const uint16_t aTotalBytes, uint8_t* destArray);

/****************************************************************************/
/**
 * @brief Get real device I2C address from table row
 *
 * @param  arCmdHead head of port
 * @param  row  row from command table for device translation
 *
 * @return device adress if load succeded
 *         -1 If failed
 *
 ****************************************************************************/
static int32_t GetDeviceI2CAdress(const cgd_CsiExtCfg_t* arExtCfg, cgd_Cmd_t row);

/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/
/****************************************************************************/
static cgd_Result PrepareCommandData(TableStatus_t* pTableStatus)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;
    cgd_Cmd_t row   = pTableStatus->mpcI2cTable[pTableStatus->mCmdIdx];

    memset(pTableStatus->mpData, 0, sizeof(pTableStatus->mpData)); // clean read and mask buffer
    uint32_t size    = 0;
    uint8_t prepared = 0;

    if (row.mCmdId == CGD_CMD_WRITE)
    {
        memset(pTableStatus->mpWriteData, 0, sizeof(pTableStatus->mpWriteData)); // clean only write buffer
        pTableStatus->mWriteDataByteCnt = row.mpData[0];
        pTableStatus->mpWriteData[0]    = row.mpData[1];
        pTableStatus->mpWriteData[1]    = row.mpData[2];
        pTableStatus->mpWriteData[2]    = row.mpData[3]; // prepare first 3 bytes

        prepared = 3;
        size     = pTableStatus->mWriteDataByteCnt;
    }
    else
    {
        pTableStatus->mByteCnt  = row.mpData[0];
        pTableStatus->mpData[0] = row.mpData[1];
        pTableStatus->mpData[1] = row.mpData[2];
        pTableStatus->mpData[2] = row.mpData[3]; // prepare first 3 bytes

        prepared = 3;
        size     = pTableStatus->mByteCnt;
    }

    switch (row.mCmdId)
    {
        case CGD_CMD_DBG: // struct for debug, short message, usefull for debugger attach
        {
            printf("\nCGD_CMD_DBG: %.4s\n\n", row.mpData);
        }
        break;
        case CGD_CMD_DELAY: // struct of mData {MSEC}
        {
            pTableStatus->mDelay = row.mpData[0]; // time in usec
        }
        break;
        case CGD_CMD_MASK: // struct of mData {XFR_SIZE, DATA, MASK (DATA, MASK, ...)}
        {
            size                  = size * 2U; // we need transfer data bytes with mask
            pTableStatus->mIsMask = true;
            lRet                  = GetConCmdData(pTableStatus, prepared, size, pTableStatus->mpData); // add next bytes from continue command
        }
        break;
        case CGD_CMD_WAIT: // struct of mData {XFR_SIZE RCNT DL DATA MASK}
        {
            size                  = 2 + size * 2U; // we need transfer count RCNT, DL + data bytes with mask
            pTableStatus->mIsMask = true;
            lRet                  = GetConCmdData(pTableStatus, prepared, size, pTableStatus->mpData); // add next bytes from continue command
            pTableStatus->mDelay  = row.mpData[2]; // time in usec
        }
        break;
        case CGD_CMD_WRITE: // struct of mData {XFR_SIZE, ADDR_H, ADDR_L, (DATA)}
        {
            lRet = GetConCmdData(pTableStatus, prepared, size, pTableStatus->mpWriteData); // add next bytes from continue command
        }
        break;

        case CGD_CMD_CNT:
        case CGD_CMD_CON:
        case CGD_CMD_READ: // struct of mData {XFR_SIZE}
        {
            break; // read dont use any continue command, preparation is done
        }
    }

    return lRet;
} // PrepareCommandData

/****************************************************************************/
static cgd_Result ExecCmdTable(TableStatus_t* pTableStatus)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;
    cgd_Cmd_t row   = pTableStatus->mpcI2cTable[pTableStatus->mCmdIdx];

    lRet = PrepareCommandData(pTableStatus);

    switch (row.mCmdId)
    {
        case CGD_CMD_DELAY:
        {
#if defined(LINUX_EMB) || defined(GHS)
            (void)usleep(pTableStatus->mDelay * 1000); // just sleep
#elif SDK_OS_FREE_RTOS
            vTaskDelay(pdMS_TO_TICKS(pTableStatus->mDelay));
#endif
        }
        break; // case CGD_CMD_DELAY

        case CGD_CMD_WRITE:
        {
            lRet = WriteData(pTableStatus);
            if (lRet != CGD_LIB_SUCCESS)
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD_CMD_WRITE IDX:%d failed waiting to data.\n", pTableStatus->mCmdIdx);
                break;
            }
        }
        break; // case CGD_CMD_WRITE

        case CGD_CMD_READ:
        {
            lRet = ReadData(pTableStatus);
            if (lRet != CGD_LIB_SUCCESS)
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD_CMD_READ IDX:%d failed waiting to data.\n", pTableStatus->mCmdIdx);
                break;
            }
        }
        break; // case CGD_CMD_READ

        case CGD_CMD_CON:
        {
            break; // continue to next command, no action here
        } // case CGD_CMD_CON

        case CGD_CMD_MASK:
        {
            lRet = MaskData(pTableStatus);
            if (lRet != CGD_LIB_SUCCESS)
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD_CMD_MASK IDX:%d failed waiting to data.\n", pTableStatus->mCmdIdx);
                break;
            }
        }
        break; // case CGD_CMD_MASK

        case CGD_CMD_WAIT:
        {
            //{XFR_SIZE RCNT DL DATA MASK}
            uint8_t repeatMaxCnt = pTableStatus->mpData[0];

            for (uint8_t i = 0; i < repeatMaxCnt; i++)
            {
                lRet = CheckWrittenData(pTableStatus);

                if (lRet == CGD_LIB_SUCCESS)
                {
                    break; // correct value
                }

#if defined(LINUX_EMB) || defined(GHS)
                (void)usleep(pTableStatus->mDelay); // just sleep
#elif SDK_OS_FREE_RTOS
                vTaskDelay(pdMS_TO_TICKS(pTableStatus->mDelay));
#endif
            }

            if (lRet != CGD_LIB_SUCCESS)
            {
                if (pTableStatus->mIsDebug)
                {
                    ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD_CMD_WAIT IDX:%d failed waiting to data.\n", pTableStatus->mCmdIdx);
                }
                break;
            }
        }
        break; // case CGD_CMD_WAIT

        case CGD_CMD_DBG:
        case CGD_CMD_CNT:
        {
            break;
        }
    }

    return lRet;
} // ExecCmdTable

/****************************************************************************/
static cgd_Result MaskData(TableStatus_t* pTableStatus)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    uint8_t size = pTableStatus->mByteCnt;

    uint8_t buffer[size];
    memset(buffer, 0, sizeof(size));

    // first receive data from device
    if (0 != i2c_Recv(pTableStatus->mI2cDevice, buffer, size))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "0x%02X CGD_CMD_MASK failed to receive data from i2c device\n", pTableStatus->mI2cDevice->mAddr);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    if (pTableStatus->mIsDebug)
    {
        printf("\t0x%02X CGD_CMD_MASK read values:", pTableStatus->mI2cDevice->mAddr);
        for (uint8_t i = 0; i < size; i++)
        {
            printf(" 0x%02X", buffer[i]);
        }
        printf("\n");
    }

    // mask received data with values from table
    for (uint8_t i = 0; i < size; i++)
    {
        uint8_t write = pTableStatus->mpData[i * 2];
        uint8_t mask  = pTableStatus->mpData[i * 2 + 1];

        buffer[i] = (buffer[i] & ~mask) | (write & mask);
    }

    if (pTableStatus->mIsDebug)
    {
        printf("\t0x%02X CGD_CMD_MASK mask values:", pTableStatus->mI2cDevice->mAddr);
        for (uint8_t i = 0; i < size; i++)
        {
            printf(" 0x%02X", buffer[i]);
        }
        printf("\n");
    }

    // write data back
    uint8_t newWriteSize = pTableStatus->mWriteDataByteCnt + size;
    uint8_t writeBuffer[newWriteSize];
    memcpy(&writeBuffer[0], &pTableStatus->mpWriteData, pTableStatus->mWriteDataByteCnt);
    memcpy(&writeBuffer[pTableStatus->mWriteDataByteCnt], &buffer, size); // merge write command with adress and new values from mask together

    if (pTableStatus->mIsDebug)
    {
        printf("\t0x%02X CGD_CMD_MASK write value:", pTableStatus->mI2cDevice->mAddr);
        for (uint8_t i = 0; i < newWriteSize; i++)
        {
            printf(" 0x%02X", writeBuffer[i]);
        }
        printf("\n");
    }

    if (CGD_LIB_FAILURE == WriteDataInternal(pTableStatus->mI2cDevice, writeBuffer, newWriteSize))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "0x%02X MaskData write to device failed after %d attemps!\n", pTableStatus->mI2cDevice->mAddr, CGD_I2C_MAX_SEND_ATTEMPS);
        lRet = CGD_LIB_FAILURE;
    }

    return lRet;
}

/****************************************************************************/
static cgd_Result ReadData(TableStatus_t* pTableStatus)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    uint8_t size = pTableStatus->mByteCnt;

    uint8_t buffer[size];
    memset(buffer, 0, sizeof(buffer));

    if (I2C_LIB_FAILURE == i2c_Recv(pTableStatus->mI2cDevice, buffer, size))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "0x%02X CGD_CMD_READ failed to receive data from device\n", pTableStatus->mI2cDevice->mAddr);
        lRet = CGD_LIB_FAILURE;
        return lRet;
    }

    if (pTableStatus->mIsDebug)
    {
        printf("\t0x%02X CGD_CMD_READ read values from {", pTableStatus->mI2cDevice->mAddr);
        for (uint8_t i = 0; i < pTableStatus->mWriteDataByteCnt; i++) // read last offset writed
        {
            printf(" 0x%02X", pTableStatus->mpWriteData[i]);
        }
        printf("}:");
        for (uint8_t i = 0; i < size; i++)
        {
            if (i % 15 == 0)
            {
                printf("\n");
                printf("\t\t0x%02X", buffer[i]);
            }
            else
            {
                printf(";0x%02X", buffer[i]);
            }
        }
        printf("\n");
    }

    /*Save data to save buffer, if is available*/
    if ((pTableStatus->mpSaveBuff != NULL) && (pTableStatus->mSaveIdx < pTableStatus->mSaveBuffSz))
    {
        uint32_t lNumberSave = size;
        lNumberSave          = ((lNumberSave + pTableStatus->mSaveIdx) > pTableStatus->mSaveBuffSz) ? (pTableStatus->mSaveBuffSz - pTableStatus->mSaveIdx) : lNumberSave; // remain size
        if (lNumberSave != 0U)
        {
            (void)memcpy(&pTableStatus->mpSaveBuff[pTableStatus->mSaveIdx], &buffer, lNumberSave);
            pTableStatus->mSaveIdx += lNumberSave;
        }
    }
    return lRet;
}

/****************************************************************************/
static cgd_Result WriteData(TableStatus_t* pTableStatus)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;
    assert(pTableStatus->mWriteDataByteCnt < INT32_MAX);

    if (CGD_LIB_FAILURE == WriteDataInternal(pTableStatus->mI2cDevice, pTableStatus->mpWriteData, pTableStatus->mWriteDataByteCnt))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "0x%02X CGD_CMD_WRITE write to device failed after %d attemps:\n", pTableStatus->mI2cDevice->mAddr, CGD_I2C_MAX_SEND_ATTEMPS);
        lRet = CGD_LIB_FAILURE;
    }

    if (pTableStatus->mIsDebug || lRet == CGD_LIB_FAILURE)
    {
        printf("\t0x%02X CGD_CMD_WRITE:", pTableStatus->mI2cDevice->mAddr);
        for (uint8_t i = 0; i < pTableStatus->mWriteDataByteCnt; i++)
        {
            printf(" 0x%02X,", pTableStatus->mpWriteData[i]);
        }
        printf("\n");
    }

    return lRet;
}

/****************************************************************************/
static cgd_Result WriteDataInternal(const i2c_Dev_t* apcDev, const uint8_t* apcBuf, const ssize_t acByteCnt)
{
    cgd_Result lRet = CGD_LIB_FAILURE;

    for (uint8_t attempsCount = 1; attempsCount <= CGD_I2C_MAX_SEND_ATTEMPS; attempsCount++)
    {
        if (I2C_LIB_SUCCESS == i2c_Send(apcDev, apcBuf, acByteCnt))
        {
            lRet = CGD_LIB_SUCCESS;
            break;
        }

#if defined(LINUX_EMB) || defined(GHS)
        (void)usleep(500); // just sleep
#elif SDK_OS_FREE_RTOS
        vTaskDelay(pdMS_TO_TICKS(500));
#endif
    }

    return lRet;
}

/****************************************************************************/
static cgd_Result CheckWrittenData(TableStatus_t* pTableStatus)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    WriteData(pTableStatus);

    uint8_t size = pTableStatus->mByteCnt;
    uint8_t read[size];
    memset(read, 0, sizeof(size));

    // first receive data from device
    if (0 != i2c_Recv(pTableStatus->mI2cDevice, read, size))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD_CMD_WAIT failed to receive data from i2c device\n");
        lRet = CGD_LIB_FAILURE;
    }

    if (pTableStatus->mIsDebug)
    {
        printf("\tCGD_CMD_WAIT read values:");
        for (uint8_t i = 0; i < size; i++)
        {
            printf(" 0x%02X", read[i]);
        }
        printf("\n");
    }

    // mask received data with values from table
    for (uint8_t i = 0; i < size; i++)
    {
        uint8_t write = pTableStatus->mpData[2 + i * 2];
        uint8_t mask  = pTableStatus->mpData[2 + i * 2 + 1];

        if ((read[i] & mask) != (write & mask))
        {
            lRet = CGD_LIB_FAILURE;
            break;
        }
    }

    return lRet;
} // CheckWrittenData

/****************************************************************************/
static cgd_Result GetConCmdData(TableStatus_t* arTabStatus, uint16_t aReadBytes, const uint16_t aTotalBytes, uint8_t* destArray)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    while (aReadBytes < aTotalBytes)
    {
        arTabStatus->mCmdIdx += 1U;
        const cgd_Cmd_t* lpCmd = &arTabStatus->mpcI2cTable[arTabStatus->mCmdIdx];

        if (lpCmd->mCmdId == CGD_CMD_CON)
        {
            (void)memcpy(&destArray[aReadBytes], &lpCmd->mpData, sizeof(lpCmd->mpData));
            aReadBytes += sizeof(lpCmd->mpData);
        } // if command is CON
        else
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "The cmd %d of table must be CMD_CON!\n", arTabStatus->mCmdIdx);
            lRet = CGD_LIB_FAILURE;
            break;
        } // if command isn't CON
    } // while loop to get data from CON cmd

    return lRet;
} // GetConCmdData

/****************************************************************************/
static int32_t GetDeviceI2CAdress(const cgd_CsiExtCfg_t* arExtCfg, cgd_Cmd_t row)
{
    int32_t lRet = -1;

    if (arExtCfg != NULL && arExtCfg->mpI2cAdr != NULL && row.mDevId < arExtCfg->mI2cAdrCnt)
    {
        lRet = arExtCfg->mpI2cAdr[row.mDevId];
    }

    return lRet;
} // GetDeviceI2CAdress

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

/****************************************************************************/
cgd_Result cgd_ExecTable(cgd_CmdTable_t* arCmdTable)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    cgd_CsiPort_t* csiPort = cgd_GetCsiPort(arCmdTable->mPortIdx);
    if (csiPort == NULL || csiPort->mPortState == CGD_STATE_OFF)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD: unable to execute command table for port %d - port is not opened\n", arCmdTable->mPortIdx);
        return CGD_LIB_FAILURE;
    }

    cgd_PortHead_t* cgdPortHead = csiPort->mcHead;
    if (cgdPortHead == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD: unable to execute command table for port %d - port is not opened\n", arCmdTable->mPortIdx);
        return CGD_LIB_FAILURE;
    }

    cgd_CsiExtCfg_t* csiExtCfg = cgd_GetCsiPortConfig(arCmdTable->mPortIdx);
    if (csiExtCfg == NULL)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD: unable to get ext config for port %d\n", arCmdTable->mPortIdx);
        return CGD_LIB_FAILURE;
    }

#ifdef LINUX_EMB
    if (0 != pthread_mutex_lock(&cgdPortHead->mI2CMtx))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_ExecTable: mutex_lock fail.\n");
        return CGD_LIB_FAILURE;
    }
#endif
#ifdef GHS
    Error result = WaitForLocalMutex(cgdPortHead->mI2CMtx);
    if (result != Success) {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_ExecTable: mutex_lock fail. (%d)\n", result);
        return CGD_LIB_FAILURE;
    }
#endif
#ifdef SDK_OS_FREE_RTOS
    if (xSemaphoreTake(cgdPortHead->mI2CMtx, portMAX_DELAY) != pdTRUE)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_ExecTable: mutex_lock fail.\n");
        return CGD_LIB_FAILURE;
    }
#endif

    TableStatus_t lTabStatus;
    memset(&lTabStatus, 0, sizeof(TableStatus_t));

    lTabStatus.mpcI2cTable = arCmdTable->ympI2cTable;
    lTabStatus.mCmdCnt     = arCmdTable->mI2cTableSz / sizeof(cgd_Cmd_t);
    lTabStatus.mCmdIdx     = 0U;
    lTabStatus.mSaveIdx    = 0U;
    lTabStatus.mpSaveBuff  = arCmdTable->mpSaveBuff;
    lTabStatus.mSaveBuffSz = arCmdTable->mSaveBuffSz;
    lTabStatus.mI2cDevice  = &cgdPortHead->mI2cPortDev;
    lTabStatus.mIsDebug    = arCmdTable->mIsDebug;

    if (lTabStatus.mIsDebug)
    {
        printf("CGD: started executing table:\n");
    }

    for (lTabStatus.mCmdIdx = 0; lTabStatus.mCmdIdx < lTabStatus.mCmdCnt; lTabStatus.mCmdIdx++) // go thru array of commands
    {
        cgd_Cmd_t row     = arCmdTable->ympI2cTable[lTabStatus.mCmdIdx];
        int32_t i2cDevAdd = GetDeviceI2CAdress(csiExtCfg, row);

        if (i2cDevAdd == -1)
        {
            ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD: get device adress failed!\n");
            lRet = CGD_LIB_FAILURE;
        }

        if (lRet == CGD_LIB_FAILURE)
        {
            break;
        }

        if (i2cDevAdd != lTabStatus.mI2cDevice->mAddr) // connect to new device addres if row contains another device
        {
            if (I2C_LIB_FAILURE == i2c_SetAdress(lTabStatus.mI2cDevice, i2cDevAdd))
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD: i2c device adress change failed!\n");
                lRet = CGD_LIB_FAILURE;
            }
        }

        if (lRet == CGD_LIB_FAILURE)
        {
            break;
        }

        lRet = ExecCmdTable(&lTabStatus);

        if (lRet == CGD_LIB_FAILURE)
        {
            cgd_Cmd_t row = lTabStatus.mpcI2cTable[lTabStatus.mCmdIdx];
            if (row.mCmdId == CGD_CMD_WAIT || row.mCmdId == CGD_CMD_CON) // don't throw timeout errors, need to be handled outside
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_WRN, "CGD: Table CGD_CMD_WAIT failed! Row idx: %d\n", lTabStatus.mCmdIdx);
            }
            else
            {
                ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "CGD: Table execution failed! Row idx: %d\n", lTabStatus.mCmdIdx);
            }

            break;
        }
    }

#ifdef LINUX_EMB
    if (0 != pthread_mutex_unlock(&cgdPortHead->mI2CMtx))
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_ExecTable: mutex_unlock fail.\n");
        lRet = CGD_LIB_FAILURE;
    }
#endif
#ifdef GHS
    result = ReleaseLocalMutex(cgdPortHead->mI2CMtx);
    if (result != Success) {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "cgd_ExecTable: mutex_lock fail. (%d)\n", result);
        return CGD_LIB_FAILURE;
    }
#endif
#ifdef SDK_OS_FREE_RTOS
    (void)xSemaphoreGive(cgdPortHead->mI2CMtx);
#endif

    if (lTabStatus.mIsDebug)
    {
        printf("CGD: executing table finished.\n");
    }

    return lRet;
} // cgd_ExecTable

/****************************************************************************/
cgd_Result cgd_PrepareAndExecTable(cgd_CsiDeviceId_t acPortIdx, cgd_CmdTable_t* ympI2cTable)
{
    cgd_Result lRet = CGD_LIB_SUCCESS;

    if (ympI2cTable == NULL || ympI2cTable->mI2cTableSz == 0)
    {
        return lRet;
    }

    ympI2cTable->mPortIdx = acPortIdx;

    if (cgd_ExecTable(ympI2cTable) != CGD_LIB_SUCCESS)
    {
        ILOG(ILOG_MOD_CGD, ILOG_LEV_ERR, "Failed to exec init table. portnum : %d \n", acPortIdx);
        lRet = CGD_LIB_FAILURE;
    } // if I2C table execution failed

    return lRet;
} // cgd_PrepareAndExecTable
