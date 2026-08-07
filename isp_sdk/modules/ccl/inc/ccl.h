/*******************************************************************************
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef CCL_H
#define CCL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ccl.h
 * @brief Main camera control library interface file
 * @details This is the only include file for external code
 */

#include "ilog.h"
#include "isp_platform.h"
#include "ccl_features.h"

#if CCL_FEATURE_CAPTURE_CGD
#include "cgd_types.h"
#endif

#if CCL_FEATURE_RUNTIME_CONTROL
#include "demo_ext_control.h"
#endif

#include "cca.h"

#define CCL_CFG_MAX_SENS_CFG_HISTORY    (10U)

#define CCL_CFG_MAX_BUF_PER_STREAM    (10U)

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/
typedef enum
{
    CCL_ERR_SUCCESS = 0,
    CCL_ERR_NOT_IMPLEMENTED = -1,
    CCL_ERR_CCA = -2,
    CCL_ERR_BAD_PARAM = -3,
    CCL_ERR_BAD_STATE = -4,
    CCL_ERR_GENERIC = -5,
    CCL_ERR_ALLOC = -6,
    CCL_ERR_TIMEOUT = -7,
    CCL_ERR_SENSOR = -8,
    CCL_ERR_ISP = -9,
    CCL_ERR_TOO_MANY_CCF_GROUPS = -10,
} ccl_Err_t;

typedef enum
{
#if CCL_FEATURE_CAPTURE_FILE
    /** @brief Pre-captured frames stored on disk */
    CCL_CAP_VIA_FILE,
#endif
#if CCL_FEATURE_CAPTURE_V4L
    /** @brief Real camera capture via Linux internal V4L API, multiple cameras, camera to mem, serialized processing on ISP to different mem */
    CCL_CAP_VIA_V4L,
#endif
#if CCL_FEATURE_CAPTURE_CGD
    /** @brief Real camera capture via SDK's internal drivers */
    CCL_CAP_VIA_CGD,
#endif
#if CCL_FEATURE_CAPTURE_FASTDDS
    /** @brief Real camera capture and streaming via ETH and FastDDs communication stack*/
    CCL_CAP_VIA_FASTDDS,
#endif
#if CCL_FEATURE_CAPTURE_MEMCPY
    /** @brief Very light weight system that copies data from main memory into ccl capture buffer */
    CCL_CAP_VIA_MEMCPY,
#endif
} ccl_CapDev_t;

typedef enum
{
    CCL_CAP_MODE_M2M,
    CCL_CAP_MODE_STREAM,
    CCL_CAP_MODE_COUNT,
}ccl_CapMode_t;

typedef enum {
    CCL_FPS_STAT_CAP,
    CCL_FPS_STAT_FW,
    CCL_FPS_STAT_CNT
}ccl_FpsStatId_t;

typedef enum
{
    CCL_TIME_ISP_CONTEXT_SWAP,
    CCL_TIME_CAPTURE_TO_APP,
    CCL_TIME_ISP_PROC_TIME,
    CCL_TIME_STAT_CNT
}ccl_TimeStat_t;

#define FILE_INPUT_MAX_PLANE_CNT    (2)

typedef struct
{
    /** @brief First frame index (inclusive) */
    uint32_t mStartIdx;
    /** @brief Last frame index (inclusive) */
    uint32_t mStopIndex;
    /** @brief Flag if sequence is to be generated indefinitely */
    bool mRepeat;
    /** @brief Frame rate generation speed */
    double mFps;
    /** @brief Size of embedded data before image as stored in files [Bytes] */
    uint32_t mEmbd0Sz;
    /** @brief Size of embedded data after image as stored in files [Bytes] */
    uint32_t mEmbd1Sz;
}ccl_CapCfgMemcpy_t;

