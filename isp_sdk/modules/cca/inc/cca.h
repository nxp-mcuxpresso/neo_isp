/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCA_H
#define CCA_H

/******************************************************************************/
/**
 * @file cca.h
 * @brief Camera abstraction library public APIs
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include <stddef.h>

#include "isp_platform.h"
#include "cgd_types.h"
#include "cca_iobuft.h"

/** @mainpage Overview
 * The CCA abstracts the usage of specific Camera Control Functions (CCF) also knows as 3A (Auto exposure, Auto white
 * balancing, Auto focus) and provides a generic API to operate it.
 *
 * @subsection feature_support Feature support
 * The CCA supports only a single specific ISP HW - in this case the NEO B from i.mx95. But it gives
 * facilities to introduce support for different camera control fuctions and sensor HW.
 *
 * @subsection int_struct Internal structure
 * The CCA library has the following internal modules:
 *  - @ref cca_pub_api = function calls and data types that are visible outside of the library
 *  - @ref cca_core_func = implementation of the public API and other CCF and sensor independent behavior
 *  - CCF & Sensor interfaces
 *      - CCA <-> CCF = functions and datatypes to be used to introduce support for particular CCF solution
 *          - \ref cca_CcfOps_t
 *      - CCA <-> Sensor = functions and datatypes to be used to introduce support for particular sensor HW
 *          - \ref cca_CamOps_t
 *          - \ref cca_I2cTab_t
 *          - \ref cca_EmbdData_t
 *          - \ref cca_SensPrm_t
 *      - integration code is also expected to access directly the CCA internal context structure from the CCA core module
 *          - \ref cca_Ctx_t \ref gCcaCtx;
 *  - integration code for particular CCF
 *      - implements the prescribed CCA <-> CCF interface.
 *      - @ref cca_ug_int and @ref cca_ugc_int
 *  - integration code for particular sensor HW
 *      - implements the prescribed CCA <-> Sensor interface.
 *      - @ref cca_ovx3c_sens = ox03c10 sensor support code
 *
 * At runtime the CCA invokes Sensor and CCF integration provided functions and keeps internal structures to serve as data
 * exchange points between the specific Sensor and CCF functionalities.
 *
 */

/** @addtogroup cca_pub_api CCA (CameraControlAbstraction) public API
 * The CCA serves a toolbox for higher SW layers to run 3A algorithms. Internally it supports handling of multiple streams
 * and different sensor HW.
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                    ENUMS
==============================================================================*/
/******************************************************************************/
/**
 * @brief   ISP control service operation result
 *******************************************************************************/
typedef enum
{
    CCA_SUCCESS = 0x00U, /**< operation succeeded */
    CCA_ERROR   = 0x01U, /**< operation failed */
} cca_Result_t;

/******************************************************************************/
/**
 * @brief   Toplevel unnamed enums.
 *******************************************************************************/
enum
{
    CCA_HDR_EXP_CNT = 4U /**< number of supported HDR exposures in sensor */
};

/******************************************************************************/
/**
 * @brief   Exposure types
 *******************************************************************************/
typedef enum
{
    CCA_EXP_NRM = 0U, /**< normal exposure with ratios used */
    CCA_EXP_HDR = 1U, /**< HDR exposure used */
} cca_ExpType_t;

/******************************************************************************/
/**
 * @brief   WhiteBalance locations
 *******************************************************************************/
typedef enum
{
    CCA_WB_ISP = 0U, /**< white balancing applied in ISP */
    CCA_WB_SEN = 1U, /**< white balancing applied in sensor */
} cca_WbLoc_t;

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/******************************************************************************/
/**
 * @brief embedded data container
 *******************************************************************************/
typedef struct
{
    uint32_t mFrmSeqNum; ///< frame sequence number
    uint8_t* mpEmbd0; ///< pointer to embedded lines before image data
    uint32_t mEmbd0Sz; ///< number of bytes in the Embd0 array
    uint8_t* mpEmbd1; ///< pointer to embedded lines after image data
    uint32_t mEmbd1Sz; ///< number of bytes in the Embd1 array
} cca_EmbdData_t;

/******************************************************************************/
/**
 * @brief i2c table container
 *******************************************************************************/
typedef struct
{
    cgd_Cmd_t* mpTable; ///< pointer to cgd command table
    uint32_t mSz; ///< number of bytes available in the table
    uint32_t mUsedBytes; ///< number of bytes used from the table
} cca_I2cTab_t;

