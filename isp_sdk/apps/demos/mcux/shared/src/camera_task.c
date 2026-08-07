/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/******************************************************************************/
/**
 * @file  camera.c
 * @brief This file handles camera(s) with image postprocessing.
 *******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "ccl.h"
#include "cca_ovx3c.h"
#include "camera_ctx.h"

#include "ilog.h"

#include "fsl_debug_console.h"

#include "utils_i2c.h"
#include "cgd_types.h"
#include "cgd_config.h"
#include "cgd.h"
#include "cgd_hal.h"

#include "fsl_dwc_mipi_csi2rx.h"

#include "ccl_cap_cgd.h"
#include "cgd_ox03c10.h"

#include "new_sensor_cca.h"
#include "new_sensor_ccl.h"

#include "utils_edma.h"

#include "display_task.h"
#if configGENERATE_RUN_TIME_STATS==1
#include "FreeRTOSAdditions.h"
#endif

#ifdef SEND_IMG_VIA_USB
    #include "virtual_com.h"
#endif

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define STACK_SIZE 2048
#define STATS_STACK_SIZE 256
/* Define number of camera streams {1..3} in example's proj.conf or prj_local.conf file */
#define NUMBER_OF_STREAMS CONFIG_ISP_SDK_CAMERA_COUNT
/* Define camera type, 3 for OVX3C, 0 for new_sensor */
#define CAMERA_TYPE CONFIG_ISP_SDK_CAMERA_TYPE

#ifdef __GCC_COVERAGE__
#define RUNTIME 2 /* seconds */
#endif

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

/*==============================================================================
*                              LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              LOCAL VARIABLES
==============================================================================*/
extern uint8_t camera_tuning_database_data[];

/*==============================================================================
*                              GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                              GLOBAL VARIABLES
==============================================================================*/
static StaticTask_t gTaskBuffer;
static StackType_t gStack[STACK_SIZE];

static StaticTask_t gStatsTskBuf;
static StackType_t gStatsTskStack[STATS_STACK_SIZE];
static uint32_t gFpsCnt = 0;

DDR_NOINIT CameraCtx_t gAppCtx;
/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/
#if defined(CCFCUSTOM)

#include "cca_ccf_custom.h"

static int32_t FillCcfCusmtomCfg(CameraCtx_t *apCtx, ccl_Cfg_t *apCclCfg)
{

    // setup selected CCF
    apCclCfg->mCcfIsParallelCapable  = false;
    apCclCfg->mCcfCfg.mpParams       = NULL;
    apCclCfg->mCcfCfg.mParamSize     = 0;
    cca_custom_GetOps(&(apCclCfg->mCcfCfg.mOps));

    for (uint32_t lStrIdx = 0; lStrIdx < ISPDRV_CAM_PIPE_ID_CNT; lStrIdx++)
    {
        ispdrv_CamPipeId_t lCamId = apCtx->mpStreamArr[lStrIdx].mCamId;

        // setup all streams to the same CCF group 1
        // group 0 means not controlled by ccf
        apCclCfg->mCcfGroups[ISP_HWID_0][lCamId] = (apCtx->mIniCfg.mCcaOn == true) ? lStrIdx % CONFIG_ISP_SDK_CAMERA_COUNT + 1 : 0;
    }

    return 0;
}
#elif defined(NEOCONTROL)

#include "cca_neo_isp_control.h"