typedef struct
{
    /** @brief Paths to files for each input plane
     * @details Setting path to zeros will lead to plane not beeing generated.
     * You can use printf notation. current frame index will be added as parameter to
     * printf() before file load. (e.g. ../img/ovx3c_dcg_%06d.pgm will become ../img/ovx3c_dcg_000001.pgm for index 1) */
    char mFilesPath[FILE_INPUT_MAX_PLANE_CNT][512];
    /** @brief First frame index (inclusive) */
    uint32_t mStartIndex;
    /** @brief Last frame index (inclusive) */
    uint32_t mStopIndex;
    /** @brief Flag if sequence is to be generated indefinitely */
    bool mRepeat;
    /** @brief Frame rate generation speed */
    double mFps;
    /** @brief Flag for loading pgm - should bytes be swapped (BE to LE)*/
    bool SwapBytes;
    /** @brief Size of embedded data before image as stored in files [Bytes] */
    uint32_t mEmbd0Sz;
    /** @brief Size of embedded data after image as stored in files [Bytes] */
    uint32_t mEmbd1Sz;
    /** @brief Preloads image data during buffer initialization and do not modify them anymore
     * @details Generally used for storage performance reasons and multiple streams. Number of repeated images are
     * equal to buffer count */
    bool mPreloadOnInit;
    /**
     * @brief Callback function for selecting next file to load index based on sensor parameters.
     * @details Gives possibility to choose raw input file based on parameters to be applied to sensor. Can be defined from demo application. If not defined, the CCL will auto-increment the file index after each load.
     *
     * The execution time of this content should be minimized.
     *
     * @param apSensPrm new sensor parameters that would be applied to the sensor
     *
     * @return >= 0 file index to be loaded next, or < 0 number in case of error
     */
    int32_t (*mpSelectIdxClbck)(cca_SensPrm_t* mpSensPrm);
}ccl_CapCfgFile_t;

typedef struct
{
    /** @brief First frame index (inclusive) */
    uint32_t mStartIdx;
    /** @brief Last frame index (inclusive) */
    uint32_t mStopIndex;
    /** @brief Width of captured image (just image data)
     * @details There will be a check if this value is supported by sensor, if not warning will be printed */
    uint32_t mOutPictureWidth;
    /** @brief Height of captured image (just image data)
     * @details There will be a check if this value is supported by sensor, if not warning will be printed */
    uint32_t mOutPictureHeight;
    /** @brief Amount of lines at the beginning of captured image that are considered embedded data
     * @details Input buffers will be allocated separately for this embedded data and for image data */
    uint32_t mEmbdLinesBefore;
    /** @brief Amount of lines at the end of captured image that are considered embedded data
     * @details Input buffers will be allocated separately for this embedded data and for image data */
    uint32_t mEmbdLinesAfter;

    /**
     * @brief sensor initialization via CGD API
     *
     * @param apParams  camera specific parameters as given for this stream during ccl_init()
     * @param mParamSize size of the apParams structure as given for this stream during ccl_init()
     *
     * @return 0 if procedure was success -1 otherwise
     *
     * @details Function specifically written for user-space control of the CGD driver that is controlling
     * sensor connected to this stream.
     * This call should do any power-up, config or reads.
     * E.g. Read and print sensor ID, read calibration data from the sensor for CCF library. */
    cgd_Result (*mSensorInit)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apParams, size_t mParamSize);

    /**
     * @brief sensor configuration update via I2C CGD API
     *
     * @param apSensPrm new sensor configuration to activate
     *
     * @return 0 if procedure was success -1 otherwise
     *
     * @details Function specifically written for user-space control of the CGD driver that is controlling
     * sensor connected to this stream.
     * This call should translate generic CCA parameters into parameters specific for this type of sensor and then
     * do appropriate CGD call so the sensor is reconfigured. */
    cgd_Result (*mSensorUpdate)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cca_SensPrm_t* apSensPrm);

    /**
     * @brief sensor start via I2C CGD API, function called after Stream ON
     *
     * @return 0 if procedure was success -1 otherwise
     */
    cgd_Result (*mSensorStart)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

    /**
     * @brief Provides CGD system configuration for the sensor
     *
     * @param aHwId ISP hardware block identifier
     * @param aCamId Camera pipe identifier within the ISP block
     * @param apExternalConfig Pointer to CGD ISP configuration structure which can be changed
     *
     * @return 0 if procedure was successful, -1 otherwise
     */
    cgd_Result (*mConfigureCgd)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, cgd_SystemExtCfg_t* newExternalConfig);

    /**
     * @brief sensor stop via I2C CGD API
     *
     * @return 0 if procedure was success -1 otherwise
     */
    cgd_Result (*mSensorStop)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

} ccl_CapCfgCgd_t;

