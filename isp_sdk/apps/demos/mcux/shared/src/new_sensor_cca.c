/*******************************************************************************
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  new_sensor_cca.c
 * @brief Implementation of the new sensor support layer
 *
 * @details based on ox03c10 sensor
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include <time.h>
#if defined(LINUX_EMB) || defined(LINUX_PC)
#include <linux/videodev2.h>
#endif
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "ilog.h"
#include "cca.h"
#include "utils_crc32c.h"
#include "new_sensor_cca.h"
#include "new_sensor_cca_int.h"
#include "cca_internal.h" // to interface with CCA internals

/*==============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
/******************************************************************************/
/**
 * @brief   Exposure types definitions and their indexes in arrays.
 *******************************************************************************/
typedef enum
{
    EXP_DCG    = 0U,
    EXP_HCG    = 0U, // intentionally the same as EXP_DCG
    EXP_LONG   = 0U, // intentionally the same as EXP_DCG
    EXP_LCG    = 1U,
    EXP_SHORT  = 1U, // intentionally the same as EXP_LCG
    EXP_VSHORT = 2U,
    EXP_SPD    = 3U,
    EXP_CNT    = 4U
} NewSensorExp_t;

/******************************************************************************/
/**
 * @brief   Sensor context
 *
 * @details Sensor instance specific data that need to be tracked at runtime
 *******************************************************************************/
typedef struct
{
    uint32_t mI2CAddIdx; /**< sensor address idx on i2c bus*/
    uint32_t mConversionRatio; /**< gain conversion ratio calculated based on OTP CG sensor data*/
    uint32_t mLpd2SpdRatio; /**< LPD/SPD sensitivity ratio calculated based on OTP CG sensor data*/
    uint8_t mFps; /**< desired fps (ovx3c with AD ser/deser 30fps, TI serdeser 60fps) */
    uint8_t mFirstActuate; /**< set to 1 at Open(), set to 0 at 1st actuate call */
} SensorCtx_t;
/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

/*==============================================================================
*                              LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/
/// array of start addresses of exposure registers for different exposures
static uint16_t spExpGainAddr[4] = {NEW_SENSOR_HCG_REAL_GAIN_REG, NEW_SENSOR_LCG_REAL_GAIN_REG, NEW_SENSOR_VS_REAL_GAIN_REG, NEW_SENSOR_SPD_REAL_GAIN_REG};
/// array of start addresses of white balancing registers for different exposures
static uint16_t spWbGainsAddr[4] = {NEW_SENSOR_WB_GAIN_B_HCG_REG, NEW_SENSOR_WB_GAIN_B_LCG_REG, NEW_SENSOR_WB_GAIN_B_VS_REG, NEW_SENSOR_WB_GAIN_B_SPD_REG};

/// array of sensor contexts
static SensorCtx_t spSensorCtx[ISPDRV_CAM_PIPE_ID_CNT] = {0};

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                         LOCAL FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Function return MAX_EXP_LINES param for requested fps
 *
 * @param [in] aChnlIdx for which channel
 ******************************************************************************/
static inline uint32_t getMaxExpLines(uint32_t aChnlIdx);

/******************************************************************************/
/**
 * @brief Function return ROW_TIME_NS param for requested fps
 *
 * @param [in] aChnlIdx for which channel
 ******************************************************************************/
static inline uint32_t getRowTimeNs(uint32_t aChnlIdx);
/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Parses embedded data byte array into sensor parameter structure
 *
 * The resulting values in sensor parameter structure are expected to match
 * configuration previously send to the sensor.
 *
 * Same parsing for 3 and 4 exposure mode.
 *
 * @param [in] acpcEmbd embedded data container
 * @param [out] acpSensPrm sensor parameter structure
 ******************************************************************************/