/*******************************************************************************/
/**
 * @brief Fill in ITEC CCF specific settings
 *
 * @param [in, out] apCtx application context
 * @param [in, out] arCclCfg CCL structure containing CCF settings to be filled in
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static int32_t FillNeocontrolCcfCfg(CameraCtx_t *apCtx, ccl_Cfg_t *apCclCfg)
{
    static cca_NeocontrolParams_t         lsNeocontrolParams;
    static cca_NeocontrolSensorIdPair_t   lsSensorIdPair;

    // get proper sensor ID
    lsSensorIdPair.mSensorId = apCtx->mSensorId[0];

    // setup selected CCF
    apCclCfg->mCcfIsParallelCapable  = false;
    apCclCfg->mCcfCfg.mpParams       = &lsNeocontrolParams;
    apCclCfg->mCcfCfg.mParamSize     = sizeof(cca_NeocontrolParams_t);
    cca_ccf_GetOps(&(apCclCfg->mCcfCfg.mOps));

    // prep specific params (set parameters to values loaded from ini file)
#if LIVE_TUNE
    lsNeocontrolParams.mTuningToolCon     = 1U;
#else
    lsNeocontrolParams.mTuningToolCon     = 0U;
#endif
    lsNeocontrolParams.mTuningServerPort  = 0U;
    lsNeocontrolParams.mpcTuningServerIp  = "0.0.0.0";

    // sensor ID map - only a single sensor type supported in this demo
    lsNeocontrolParams.mSensorIdMap.mpMap     = &lsSensorIdPair;
    lsNeocontrolParams.mSensorIdMap.mLength   = 1U; // only one sensor type per application supported now

    lsNeocontrolParams.mpCcfAlgoConfig = camera_tuning_database_data;

    uint8_t ActiveCamCnt = 0;
    for (uint32_t lStrIdx = 0; lStrIdx < ISPDRV_CAM_PIPE_ID_CNT; lStrIdx++)
    {
        ispdrv_CamPipeId_t lCamId = apCtx->mpStreamArr[lStrIdx].mCamId;
        assert(lCamId < ISPDRV_CAM_PIPE_ID_CNT);

        if (apCclCfg->mpCams[ISP_HWID_0][lCamId].mEnabled == 0)
        {
            // no sensor connected on this ISP pipe
            lsNeocontrolParams.mpIspActive[lCamId] = 0U;
            // group 0 means not controlled by ccf
            apCclCfg->mCcfGroups[ISP_HWID_0][lCamId] = 0;
        }
        else
        {
            lsNeocontrolParams.mpIspActive[lCamId] = 1U;
            ActiveCamCnt++;
            // non-zero group id means controlled by ccf
            apCclCfg->mCcfGroups[ISP_HWID_0][lCamId] = (apCtx->mIniCfg.mCcaOn == true) ? ActiveCamCnt : 0;
        }
    }

    return 0;
}
#else // CCF selecction
	#error No CCF selected
#endif
/*******************************************************************************/
/**
 * @brief Fill in CCF configuration
 *
 * @param [in, out] apCtx application context
 * @param [in, out] arCclCfg CCL structure containing CCF settings to be filled in
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/

static int32_t FillCcfCfg(CameraCtx_t *apCtx, ccl_Cfg_t *apCclCfg)
{
    int32_t lRet;

#if defined(CCFCUSTOM)
	lRet = FillCcfCusmtomCfg(apCtx, apCclCfg);
#elif defined(NEOCONTROL)
    lRet = FillNeocontrolCcfCfg(apCtx, apCclCfg);
#else  // ifdef NEOCONTROL
	#error No CCF selected
#endif // else from ifdef NEOCONTROL
    return lRet;
} // FillCcfCfg()

/*******************************************************************************/
/**
 * @brief Initializes CCL operation
 *
 * @details sets system -> what is connected and how is it operated
 *
 * @param apCtx application context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/

static int32_t InitCcl(CameraCtx_t *apCtx)
{
    // setting to 0 automatically disables all cameras (.mEnabled = 0), so no need to config unused ISP pipes
    ccl_Cfg_t lCclCfg;
    memset(&lCclCfg, 0, sizeof(ccl_Cfg_t));

    // * Capture sensors *
    // Each sensor needs to be associated with specific ISP processing pipe.
    // This is done below by specifying sensor for each active ISP pipe.
    // Some sensors are supported by CCA library so handling functions can be
    // obtained by simply calling appropriate cca_xxxx_GetOps() over target ISP pipe's mCamOps.
    // Unsupported sensors need to have its handling functions implemented manually in
    // this app see cca_CamOps_t for details
    for (uint32_t lStrIdx = 0; lStrIdx < apCtx->mStrCnt; lStrIdx++)
    {
        ispdrv_CamPipeId_t lCamId = apCtx->mpStreamArr[lStrIdx].mCamId;
        assert(lCamId < ISPDRV_CAM_PIPE_ID_CNT);

        lCclCfg.mpCams[ISP_HWID_0][lCamId].mEnabled   = 1;
        lCclCfg.mpCams[ISP_HWID_0][lCamId].mModeId = 0U; // now used only for HDR/noHDR mode for OS08A20
        lCclCfg.mpCams[ISP_HWID_0][lCamId].mSensorId  = apCtx->mSensorId[0]; // expect the same sensor on all streams

        // set initial ISP cfg to values loaded from ini file -> some initial values may be updated by CCF algorithms
        lCclCfg.mpCams[ISP_HWID_0][lCamId].mcpExtDef = apCtx->mDefCtx[lStrIdx];

        switch (lCclCfg.mpCams[ISP_HWID_0][lCamId].mSensorId)
        {
            case SENSOR_OV_X3C:
            {
                lCclCfg.mpCams[ISP_HWID_0][lCamId].mpParams   = &apCtx->mOvx3cParams[lCamId]; // TODO: replace with apCtx->mNewSensorParams
                lCclCfg.mpCams[ISP_HWID_0][lCamId].mParamSize = sizeof(cca_ovx3c_Params_t); // TODO: replace with new_sensor_Params_t
                apCtx->mOvx3cParams[lCamId].mFps              = 30;
                cca_ovx3c_GetOps(&(lCclCfg.mpCams[ISP_HWID_0][lCamId].mCamOps));
                break;
            }
            case NEW_SENSOR:
            {
                lCclCfg.mpCams[ISP_HWID_0][lCamId].mpParams   = &apCtx->mNewSensorParams[lCamId]; // TODO: replace with apCtx->mNewSensorParams
                lCclCfg.mpCams[ISP_HWID_0][lCamId].mParamSize = sizeof(new_sensor_Params_t); // TODO: replace with new_sensor_Params_t
                apCtx->mNewSensorParams[lCamId].mFps          = 30;
                new_sensor_GetCcaOps(&(lCclCfg.mpCams[ISP_HWID_0][lCamId].mCamOps));
                break;
            }
        }
    }

    // setup event handlers
    lCclCfg.mEventHandlers.mpUpdateContext = NULL;
    lCclCfg.mEventHandlers.mpUsrPrm = NULL;

    // fill CCF config
    if(0 != FillCcfCfg(apCtx, &lCclCfg))
    {
        PRINTF("Failed to fill CCF config\r\n");
        return -1;
    }

    // Initialize the library (sets CCF library and available cameras with their resolutions)
    if (0 != ccl_Init(&lCclCfg))
    {
        PRINTF("Error initializing camera library\r\n");
        return -1;
    }
    return 0;
} // InitCcl()

/*******************************************************************************/
/**
 * @brief Fills in CCL configuration for CGD data source
 *
 * @param arCapCfg [out] capture configuration parameters
 * @param lpStrCtx [in] stream context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static int32_t ConfigCclStreamCGD(ccl_CapCfg_t *apCapCfg, StreamCtx_t *apStrCtx)
{
    CameraCtx_t* lpCamCtx = apStrCtx->mpCameraCtx; // get application context reference

    // number of img work buffers to allocate for this stream
    // NOTE: at one point a buffer can be processed by: sensor, ISP, CCA or this APP, make sure to
    // allocate enough buffers for your target frame rate and processing speed
    apCapCfg->mBufCnt = 6;
    // max latency of sensor reconfiguration [in frames] (max amount of frames needed to deliver a frame with new sensor settings)
    apCapCfg->mMaxCfgHistCnt             = 4;
    apCapCfg->mCapParams.mCgd.mStartIdx  = 1;
    apCapCfg->mCapParams.mCgd.mStopIndex = 0;

    switch (lpCamCtx->mSensorId[0])
    {
        case SENSOR_OV_X3C:
        {
            //  this is just picture size -> not including embedded data
            apCapCfg->mCapParams.mCgd.mOutPictureWidth  = 1920;
            apCapCfg->mCapParams.mCgd.mOutPictureHeight = 1280;
            apCapCfg->mCapParams.mCgd.mEmbdLinesBefore  = 2;
            apCapCfg->mCapParams.mCgd.mEmbdLinesAfter   = 0;

            apCapCfg->mCapParams.mCgd.mSensorInit   = cgd_ovx3c_Init;
            apCapCfg->mCapParams.mCgd.mSensorUpdate = cgd_ovx3c_Update;
            apCapCfg->mCapParams.mCgd.mConfigureCgd = cgd_ovx3c_ConfigureCgd;
            apCapCfg->mCapParams.mCgd.mSensorStart  = cgd_ovx3c_Start;
            break;
        }
        case NEW_SENSOR:
        {
            //  this is just picture size -> not including embedded data
            apCapCfg->mCapParams.mCgd.mOutPictureWidth  = 1920;
            apCapCfg->mCapParams.mCgd.mOutPictureHeight = 1280;
            apCapCfg->mCapParams.mCgd.mEmbdLinesBefore  = 2;
            apCapCfg->mCapParams.mCgd.mEmbdLinesAfter   = 0;

            apCapCfg->mCapParams.mCgd.mSensorInit   = cgd_New_Sensor_Init;
            apCapCfg->mCapParams.mCgd.mSensorUpdate = cgd_New_Sensor_Update;
            apCapCfg->mCapParams.mCgd.mConfigureCgd = cgd_New_Sensor_ConfigureCgd;
            apCapCfg->mCapParams.mCgd.mSensorStart  = cgd_New_Sensor_Start;
            break;
        }
        default:
            PRINTF("Unknown sensor type %lu.\r\n", lpCamCtx->mSensorId[0]);
            return -1;
    }

    return 0;
}

/*******************************************************************************/
/**
 * @brief Fills in CCL configuration for file RAW data source
 *
 * @param arCapCfg [out] capture configuration parameters
 * @param lpStrCtx [in] stream context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static int32_t ConfigCclStreamMemCpy(ccl_CapCfg_t *apCapCfg, StreamCtx_t *apStrCtx)
{
    // number of img work buffers to allocate for this stream
    // NOTE: at one point a buffer can be processed by: sensor, ISP, CCA or this APP, make sure to
    // allocate enough buffers for your target frame rate and processing speed

    apCapCfg->mBufCnt = 5;
    // max latency of sensor reconfiguration [in frames] (max amount of frames needed to deliver a frame with new sensor settings)
    apCapCfg->mMaxCfgHistCnt = 1;

    assert(gAppCtx.mIniCfg.mEmbdCnt0 < 100);
    assert(gAppCtx.mIniCfg.mEmbdCnt1 < 100);

    apCapCfg->mCapParams.mMemcpy.mStartIdx = gAppCtx.mIniCfg.mStartIdx;
    apCapCfg->mCapParams.mMemcpy.mStopIndex = gAppCtx.mIniCfg.mStopIdx;
    apCapCfg->mCapParams.mMemcpy.mEmbd0Sz = gAppCtx.mIniCfg.mEmbdCnt0 * gAppCtx.mDefCtx[0]->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mWIDTH * gAppCtx.mDefCtx[0]->mRegs.mPIPE_CONF.mIMG_CONF_CAM0.mIBPP0;
    apCapCfg->mCapParams.mMemcpy.mEmbd1Sz = gAppCtx.mIniCfg.mEmbdCnt1 * gAppCtx.mDefCtx[0]->mRegs.mPIPE_CONF.mIMG_SIZE_CAM0.mWIDTH * gAppCtx.mDefCtx[0]->mRegs.mPIPE_CONF.mIMG_CONF_CAM0.mIBPP0;
    if (gAppCtx.mIniCfg.mStopIdx < gAppCtx.mIniCfg.mStartIdx)
    {
        apCapCfg->mCapParams.mMemcpy.mRepeat = true;
    }
    else
    {
        apCapCfg->mCapParams.mMemcpy.mRepeat = false;
    }
    // simulated frame rate
#ifdef ISIM
    apCapCfg->mCapParams.mMemcpy.mFps = 0; // go as fast as ISIM allows without drops
#else
    apCapCfg->mCapParams.mMemcpy.mFps = 0.0; // simulated fps on SoC
#endif

    return 0;
} // ConfigCclStreamMemCpy()

/*******************************************************************************/
/**
 * @brief Opens active CCL streams
 *
 * @param apCtx application context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static int32_t OpenCclStreams(CameraCtx_t *apCtx)
{
    int32_t lRet = 0;

    // activate individual video streams
    for (uint32_t lStrIdx = 0; lStrIdx < apCtx->mStrCnt; lStrIdx++)
    {
        StreamCtx_t *lpStrCtx = &apCtx->mpStreamArr[lStrIdx];
        ccl_CapCfg_t lCapCfg;
        memset(&lCapCfg, 0, sizeof(lCapCfg));

        // * Capture Method (API) *
        // select capture method
        lCapCfg.mCaptureMethod = CCL_CAP_VIA_CGD;
        // select ISP mode - either set in example's proj.conf or prj_local.conf file
        // or choose directly CCL_CAP_MODE_M2M, CCL_CAP_MODE_STREAM
        lCapCfg.mCapMode = CONFIG_ISP_SDK_CAPTURE_MODE;
        // delay of new HDR cfg from CCA in Streaming mode (for M2M there is context matching)
        lCapCfg.mHdrDelay = 3;
#if CCL_FEATURE_LOG_FRAME_TIMES
        // Setting this variable to true will result in creation of FrameTimes_x_y.csv where will each frame has absolute time of entry into various states of processing
        lCapCfg.mLogFrameTimes = false;
#endif
#if CCL_FEATURE_LOG_SENS_CFG
        // Setting this variable to true will result sensor cfg updates beeing logged
        lCapCfg.mLogSensCfg = false;
#endif
        // Sensor update period [in frame count] -> 1 means after every processed frame (after cca)
        lCapCfg.mSensUpdPeriod = 1;

        if(lCapCfg.mCaptureMethod == CCL_CAP_VIA_MEMCPY)
        {
            lRet |= ConfigCclStreamMemCpy(&lCapCfg, lpStrCtx);
        }
        else if (lCapCfg.mCaptureMethod == CCL_CAP_VIA_CGD)
        {
            lRet |= ConfigCclStreamCGD(&lCapCfg, lpStrCtx);
        }
        else
        {
            PRINTF("Capture method not implemented %d. hwid: %d, camid: %d\r\n", lCapCfg.mCaptureMethod, ISP_HWID_0, lpStrCtx->mCamId);
            lRet |= -1;
        }

        if(lRet == 0)
        {
            // Prepare selected capture device for operation (resource allocation, power-up)
            if (0 != ccl_Open(ISP_HWID_0, lpStrCtx->mCamId, &lCapCfg))
            {
                lRet |= -1;
                PRINTF("Failed to open stream hwid: %d, camid: %d\r\n", ISP_HWID_0, lpStrCtx->mCamId);
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(lRet != 0)
    {
        (void)ccl_Deinit();
    }
    return lRet;
} // OpenCclStreams()

/*******************************************************************************/
/**
 * @brief Initializes application
 *
 * @param apCtx application context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static int32_t Open(CameraCtx_t *apCtx)
{
    for (uint32_t lIter = 0; lIter < apCtx->mStrCnt; lIter++)
    {
        switch (CAMERA_TYPE)
        {
            case 3:
            {
                apCtx->mSensorId[lIter] = SENSOR_OV_X3C;
                break;
            }
            case 0:
            {
                apCtx->mSensorId[lIter] = NEW_SENSOR;
                break;
            }
            default:
            {
                PRINTF("Unknown camera type: %d!\r\n", CAMERA_TYPE);
                return -1;
            }
        }
    }
    // *********** initialize ccl library    *************
    if(0 != InitCcl(apCtx))
    {
        PRINTF("Failed to initialize CameraControlLibrary (CCL)\r\n");
        return -1;
    }

    // ************** Open Streams ****************
    if(0 != OpenCclStreams(apCtx))
    {
        PRINTF("Failed to open CCL streams\r\n");
        return -1;
    }

    return 0;
} // Open()

#ifdef SEND_IMG_VIA_USB
extern uint32_t ShouldSendOutImg;
static void SendFrame(StreamCtx_t *apStr)
{
    if (apStr == NULL || apStr->mpFrameNew == NULL)
    {
        return;
    }

    if (ShouldSendOutImg > 0)
    {
        uint8_t Prefix[60];
        snprintf((char*)Prefix, sizeof(Prefix), "Store binary data to OutImg_%u_#%lu.raw (%lu)\n", apStr->mCamId, (uint32_t)apStr->mpFrameNew->mMetaData.mFrameNo, apStr->mpFrameNew->mImg[CCA_IOBUFT_C012].mSizeBytes);
        uint8_t *lpData[2] = {Prefix, (uint8_t*)apStr->mpFrameNew->mImg[CCA_IOBUFT_C012].mpData};
        size_t lSizes[2] = {strlen((const char*)Prefix), apStr->mpFrameNew->mImg[CCA_IOBUFT_C012].mSizeBytes};

        ShouldSendOutImg--;
        UsbTransmitMultiple(lpData, lSizes, 2, 0);
    }
}
#endif

static int32_t GetFramesBlocking(CameraCtx_t *apCtx)
{
    ccl_Err_t lRes;
    // get frames from all streams
    for (uint32_t lStrIdx = 0; lStrIdx < apCtx->mStrCnt; lStrIdx++)
    {
        StreamCtx_t *lpStr = &apCtx->mpStreamArr[lStrIdx];
        // back up last frame pointer for the case of Get call issue
        ccl_Frame_t* lpNewFrame = NULL;
        // enter wait for a an image to be generated
        // NOTE: any previously obtained buffers are now inaccessible
        lRes = ccl_PopFrame(ISP_HWID_0, lpStr->mCamId, &lpNewFrame, MAX_FRAME_WAIT_MS);
        if (0 != lRes)
        {
            PRINTF("Error while waiting for new image %d @ stream %lu %lu\r\n", lRes, lStrIdx, apCtx->mStrCnt);
            apCtx->mpStop = true;
            break;
        }

        if (pdTRUE != xSemaphoreTake(lpStr->mLockHandle, portMAX_DELAY))
        {
            PRINTF("Failed to take semaphore for stream %lu\r\n", lStrIdx);
            apCtx->mpStop = true;
            break;
        }

        if (lpStr->mpFrameNew != NULL)
        {
            if (CCL_ERR_SUCCESS != ccl_PushFrame(ISP_HWID_0, lpStr->mCamId, lpStr->mpFrameNew))
            {
                PRINTF("Error returning frame for stream %lu\r\n", lStrIdx);
            }
        }
        lpStr->mpFrameNew = lpNewFrame;

        if (pdTRUE != xSemaphoreGive(lpStr->mLockHandle))
        {
            PRINTF("Failed to release semaphore for stream %lu\r\n", lStrIdx);
            apCtx->mpStop = true;
            break;
        }

#ifdef SEND_IMG_VIA_USB
        SendFrame(lpStr);
#endif
    } // for all streams

    return (lRes == 0)?0:-1;
}

int DisplayStreams(CameraCtx_t *apCtx)
{
    for (uint32_t lStrIdx = 0; lStrIdx < 1; lStrIdx++)
    {
        if (pdTRUE != xTaskNotify(gDisplayTask, 0, eNoAction))
        {
            PRINTF("fail to wake display task\n\r");
            vTaskDelay(configTICK_RATE_HZ);
        }
    }

    return 0;
}


static void StatsTask(void *apParameters)
{
    #ifdef __GCC_COVERAGE__
    uint32_t lIter = 0;
    #endif

    while (1)
    {
        // wait 1sec
        vTaskDelay(configTICK_RATE_HZ);

        float CclFwPixelrate = ccl_GetPixelRate(ISP_HWID_0, (ispdrv_CamPipeId_t)-1, CCL_FPS_STAT_FW, true);
        uint32_t ccl_fps_in = ccl_GetFps(ISP_HWID_0, (ispdrv_CamPipeId_t)-1, CCL_FPS_STAT_CAP, true);
        uint32_t ccl_fps_out = ccl_GetFps(ISP_HWID_0, (ispdrv_CamPipeId_t)-1, CCL_FPS_STAT_FW, true);
        uint32_t ccl_CcaAll = ccl_GetCcaStats(0);
        uint32_t ccl_CcaGroups[3];
        for (int i = 1; i <= 3; i++)
        {
            ccl_CcaGroups[i-1] = ccl_GetCcaStats(i);
        }
        float ccl_LatCtx[3];
        float ccl_LatRdy[3];
        float ccl_CcaTime[3];
        ccl_GetTiming(ISP_HWID_0, ISPDRV_CAM_PIPE_ID_0, CCL_TIME_ISP_CONTEXT_SWAP, ccl_LatCtx);
        ccl_GetTiming(ISP_HWID_0, ISPDRV_CAM_PIPE_ID_0, CCL_TIME_CAPTURE_TO_APP, ccl_LatRdy);
        ccl_GetCcaTiming(ccl_CcaTime);
        float IspUtil = ccl_GetIspUse(ISP_HWID_0);

        PRINTF("\n\rIsp: %.1f%%,Fps: %lu/%lu/%lu, CclPixelRate: %f Mpps, display fps: %lu, ccf: %lu (%u/%u/%u)\n\r",
               IspUtil,
               gFpsCnt,
               ccl_fps_in,
               ccl_fps_out,
               CclFwPixelrate / 1e6,
               gDisplayFps,
               ccl_CcaAll,
               ccl_CcaGroups[0],
               ccl_CcaGroups[1],
               ccl_CcaGroups[2]);
        PRINTF("Times cam0 (min/avg/max) ctx_swp[us]: %.1f/%.1f/%.1f, CSI->App[ms] %.1f/%.1f/%.1f, CCA[ms] %.1f/%.1f/%.1f\n\r",
            ccl_LatCtx[0]/1e3, ccl_LatCtx[1]/1e3, ccl_LatCtx[2]/1e3, ccl_LatRdy[0]/1e6, ccl_LatRdy[1]/1e6, ccl_LatRdy[2]/1e6, ccl_CcaTime[0]/1e6, ccl_CcaTime[1]/1e6, ccl_CcaTime[2]/1e6);
        gFpsCnt = 0;
        gDisplayFps = 0;

        #ifdef __GCC_COVERAGE__
        lIter++;
        if (lIter >= RUNTIME)
        {
            PRINTF("\n\r\n\r___ RUNTIME time finished - app ending.\r\n");
            __gcov_dump();

            PRINTF("\n\r___ RUNTIME Dump Ended.\r\n\n\r");
        }
        #endif
    }
}

/*******************************************************************************/
/**
 * @brief Runs camera pipeline
 *
 * @param apCtx application context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static int32_t Run(CameraCtx_t *apCtx)
{
    {
        TaskHandle_t TaskHandle = xTaskCreateStatic(StatsTask, "Statistics task", STATS_STACK_SIZE, NULL, configMAX_PRIORITIES/2, gStatsTskStack, &gStatsTskBuf);

        if (NULL == TaskHandle)
        {
            PRINTF("Stats task creation failed!.\r\n");
            while (1)
                ;
        }
    }

    // ********************* start the stream run *************************
    for (uint32_t lStrIdx = 0; lStrIdx < apCtx->mStrCnt; lStrIdx++)
    {
        ispdrv_CamPipeId_t lCamId = apCtx->mpStreamArr[lStrIdx].mCamId;
        if (0 != ccl_Start(ISP_HWID_0, lCamId))
        {
            PRINTF("Failed to start stream @ index: %ld\r\n", lStrIdx);
            return -1;
        }
    } // for all possible streams

    #if configGENERATE_RUN_TIME_STATS==1
        vTaskClearUsage();
    #endif

    // ********************* image grabbing loop *************************
    while (false == apCtx->mpStop)
    {
        // if OpenCV display enabled, output done here.
        if(0 != GetFramesBlocking(apCtx))
        {
            PRINTF("Failed to get ISP buffers from streams.\r\n");
            apCtx->mpStop = true;
            break;
        }

        if (gFpsCnt == UINT32_MAX)
        {
            PRINTF("Frame counter overflow.\n");
            gFpsCnt = 0;
        }
        gFpsCnt++;

        if(0 != DisplayStreams(apCtx))
        {
            PRINTF("Failed to display streams\r\n");
            apCtx->mpStop = true;
            break;
        }

        apCtx->mFrameCnt++;
    } // while not to stop

    if (apCtx->mFrameCnt == 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
    //return 0;
} // Run()

/*******************************************************************************/
/**
 * @brief Terminates application
 *
 * @param apCtx application context
 *
 * @return 0 if all ok, -1 otherwise
 *******************************************************************************/