typedef struct
{
    bool enabled;
    int32_t mLeft;
    int32_t mTop;
    int32_t mWidth;
    int32_t mHeight;
}CropCfg_t;

typedef struct
{
    /** @brief Id of V4L device e.g. what /dev/videoXX to use as frame source*/
    uint32_t mVideoDevId;
    /** @brief Id of V4L control device e.g. what /dev/v4l-subdevXX to use for sensor control */
    uint32_t mVideoSubDevId;
    /** @brief 0 if single exposure capture => in0 only, !=0 both in0 and in1 are provided
     * @details can have negative value; /dev/video[mVideoDevId + mIn1DevOffset] used as source of in1 raw data */
    int32_t mIn1DevOffset;
    /** @brief FPS value that will be configured via v4l API */
    double mFps;
    /** @brief Width of captured image (just image data)
     * @details There will be a check if this value is supported by sensor, if not warning will be printed */
    uint32_t mOutPictureWidth;
    /** @brief Height of captured image (just image data)
     * @details There will be a check if this value is supported by sensor, if not warning will be printed */
    uint32_t mOutPictureHeight;
    /** @brief Number of planes device will capture.
     * @details This will be used to adjust image buffer allocation and as parameter for V4l output format (for format will be used this exact value) */
    uint32_t mPlaneCnt;
    /** @brief Parameter for V4l output format, this exact value will be passed to V4L */
    uint32_t mCapFmt;
    /** @brief Value of horizontal flip 0 - no flip, 1 - flip */
    uint32_t mHFlip;
    /** @brief Value of vertical flip 0 - no flip, 1 - flip */
    uint32_t mVFlip;
    /** @brief Crop configuration */
    CropCfg_t mCrop;
    /** @brief Minimum buffers that we will try to keep queued on V4L subsystem */
    uint8_t mMinQueuedBufs;
    /** @brief Start dropping if queue reaches this number */
    int8_t mStartDropCount;

    /** @brief Amount of lines at the beginning of captured image that are considered embedded data
     * @details Input buffers will be allocated separately for this embedded data and for image data */
    uint32_t mEmbdLinesBefore;
    /** @brief Amount of lines at the end of captured image that are considered embedded data
     * @details Input buffers will be allocated separately for this embedded data and for image data */
    uint32_t mEmbdLinesAfter;

    /**
     * @brief sensor initialization via V4L API
     *
     * @param aFd File descriptor for stream's control file (sub-device)
     * @param apParams  camera specific parameters as given for this stream during ccl_init()
     * @param mParamSize size of the apParams structure as given for this stream during ccl_init()
     *
     * @return 0 if procedure was success -1 otherwise
     *
     * @details Function specifically written for user-space control of the V4L driver that is controlling
     * sensor connected to this stream.
     * This call should do any power-up, config or reads.
     * E.g. Read and print sensor ID, read calibration data from the sensor for CCF library. */
    int32_t (*mSensorInit)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, int aFd, void* apParams, size_t mParamSize);

    /**
     * @brief sensor configuration update via V4L API
     *
     * @param aFd File descriptor for stream's control file (sub-device)
     * @param apSensPrm New configuration to activate
     * @param apParams  camera specific parameters as given for this stream during ccl_init()
     * @param mParamSize size of the apParams structure as given for this stream during ccl_init()
     *
     * @return 0 if procedure was success -1 otherwise
     *
     * @details Function specifically written for user-space control of the V4L driver that is controlling
     * sensor connected to this stream.
     * This call should translate generic CCA parameters into parameters specific for this type of sensor and then
     * do appropriate V4L call so the sensor is reconfigured. */
    int32_t (*mSensorUpdate)(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, int aFd, cca_SensPrm_t *apSensPrm, void* apParams, size_t mParamSize);
}ccl_CapCfgV4l_t;

