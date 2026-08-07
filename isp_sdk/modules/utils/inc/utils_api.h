/*******************************************************************************
 * Copyright 2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_API_H
#define UTILS_API_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  utils_api.h
 * @brief Brief provides public API for the utility library
 *
 * @details Implements:
 *          - image buffer container
 *          - embedded data container
 *          - ini, yml, pgm and raw reader/writer
 *******************************************************************************/
#include <stdbool.h>

#include "isp_task.h"
#include "utils_container.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

typedef struct
{
    char* mpCameraType;
    char* mpYamlPath;

    char* mpDcgPath;
    char* mpVsPath;
    char* mpMMS_DTP;
    char* mpMMS_UgcIp;
    char* mpVigLut;
    char* mpLtmLut;
    char* mpGtmLut;

    char* mpCCF_TDS;
    char* mpCCF_TDS_METADATA;

    // dewarping
    char* mpDewarpMap; ///< path to the dewarp map binary file
    uint32_t mDewarpMapFormat; ///< format of dewarp map binary: 0 = point, 1 = delta, 2 = delta delta
    uint32_t mDewarpArbStartX; ///< delta delta dewarping parameter
    uint32_t mDewarpArbStartY; ///< delta delta dewarping parameter
    uint32_t mDewarpArbDeltaXX; ///< delta delta dewarping parameter
    uint32_t mDewarpArbDeltaXY; ///< delta delta dewarping parameter
    uint32_t mDewarpArbDeltaYX; ///< delta delta dewarping parameter
    uint32_t mDewarpArbDeltaYY; ///< delta delta dewarping parameter

    uint32_t mStartIdx; ///< first file to process
    uint32_t mStopIdx; ///< last file to process
    uint32_t mIgnMax; ///< maximum number of allowed file load failures
    uint32_t mIgnCnt; ///< number of already ignored file load failures

    uint32_t mFileFps; ///< simulated frames per second for file based input, 0 => as fast as possible
    uint32_t mFileRepeat; ///< 1 to repeat input files in a loop, 0 to stop after last file index
    uint32_t mFilePreloadOnInit; ///< 1 to preload input files on initialization, 0 to load on demand
    uint32_t mBfrCnt; ///< number of buffer sets for frame processing pipeline

    uint32_t mEmbdCnt0; ///< # of embedded lines before image data
    uint32_t mEmbdCnt1; ///< # of embedded lines after image data
    uint32_t mEmbdBypass; ///< Enable embedded data bypass by setting to 1 or higher (value = frame delay count). Set to 0 to disable.

    uint32_t mCcaOn; ///< 0 if CCA disabled, >0 if CCA enabled
    uint32_t mMMS_TtOn; ///< 0 if TT is not to be used, 1 if TT to be used
    uint32_t mMMS_UgcPort;
} utils_IniCfg_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/*******************************************************************************/
/**
 * @brief Loads configuration for the imx9x
 *
 * @param apPath path to the ini file
 * @param apIniCfg ini contents
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadCfg(const char* apPath, utils_IniCfg_t* apIniCfg, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves imx9x register configuration to YAML file
 *
 * @param apPath path to the YAML file to be written into
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveRegs2Yaml(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Loads vignetting LUT to file
 *
 * @param apPath path to the vignetting file to be read
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadVigLut(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves vignetting LUT to file
 *
 * @param apPath path to the vignetting file to be written into
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveVigLut(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Loads local tone mapping LUT to file
 *
 * @param apPath path to the LTM file to be read
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadLTMLut(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves local tone mapping LUT to file
 *
 * @param apPath path to the LTM file to be written into
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveLTMLut(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Loads global tone mapping LUT to file
 *
 * @param apPath path to the GTM file to be read
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadGTMLut(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves global tone mapping LUT to file
 *
 * @param apPath path to the GTM file to be written into
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveGTMLut(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves statistic memories to file
 *
 * @param apPath path to the statistics file to be written
 * @param apCtx registers and internal memories content
 *
 * @return  true if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveStatsRaw(const char* apcPath, const isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves statistic memories to file (formatted)
 *
 * @param apPath path to the statistics file to be written
 * @param apCtx registers and internal memories content
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveStatsFormatted(const char* apcPath, const isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Saves statistic from aliasing region to file
 *
 * @param apPath path to the aliased statistics file to be written
 * @param apCtx registers and internal memories content
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveAlias(const char* apcPath, isp_CtxHw_t* apCtx);

/*******************************************************************************/
/**
 * @brief Destroys cloned stings in the ini configuration structure.
 *
 * @param apIniCfg ini contents
 *******************************************************************************/