/******************************************************************************/
/**
 * @brief   Exposure gains.
 *******************************************************************************/
typedef struct
{
    uint32_t mExp; /**< exposure time in us */
    uint32_t mDgain; /**< Digital gain */
    uint32_t mAgain; /**< Analog gain */
} cca_Exp_t;

/******************************************************************************/
/**
 * @brief   White balancing color gains.
 *******************************************************************************/
typedef struct
{
    uint16_t mRed;
    uint16_t mGreen;
    uint16_t mBlue;
} cca_WbGains_t;

/******************************************************************************/
/**
 * @brief   White balancing color offfsets.
 *******************************************************************************/
typedef struct
{
    int16_t mRed;
    int16_t mGreen;
    int16_t mBlue;
} cca_WbOffsets_t;

/******************************************************************************/
/**
 * @brief   White balancing configuration.
 *******************************************************************************/
typedef struct
{
    cca_WbGains_t mGains; /**< wb gains */
    cca_WbOffsets_t mOffsets; /**< wb offsets */
    uint16_t mColorTemp; /**< color temperature */
    cca_WbLoc_t mWbLoc; /**< location of wb application*/
} cca_Wb_t;

/******************************************************************************/
/**
 * @brief   Contains configuration of a single sensor.
 *******************************************************************************/
typedef struct
{
    cca_ExpType_t mExpType; /**< either normal or HDR */
    cca_Exp_t mpExp[CCA_HDR_EXP_CNT]; /**< sensor exposure settings */
    uint32_t mL2sRatio; /**< HDR exposure ratio if HDR used */
    uint32_t mL2vsRatio; /**< HDR exposure ratio if HDR used */
    int32_t mSensorTemp; /**< sensor provided temperature */
    cca_Wb_t mWb; /**< sensor white balance settings */
    uint32_t mFrameCount; /**< sensor based frame counter */
    int32_t mLensPos; /**< AF lens position = precise physical location of the lens along its focus travel if min and max are correctly tuned. */
    bool mDataInvalid; /** Indicates that received data are invalid (corrupted, not available, incomplete,etc. ) */
} cca_SensPrm_t;

