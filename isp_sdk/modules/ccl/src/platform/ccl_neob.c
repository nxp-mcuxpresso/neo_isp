/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  ccl_neob.c
 * @brief CCL NEOB platform implementation
 *******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "ilog.h"
#include "ccl_isp.h"
#include "ccl_cca.h"
#include "ccl_internal.h"

#include "mshr.h"

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/
#define WRITE_OUTBUF_CONFIG(reg_prefix, buf_type, addr_shift) \
    { \
        lpIspHwCfg->mRegs.mPIPE_CONF.reg_prefix##_ADDR_CAM0 = (uint32_t)( (apFrTsk->mImgBufsBasePhy[buf_type] >> addr_shift) & 0xFFFFFFFFU); \
    }

#define WRITE_INBUF_CONFIG(reg_prefix, buf_type, addr_shift) \
    { \
        lpIspHwCfg->mRegs.mPIPE_CONF.reg_prefix##_ADDR_CAM0 = (uint32_t)( (apFrTsk->FrameData.mImg[buf_type].mHwAddr >> addr_shift) & 0xFFFFFFFFU); \
    }

// The width of the line buffers can be maximum of 4096 pixels for NEOB
#define CCL_MAX_WIDTH 4096
// There is no restriction for height, max is 16 bit
#define CCL_MAX_HEIGHT UINT16_MAX
/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/**
 * @brief Enumeration for input buffer plane identifiers
 * @details specific to neob ISP hardware platform
 */
typedef enum
{
    CCL_INBUF_PLANE_IN0 = 0U,
    CCL_INBUF_PLANE_IN1 = 1U
}ccl_InBufPlane_t;
/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

uint8_t ccl_GetIspIbufBitsPP(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint32_t aPlaneId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    cca_CamInit_t *StreamCfg = &gCclState.mInitCfg.mpCams[aHwId][aCamId];
    uint8_t lBpp;

    if(StreamCfg->mcpExtDef == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "StreamCfg->mcpExtDef is NULL");
        lBpp = 0U;
    }
    else
    {
        switch (aPlaneId)
        {
            case CCL_INBUF_PLANE_IN0:
                lBpp = ccl_GetIspIbufBitsPPCode2Val(StreamCfg->mcpExtDef->mRegs.mPIPE_CONF.mIMG_CONF_CAM0.mIBPP0);
                break;

            case CCL_INBUF_PLANE_IN1:
                lBpp = ccl_GetIspIbufBitsPPCode2Val(StreamCfg->mcpExtDef->mRegs.mPIPE_CONF.mIMG_CONF_CAM0.mIBPP1);
                break;

            default:
                ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "StreamCfg->mcpExtDef is NULL");
                lBpp = 0U;
                break;
        } // switch (aPlaneId)
    } // else from if(StreamCfg->mcpExtDef == NULL)

    return lBpp;
} // ccl_GetIspIbufBitsPP()

/*******************************************************************************/

int32_t ccl_SetupImgBufsOut(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, ccl_FrameTask_t* apFrTsk, uint32_t aIspProcessingHeight)
{
    uint32_t lWidth  = apFrTsk->FrameData.mImg[CCA_IOBUFT_IN0].mWidth;
    assert(lWidth <= CCL_MAX_WIDTH);

    uint32_t lHeight = apFrTsk->FrameData.mImg[CCA_IOBUFT_IN0].mHeight;
    assert(lHeight <= CCL_MAX_HEIGHT && aIspProcessingHeight <= CCL_MAX_HEIGHT);

    cca_ImgBuf_t* lpIoBuf;
    cca_CamInit_t* StreamCfg = &gCclState.mInitCfg.mpCams[aHwId][aCamId];

    if (StreamCfg == NULL || StreamCfg->mcpExtDef == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "StreamCfg or mcpExtDef is NULL");
        return -1;
    }

    // check registers to understand output buffer config
    const isp_Packetizer_t* lrPacketizerRegs = &StreamCfg->mcpExtDef->mRegs.mPacketizer;

#if 0
  // force packetizer output format
  lrPacketizerRegs->PACK_CTRL_CAM0_TYPE = 1U; // 0 == semiplanar, 1 == interleaved
  lrPacketizerRegs->CH0_CTRL_CAM0_OBPP = 6; // 0: 12bpp, 2: 16bpp (CH0 only), 4: 10bpp, 6: 8bpp
  lrPacketizerRegs->CH12_CTRL_CAM0_OBPP = 6;
  lrPacketizerRegs->PACK_CTRL_CAM0_A0S = 0; // appends 0s
  lrPacketizerRegs->CH12_CTRL_CAM0_SUBSAMPLE = 0; // C12 subsumpling: 0: 444, 1: 422 h, 2: 420 h&v.