typedef struct
{
    uint8_t Dummy;
}ccl_CapCfgSdk_t;

typedef struct
{
    /** @brief name of the DDS topic to listen to */
    const char* mpcTopicName;
    /** @brief First frame index (inclusive) */
    uint32_t mStartIdx;
    /** @brief Last frame index (inclusive) */
    uint32_t mStopIndex;
    /** @brief Size of embedded data before image as stored in files [Bytes] */
    uint32_t mEmbd0Sz;
    /** @brief Size of embedded data after image as stored in files [Bytes] */
    uint32_t mEmbd1Sz;
}ccl_CapCfgDds_t;

typedef union
{
    ccl_CapCfgMemcpy_t mMemcpy;
    ccl_CapCfgFile_t mFile;
    ccl_CapCfgV4l_t mV4l;
    ccl_CapCfgSdk_t mSdk;
    ccl_CapCfgDds_t mDds;
    ccl_CapCfgCgd_t mCgd;
}ccl_CapParams_t;

typedef struct
{
    /** @brief Method of capturing image data (what API to use) */
    ccl_CapDev_t mCaptureMethod;
    /** @brief Capture method specific parameters */
    ccl_CapParams_t mCapParams;
    /** @brief Capture mode */
    ccl_CapMode_t mCapMode;

    /** @brief ONLY FOR streaming mode -> How many frames to wait before new HDR cfg is used in ISP
     * @details e.g. How many frames it takes from sensor cfg update to captured frame */
    uint8_t mHdrDelay;

    /** @brief Number of internal buffers to allocate
     * @details At one point a buffer can be processed by: sensor, ISP, CCA or APP, make sure to
    allocate enough buffers for your target frame rate and processing speed.
    Enforced min is 3 (MIN_FRAME_TASKS_PER_STREAM) recommended minimum is 5 */
    uint32_t mBufCnt;
    /** @brief Indicates how long should be history for ISP&sensor configurations
     * @details This should be greater or equal to mBufCnt */
    uint32_t mMaxCfgHistCnt;
    /** @brief Sensor control period (in frames)
     * @details For the duration of this period no update is sent to sensor, so it can stabilize
     * value of 1 means that new configuration is send with every generated frame,
     * value of 10 means that after new configuration is sent 9 frames are generated with the same values */
    uint64_t mSensUpdPeriod;

#if CCL_FEATURE_LOG_FRAME_TIMES
    /** @brief If set to true it will log timing of individual frames */
    bool mLogFrameTimes;
#endif
#if CCL_FEATURE_LOG_SENS_CFG
    /** @brief If set to true it sensor cfg history will be saved */
    bool mLogSensCfg;
#endif
}ccl_CapCfg_t;

typedef struct
{
    /** @brief Frame Id as set by capture API (do not confuse with isp_Task_t's meta info mFrameIdx)*/
    uint64_t mFrameNo;
    /** @brief ISP configuration that was used to process this frame */
    isp_CtxHw_t *mIspcfg;
}ccl_FrameMeta_t;