void utils_DestroyCfg(utils_IniCfg_t* apIniCfg);

/**
 * @brief This is just C wrapper for C++ version of utils_LoadPgmEmbd
 *
 * @param apcName
 * @param arImgBuf
 * @param arEmdData
 * @param aSwapBytes
 * @return int32_t
 */
int32_t utils_LoadPgmEmbdCwrap(const char* apcName, utils_ImgBuf_t *arImgBuf, utils_EmbdData_t *arEmdData, const bool aSwapBytes);
/*******************************************************************************/
/**
 * @brief Computes the mean value of raw image pixels
 *
 * @param acpRawPixels pointer to raw pixel data
 * @param acSizeBytes size of the raw pixel data in bytes
 * @param aSensId sensor ID
 * @return mean value of the raw image pixels
 *******************************************************************************/
double utils_ComputeRawImgMean(const uint16_t* acpRawPixels, const uint32_t acSizeBytes, uint32_t aSensId);

/*******************************************************************************/
/**
 * @brief Get AEC histogram data from ISP context.
 *
 * Extracts 6 histograms from ISP hardware context.
 * Returns H0-H3 from HISTOGRAM_STATISTICS and IR H0-H1 from RGBIR_HISTOGRAM.
 *
 * @param[in]  acpCtx       Pointer to ISP hardware context.
 * @param[out] apHistograms Pointer to histogram buffers.
 *                          [0]=H0, [1]=H1, [2]=H2, [3]=H3, [4]=IR_H0, [5]=IR_H1.
 * @param[in]  acBinCnt     Number of bins per histogram.
 *
 * @return 0 on success, -1 on error.
 *******************************************************************************/
int32_t utils_GetAecHist(const isp_CtxHw_t* acpCtx,
                         uint32_t* apHistograms,
                         const uint32_t acBinCnt);

#if defined(OCV_ON)
//These functions are used only if the Histogram visualization is enabled
/*******************************************************************************/
/**
 * @brief Initializes AEC histogram
 *
 * @param acCamCmt camera count
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_InitAecHist(const uint16_t acCamCmt);

/*******************************************************************************/
/**
 * @brief Display AEC histograms in interactive window.
 *
 * Shows multiple histograms in grid layout.
 * Supports linear/log scale toggle and multi-camera navigation.
 *
 * @param[in] apHistograms  Pointer to histogram data.
 * @param[in] acHistCount   Number of histograms to display.
 * @param[in] acBinCount    Number of bins per histogram.
 * @param[in] acCamId       Camera ID for display filtering.
 *******************************************************************************/
void utils_ShowAecHist(uint32_t* apHistograms,
                       const uint32_t acHistCount,
                       const uint32_t acBinCount,
                       const uint8_t acCamId);



#endif // #if defined(OCV_ON)

#ifdef __cplusplus
// NOTE: functions below are using references, therefore they can only by compiled as c++

/******************************************************************************
 *
 * @brief Loads PGM from file
 *
 * @param apcName pgm file name
 * @param arImgBuf preallocated image buffer
 * @param arEmdData preallocated embedded data buffer
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadPgmEmbd(const char* apcName, utils_ImgBuf_t& arImgBuf, utils_EmbdData_t& arEmdData, const bool aSwapBytes);

/*******************************************************************************/
/**
 * @brief Loads input image data
 * @details Assumes no embedded data inside the pgm
 *
 * @param arCtx demo context
 * @param aIoIdx index of the buffer to load data into
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_LoadPgm(const char* apcName, utils_ImgBuf_t& arImgBuf, const bool aSwapBytes);

/*******************************************************************************/
/**
 * @brief Saves PGM from file
 *
 * @param apcName pgm file name
 * @param arImgBuf image buffer to save
 * @param apcCustomHeader pointer to custom PGM header
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SavePgm(  const char*     apcName,
                        utils_ImgBuf_t& arImgBuf,
                        const char*     apcCustomHeader);

/*******************************************************************************/
/**
 * @brief Saves RAW from file
 *
 * @param apcName file name
 * @param arImgBuf image buffer to save
 *
 * @return  0 if all ok, -1 otherwise
 *******************************************************************************/
int32_t utils_SaveRaw(const char* apcName, utils_ImgBuf_t& arImgBuf);

#endif // __cplusplus

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UTILS_API_H */