/******************************************************************************/
/**
 * @brief   Pointer to sensor specific Open and Config function.
 *
 * @details Initializes or configures particular implementation of sensor abstraction layer.
 *
 * @param[in]  aChnlIdx channel index to open/configure
 * @param[in]  acpcData pointer to arbitrary data structure with parameters
 * @param[in]  aDataSzBytes size of the provided data pointer
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CamOpenConfigFunc_t)(uint32_t aChnlIdx, const void* const acpcData, uint32_t aDataSzBytes);

/******************************************************************************/
/**
 * @brief Parse embedded data byte array.
 *
 * @details Represents the first step of embedded data decoding.
 * Parses the embedded data array and fills the acpSensPrmEmbd with values
 * as provided directly by the sensor.
 * Invoked by CCA as part of \ref cca_Embd2Prm call.
 *
 * @param [in] aChnlIdx channel index
 * @param [in] acpcEmbd embedded data byte array as received from the sensor
 * @param [out] acpSensPrmEmbd sensor parameters parsed from embedded data array
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CamParseEmbdFunc_t)(uint32_t aChnlIdx,
                                             const cca_EmbdData_t* const acpcEmbd,
                                             cca_SensPrm_t* const acpSensPrmEmbd);

/******************************************************************************/
/**
 * @brief Interprets embedded data.
 *
 * @details Represents second step in embedded data decoding.
 * Translates the embedded data values into total exposure format needed by control functions.
 * Invoked by CCA as part of \ref cca_ProcCcf call, before the control functions are executed.
 *
 * @param [in] aChnlIdx channel index
 * @param [in] acpcSensPrmEmbd sensor parameters parsed from embedded data array
 * @param [out] acpSensPrmCcf values from embedded data translated from sensor format (multiple exposures, gains...)
 * into total exposure fitting the control functions (3A) input needs.
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CamInterpretEmbdFunc_t)(uint32_t aChnlIdx,
                                             const cca_SensPrm_t* const acpcSensPrmEmbd,
                                             cca_SensPrm_t* const acpSensPrmCcf);

/******************************************************************************/
/**
 * @brief   Pointer to sensor specific Actuate function.
 *
 * @details Creates new sensor parameters from sensor settings generated by ctl func.
 * !Implementation must properly deal with in place operation (acpSensPrmNew == acpSensPrmCcf!
 *
 * @param[in]  aChnlIdx channel index to open/configure
 * @param[out]  acpSensPrmNew sensor parameter structure with new values in distributed (exposure, gains) format
 * @param[in]  acpSensPrmCcf sensor parameter structure with new values from CCF in total exposure format
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CamActuateFunc_t)(uint32_t aChnlIdx, cca_SensPrm_t* const acpSensPrmNew, cca_SensPrm_t* const acpSensPrmCcf);

/******************************************************************************/
/**
 * @brief   Pointer to sensor specific Close function.
 *
 * @details Deintializes particular implementation of sensor abstraction layer.
 *
 * @param[in]  aChnlIdx channel index to close
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CamCloseFunc_t)(uint32_t aChnlIdx);

/******************************************************************************/
/**
 * @brief   Pointer to sensor specific I2C table generator function.
 *
 * @details Creates I2C table from sensor parameter structure (distributed format). Not relevant for V4L2 sensor driver.
 *
 * @param[in]  aChnlIdx channel index
 * @param[out] acpI2cTab container for generated I2C commands
 * @param[in] apcSensPrmNew new parameters to translate into the I2C table
 * @param[in] apcSensPrmPrv last set of paremeters confirmed to be configured (identical values will be skipped)
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_GetI2cTabFunc_t)(uint32_t aChnlIdx, cca_I2cTab_t* const acpI2cTab, const cca_SensPrm_t* apcSensPrmNew, const cca_SensPrm_t* apcSensPrmPrv);

/******************************************************************************/
/**
 * @brief   Pointer to sensor specific sensor parameter matching function.
 *
 * Function compares two input parameter structures and decides if this pair can be considered to match or not.
 * Based on the sensor parameter match a proper corresponding ISP parameter set can be identified.
 *
 * @param acpcPrmA sensor parameter structure A
 * @param acpcPrmB sensor parameter structure B
 *
 * @return 0 if context not matching or NULL, >0 otherwise.
 *******************************************************************************/
typedef uint32_t (*cca_CamMatchPrm_t)(const cca_SensPrm_t* const acpcPrmA, const cca_SensPrm_t* const acpcPrmB);

/******************************************************************************/
/**
 * @brief   Pointer to control functions specific Open function.
 *
 * Initializes particular implementation of control functions.
 * @return CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CcfOpenFunc_t)(void);

/******************************************************************************/
/**
 * @brief   Pointer to control functions specific Process function.
 *
 * Invokes single processing step for particular control functions.
 * @return CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CcfGetInitFunc_t)(void);

/******************************************************************************/
/**
 * @brief   Pointer to control functions specific Process function.
 *
 * Invokes single processing step for particular control functions.
 * @return CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CcfProcessFunc_t)(void);

/******************************************************************************/
/**
 * @brief   Pointer to control functions specific Close function.
 *
 * Deinitializes use of particular control functions.
 * @return CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
typedef cca_Result_t (*cca_CcfCloseFunc_t)(void);

/******************************************************************************/
/**
 * @brief   Sensor specific operations
 *
 * @details To be filled with sensor specific function pointers.
 *******************************************************************************/
typedef struct
{
    cca_CamOpenConfigFunc_t mOpen; /**< sensor support initialization */
    cca_CamOpenConfigFunc_t mConfig; /**< configure sensor support*/
    cca_CamParseEmbdFunc_t mParseEmbd; /**< parses sensor embedded data array to CCA data structure */
    cca_CamInterpretEmbdFunc_t mInterpretEmbd; /**< translates sensor embedded data to CCF expected format */
    cca_CamActuateFunc_t mActuate; /**< converts ctlfunc results to new sensor parameters */
    cca_GetI2cTabFunc_t mGetI2cTab; /**< converts sensor parameters to i2c table */
    cca_CamMatchPrm_t mMatchPrm; /**< compares two sensor parameters structs and decides if matching or not */
    cca_CamCloseFunc_t mClose; /**< sensor de-initialization */
} cca_CamOps_t;

/******************************************************************************/
/**
 * @brief   Control functions operations
 *
 * @details To be filled with control functions specific function pointers.
 *******************************************************************************/