static void Close(CameraCtx_t *apCtx)
{
    for (uint32_t lStrIdx = 0; lStrIdx < apCtx->mStrCnt; lStrIdx++)
    {
        ispdrv_CamPipeId_t lCamId = apCtx->mpStreamArr[lStrIdx].mCamId;

        if (0 != ccl_Stop(ISP_HWID_0, (ispdrv_CamPipeId_t)lCamId))
        {
            PRINTF("Error stopping stream\r\n");
        }

        if (0 != ccl_Close(ISP_HWID_0, (ispdrv_CamPipeId_t)lCamId))
        {
            PRINTF("Error closing stream\r\n");
        }
    }

    if (0 != ccl_Deinit())
    {
        PRINTF("ccl_Deinit() Error.\r\n");
    }

    return;
} //Close()

int32_t InitCtx(CameraCtx_t *apCtx)
{
    // initialize the ctx to 0s
    memset(apCtx, 0, sizeof(CameraCtx_t));
    // keep running
    apCtx->mpStop = false;
    // set number of active streams
    apCtx->mStrCnt = NUMBER_OF_STREAMS;
    // no frames processed yet
    apCtx->mFrameCnt = 0U;
    // set default context ptrs to NULL
    memset(&apCtx->mDefCtx, 0, sizeof(apCtx->mDefCtx));

    /*** Init Streams *********/
    StreamCtx_t *lpStr = &apCtx->mpStreamArr[0];

    // init members
    lpStr->mpCameraCtx          = apCtx;
    lpStr->mStrIdx              = 0U;
    lpStr->mHwId                = ISP_HWID_0;
    lpStr->mCamId               = ISPDRV_CAM_PIPE_ID_0;
    lpStr->mpFrameNew           = NULL;
    lpStr->mpFrameOnDisplay     = NULL;
    apCtx->mDefCtx[0]           = (isp_CtxHw_t *) &gIspCfg;
    apCtx->mpStreamArr[0].mLockHandle = xSemaphoreCreateMutexStatic(&apCtx->mpStreamArr[0].mLockData);
    if (apCtx->mpStreamArr[0].mLockHandle == NULL)
    {
        PRINTF("Failed to create mutex for stream %lu\r\n", 0);
        return -1;
    }

    // distribute initial settings into all streams
    for(uint32_t lIter = 1U; lIter < ISPDRV_CAM_PIPE_ID_CNT; lIter++)
    {
        apCtx->mpStreamArr[lIter] = apCtx->mpStreamArr[0];
        apCtx->mpStreamArr[lIter].mStrIdx = lIter; // set stream index
        apCtx->mpStreamArr[lIter].mCamId  = (ispdrv_CamPipeId_t) lIter; // set default cam id - can override from CMD line

        // load default mems and regs
        apCtx->mDefCtx[lIter] = apCtx->mDefCtx[0];

        apCtx->mpStreamArr[lIter].mLockHandle = xSemaphoreCreateMutexStatic(&apCtx->mpStreamArr[lIter].mLockData);
        if (apCtx->mpStreamArr[lIter].mLockHandle == NULL)
        {
            PRINTF("Failed to create mutex for stream %lu\r\n", lIter);
            return -1;
        }
    } // for all application streams

    apCtx->mIniCfg.mpMMS_DTP = NULL;
    apCtx->mIniCfg.mpMMS_UgcIp = "0.0.0.0";
    apCtx->mIniCfg.mMMS_TtOn = 0; ///< 0 if TT is not to be used, 1 if TT to be used
    apCtx->mIniCfg.mMMS_UgcPort = 50069;

    // dewarping
    apCtx->mIniCfg.mpDewarpMap = NULL; ///< path to the dewarp map binary file
    apCtx->mIniCfg.mDewarpMapFormat = 0; ///< format of dewarp map binary: 0 = point, 1 = delta, 2 = delta delta
    apCtx->mIniCfg.mDewarpArbStartX = 12506; ///< delta delta dewarping parameter
    apCtx->mIniCfg.mDewarpArbStartY = 8316; ///< delta delta dewarping parameter
    apCtx->mIniCfg.mDewarpArbDeltaXX = 12; ///< delta delta dewarping parameter
    apCtx->mIniCfg.mDewarpArbDeltaXY = 252; ///< delta delta dewarping parameter
    apCtx->mIniCfg.mDewarpArbDeltaYX = 252; ///< delta delta dewarping parameter
    apCtx->mIniCfg.mDewarpArbDeltaYY = 11; ///< delta delta dewarping parameter

    apCtx->mIniCfg.mStartIdx = 1; ///< first file to process
    apCtx->mIniCfg.mStopIdx = 0; ///< last file to process
    apCtx->mIniCfg.mEmbdCnt0 = 2; ///< # of embedded lines before image data
    apCtx->mIniCfg.mEmbdCnt1 = 0; ///< # of embedded lines after image data
    apCtx->mIniCfg.mCcaOn = 1; ///< 0 if CCA disabled, >0 if CCA enabled

    return 0;
}