typedef struct
{
    /** @brief Image data (mix of in and out planes) */
    cca_ImgBuf_t mImg[CCA_IOBUFT_CNT];

    /** @brief Embedded data that came from camera along with image data */
    cca_EmbdData_t mEmbed;

    /** @brief Metadata from image processing */
    ccl_FrameMeta_t mMetaData;
}ccl_Frame_t;

/** @brief Set of parameters for the UpdateContext user callback. */
typedef struct
{
    ispdrv_HwId_t mHwId; /** @brief ISP HW ID */
    ispdrv_CamPipeId_t mCamId; /** @brief identifies the camera stream */
    isp_Ctx_t* mpNewCtx; /** @brief ISP context values generated by CCA */
    cca_SensPrm_t* mpNewSensPrm; /** @brief sensor parameter values (after distribution) generated by CCA */
    uint32_t mFrameId; /** @brief index number of the frame the new IP sensor parameter and context are base on*/
} ccl_UpdCtx_t;

/** @brief Set of user callback function pointers.
 *
 * If set to NULL the event handler is not invoked.
*/
typedef struct
{
    /** @brief pointer to user data that will be passed to each eventhandler */
    void* mpUsrPrm;

    /**
     * @brief User callback function invoked by CCL for each new context (sensor and ISP settings) generated by CCA.
     *
     * Gives option to application code to access and modify parameter values of the ISP and sensor immediately after being
     * generated by the CCA.
     *
     * @param apUpdCtx identifies context and gives access to parameter values
     * @param apUsrPrm user provided pointer
     *
     * @details Please note that stream variables are locked for the duration of this call. No major delays should be present.
     */
    void (*mpUpdateContext)(ccl_UpdCtx_t *apUpdCtx, void* apUsrPrm);
}ccl_EventHandlers_t;

typedef struct
{
    /** @brief CCF configuration for CCA library */
    cca_CcfCfg_t mCcfCfg;

    /** @brief Sensors connected to this system
     * @details This array lists all available ISP slots, allocate connected sensors to them.
     * Set mEnabled to 0 for slots that have no sensor connected to it. */
    cca_CamInit_t mpCams[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];

    /** @brief Signals if CCF library is capable of running multiple instances of its algos
     * @details In essence this signal if one sensor need to wait for previous sensor's CCF to finish or it can run its CCFs in parallel.
     * true - run in parallel
     * false - run in sequence */
    bool mCcfIsParallelCapable;

    /** @brief Grouping of sensors for CCF algos
     * @details Some cameras might need to have its parameters synchronized and therefore need to be processed in CCF in one go.
     * This parameter holds "CCF group id" for each stream. All streams with the same "group" go for CCF processing together.
     * Value of 0 means that stream is not controlled by CCF algorithms
     * Example System have 5 streams enabled. All on hwid 0, camid 0..4.
     * camid 0,1 should be controlled as a pair
     * camid 2,3 should be controlled as a pair
     * camid 4 should not be controlled
     * then set mCcfGroups[0][0] = 1; mCcfGroups[0][1] = 1; mCcfGroups[0][2] = 2; mCcfGroups[0][3] = 2; mCcfGroups[0][4] = 0; */
    uint32_t mCcfGroups[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];

    /** @brief event handlers definition */
    ccl_EventHandlers_t mEventHandlers;

#if CCL_FEATURE_RUNTIME_CONTROL
    /** @brief pointer to external runtime control context */
    utils_ExtControlCtx_t* mpExtCtrlCtx;
#endif
}ccl_Cfg_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  FUNCTIONS
==============================================================================*/

/**
 * @brief Initialize library as a whole
 *
 * @param apCclCfg - system setup, see ccl_Cfg_t for details
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Initialize internal workings of library.
 * - Creates connection to control (driver) part of image processing (=> verify the driver is running)
 * - initialize camera sensor configuration library
 */
ccl_Err_t ccl_Init(ccl_Cfg_t *apCclCfg);

/**
 * @brief Stops all streams frees all resources.
 *
 * @return 0 on success or negative error otherwise.
 */
