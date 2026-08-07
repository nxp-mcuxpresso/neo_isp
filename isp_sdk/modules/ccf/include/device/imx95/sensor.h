 /*******************************************************************************
 * Copyright 2020-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/**
 * @file     sensor.h
 * @brief    Type definition of sensor related data.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "drc_defines.h"
#include <stdint.h>

/**
 * @brief   Camera related data
 *******************************************************************************/
typedef struct
{
    // AEC related parameters
    uint32_t    mExpCcfIn;              /**< exposure time in us; value from CAM to CCF */
    uint32_t    mDGainFixCcfIn;         /**< Digital gain UQ16.16; value from CAM to CCF */
    uint32_t    mAGainFixCcfIn;         /**< Analog gain UQ16.16; value from CAM to CCF */

    uint32_t    mSensID;                /**< identification of used sensor */

    uint32_t    mExpCcfOut;             /**< exposure time in us; value from CCF to CAM */
    uint32_t    mDGainFixCcfOut;        /**< Digital gain UQ16.16; value from CCF to CAM */
    uint32_t    mAGainFixCcfOut;        /**< Analog gain UQ16.16; value from CCF to CAM */

    // histograms and related data
    uint32_t    mHistLow[STAT_HIST_BINS];     /**< histogram long array */
    uint32_t    mHistMid[STAT_HIST_BINS];     /**< histogram short array */
    uint32_t    mHistHigh[STAT_HIST_BINS];    /**< histogram very short array */

    uint32_t    mHistScLow;            /**< scales used for relative picture average calculation */
    uint32_t    mHistScMid;
    uint32_t    mHistScHigh;

    uint32_t    mHistSum;              /**< The sum of all the bins in a single array (histogram) */
    uint16_t    mHistBins;             /**< The number of bins in the histogram */

    // WB related variables
    uint16_t mSensBlueGain;             /**< sensor WB gains in UQ8.8; CCF -> CAM */
    uint16_t mSensGreenGain;
    uint16_t mSensRedGain;
    /*
     - location where WB is done
     - ISP = 0; sensor = 1
     - matches the dataype defined in cca.h (cca_WbLoc_t)
    */
    uint8_t  mWbLoc;                    /**< white balancing mode */
    // input WB gains get from sensor embedded data
    uint16_t mSensGainRIn;              /**< WB gains from sensor in UQ8.8; CAM -> CCF */
    uint16_t mSensGainGIn;
    uint16_t mSensGainBIn;

} ccf_SensData_t;

#endif /* SENSOR_H */