#ifdef __GCC_COVERAGE__
int _open(const char *path, int flags, ...)
{
    PRINTF("GCOV_OPEN:%s\n", path);
}

int _write(int fd, char *ptr, int len)
{
    (void)fd;
    /////PRINTF("x");
    // extern void BOARD_UART_WriteBlocking(uint8_t *data, size_t len);
    // BOARD_UART_WriteBlocking((uint8_t *)ptr, len);
    PRINTF(ptr);
    return len;
}

int _close(int fd)
{
    PRINTF("GCOV_CLOSE\n");
}
#endif

// #pragma GCC push_options
// #pragma GCC optimize("O0")
__attribute__((used)) __attribute__((section(".ddr_data.init"))) __attribute__((aligned(32U))) static uint32_t IspPreloadComplete = 0xA5AA550F;

static void CameraTask(void *apParameters)
{
    int32_t lRet = 0;

    while (IspPreloadComplete != 0xA5AA550F)
    {
        PRINTF("USE LAUTERBACH to load ISP configuration.\r\n");
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "cfg img: %p\r\n", gIspCfg);
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "once done set %p to 0xA5AA550F\r\n", &IspPreloadComplete);
        ccl_Sleep(10);
    }

    PRINTF("ISP config loaded.\r\n");

    // initialize demo context
    if (InitCtx(&gAppCtx) != 0)
    {
        PRINTF("Failed to initialize application context.\r\n");
        lRet |= -1;
    } // if InitCtx() failed

    // set logging level - by default to ILOG_LEV_ERR, this can be changed by arguments ('-w', '-l')
    ilog_SetLevel(ILOG_LEV_ERR);

    utils_EdmaOpen();

    if (lRet == 0)
    {
        // **************** Initialize application *************************
        if (0 != Open(&gAppCtx))
        {
            PRINTF("Failed to initialize application.\r\n");
            lRet |= -1;
        }
        // **************** Start capturing ********************************
        else if (0 != Run(&gAppCtx))
        {
            PRINTF("Failed to execute camera pipeline.\r\n");
            lRet |= -1;
        }
        // ********************* Clean up **********************************
        Close(&gAppCtx);
    } // if demo to be executed

    PRINTF("Error: CCL task stoped.\r\n");

    vTaskDelete(NULL);
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/
void StartCameraTask(void)
{
    TaskHandle_t TaskHandle = xTaskCreateStatic(CameraTask, "Camera task", STACK_SIZE, NULL, 1, gStack, &gTaskBuffer);

    if (NULL == TaskHandle)
    {
        PRINTF("Camera task creation failed!.\r\n");
        while (1)
            ;
    }
}