#endif
    // ** planar vs interleaved output
    if (lrPacketizerRegs->mPACK_CTRL_CAM0.mTYPE == 1U)
    {
        // use interleaved buffer
        lpIoBuf          = &apFrTsk->FrameData.mImg[CCA_IOBUFT_C012];
        lpIoBuf->mWidth  = lWidth;
        lpIoBuf->mHeight = lHeight;
        lpIoBuf->mChan   = 3U;
        // check bpp

        if ((lrPacketizerRegs->mCH0_CTRL_CAM0.mOBPP == 6U) && // Y is 8bpp
            (lrPacketizerRegs->mCH12_CTRL_CAM0.mOBPP == 6U) && // UV is 8bpp
            (lrPacketizerRegs->mPACK_CTRL_CAM0.mA0S == 0U)) // no 0 padding
        {
            if (lrPacketizerRegs->mCH12_CTRL_CAM0.mSUBSAMPLE == 0U) // no subsampling
            {
                lpIoBuf->mBpp = 24U;
            } // if no subsampling
            else // horizontal subsampling (YUV422)
            {
                lpIoBuf->mBpp = 16U; //2pix per 32bits 32/2
                lpIoBuf->mChan = 2U; // 2channels per byte
            } // else from if no subsampling
            // YUV420 interleaved NOT supported
        } // if 888
        else
        {
            // no subsampling supported in case of > 24bpp
            lpIoBuf->mBpp = 32U;
        } // else from if 888
        lpIoBuf->mSizeBytes = lpIoBuf->mWidth * aIspProcessingHeight * (lpIoBuf->mBpp / 8U);
        // ** Y and UV buffers are not used
        apFrTsk->FrameData.mImg[CCA_IOBUFT_C0].mSizeBytes  = 0U;
        apFrTsk->FrameData.mImg[CCA_IOBUFT_C12].mSizeBytes = 0U;
    } // if interleaved output
    else
    {
        // ** use Y buffer
        // no 0 padding support in semiplanar mode
        lpIoBuf          = &apFrTsk->FrameData.mImg[CCA_IOBUFT_C0];
        lpIoBuf->mWidth  = lWidth;
        lpIoBuf->mHeight = lHeight;

        if (lrPacketizerRegs->mCH0_CTRL_CAM0.mOBPP == 6U) // Y is 8bpp
        {
            lpIoBuf->mBpp = 8U;
        } // if 8bpp
        else
        {
            lpIoBuf->mBpp = 16U;
        } // else from if 8bpp
        lpIoBuf->mChan      = 1U;
        lpIoBuf->mSizeBytes = lpIoBuf->mWidth * aIspProcessingHeight * (lpIoBuf->mBpp / 8U);

        // ** use UV buffer
        lpIoBuf          = &apFrTsk->FrameData.mImg[CCA_IOBUFT_C12];
        lpIoBuf->mWidth  = lWidth;
        lpIoBuf->mHeight = lHeight;

        // for YUV420 semiplanar, also the height is changed
        //  so we can not use the aIspProcessingHeight directly for SizeBytes computation
        uint8_t lEmbbDataHeight = aIspProcessingHeight - lpIoBuf->mHeight;
        // apply subsampling
        lpIoBuf->mWidth /= (lrPacketizerRegs->mCH12_CTRL_CAM0.mSUBSAMPLE == 0U) ? 1U : 2U;
        lpIoBuf->mHeight /= (lrPacketizerRegs->mCH12_CTRL_CAM0.mSUBSAMPLE == 2U) ? 2U : 1U;

        if (lrPacketizerRegs->mCH12_CTRL_CAM0.mOBPP == 6U) // UV is 8bpp
        {
            lpIoBuf->mBpp = 16U; // 2chanels with 8bpp
        } // if 8bpp
        else
        {
            lpIoBuf->mBpp = 32U; // 2chanels with 16bpp
        } // else from if 8bpp
        lpIoBuf->mChan      = 2U;

        assert((lpIoBuf->mHeight + lEmbbDataHeight) <= CCL_MAX_HEIGHT);
        lpIoBuf->mSizeBytes = lpIoBuf->mWidth * (lpIoBuf->mHeight + lEmbbDataHeight) * (lpIoBuf->mBpp / 8U);

        // ** set YUV as not used
        apFrTsk->FrameData.mImg[CCA_IOBUFT_C012].mSizeBytes = 0U;
    } // else from if interleaved output

    // ** IR output
    if (StreamCfg->mcpExtDef->mRegs.mRGBIR.mCTRL_CAM0.mENABLE == 1U)
    {
        // use IR buffer, 16bpp
        lpIoBuf             = &apFrTsk->FrameData.mImg[CCA_IOBUFT_IR];
        lpIoBuf->mWidth     = lWidth;
        lpIoBuf->mHeight    = lHeight;
        lpIoBuf->mBpp       = (StreamCfg->mcpExtDef->mRegs.mIR_COMPRESS.mCTRL_CAM0.mOBPP == 0)? 8U: 16U;
        lpIoBuf->mChan      = 1U;
        lpIoBuf->mSizeBytes = lpIoBuf->mWidth * aIspProcessingHeight * (lpIoBuf->mBpp / 8U);
    } // if IR enabled
    else
    {
        apFrTsk->FrameData.mImg[CCA_IOBUFT_IR].mSizeBytes = 0U;
        apFrTsk->FrameData.mImg[CCA_IOBUFT_IR].mpData     = NULL;
    } // else from if IR enabled