static void ParseEmdData(const cca_EmbdData_t* const acpcEmbd, cca_SensPrm_t* const acpSensPrm)
{
    uint16_t* lpData;
    uint32_t lWbGainGbGr;
    uint32_t lTmp32u;


    uint8_t lRegValues[NEW_SENSOR_EMB_DATA_SIZE];
    for (uint32_t RegId = 0; RegId < (uint32_t)NEW_SENSOR_EMB_DATA_SIZE; RegId++)
    {
        lRegValues[RegId] = acpcEmbd->mpEmbd0[4U * RegId + 3U];
    }
    // we pass the entire reg values, excluding last 4 bytes -> CRC its self
    uint32_t ComputedCrc = utils_CalcCrc32c(lRegValues, (uint32_t)NEW_SENSOR_EMB_DATA_SIZE - 4U);
    uint32_t ReceivedCrc = (((uint32_t)lRegValues[CRC32_B3_MSB]) << 24) | (((uint32_t)lRegValues[CRC32_B2]) << 16) |(((uint32_t)lRegValues[CRC32_B1]) << 8) | (lRegValues[CRC32_B0_LSB] << 0);

    if (ComputedCrc != ReceivedCrc)
    {
        acpSensPrm->mDataInvalid = true;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "CRC check of embedded data did not pass, parsing skipped.\n");
        return;
    }

    // read values from emebdded data
    lpData = (uint16_t*)acpcEmbd->mpEmbd0;

    // DCG EXP
    acpSensPrm->mpExp[EXP_DCG].mExp = ((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_DCG_EXP_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((uint32_t)lpData[NEW_SENSOR_EMB_LINE_DCG_EXP_L_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);
    // VS EXP
    acpSensPrm->mpExp[EXP_VSHORT].mExp  = ((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_VS_EXP_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((uint32_t)lpData[NEW_SENSOR_EMB_LINE_VS_EXP_L_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);
    // SPD EXP
    acpSensPrm->mpExp[EXP_SPD].mExp  = ((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_SPD_EXP_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((uint32_t)lpData[NEW_SENSOR_EMB_LINE_SPD_EXP_L_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);

    // AnalogGains
    acpSensPrm->mpExp[EXP_HCG].mAgain = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_HCG_REAL_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 7) | ((lpData[NEW_SENSOR_EMB_LINE_HCG_REAL_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU);
    acpSensPrm->mpExp[EXP_HCG].mAgain = acpSensPrm->mpExp[EXP_HCG].mAgain << 12; /* Q16_1 */
    acpSensPrm->mpExp[EXP_LCG].mAgain = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_LCG_REAL_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 7) | ((lpData[NEW_SENSOR_EMB_LINE_LCG_REAL_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU);
    acpSensPrm->mpExp[EXP_LCG].mAgain = acpSensPrm->mpExp[EXP_LCG].mAgain << 12; /* Q16_1 */
    acpSensPrm->mpExp[EXP_VSHORT].mAgain  = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_VS_REAL_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 7) | ((lpData[NEW_SENSOR_EMB_LINE_VS_REAL_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU);
    acpSensPrm->mpExp[EXP_VSHORT].mAgain  = acpSensPrm->mpExp[EXP_VSHORT].mAgain << 12; /* Q16_1 */
    acpSensPrm->mpExp[EXP_SPD].mAgain  = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_SPD_REAL_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 7) | ((lpData[NEW_SENSOR_EMB_LINE_SPD_REAL_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU);
    acpSensPrm->mpExp[EXP_SPD].mAgain  = acpSensPrm->mpExp[EXP_SPD].mAgain << 12; /* Q16_1 */

    // DigitalGains
    acpSensPrm->mpExp[EXP_HCG].mDgain = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_HCG_DIG_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x0FU) << 16) | ((lpData[NEW_SENSOR_EMB_LINE_HCG_DIG_GAIN_M_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((lpData[NEW_SENSOR_EMB_LINE_HCG_DIG_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 6); /* Q16_1 */
    acpSensPrm->mpExp[EXP_LCG].mDgain = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_LCG_DIG_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x0FU) << 16) | ((lpData[NEW_SENSOR_EMB_LINE_LCG_DIG_GAIN_M_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((lpData[NEW_SENSOR_EMB_LINE_LCG_DIG_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 6); /* Q16_1 */
    acpSensPrm->mpExp[EXP_VSHORT].mDgain  = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_VS_DIG_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x0FU) << 16) | ((lpData[NEW_SENSOR_EMB_LINE_VS_DIG_GAIN_M_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((lpData[NEW_SENSOR_EMB_LINE_VS_DIG_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 6); /* Q16_1 */
    acpSensPrm->mpExp[EXP_SPD].mDgain  = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_SPD_DIG_GAIN_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x0FU) << 16) | ((lpData[NEW_SENSOR_EMB_LINE_SPD_DIG_GAIN_M_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) << 8) | (((lpData[NEW_SENSOR_EMB_LINE_SPD_DIG_GAIN_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x03U) << 6); /* Q16_1 */

    // WhiteBalancing gains
    acpSensPrm->mWb.mGains.mBlue  = (((lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_B_HCG_H_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU) << 8) | (lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_B_HCG_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);
    lWbGainGbGr =  (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_GB_HCG_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU) << 8) | (lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_GB_HCG_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);
    lWbGainGbGr += (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_GR_HCG_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU) << 8) | (lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_GR_HCG_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);
    acpSensPrm->mWb.mGains.mGreen = (uint16_t)(lWbGainGbGr >> 1); // divide by 2
    acpSensPrm->mWb.mGains.mRed  = (((lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_R_HCG_H_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0x7FU) << 8) | (lpData[NEW_SENSOR_EMB_LINE_WB_GAIN_R_HCG_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);

    acpSensPrm->mFrameCount = (((((uint32_t)lpData[NEW_SENSOR_EMB_LINE_FRAME_COUNT_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0xFFU) << 8) | (lpData[NEW_SENSOR_EMB_LINE_FRAME_COUNT_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);

    lTmp32u = (((((uint32_t)lpData[NEW_SENSOR_EMB_TEMPERATURE_SENSOR_H_IDX]) >> NEW_SENSOR_EMB_LINE_DATA_SHIFT) & 0xFFU) << 8) | (lpData[NEW_SENSOR_EMB_TEMPERATURE_SENSOR_L_IDX] >> NEW_SENSOR_EMB_LINE_DATA_SHIFT);
    acpSensPrm->mSensorTemp = (int32_t)lTmp32u;

    acpSensPrm->mDataInvalid = false;
} // ParseEmdData()

/******************************************************************************/
/**
 * @brief Interprets the sensor parameters as parsed from embedded data array into CCF specific prameters.
 *
 * @param [in] aChnlIdx camera channel index
 *
 * @param [out] acpSensPrmCcf CCF format of sensor parameter values
 * @param [in] acpcSensPrmEmbd sensor parameter values as parsed from embedded data array
 * @param [in] apSensorCtx conversion ratios in sensor context
 *
 * @return CCA_SUCCESS if all ok, CCA_ERROR otherwise
 ******************************************************************************/
static cca_Result_t InterpretEmdData(uint32_t aChnlIdx,cca_SensPrm_t* const acpSensPrmCcf, const cca_SensPrm_t* const acpcSensPrmEmbd, SensorCtx_t* apSensorCtx)
{
    uint64_t lRatioA, lRatioB;

    // compute parameters
    acpSensPrmCcf->mpExp[EXP_LONG].mExp   = acpcSensPrmEmbd->mpExp[EXP_DCG].mExp * getRowTimeNs(aChnlIdx) / PREC_MULT;
    acpSensPrmCcf->mpExp[EXP_LONG].mAgain = (uint32_t)((uint64_t)acpcSensPrmEmbd->mpExp[EXP_HCG].mAgain * acpcSensPrmEmbd->mpExp[EXP_HCG].mDgain / Q16_1);
    acpSensPrmCcf->mpExp[EXP_LONG].mAgain = (uint32_t)((uint64_t)acpSensPrmCcf->mpExp[EXP_LONG].mAgain * apSensorCtx->mConversionRatio / Q16_1);
    acpSensPrmCcf->mpExp[EXP_LONG].mDgain = Q16_1;

    acpSensPrmCcf->mpExp[EXP_SHORT].mExp   = acpSensPrmCcf->mpExp[EXP_LONG].mExp;
    acpSensPrmCcf->mpExp[EXP_SHORT].mAgain = (uint32_t)((uint64_t)acpcSensPrmEmbd->mpExp[EXP_LCG].mAgain * acpcSensPrmEmbd->mpExp[EXP_LCG].mDgain / Q16_1);
    acpSensPrmCcf->mpExp[EXP_SHORT].mDgain = Q16_1;

    acpSensPrmCcf->mpExp[EXP_VSHORT].mExp   = acpcSensPrmEmbd->mpExp[EXP_SPD].mExp * getRowTimeNs(aChnlIdx) / PREC_MULT;
    acpSensPrmCcf->mpExp[EXP_VSHORT].mAgain = (uint32_t)((uint64_t)acpcSensPrmEmbd->mpExp[EXP_SPD].mAgain * acpcSensPrmEmbd->mpExp[EXP_SPD].mDgain / Q16_1);

    acpSensPrmCcf->mpExp[EXP_VSHORT].mDgain = Q16_1;

    /* Exposure ratios */
    lRatioA = ((uint64_t)acpSensPrmCcf->mpExp[EXP_LONG].mExp) * ((uint64_t)acpSensPrmCcf->mpExp[EXP_LONG].mAgain);
    lRatioB = ((uint64_t)acpSensPrmCcf->mpExp[EXP_SHORT].mExp) * ((uint64_t)acpSensPrmCcf->mpExp[EXP_SHORT].mAgain);
    if (lRatioB == 0U)
    {
        lRatioB = 1U;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Short exp * aGain = 0. Set to 1 to avoid division by 0.\n");
    }
    acpSensPrmCcf->mL2sRatio = (uint32_t)((lRatioA * Q8_1) / lRatioB);

    lRatioA = lRatioA * apSensorCtx->mLpd2SpdRatio / Q16_1;

    lRatioB = ((uint64_t)acpSensPrmCcf->mpExp[EXP_VSHORT].mExp) * ((uint64_t)acpSensPrmCcf->mpExp[EXP_VSHORT].mAgain);
    if (lRatioB == 0U)
    {
        lRatioB = 1U;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "VeryShort exp * aGain = 0. Set to 1 to avoid division by 0.\n");
    }
    acpSensPrmCcf->mL2vsRatio = (uint32_t)((lRatioA * Q8_1) / lRatioB);

    // go back from ovx03c 5.10 to CCF 8.8
    acpSensPrmCcf->mWb.mGains.mRed     = acpcSensPrmEmbd->mWb.mGains.mRed >> 2;
    acpSensPrmCcf->mWb.mGains.mGreen   = acpcSensPrmEmbd->mWb.mGains.mGreen >> 2;
    acpSensPrmCcf->mWb.mGains.mBlue    = acpcSensPrmEmbd->mWb.mGains.mBlue >> 2;
    acpSensPrmCcf->mWb.mOffsets.mRed   = 0;
    acpSensPrmCcf->mWb.mOffsets.mGreen = 0;
    acpSensPrmCcf->mWb.mOffsets.mBlue  = 0;

    acpSensPrmCcf->mFrameCount = acpcSensPrmEmbd->mFrameCount;

    if (acpcSensPrmEmbd->mSensorTemp <= 0xC000)
    {
        acpSensPrmCcf->mSensorTemp = (acpcSensPrmEmbd->mSensorTemp * 1000) / 256; // positive
    }
    else
    {
        acpSensPrmCcf->mSensorTemp = acpcSensPrmEmbd->mSensorTemp - 0xC000; // negative
        acpSensPrmCcf->mSensorTemp = (acpSensPrmCcf->mSensorTemp * -1000) / 256;
    }

#ifdef DBG_NEW_SENSOR_AE_DISTRIBUTION
    ILOG(ILOG_MOD_CCA, ILOG_LEV_NOTE, "AE distribution:\n\t<-%u, %u, %f, %f, %f, %f, %f, %f\n", lExpDcg, lExpVs, aGainHCG / 65536., lDGainHcg / 65536., lAGainLcg / 65536., lDGainLcg / 65536., lAGainVs / 65536., lDGainVs / 65536.);
#endif

    return CCA_SUCCESS;
} // InterpretEmdData()

/******************************************************************************/
/**
 * @brief Distributes the analog gain according to sensor analog gains limitations
 *
 * @param [in] acAgain analog gain
 * @param [in] acMinAgain minimal analog gain
 * @param [in] acMaxAgain maximal analog gain
 * @param [in, out] apDgain pointer to digital gain where excess gain is carried
 * @return uint32_t the analog gain to be written in the sensor
 ******************************************************************************/
static uint32_t DistWdr3Again(const uint32_t acAgain,
                              const uint32_t acMinAgain,
                              const uint32_t acMaxAgain,
                              uint32_t* apDgain)
{
    uint64_t lTmpGain;
    uint64_t lDgainLoc = *apDgain;
    uint32_t lRetAgain = acAgain;

    if (acMaxAgain < lRetAgain)
    {
        lTmpGain  = lRetAgain;
        lRetAgain = acMaxAgain;
        lDgainLoc = (lTmpGain * lDgainLoc) / lRetAgain; /* Carry overflow acAgain into apDgain */
    }
    else
    {
        /* Should not enter here, fractional gain is bad, but just for completeness sake */
        if (acMinAgain > lRetAgain)
        {
            lRetAgain = acMinAgain;
        }
    }

    lTmpGain = lRetAgain;

    /* Select appropriate mask for AGAIN. See page 42 of data sheet */
    if (lRetAgain <= AGAIN_RANGE_1_MAX)
    {
        lRetAgain = lRetAgain & NEW_SENSOR_AGAIN_MASK_RANGE_1;
    }
    else if (lRetAgain <= AGAIN_RANGE_2_MAX)
    {
        lRetAgain = lRetAgain & NEW_SENSOR_AGAIN_MASK_RANGE_2;
    }
    else if (lRetAgain <= AGAIN_RANGE_3_MAX)
    {
        lRetAgain = lRetAgain & NEW_SENSOR_AGAIN_MASK_RANGE_3;
    }
    else
    {
        lRetAgain = lRetAgain & NEW_SENSOR_AGAIN_MASK_RANGE_4;
    }

    /* Attempt to carry masked acAgain into digital */
    *apDgain = (uint32_t)((lDgainLoc * lTmpGain + lRetAgain / 2U) / lRetAgain);

    return lRetAgain;
} // DistWdr3Again()

/******************************************************************************/
/**
 * @brief Digital gain distribution according to sensor digital gains limitations
 *
 * @param [in] apDgain digital gain
 * @param [in] acMinDgain minimal digital gain
 * @param [in] acMaxDgain maximal digital gain
 *
 * @return uint32_t resulting digital gain
 ******************************************************************************/
static uint32_t aDistWdr3Dgain(uint32_t aDgain,
                               uint32_t acMinDgain,
                               uint32_t acMaxDgain)
{
    /* Clip to valid range */
    if (acMaxDgain < aDgain)
    {
        aDgain = acMaxDgain;
    }

    if (acMinDgain > aDgain)
    {
        aDgain = acMinDgain;
    }

    /* Mask acDgain to valid settings */
    aDgain = aDgain & NEW_SENSOR_DGAIN_MASK;

    return aDgain;
} // aDistWdr3Dgain()

/******************************************************************************/
/**
 * @brief Distribution of CCF total exposure into sensor specific exposures and gains.
 *
 * @param [in] aChnlIdx for which channel
 * @param [out] acpSensPrmNew new parameter values to apply to the sensor
 * @param [in] acpSensPrmCcf sensor configuration computed by CCF
 ******************************************************************************/
static void DistributeWdr3(uint32_t aChnlIdx, cca_SensPrm_t* const acpSensPrmNew, cca_SensPrm_t* const acpSensPrmCcf)
{
    uint64_t lAgainL, lAgainS, lAgainVS, lAgainSPD;
    uint32_t lDgainL, lDgainS, lDgainVS, lDgainSPD;
    uint64_t lAddGain;
    uint32_t lExpIn = acpSensPrmCcf->mpExp[0].mExp;
    uint32_t lExpLinRows;
    uint32_t lExpVSinRows, lExpSPDinRows;
    uint64_t lExpTotalInRows;
    uint64_t lExpTotalL;
    uint64_t lRatioL2S, lRatioL2VS, lRatioL2SPD;
    uint64_t lMinGainL;
    float lTmpF;

    cca_Exp_t* lpExpOut = acpSensPrmNew->mpExp;

    /* Minimum total gain for the Short frame */
    lTmpF = NEW_SENSOR_MIN_AGAIN_SHORT * NEW_SENSOR_MIN_DGAIN_SHORT;
    uint64_t lMinGainS = ((uint64_t)lTmpF) / Q16_1;
    /* Minimum total gain for the VS frame */
    lTmpF = NEW_SENSOR_MIN_AGAIN_VS * NEW_SENSOR_MIN_DGAIN_VS;
    uint64_t lMinGainVS = ((uint64_t)lTmpF) / Q16_1;
    /* Minimum total gain for the SPD frame */
    lTmpF = NEW_SENSOR_MIN_AGAIN_SPD * NEW_SENSOR_MIN_DGAIN_SPD;
    uint64_t lMinGainSPD = ((uint64_t)lTmpF) / Q16_1;

    /* make exposure a multiple of row time as driver needed it */
    lExpLinRows = (uint32_t)((lExpIn * PREC_MULT + ROW_TIME_NS / 2U) / ROW_TIME_NS); /* in rows */
    if (lExpLinRows < MIN_EXP_LINES) /* distribution should provide valid values, here an extra check */
    {
        lExpLinRows = MIN_EXP_LINES;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Long exposure distribution < MIN_EXP_LINES. Value adjusted.\n");
    }
    else
    {
        if (lExpLinRows > getMaxExpLines(aChnlIdx))
        {
            lExpLinRows = getMaxExpLines(aChnlIdx);
            ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Long exposure distribution > MAX_EXP_LINES. Value adjusted.\n");
        }
    }

#ifdef USE_FIXED_EXP_RATIO
    lRatioL2S   = NEW_SENSOR_RATIO_LONG_TO_SHORT;
    lRatioL2SPD = NEW_SENSOR_RATIO_LONG_TO_SPD;
    lRatioL2VS  = NEW_SENSOR_RATIO_LONG_TO_VS;
#else
    lRatioL2S  = acpSensPrmCcf->mL2sRatio << (16 - 10); /* recommended L2S ratio in Q10 to Q16 */
    lRatioL2VS = acpSensPrmCcf->mL2vsRatio << (16 - 10); /* recommended L2VS ratio in Q10 to Q16 */
#endif

    if (lRatioL2S < spSensorCtx[aChnlIdx].mConversionRatio)
    {
        /* minGain for L, recalculated by conv_ratio */
        lMinGainL = lMinGainS * spSensorCtx[aChnlIdx].mConversionRatio / Q16_1;
    }
    else
    {
        /* minGain for S, recalculated by ratio */
        lMinGainL = lMinGainS * lRatioL2S / Q16_1;
    }

    /* L gain set to AEC total gain decision */
    lAgainL = (uint64_t)acpSensPrmCcf->mpExp[0].mAgain * acpSensPrmCcf->mpExp[0].mDgain / Q16_1;

    /* check lAgainL for minimum converted level, and re-calc for MIN */
    if (lAgainL < lMinGainL)
    {
        lExpTotalL = (uint64_t)lExpIn * PREC_MULT * lAgainL; /* in e[us]*1000*[g*65536] */
        lAgainL    = lMinGainL;
        lExpIn     = (uint32_t)(lExpTotalL / (PREC_MULT * lAgainL)); /* LFM restriction may not strict */
        /* make exposure a multiple of row time as driver needed it */
        lExpLinRows = (uint32_t)((lExpIn * PREC_MULT + ROW_TIME_NS / 2U) / ROW_TIME_NS); /* in rows */
        if (lExpLinRows < MIN_EXP_LINES) /* distribution should provide valid values, here an extra check */
        {
            lExpLinRows = MIN_EXP_LINES;
        }
    }

    lAddGain = ((uint64_t)lExpIn * PREC_MULT * MULT256 + (lExpLinRows * ROW_TIME_NS) / 2U) / (lExpLinRows * ROW_TIME_NS);
    lAgainL  = (lAgainL * lAddGain + MULT256 / 2U) / MULT256; /* correct gain */

    if (lAgainL < lMinGainL) /* as result of lExpLinRows limitation causing fractional lAddGain */
    {
        lAgainL = lMinGainL;
    }

    /* get lAgainS in ratio to lAgainL */
    lAgainS      = lAgainL * Q16_1 / lRatioL2S;

    lExpSPDinRows = getMaxExpLines(aChnlIdx);
    /* calculations of lAgainSPD with restrictions for SPD max exposure and min SPD gain */
    lExpTotalInRows = lAgainL * ((uint64_t)lExpLinRows * spSensorCtx[aChnlIdx].mLpd2SpdRatio / Q16_1); /* total exposure (with rows...) */
    lAgainSPD = lExpTotalInRows * Q16_1 / (lRatioL2SPD * (uint64_t)lExpSPDinRows);
    if (lAgainSPD < lMinGainSPD) /* but if min gain restriction fails */
    {
        lAgainSPD     = lMinGainSPD; /* set mandatory min gain and decrease exposure */
        lExpSPDinRows = (uint32_t)(lExpTotalInRows * Q16_1 / (lRatioL2SPD * lAgainSPD));
        if (lExpSPDinRows < MIN_EXP_LINES)
        {
            lExpSPDinRows = MIN_EXP_LINES;
        }
        lAgainSPD = lExpTotalInRows * Q16_1 / (lRatioL2SPD * (uint64_t)lExpSPDinRows);
    }

    lExpVSinRows = MAX_VS_EXP_LINES; /* default VS exposure to max */

    /* calculations of lAgainVS with restrictions for VS max exposure and min VS gain */
    lTmpF = OFFSET_M * (float)Q10_1;
    lExpTotalInRows = lAgainL * ((uint64_t)lExpLinRows * Q10_1 + (uint64_t)lTmpF); /* total exposure (with rows...) */
    lTmpF = OFFSET_VS * (float)Q10_1;
    lAgainVS        = lExpTotalInRows * Q16_1 / (lRatioL2VS * ((uint64_t)lExpVSinRows * Q10_1 + (uint64_t)lTmpF));
    if (lAgainVS < lMinGainVS) /* but if min gain restriction fails */
    {
        lExpVSinRows = (uint32_t)(lExpTotalInRows * Q16_1 / (lRatioL2VS * lMinGainVS * Q10_1));
#if MIN_VS_EXP_LINES > 0U
        if (lExpVSinRows < MIN_VS_EXP_LINES)
        {
            lExpVSinRows = MIN_VS_EXP_LINES;
        }
#endif
        while (lAgainVS < lMinGainVS)
        {
            lAgainVS = lExpTotalInRows * Q16_1 / (lRatioL2VS * ((uint64_t)lExpVSinRows * Q10_1 + (uint32_t)lTmpF));
            if ((lExpVSinRows > MIN_VS_EXP_LINES) && (lAgainVS < lMinGainVS))
            {
            lExpVSinRows--;
            }
            else
            {
                break;
            }
        }
    }

    lpExpOut[EXP_DCG].mExp = lExpLinRows; // DCG_exp_row
    lpExpOut[EXP_SPD].mExp = lExpSPDinRows; // SPD_exp_row
    lpExpOut[EXP_VSHORT].mExp = lExpVSinRows; // VS_exp_row

    lTmpF     = NEW_SENSOR_MIN_DGAIN_LONG;
    lDgainL  = (uint32_t)lTmpF;
    lTmpF     = NEW_SENSOR_MIN_DGAIN_SHORT;
    lDgainS  = (uint32_t)lTmpF;
    lTmpF     = NEW_SENSOR_MIN_DGAIN_VS;
    lDgainVS = (uint32_t)lTmpF;
    lTmpF     = NEW_SENSOR_MIN_DGAIN_SPD;
    lDgainSPD = (uint32_t)lTmpF;

    lAgainL  = (lAgainL * Q16_1) / (((uint64_t)lDgainL * spSensorCtx[aChnlIdx].mConversionRatio) / Q16_1);
    lAgainS  = (lAgainS * Q16_1) / lDgainS;
    lAgainVS = (lAgainVS * Q16_1) / lDgainVS;
    lAgainSPD = (lAgainSPD * Q16_1) / lDgainSPD;

    {
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_AGAIN_LONG;
        uint32_t lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_AGAIN_LONG;
        uint32_t lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        /* all gains are calculated as absolute gains. To get reg values divide by conversion gain and sensitivity */
        lpExpOut[EXP_LONG].mAgain = DistWdr3Again((uint32_t)(lAgainL),
                                        lNEW_SENSORMinGain,
                                        lNEW_SENSORMaxGain,
                                        &lDgainL); /* lDgainL corrected to get fraction not covered by againL */
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_AGAIN_SHORT;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_AGAIN_SHORT;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_SHORT].mAgain = DistWdr3Again((uint32_t)(lAgainS),
                                        lNEW_SENSORMinGain,
                                        lNEW_SENSORMaxGain,
                                        &lDgainS); /* lDgainS corrected to get fraction not covered by lAgainS */
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_AGAIN_SPD;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_AGAIN_SPD;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_SPD].mAgain = DistWdr3Again((uint32_t)(lAgainSPD),
                                        lNEW_SENSORMinGain,
                                        lNEW_SENSORMaxGain,
                                        &lDgainSPD); /* SPD corrected to get fraction not covered by lAgainSPD */
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_AGAIN_VS;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_AGAIN_VS;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_VSHORT].mAgain = DistWdr3Again((uint32_t)(lAgainVS),
                                        lNEW_SENSORMinGain,
                                        lNEW_SENSORMaxGain,
                                        &lDgainVS); /* lDgainVS corrected to get fraction not covered by lAgainVS */

        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_DGAIN_LONG;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_DGAIN_LONG;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_LONG].mDgain = aDistWdr3Dgain(lDgainL, lNEW_SENSORMinGain, lNEW_SENSORMaxGain);
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_DGAIN_SHORT;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_DGAIN_SHORT;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_SHORT].mDgain = aDistWdr3Dgain(lDgainS, lNEW_SENSORMinGain, lNEW_SENSORMaxGain);
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_DGAIN_SPD;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_DGAIN_SPD;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_SPD].mDgain = aDistWdr3Dgain(lDgainSPD, lNEW_SENSORMinGain, lNEW_SENSORMaxGain);
        /* avoid Misra composite expression type conversion issues*/
        lTmpF = NEW_SENSOR_MIN_DGAIN_VS;
        lNEW_SENSORMinGain = (uint32_t)lTmpF;
        lTmpF = NEW_SENSOR_MAX_DGAIN_VS;
        lNEW_SENSORMaxGain = (uint32_t)lTmpF;
        lpExpOut[EXP_VSHORT].mDgain = aDistWdr3Dgain(lDgainVS, lNEW_SENSORMinGain, lNEW_SENSORMaxGain);
    }
} // DistributeWdr3()

/******************************************************************************/
/**
 * @brief Computes conversion ratio L2S in Q16
 *
 * @param [in] aNEW_SENSORCgRatio floating point conversion ratio
 *
 * @return conversion ratio in Q16
 ******************************************************************************/
inline static uint32_t ConvRatio(float aNEW_SENSORCgRatio)
{
    float lRet = aNEW_SENSORCgRatio * NEW_SENSOR_MIN_AGAIN_LONG_F * NEW_SENSOR_MIN_DGAIN_LONG_F;
    return (uint32_t)lRet;
} // ConvRatio()

/******************************************************************************/
/**
 * @brief Remebers sensor parameters
 *
 * @param [in] aChnlIdx channel index
 * @param [in] acpParams source of new parameter values
 ******************************************************************************/
static void SetNewSensorParams(uint32_t aChnlIdx, const new_sensor_Params_t* acpParams)
{
    uint32_t lCgRatio;
    /*
        compute conversion ratio and Lpd/Spd sensitivity ratio for this sensor based on OTP data
        mOtpSensHCG (0x7057-0x7059)
        mOtpSensLCG (0x705b-0x705d)
        mOtpSensSPD (0x705f-0x7061)
    */
    if((acpParams->mOtpSensHCG > 0U) && (acpParams->mOtpSensLCG > 0U))
    {
        lCgRatio = (uint32_t)(((uint64_t)acpParams->mOtpSensHCG) * Q16_1 / acpParams->mOtpSensLCG);
    }
    else
    {
        float lCgRatioDef = NEW_SENSOR_DEFAULT_CG_RATIO;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "OTP HCG or LCG is 0. Using default conversion gain ration (0x%x).\n", (uint32_t)lCgRatioDef);
        lCgRatio = (uint32_t)lCgRatioDef;
    }
    spSensorCtx[aChnlIdx].mConversionRatio = (ConvRatio((float)lCgRatio));

    uint32_t lpdSpdRatio;
    if((acpParams->mOtpSensLCG > 0U) && (acpParams->mOtpSensSPD > 0U))
    {
        lpdSpdRatio = (uint32_t)(((uint64_t)acpParams->mOtpSensLCG) * Q16_1 / acpParams->mOtpSensSPD);
    }
    else
    {
        float lpdSpdRatioDef = NEW_SENSOR_DEFAULT_LPD_SPD_RATIO;
        ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "OTP LCG or SPD is 0. Using default LPD/SPD sensitivity ratio (0x%x).\n", (uint32_t)lpdSpdRatioDef);
        lpdSpdRatio = (uint32_t)lpdSpdRatioDef;
    }
    spSensorCtx[aChnlIdx].mLpd2SpdRatio = lpdSpdRatio;

    // store i2c address from OTP data (i.e. pair OTP data with sensor)
    spSensorCtx[aChnlIdx].mI2CAddIdx = acpParams->mI2CAddIdx;
    // store fps
    spSensorCtx[aChnlIdx].mFps = acpParams->mFps;
} // SetOvx3cParams

/*==============================================================================
*                           CCA SENSOR OPERATIONS
==============================================================================*/

/******************************************************************************/
/**
 * @brief Initializes interaction with the sensor support code.
 *
 * @details Invoked by CCA before any other sensor ops are called.
 * Gives ability to configure the sensor with initial parameters.
 *
 * TODO: reimplement to meet new sensor needs
 *
 * @param [in] aChnlIdx channel index
 * @param [in] acpcData sensor specific data structure
 * @param [in] aDataSzBytes size of the sensor specific data structure
 *
 * @return CCA_SUCCESS on success
 ******************************************************************************/
static cca_Result_t Open(uint32_t aChnlIdx, const void* const acpcData, uint32_t aDataSzBytes)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if(acpcData == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Sensor parameter data is NULL (CID %u)\n", aChnlIdx);
        lRet = CCA_ERROR;
    }
    if (aDataSzBytes != sizeof(new_sensor_Params_t))
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Sensor parameter data size (%ubytes) mismatch (expected %lu)\n", aDataSzBytes, sizeof(new_sensor_Params_t));
        lRet = CCA_ERROR;
    }
    if (aChnlIdx >= (uint32_t) ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Channel index (%u) out of bounds(>= %u).\n", aChnlIdx, ISPDRV_CAM_PIPE_ID_CNT);
        lRet = CCA_ERROR;
    }

    if(lRet == CCA_SUCCESS)
    {
        SetNewSensorParams(aChnlIdx, (const new_sensor_Params_t*)acpcData);

        // set flag for first actuation call
        spSensorCtx[aChnlIdx].mFirstActuate = 1U;
    } // if all ok so far

    return lRet;
} // Open()

/******************************************************************************/
/**
 * @brief Parse embedded data byte array.
 *
 * @details Invoked by CCA as part of cca_ProcCcf() call, before the control
 * functions are executed.
 * Represents the first step of embedded data decoding.
 * Parses the embedded data array and fills the acpSensPrmEmbd with values as provided
 * directly by the sensor.
 *
 * @param [in] aChnlIdx channel index
 * @param [in] acpcEmbd embedded data byte array as received from the sensor
 * @param [out] acpSensPrmEmbd sensor parameters parsed from embedded data array
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
static cca_Result_t Parse(  uint32_t aChnlIdx,
                            const cca_EmbdData_t* const acpcEmbd,
                            cca_SensPrm_t* const acpSensPrmEmbd)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if (aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Invalid channel index.\n");
        lRet = CCA_ERROR;
    }

    // check embedded data array
    if ((acpcEmbd == NULL) || (acpcEmbd->mpEmbd0 == NULL))
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Null embedded data array.\n");
        lRet = CCA_ERROR;
    }

    // check output structure
    if (acpSensPrmEmbd== NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Null sensor parameter structure.\n");
        lRet = CCA_ERROR;
    }

    if (lRet == CCA_SUCCESS)
    {
        ParseEmdData(acpcEmbd, acpSensPrmEmbd);
    } // if all ok so far

    return lRet;
} // Parse()

/******************************************************************************/
/**
 * @brief Interprets embedded data.
 *
 * @details Invoked by CCA as part of cca_ProcCcf() call, before the control
 * functions are executed.
 * Represents second step in embedded data decoding.
 * Translates the embedded data values into total exposure format needed by control functions.
 *
 * @param [in] aChnlIdx channel index
 * @param [in] acpSensPrmEmbd sensor parameters parsed from embedded data array
 * @param [out] acpSensPrmCcf values from embedded data translated from sensor format (multiple exposures, gains...)
 * into total exposure fitting the control functions (3A) input needs.
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
static cca_Result_t Interpret(uint32_t aChnlIdx,
                                const cca_SensPrm_t* const acpcSensPrmEmbd,
                                cca_SensPrm_t* const acpSensPrmCcf)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if (aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Invalid channel index\n");
        lRet = CCA_ERROR;
    }

    // check input structure
    if (acpcSensPrmEmbd== NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Null sensor parameter structure.\n");
        lRet = CCA_ERROR;
    }

    // check output structure
    if (acpSensPrmCcf == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Null CCF parameter structure.\n");
        lRet = CCA_ERROR;
    }

    if (lRet == CCA_SUCCESS)
    {
        if (InterpretEmdData(aChnlIdx, acpSensPrmCcf, acpcSensPrmEmbd, &spSensorCtx[aChnlIdx]) != CCA_SUCCESS)
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Failed to decode sensor params from embedded parameters\n");
            lRet = CCA_ERROR;
        }
        else
        {
            lRet = CCA_SUCCESS;
        }
    } // if all ok so far

    return lRet;
} // Interpret()

/******************************************************************************/
/**
 * @brief Converts sensor settings from control functions into values applicable to the specific sensor.
 *
 * @details Invoked by CCA as part of cca_ProcCcf() call, after the control
 * functions were executed.
 * Control functions provide new sensor parameters in a sensor abstracted
 * format with single total exposure value. Such value needs to be distributed
 * into different exposures and gains available in the sensor.
 * !Implementation must properly deal with in place operation (acpSensPrmNew == acpSensPrmCcf)!
 *
 * TODO: reimplement to meet new sensor needs
 *
 * @param [in] aChnlIdx channel index
 * @param [out] apcSensPrmNew values to be sent to the sensor
 * @param [in] acpSensPrmCcf parameters in total exposure format from the from control functions (3A)
 *
 * @return CCA_SUCCESS on success
 ******************************************************************************/
static cca_Result_t Actuate(uint32_t aChnlIdx, cca_SensPrm_t* const acpSensPrmNew, cca_SensPrm_t* const acpSensPrmCcf)
{
    cca_Result_t lRet = CCA_SUCCESS;

    if (acpSensPrmNew == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Output sensor parameter structure is NULL (CID %u)\n", aChnlIdx);
        lRet = CCA_ERROR;
    }
    if (acpSensPrmCcf == NULL)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "CCF sensor parameter structure is NULL (CID %u)\n", aChnlIdx);
        lRet = CCA_ERROR;
    }
    if (aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Invalid channel index\n");
        lRet = CCA_ERROR;
    }

    if (lRet == CCA_SUCCESS)
    {
        if (acpSensPrmNew->mExpType == CCA_EXP_NRM)
        {
            DistributeWdr3(aChnlIdx, acpSensPrmNew, acpSensPrmCcf);
        }
        else
        {
            /* Other types of exposures are not supported yet! */
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Unsupported exposure type: %d\n", acpSensPrmNew->mExpType);
            lRet = CCA_ERROR;
        }

        // adjust WB gains. CCF is 8.8 vs 0NEW_SENSOR is 5.10 format
        acpSensPrmNew->mWb.mGains.mBlue  *= 4U;
        acpSensPrmNew->mWb.mGains.mGreen *= 4U;
        acpSensPrmNew->mWb.mGains.mRed   *= 4U;

        if(spSensorCtx[aChnlIdx].mFirstActuate == 1U)
        {
            // make sure initial VS values are <= 4; workaround to avoid corrupted image
            if(acpSensPrmNew->mpExp->mExp > 4U)
            {
                ILOG(ILOG_MOD_CCA, ILOG_LEV_WRN, "Initial ox03c10[%u] VS settings: %u > 4. Forcing to VS = 4 to avoid image artefacts.\n", aChnlIdx, acpSensPrmNew->mpExp[EXP_VSHORT].mExp);
                acpSensPrmNew->mpExp[EXP_VSHORT].mExp = 4U;
            }
            // fix VS only for the first time => initial settings
            spSensorCtx[aChnlIdx].mFirstActuate = 0U;
        }
    } // else from if settings initialized

    return lRet;
} // Actuate()

/******************************************************************************/
/**
 * @brief Translates sensor settings into I2C commands
 *
 * TODO: reimplement to meet new sensor needs
 *
 * @param[in]  aChnlIdx channel index
 * @param [out] acpI2cTab I2C table to be generated
 * @param [in] apcSensPrmNew new settings to be translated into I2C table
 * @param [in] apcSensPrmPrv optional, previous settings to check if not set already, if NULL => update everything
 *
 * @return CCA_SUCCESS on success
 ******************************************************************************/
static cca_Result_t GenI2cTable(uint32_t aChnlIdx, cca_I2cTab_t* const acpI2cTab, const cca_SensPrm_t* apcSensPrmNew, const cca_SensPrm_t* apcSensPrmPrv)
{
    uint8_t lpData[16];
    cca_I2cCmd_t lI2cCmd;
    cca_Result_t lRet = CCA_SUCCESS;

    if (aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Channel index (%u) out of bounds(>= %u).\n", aChnlIdx, ISPDRV_CAM_PIPE_ID_CNT);
        lRet = CCA_ERROR;
    }

    // prefill command structure
    if (lRet == CCA_SUCCESS)
    {
        lI2cCmd.mDevId = spSensorCtx[aChnlIdx].mI2CAddIdx; // sensor address used for table generation
        lI2cCmd.mpData = lpData;
    }

#ifdef USE_GROUP_HOLD
    if (lRet == CCA_SUCCESS)
    {
        // *** GROUP PARAMETER HOLD START
        cca_ConvUint32ToByteArray(0x00U, lI2cCmd.mpData, 1); /* group 0 hold start */
        lI2cCmd.mAddr    = NEW_SENSOR_GROUP_HOLD_REG;
        lI2cCmd.mByteCnt = 1U;
        lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
    }
#endif

    // *** exp time
    // DCG
    if (lRet == CCA_SUCCESS)
    {
        if (!((apcSensPrmPrv != NULL) && (apcSensPrmNew->mpExp[EXP_DCG].mExp == apcSensPrmPrv->mpExp[EXP_DCG].mExp)))
        {
            cca_ConvUint32ToByteArray(apcSensPrmNew->mpExp[EXP_DCG].mExp, lI2cCmd.mpData, 2);
            lI2cCmd.mAddr    = NEW_SENSOR_DCG_EXP_REG;
            lI2cCmd.mByteCnt = 2U;
            lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
        }
    }
    // SPD
    if (lRet == CCA_SUCCESS)
    {
        if (!((apcSensPrmPrv != NULL) && (apcSensPrmNew->mpExp[EXP_SPD].mExp == apcSensPrmPrv->mpExp[EXP_SPD].mExp)))
        {
            cca_ConvUint32ToByteArray(apcSensPrmNew->mpExp[EXP_SPD].mExp, lI2cCmd.mpData, 2);
            lI2cCmd.mAddr    = NEW_SENSOR_SPD_EXP_REG;
            lI2cCmd.mByteCnt = 2U;
            lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
        }
    }
    // VS
    if (lRet == CCA_SUCCESS)
    {
        if (!((apcSensPrmPrv != NULL) && (apcSensPrmNew->mpExp[EXP_VSHORT].mExp == apcSensPrmPrv->mpExp[EXP_VSHORT].mExp)))
        {
            cca_ConvUint32ToByteArray(apcSensPrmNew->mpExp[EXP_VSHORT].mExp, lI2cCmd.mpData, 2);
            lI2cCmd.mAddr    = NEW_SENSOR_VS_EXP_REG;
            lI2cCmd.mByteCnt = 2U;
            lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
        }
    }
    // *** exp gains: HCG/LCG/VS/SPD
    for (uint32_t lIter = (uint32_t)EXP_HCG; lIter < (uint32_t)EXP_CNT; lIter++)
    {
        if (lRet != CCA_SUCCESS)
        {
            break;
        }
        if (apcSensPrmPrv != NULL)
        {
            if ((apcSensPrmNew->mpExp[lIter].mAgain == apcSensPrmPrv->mpExp[lIter].mAgain) && (apcSensPrmNew->mpExp[lIter].mDgain == apcSensPrmPrv->mpExp[lIter].mDgain))
            {
                continue;
            }
        }
        cca_ConvUint32ToByteArray(apcSensPrmNew->mpExp[lIter].mAgain >> 8, lI2cCmd.mpData, 2);
        cca_ConvUint32ToByteArray(apcSensPrmNew->mpExp[lIter].mDgain, &lI2cCmd.mpData[2], 3);
        lI2cCmd.mAddr    = spExpGainAddr[lIter];
        lI2cCmd.mByteCnt = 5U;
        lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
    } // for all exp gains

    // *** WB gains
    if (apcSensPrmNew->mWb.mWbLoc == CCA_WB_SEN)
    {
        // update WB in sensor
        // if previous sensor config not provided
        // or if previous sensor provided and there was some change
        if (!((apcSensPrmPrv != NULL) && (apcSensPrmNew->mWb.mGains.mRed == apcSensPrmPrv->mWb.mGains.mRed) && (apcSensPrmNew->mWb.mGains.mGreen == apcSensPrmPrv->mWb.mGains.mGreen) && (apcSensPrmNew->mWb.mGains.mBlue == apcSensPrmPrv->mWb.mGains.mBlue)))
        {
            for (uint32_t lRegIdx = (uint32_t)EXP_HCG; lRegIdx < (uint32_t)EXP_CNT; lRegIdx++)
            {
                if (lRet != CCA_SUCCESS)
                {
                    break;
                }
                cca_ConvUint32ToByteArray((uint32_t)apcSensPrmNew->mWb.mGains.mBlue, lI2cCmd.mpData, 2U);
                cca_ConvUint32ToByteArray((uint32_t)apcSensPrmNew->mWb.mGains.mGreen, lI2cCmd.mpData + 2, 2U);
                cca_ConvUint32ToByteArray((uint32_t)apcSensPrmNew->mWb.mGains.mGreen, lI2cCmd.mpData + 4, 2U); // green applied twice (GR&GB)
                cca_ConvUint32ToByteArray((uint32_t)apcSensPrmNew->mWb.mGains.mRed, lI2cCmd.mpData + 6, 2U);
                lI2cCmd.mAddr    = spWbGainsAddr[lRegIdx];
                lI2cCmd.mByteCnt = 8U;
                lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
            } // for all exposures
        } // if some change in WB gains
    } // if WB in sensor and all ok so far

#ifdef USE_GROUP_HOLD
    // *** GROUP PARAMETER HOLD END
    if (lRet == CCA_SUCCESS)
    {
        cca_ConvUint32ToByteArray(0x10U, lI2cCmd.mpData, 1); /* group 0 hold end */
        lI2cCmd.mAddr    = NEW_SENSOR_GROUP_HOLD_REG;
        lI2cCmd.mByteCnt = 1U;
        lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
    }

    if (lRet == CCA_SUCCESS)
    {
        cca_ConvUint32ToByteArray(0x00U, lI2cCmd.mpData, 1); /* manual launch on */
        lI2cCmd.mAddr    = NEW_SENSOR_GROUP_LAUNCH_REG;
        lI2cCmd.mByteCnt = 1U;
        lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
    }

    if (lRet == CCA_SUCCESS)
    {
        cca_ConvUint32ToByteArray(0xE0U, lI2cCmd.mpData, 1); /* immediate group 0 launch */
        lI2cCmd.mAddr    = NEW_SENSOR_GROUP_HOLD_REG;
        lI2cCmd.mByteCnt = 1U;
        lRet             = cca_AddI2cWrite(acpI2cTab, &lI2cCmd);
    }
#endif

#ifdef DBG_NEW_SENSOR_AE_DISTRIBUTION
    ILOG(ILOG_MOD_CCA, ILOG_LEV_NFO, "->%u, %u, %f, %f, %f, %f, %f, %f\n", apcSensPrmNew->mpExp[EXP_DCG].mExp, apcSensPrmNew->mpExp[EXP_VSHORT].mExp, apcSensPrmNew->mpExp[EXP_HCG].mAgain / 65536., apcSensPrmNew->mpExp[EXP_HCG].mDgain / 65536., apcSensPrmNew->mpExp[EXP_LCG].mAgain / 65536., apcSensPrmNew->mpExp[EXP_LCG].mDgain / 65536., apcSensPrmNew->mpExp[EXP_VSHORT].mAgain / 65536., apcSensPrmNew->mpExp[EXP_VSHORT].mDgain / 65536.);
#endif
    return CCA_SUCCESS;
} // GenI2cTable()

/******************************************************************************/
/**
 * @brief Compares two sensor parameter structures and decides if matching or not
 *
 * @details Used by CameraControlLibrary (CCL).
 * Input sensor parameters are in a sensor specific form (e.g. as parsed from embedded data) not generalized to CCF input yet through the interpretation step.
 * Intended as a tool for ISP configuration history look up.
 * For each invokation the 3A algorithms generate pair of parameter update
 * structures: <for the sensor; for the ISP>. In many cases it is required to apply
 * the specific ISP settings to RAW frame data processing that was captured using
 * the corresponding ISP parameters from the same pair. Otherwise some image artefacts
 * may be observed at the ISP output.
 * This means that each new parameter update pair is added to a list. Once new RAW
 * frame is captured the frame parameters are extracted from the frame embedded data
 * and the list is seareched for a matching entry. If match was confirmed using this
 * function the ISP settings from matching pair are used for the RAW frame processing.
 *
 * TODO: reimplement to meet new sensor needs
 *
 * @param [in] acpcPrmA first set of sensor parameters
 * @param [in] acpcPrmB second set of parameters
 *
 * @return 1 if parameter structures passed matching criteria, 0 if not matching
 ******************************************************************************/
static uint32_t MatchSensPrm(const cca_SensPrm_t* const acpcPrmA, const cca_SensPrm_t* const acpcPrmB)
{
    uint32_t lRet;

    if((acpcPrmA == NULL) || (acpcPrmA == NULL))
    {
        lRet = 0U;
    } // if NULL input
    else
    {
        const int32_t lcExpCnt = 4;
        lRet = 1U;
        // in case of ox03c10 only the exposure and analog gains are checked
        for(int32_t lIter = 0; lIter < lcExpCnt; lIter++)
        {
            if(acpcPrmA->mpExp[lIter].mExp != acpcPrmB->mpExp[lIter].mExp)
            {
                lRet = 0U;
                break;
            }
            if(acpcPrmA->mpExp[lIter].mAgain != acpcPrmB->mpExp[lIter].mAgain)
            {
                lRet = 0U;
                break;
            }
            if(acpcPrmA->mpExp[lIter].mDgain != acpcPrmB->mpExp[lIter].mDgain)
            {
                lRet = 0U;
                break;
            }
        } // for all exposures
    } // else if NULL input

    return lRet;
} // MatchSensPrm()

/******************************************************************************/
static inline uint32_t getMaxExpLines(uint32_t aChnlIdx)
{
    switch (spSensorCtx[aChnlIdx].mFps)
    {
        case 25:
        case 30:
        {
            return MAX_EXP_LINES;
        }
        case 60:
        {
            return MAX_EXP_LINES_60;
        }
        default:
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Incorectly selected fps (%d) on channel %d. Set default 30 fps.\n", spSensorCtx[aChnlIdx].mFps, aChnlIdx);
        }
    }

    return MAX_EXP_LINES;
}

/******************************************************************************/
static inline uint32_t getRowTimeNs(uint32_t aChnlIdx)
{
    switch (spSensorCtx[aChnlIdx].mFps)
    {
        case 30:
        {
            return ROW_TIME_NS;
        }
        case 60:
        {
            return ROW_TIME_NS_60;
        }
        default:
        {
            ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Incorectly selected fps (%d) on channel %d. Set default 30 fps.\n", spSensorCtx[aChnlIdx].mFps, aChnlIdx);
        }
    }

    return ROW_TIME_NS;
}

/******************************************************************************/
/**
 * @brief Terminates interaction with the sensor support code
 *
 * @details Invoked by CCA once no more calls to any other sensor ops are expected.
 * Gives ability to cleanup the sensor support internals.
 *
 * TODO: reimplement to meet new sensor needs
 *
 * @param [in] aChnlIdx channel index
 *
 * @return CCA_SUCCESS on success
 ******************************************************************************/
static cca_Result_t Close(uint32_t aChnlIdx)
{
    cca_Result_t lRet;
    if (aChnlIdx >= (uint32_t)ISPDRV_CAM_PIPE_ID_CNT)
    {
        ILOG(ILOG_MOD_CCA, ILOG_LEV_ERR, "Channel index (%u) out of bounds(< %u).\n", aChnlIdx, ISPDRV_CAM_PIPE_ID_CNT);
        lRet = CCA_ERROR;
    }
    else
    {
        spSensorCtx[aChnlIdx].mConversionRatio = 0U;
        lRet = CCA_SUCCESS;
    }
    return lRet;
} // Close()

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

///< TODO: update to support the new sensor
void new_sensor_GetCcaOps(cca_CamOps_t* const acpOps)
{
    if (acpOps != NULL)
    {
        acpOps->mOpen = Open;
        acpOps->mConfig = Open; // could be the same as open
        acpOps->mParseEmbd = Parse;
        acpOps->mInterpretEmbd = Interpret;
        acpOps->mActuate = Actuate;
        acpOps->mGetI2cTab = GenI2cTable;
        acpOps->mMatchPrm = MatchSensPrm;
        acpOps->mClose = Close;
    }
} // new_sensor_GetCcaOps()