ccl_Err_t ccl_Deinit(void);

/**
 * @brief Initialize stream (allocates hw resources, powers capture device)
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apCapCfg - Frame capture configuration
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details This call allocates all resources needed for capturing on device associated
 * with the given stream. Type and configuration of capture device is set for all devices
 * during the ccl_Init() call.
 * Individual tasks performed during this call:
 * - requests given processing pipe on ISP
 * - allocate all memory blocks from appropriate pools, based on capture device
 * - initialize capture device
 */
ccl_Err_t ccl_Open(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_CapCfg_t *apCapCfg);

/**
 * @brief Stops capture device, powers it down and frees all allocated resources.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details The device can be prepared for operation with another cc
 */
ccl_Err_t ccl_Close(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/**
 * @brief Sends start signal to already prepared capture device
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 */
ccl_Err_t ccl_Start(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/**
 * @brief Sends stop signal to capture device
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details Stops the capture device, but keeps it prepared for another start signal.
 * All resources are still allocated.
 */
ccl_Err_t ccl_Stop(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId);

/**
 * @brief Requests next captured frame. Blocks until frame is ready.
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - variable that will receive the frame
 * @param aTimeout - maximum time to wait for the frame
 *
 * @return 0 on success or negative error otherwise.
 *
 * @details The returned frame is locked to client code until next call to this function.
 * There is no need to call any other API to read frames if one by one readout is ok strategy.
 */
ccl_Err_t ccl_GetFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t **apFrame, uint32_t aTimeout);

/**
 * @brief Retrieves the next available frame from the specified camera stream, blocking until a frame is ready or timeout occurs
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - variable that will receive the frame
 * @param aTimeout - maximum time to wait for the frame
 * @return ccl_Err_t
 *
 * @details Retreived frame should be returned to frame pool using ccl_PushFrame() after processing
 */
ccl_Err_t ccl_PopFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t **apFrame, uint32_t aTimeout);

/**
 * @brief Returns frame previously obtained via ccl_PopFrame()
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param apFrame - frame to be returned to the frame pool
 * @return ccl_Err_t
 */
ccl_Err_t ccl_PushFrame(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_Frame_t *apFrame);

/**
 * @brief Returns internal fps statistic
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aStatId - target statistic
 * @param aGlobal - if true returns values for entire CCL (aHwId and aCamId ignored)
 *
 * @return internal statistic value
 */
float ccl_GetFps(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FpsStatId_t aStatId, bool aGlobal);

/**
 * @brief Returns internal Pixel rate statistic
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aStatId - target statistic
 * @param aGlobal - if true returns values for entire CCL (aHwId and aCamId ignored)
 *
 * @return internal statistic value
 */
float ccl_GetPixelRate(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FpsStatId_t aStatId, bool aGlobal);

/**
 * @brief Retrieves given latency statistics for a specific ISP block and camera pipe
 *
 * @param aHwId - ISP block the stream is running on
 * @param aCamId - ISP pipe in the block stream is running on
 * @param aStatId - target statistic
 * @param apLatency - array of size 3 that will receive latency values (min/avg/max)
 */
void ccl_GetTiming(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_TimeStat_t aStatId, float apLatency[3]);

/**
 * @brief Retrieves statistics for CCA processing times
 *
 * @param apCcaTimes - array of size 3 that will receive stat values (min/avg/max)
 */
void ccl_GetCcaTiming(float apCcaTimes[3]);

/**
 * @brief Returns ISP utilization statistics
 *
 * @param aHwId - ISP block to get utilization for
 * @return ISP utilization percentage
 */
float ccl_GetIspUse(ispdrv_HwId_t aHwId);

/**
 * @brief Returns internal cca stats
 *
 * @param GroupId - target group, use 0 for global statistic
 * @return number of passes per second
 */
float ccl_GetCcaStats(uint32_t GroupId);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_H */