#ifdef ISIM
    // ** ISIM specific RGB888 output allocate always
    lpIoBuf             = &apFrTsk->FrameData.mImg[CCA_IOBUFT_RGB];
    lpIoBuf->mWidth     = lWidth;
    lpIoBuf->mHeight    = lHeight;
    lpIoBuf->mBpp       = 24U; // always 888 => 24bits
    lpIoBuf->mChan      = 3U;
    lpIoBuf->mSizeBytes = lpIoBuf->mWidth * aIspProcessingHeight * 3U;

    apFrTsk->FrameData.mImg[CCA_IOBUFT_RGB].mpData  = MShr_malloc(aHwId, (Mu_ChannelId_t)(aCamId+1), apFrTsk->FrameData.mImg[CCA_IOBUFT_RGB].mSizeBytes, -1);
    if (NULL == apFrTsk->FrameData.mImg[CCA_IOBUFT_RGB].mpData)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Out buf alloc failed size: 0x%x, hwid: %d, camid: %d\n",  lpIoBuf->mSizeBytes, aHwId, aCamId);
        return -1;
    }
#endif // ifdef ISIM

    return 0;
}

/*******************************************************************************/

int32_t ccl_WriteIspBufConfig(ccl_FrameTask_t *apFrTsk, cca_IoBufTypes_t aBufType)
{
    if( apFrTsk == NULL || apFrTsk->mIspCfg == NULL)
    {
        ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "apFrTsk or mIspCfg is NULL");
        return -1;
    }
    isp_CtxHw_t *lpIspHwCfg = &apFrTsk->mIspCfg->mIspCtx.mCtxHw;

    /* All image addresses in ISP configs store only first 32 MSB the rest (4LSB) is considerd zero
    => all ISP image addresses must be aligned to 4bits */
    const uint8_t lcAddrShift = 4U;

    switch(aBufType)
    {
        case CCA_IOBUFT_IN0:
            WRITE_INBUF_CONFIG(mIMG0_IN, CCA_IOBUFT_IN0, lcAddrShift);
            break;
        case CCA_IOBUFT_IN1:
            WRITE_INBUF_CONFIG(mIMG1_IN, CCA_IOBUFT_IN1, lcAddrShift);
            break;
        case CCA_IOBUFT_C012:
            WRITE_OUTBUF_CONFIG(mOUTCH1, CCA_IOBUFT_C012, lcAddrShift);
            break;
        case CCA_IOBUFT_C0:
            WRITE_OUTBUF_CONFIG(mOUTCH0, CCA_IOBUFT_C0, lcAddrShift);
            break;
         case CCA_IOBUFT_C12:
            WRITE_OUTBUF_CONFIG(mOUTCH1, CCA_IOBUFT_C12, lcAddrShift);
            break;
        case CCA_IOBUFT_IR:
            WRITE_OUTBUF_CONFIG(mOUTIR, CCA_IOBUFT_IR, lcAddrShift);
            break;
#ifdef ISIM
        case CCA_IOBUFT_RGB:
            // RGB buffer is ISIM-only (simulated), no HW registers to write
            lpIspHwCfg->mpIsimRgb = apFrTsk->FrameData.mImg[CCA_IOBUFT_RGB].mHwAddr;
            break;
#endif
        case CCA_IOBUFT_CNT:
            // Not an actual buffer type
            ILOG(ILOG_MOD_CCL, ILOG_LEV_ERR, "Invalid buffer type: CCA_IOBUFT_CNT");
            return -1;
    }

    return 0;
}

/*******************************************************************************/

void ccl_CopyIspCfg(isp_CtxHw_t *apDst, const isp_CtxHw_t *apSrc)
{
    // copy only relevant config (e.g. skip buffer addresses and alias region)
    cca_CopyIspCfg(apDst, apSrc);
} // ccl_CopyIspCfg()