typedef struct
{
    cca_CcfOpenFunc_t mOpen; /**< CCF initialization */
    cca_CcfGetInitFunc_t mGetInit; /**< provides initial camera settings */
    cca_CcfProcessFunc_t mProcess; /**< generates new camera settings based on statistics */
    cca_CcfCloseFunc_t mClose; /**< CCF termination */
} cca_CcfOps_t;

/******************************************************************************/
/**
 * @brief   Control functions configuration structure
 *******************************************************************************/
typedef struct
{
    cca_CcfOps_t mOps; /**< control functions operations */
    void* mpParams; /**< control functions specific parameters */
    size_t mParamSize; /**< size of the mpParams structure */
} cca_CcfCfg_t;

/******************************************************************************/
/**
 * @brief   Single camera initialization configuration.
 *******************************************************************************/
typedef struct
{
    uint32_t mEnabled; /**< set 1 if camera channel is used, 0 otherwise */
    uint32_t mSensorId; /**< type of the sensor, e.g. ovx3c */
    uint32_t mModeId; /**< operation mode of the sensor */
    const isp_CtxHw_t *mcpExtDef; /**< default ISP values - external origin*/
    cca_CamOps_t mCamOps; /**< sensor specific operations, each sensor can be different */
    void* mpParams; /**< camera specific parameters */
    size_t mParamSize; /**< size of the mpParams structure */
    uint32_t mEmbdBypass; /**< Enable embedded data bypass by setting to 1 or higher (value = frame delay count). Set to 0 to disable. */
} cca_CamInit_t;

/******************************************************************************/
/**
 * @brief   Main configuration structure of the camera abstraction library.
 *
 * @details To be used as parameter to cca_Open().
 *******************************************************************************/
typedef struct
{
    ///< NOTE: only a single CCF type allowed now per camabs instance.
    cca_CcfCfg_t mCcfCfg; /**< control functions specific configuration*/
    cca_CamInit_t mpCam[ISPDRV_CAM_PIPE_ID_CNT]; /**< camera channels */
} cca_Cfg_t;

/******************************************************************************/
/**
* @brief   image buffer container
*
*******************************************************************************/
typedef struct
{
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mBpp;
    uint16_t mChan;
    uint32_t mSizeBytes;
    uint32_t* mpData;
    uint64_t mHwAddr;
    uint32_t mIdx;
    uint64_t mTimeStamp;
} cca_ImgBuf_t;

/******************************************************************************/
/**
 * @brief   Data structures needed for control functions processing.
 *
 * @details Includes frame ID and timestamp, sensor embedded data, ISP context, new sensor paramters and IO buffers.
 *******************************************************************************/
typedef struct
{
    // ccf required input and output data structures
    const cca_SensPrm_t* mpEmbdPrm; /**< optional input data => parsed sensor embedded data. If NULL, no sensor cfg will be passed to CCF. */
    isp_Ctx_t* mpIspCtxIn; /**< input/output data => statistics, required field, will be used also for output if no Out pointer provided */
    isp_Ctx_t* mpIspCtxOut; /**< optional output data => new config, if NULL the mpIspCtxIn pointer will be used as output */
    cca_SensPrm_t* mpSensPrmNew; /**< output data => new sensor properties to be set, output only */

    // optional tuning tool shared data
    /**< optional input data => input and output image buffers. Shared with tuning tool. */
    const cca_ImgBuf_t* mppcIoBufs[CCA_IOBUFT_CNT];
    /**< optional input data => embedded data byte array. Shared with tuning tool. */
    const cca_EmbdData_t* mpcEmbdData;
} cca_CcfIo_t;

/******************************************************************************/
/**
 * @brief   Data structures needed for control functions processing.
 *
 * @details Includes frame ID and timestamp, sensor embedded data and ISP context.
 *******************************************************************************/
typedef struct
{
    cca_CcfIo_t* mppCcfIo[ISPDRV_CAM_PIPE_ID_CNT]; /**< one pointer to io data per CID*/
} cca_CcfIoArr_t;

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Initialize interaction with camera abstraction library.
 *
 * @details Generates initial configuration (isp context and sensor I2C table),
 * which can be requested through cca_Get() calls.
 *
 * @param[in]  acpCfg  initialization parameters
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_Open(cca_Cfg_t* apcCfg);

/******************************************************************************/
/**
 * @brief Allows to pass parameters to sensor support code at specific channel index.
 *
 * @details internaly calls the Config() API cca_CamOps_t structure
 *
 * @param[in]  aChnlIdx channel index to open/configure
 * @param[in]  acpcData pointer to arbitrary data structure with parameters
 * @param[in]  aDataSzBytes size of the provided data pointer
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_ConfigCam(uint32_t aChnlIdx, const void* const acpcData, uint32_t aDataSzBytes);

/******************************************************************************/
/**
 * @brief Gets initial settings for requested channels.
 *
 * @details Provides initial camera configuration (isp context and sensor I2C table).
 *
 * @param[in,out] apCcfIoArr array of input/output data for the control functions operation
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_GetIniSet(cca_CcfIoArr_t* apCcfIoArr);

/******************************************************************************/
/**
 * @brief Triggers control functions processing.
 *
 * @details Provides configuration (isp context and sensor I2C table) based on
 *          sensor embedded data and isp context.
 *
 * @param[in,out] apCcfIoArr array of input/output data for the control functions operation
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_ProcCcf(cca_CcfIoArr_t* apCcfIoArr);

/******************************************************************************/
/**
 * @brief Parses sensor embedded data into sensor parameters
 *
 * Internally invokes ParseEmbd() function from channel related sensor support implementation.
 *
 * @param[in] aChnlIdx index of coresponding sensor channel
 * @param[in] acpcEmbd embedded data container to parse
 * @param[out] acpSensPrmEmbd container for parsed sensor paremters
 *
 * @return CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_Embd2Prm(uint32_t aChnlIdx, const cca_EmbdData_t* const acpcEmbd, cca_SensPrm_t* const acpSensPrmEmbd);

/******************************************************************************/
/**
 * @brief   Sensor specific parameter matching function.
 *
 * Function compares two input parameter structures and decides if this pair can be considered to match or not.
 * CCF provides recommended settings for ISP and sensor in pairs. To avoid artefacts these settings should be applied in sync.
 * Due to delay in sensor paremeter application (about 2 frames) compared to ISP parameters (can be applied immediately) it
 * is reasonable to build a queue of settings and wait with ISP parameters till new frame data with the right sensor
 * parameters have been captured (understood based on embedded data content). Based on the sensor parameter match a proper
 * corresponding ISP paramter set can be identified.
 *
 * @details Internally invokes matching function from channel related sensor support implementation.
 *
 * @param[in] aChnlIdx index of coresponding sensor channel
 * @param[in] acpcPrmA sensor parameter structure A
 * @param[in] acpcPrmB sensor parameter structure B
 *
 * @return 0 if context not matching or NULL, >0 otherwise.
 *******************************************************************************/
uint32_t cca_MatchSensPrm(uint32_t aChnlIdx, const cca_SensPrm_t* const acpcPrmA, const cca_SensPrm_t* const acpcPrmB);

/******************************************************************************/
/**
 * @brief Generates I2C commands table based on sensor parameter settings
 *
 * @details Provides initial camera configuration (isp context and sensor I2C table).
 *
 * @param[out] acpI2cTab I2C command table to be filled
 * @param[in] apcSensPrmNew new parameters to be set
 * @param[in] apcSensPrmPrv optional, parameter values set previously will not be added to the I2C table. If null all params apply.
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_GetI2cTab(cca_I2cTab_t* const acpI2cTab, const cca_SensPrm_t* apcSensPrmNew, const cca_SensPrm_t* apcSensPrmPrv, uint32_t aChnlIdx);

/******************************************************************************/
/**
 * @brief Terminate interaction with ISP driver user service.
 *
 * @details One call per CamID required.
 *
 * @return  CCA_SUCCESS if all ok, CCA_ERROR otherwise
 *******************************************************************************/
cca_Result_t cca_Close(void);

/******************************************************************************/
/**
 * @brief Copy relevant members of ISP config.
 * @param[in] apSrc Source ISP hardware context to copy from
 * @param[out] apDst Destination ISP hardware context to copy to
 *******************************************************************************/
void cca_CopyIspCfg(isp_CtxHw_t *apDst, const isp_CtxHw_t *apSrc);

#ifdef __cplusplus
}
#endif

/** @} */ // addtogroup cca_pub_api
#endif /* #ifndef CCA_H */
