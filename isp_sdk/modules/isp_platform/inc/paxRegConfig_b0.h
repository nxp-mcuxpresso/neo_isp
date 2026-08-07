/*******************************************************************************
 * Copyright  2021-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

//****************************************************************************
// File copied from ISIM (C_simulator/src/imx9x/registers/registers.h)
// ISIM Commit: fde6c3cb5de2c53a78bcc9b815c258ebddefe8ba
//
// NOTE: Use following command to extract the commit ID:
// sed -E -n 's/\/\/ ISIM Commit: ([0-9,a-f]{40})/\1/p' registers.h
//****************************************************************************

#ifndef __include_RegConfig_h
#define __include_RegConfig_h

typedef volatile uint32_t vuint32_t;
typedef volatile int32_t  vint32_t;

typedef struct
{
    vuint32_t mSOFT_RESET : 1;
    vuint32_t mHARD_RESET : 1;
    uint32_t  mUnused2 : 30;
} isp_PIPE_CONF_SOFT_RESET_t;

typedef struct
{
    vuint32_t mOTLT : 8;
    vuint32_t mOTHT : 8;
    vuint32_t mPOSTQOS : 8;
    vuint32_t mBSIZE : 8;
} isp_PIPE_CONF_BUS_TXPARAM_t;

typedef struct
{
    uint32_t  mUnused0 : 31;
    vuint32_t mXFR_ERR_DIS : 1;
} isp_PIPE_CONF_REG_XFR_DIS_t;

typedef struct
{
    vuint32_t mVID0 : 5;
    uint32_t  mUnused5 : 3;
    vuint32_t mVID1 : 5;
    uint32_t  mUnused13 : 16;
    vuint32_t mSSEN : 1;
    vuint32_t mDEVL : 1;
    vuint32_t mCSI_EN : 1;
} isp_PIPE_CONF_CSI_CTRL_t;

typedef struct
{
    vuint32_t mCURR_FRAME : 16;
    vuint32_t mSHD_FRAME : 16;
} isp_PIPE_CONF_FRAME_NUM_t;

typedef struct
{
    uint32_t  mUnused0 : 31;
    vuint32_t mCTRL : 1;
} isp_PIPE_CONF_REG_SHD_CTRL_t;

typedef struct
{
    vuint32_t mEN_FS1 : 1;
    vuint32_t mEN_FS2 : 1;
    vuint32_t mEN_FD1 : 1;
    vuint32_t mEN_FD2 : 1;
    vuint32_t mEN_STATD : 1;
    vuint32_t mEN_DRCD : 1;
    uint32_t  mUnused6 : 10;
    vuint32_t mEN_BUS_ERR : 4;
    uint32_t  mUnused20 : 9;
    vuint32_t mEN_CSI_TERR : 1;
    vuint32_t mEN_TRIG_ERR : 1;
    uint32_t  mUnused31 : 1;
} isp_PIPE_CONF_INT_EN0_t;

typedef struct
{
    vuint32_t mS_FS1 : 1;
    vuint32_t mS_FS2 : 1;
    vuint32_t mS_FD1 : 1;
    vuint32_t mS_FD2 : 1;
    vuint32_t mS_STATD : 1;
    vuint32_t mS_DRCD : 1;
    uint32_t  mUnused6 : 10;
    vuint32_t mS_BUS_ERR : 4;
    uint32_t  mUnused20 : 9;
    vuint32_t mS_CSI_TERR : 1;
    vuint32_t mS_TRIG_ERR : 1;
    vuint32_t mBUSY : 1;
} isp_PIPE_CONF_INT_STAT0_t;

typedef struct
{
    vuint32_t mS_SL_LP0 : 1;
    vuint32_t mS_SF_LP0 : 1;
    vuint32_t mS_DO_LP0 : 1;
    vuint32_t mS_LOC_LP0 : 1;
    vuint32_t mS_LO_LP0 : 1;
    vuint32_t mS_CMD_LP0 : 1;
    vuint32_t mS_LL_LP0 : 1;
    vuint32_t mS_DATA_LP0 : 1;
    uint32_t  mUnused8 : 8;
    vuint32_t mS_SL_LP1 : 1;
    vuint32_t mS_SF_LP1 : 1;
    vuint32_t mS_DO_LP1 : 1;
    vuint32_t mS_LOC_LP1 : 1;
    vuint32_t mS_LO_LP1 : 1;
    vuint32_t mS_CMD_LP1 : 1;
    vuint32_t mS_LL_LP1 : 1;
    vuint32_t mS_DATA_LP1 : 1;
    uint32_t  mUnused24 : 7;
    vuint32_t mS_STOP : 1;
} isp_PIPE_CONF_CSI_STAT_t;

typedef struct
{
    vuint32_t mIBPP0 : 4;
    vuint32_t mINALIGN0 : 1;
    vuint32_t mLPALIGN0 : 1;
    uint32_t  mUnused6 : 10;
    vuint32_t mIBPP1 : 4;
    vuint32_t mINALIGN1 : 1;
    vuint32_t mLPALIGN1 : 1;
    uint32_t  mUnused22 : 10;
} isp_PIPE_CONF_IMG_CONF_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_PIPE_CONF_IMG_SIZE_CAM0_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mLS : 28;
} isp_PIPE_CONF_IMG0_IN_LS_CAM0_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mLS : 28;
} isp_PIPE_CONF_IMG1_IN_LS_CAM0_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mLS : 28;
} isp_PIPE_CONF_OUTCH0_LS_CAM0_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mLS : 28;
} isp_PIPE_CONF_OUTCH1_LS_CAM0_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mLS : 28;
} isp_PIPE_CONF_OUTIR_LS_CAM0_t;

typedef struct
{
    vuint32_t mPRESKIP : 16;
    vuint32_t mPOSTSKIP : 16;
} isp_PIPE_CONF_SKIP_CTRL0_t;

typedef struct
{
    isp_PIPE_CONF_SOFT_RESET_t      mSOFT_RESET;
    isp_PIPE_CONF_BUS_TXPARAM_t     mBUS_TXPARAM;
    isp_PIPE_CONF_REG_XFR_DIS_t     mREG_XFR_DIS;
    uint32_t                        mpUnused0[1];
    isp_PIPE_CONF_CSI_CTRL_t        mCSI_CTRL;
    isp_PIPE_CONF_FRAME_NUM_t       mFRAME_NUM;
    isp_PIPE_CONF_REG_SHD_CTRL_t    mREG_SHD_CTRL;
    vuint32_t                       mREG_SHD_CMD;
    vuint32_t                       mTRIG_CAM0;
    isp_PIPE_CONF_INT_EN0_t         mINT_EN0;
    isp_PIPE_CONF_INT_STAT0_t       mINT_STAT0;
    isp_PIPE_CONF_CSI_STAT_t        mCSI_STAT;
    isp_PIPE_CONF_IMG_CONF_CAM0_t   mIMG_CONF_CAM0;
    isp_PIPE_CONF_IMG_SIZE_CAM0_t   mIMG_SIZE_CAM0;
    uint32_t                        mpUnused1[1];
    vuint32_t                       mIMG0_IN_ADDR_CAM0;
    vuint32_t                       mIMG1_IN_ADDR_CAM0;
    vuint32_t                       mOUTCH0_ADDR_CAM0;
    vuint32_t                       mOUTCH1_ADDR_CAM0;
    vuint32_t                       mOUTIR_ADDR_CAM0;
    isp_PIPE_CONF_IMG0_IN_LS_CAM0_t mIMG0_IN_LS_CAM0;
    isp_PIPE_CONF_IMG1_IN_LS_CAM0_t mIMG1_IN_LS_CAM0;
    isp_PIPE_CONF_OUTCH0_LS_CAM0_t  mOUTCH0_LS_CAM0;
    isp_PIPE_CONF_OUTCH1_LS_CAM0_t  mOUTCH1_LS_CAM0;
    isp_PIPE_CONF_OUTIR_LS_CAM0_t   mOUTIR_LS_CAM0;
    isp_PIPE_CONF_SKIP_CTRL0_t      mSKIP_CTRL0;
} isp_PIPE_CONF_t;

typedef struct
{
    vuint32_t mHOFFSET : 2;
    vuint32_t mVOFFSET : 2;
    uint32_t  mUnused4 : 28;
} isp_HC_CTRL_CAM0_t;

typedef struct
{
    isp_HC_CTRL_CAM0_t mCTRL_CAM0;
} isp_HC_t;

typedef struct
{
    uint32_t  mUnused0 : 31;
    vuint32_t mENABLE : 1;
} isp_HDR_Decompress0_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_POINT1_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_POINT2_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_POINT3_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_POINT4_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_OFFSET0_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_OFFSET1_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_OFFSET2_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_OFFSET3_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress0_KNEE_OFFSET4_CAM0_t;

typedef struct
{
    vuint32_t mRATIO0 : 12;
    uint32_t  mUnused12 : 4;
    vuint32_t mRATIO1 : 12;
    uint32_t  mUnused28 : 4;
} isp_HDR_Decompress0_KNEE_RATIO01_CAM0_t;

typedef struct
{
    vuint32_t mRATIO2 : 12;
    uint32_t  mUnused12 : 4;
    vuint32_t mRATIO3 : 12;
    uint32_t  mUnused28 : 4;
} isp_HDR_Decompress0_KNEE_RATIO23_CAM0_t;

typedef struct
{
    vuint32_t mRATIO4 : 12;
    uint32_t  mUnused12 : 20;
} isp_HDR_Decompress0_KNEE_RATIO4_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_HDR_Decompress0_KNEE_NPOINT0_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_HDR_Decompress0_KNEE_NPOINT1_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_HDR_Decompress0_KNEE_NPOINT2_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_HDR_Decompress0_KNEE_NPOINT3_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_HDR_Decompress0_KNEE_NPOINT4_CAM0_t;

typedef struct
{
    isp_HDR_Decompress0_CTRL_CAM0_t         mCTRL_CAM0;
    isp_HDR_Decompress0_KNEE_POINT1_CAM0_t  mKNEE_POINT1_CAM0;
    isp_HDR_Decompress0_KNEE_POINT2_CAM0_t  mKNEE_POINT2_CAM0;
    isp_HDR_Decompress0_KNEE_POINT3_CAM0_t  mKNEE_POINT3_CAM0;
    isp_HDR_Decompress0_KNEE_POINT4_CAM0_t  mKNEE_POINT4_CAM0;
    isp_HDR_Decompress0_KNEE_OFFSET0_CAM0_t mKNEE_OFFSET0_CAM0;
    isp_HDR_Decompress0_KNEE_OFFSET1_CAM0_t mKNEE_OFFSET1_CAM0;
    isp_HDR_Decompress0_KNEE_OFFSET2_CAM0_t mKNEE_OFFSET2_CAM0;
    isp_HDR_Decompress0_KNEE_OFFSET3_CAM0_t mKNEE_OFFSET3_CAM0;
    isp_HDR_Decompress0_KNEE_OFFSET4_CAM0_t mKNEE_OFFSET4_CAM0;
    isp_HDR_Decompress0_KNEE_RATIO01_CAM0_t mKNEE_RATIO01_CAM0;
    isp_HDR_Decompress0_KNEE_RATIO23_CAM0_t mKNEE_RATIO23_CAM0;
    isp_HDR_Decompress0_KNEE_RATIO4_CAM0_t  mKNEE_RATIO4_CAM0;
    isp_HDR_Decompress0_KNEE_NPOINT0_CAM0_t mKNEE_NPOINT0_CAM0;
    isp_HDR_Decompress0_KNEE_NPOINT1_CAM0_t mKNEE_NPOINT1_CAM0;
    isp_HDR_Decompress0_KNEE_NPOINT2_CAM0_t mKNEE_NPOINT2_CAM0;
    isp_HDR_Decompress0_KNEE_NPOINT3_CAM0_t mKNEE_NPOINT3_CAM0;
    isp_HDR_Decompress0_KNEE_NPOINT4_CAM0_t mKNEE_NPOINT4_CAM0;
} isp_HDR_Decompress0_t;

typedef struct
{
    uint32_t  mUnused0 : 31;
    vuint32_t mENABLE : 1;
} isp_HDR_Decompress1_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_POINT1_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_POINT2_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_POINT3_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_POINT4_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_OFFSET0_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_OFFSET1_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_OFFSET2_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_OFFSET3_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_OFFSET4_CAM0_t;

typedef struct
{
    vuint32_t mRATIO0 : 12;
    uint32_t  mUnused12 : 4;
    vuint32_t mRATIO1 : 12;
    uint32_t  mUnused28 : 4;
} isp_HDR_Decompress1_KNEE_RATIO01_CAM0_t;

typedef struct
{
    vuint32_t mRATIO2 : 12;
    uint32_t  mUnused12 : 4;
    vuint32_t mRATIO3 : 12;
    uint32_t  mUnused28 : 4;
} isp_HDR_Decompress1_KNEE_RATIO23_CAM0_t;

typedef struct
{
    vuint32_t mRATIO4 : 12;
    uint32_t  mUnused12 : 20;
} isp_HDR_Decompress1_KNEE_RATIO4_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_NPOINT0_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_NPOINT1_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_NPOINT2_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_NPOINT3_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_Decompress1_KNEE_NPOINT4_CAM0_t;

typedef struct
{
    isp_HDR_Decompress1_CTRL_CAM0_t         mCTRL_CAM0;
    isp_HDR_Decompress1_KNEE_POINT1_CAM0_t  mKNEE_POINT1_CAM0;
    isp_HDR_Decompress1_KNEE_POINT2_CAM0_t  mKNEE_POINT2_CAM0;
    isp_HDR_Decompress1_KNEE_POINT3_CAM0_t  mKNEE_POINT3_CAM0;
    isp_HDR_Decompress1_KNEE_POINT4_CAM0_t  mKNEE_POINT4_CAM0;
    isp_HDR_Decompress1_KNEE_OFFSET0_CAM0_t mKNEE_OFFSET0_CAM0;
    isp_HDR_Decompress1_KNEE_OFFSET1_CAM0_t mKNEE_OFFSET1_CAM0;
    isp_HDR_Decompress1_KNEE_OFFSET2_CAM0_t mKNEE_OFFSET2_CAM0;
    isp_HDR_Decompress1_KNEE_OFFSET3_CAM0_t mKNEE_OFFSET3_CAM0;
    isp_HDR_Decompress1_KNEE_OFFSET4_CAM0_t mKNEE_OFFSET4_CAM0;
    isp_HDR_Decompress1_KNEE_RATIO01_CAM0_t mKNEE_RATIO01_CAM0;
    isp_HDR_Decompress1_KNEE_RATIO23_CAM0_t mKNEE_RATIO23_CAM0;
    isp_HDR_Decompress1_KNEE_RATIO4_CAM0_t  mKNEE_RATIO4_CAM0;
    isp_HDR_Decompress1_KNEE_NPOINT0_CAM0_t mKNEE_NPOINT0_CAM0;
    isp_HDR_Decompress1_KNEE_NPOINT1_CAM0_t mKNEE_NPOINT1_CAM0;
    isp_HDR_Decompress1_KNEE_NPOINT2_CAM0_t mKNEE_NPOINT2_CAM0;
    isp_HDR_Decompress1_KNEE_NPOINT3_CAM0_t mKNEE_NPOINT3_CAM0;
    isp_HDR_Decompress1_KNEE_NPOINT4_CAM0_t mKNEE_NPOINT4_CAM0;
} isp_HDR_Decompress1_t;

typedef struct
{
    uint32_t  mUnused0 : 2;
    vuint32_t mOBPP : 2;
    uint32_t  mUnused4 : 28;
} isp_OB_WB0_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB0_R_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB0_GR_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB0_GB_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB0_B_CTRL_CAM0_t;

typedef struct
{
    isp_OB_WB0_CTRL_CAM0_t    mCTRL_CAM0;
    isp_OB_WB0_R_CTRL_CAM0_t  mR_CTRL_CAM0;
    isp_OB_WB0_GR_CTRL_CAM0_t mGR_CTRL_CAM0;
    isp_OB_WB0_GB_CTRL_CAM0_t mGB_CTRL_CAM0;
    isp_OB_WB0_B_CTRL_CAM0_t  mB_CTRL_CAM0;
} isp_OB_WB0_t;

typedef struct
{
    uint32_t  mUnused0 : 2;
    vuint32_t mOBPP : 2;
    uint32_t  mUnused4 : 28;
} isp_OB_WB1_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB1_R_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB1_GR_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB1_GB_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB1_B_CTRL_CAM0_t;

typedef struct
{
    isp_OB_WB1_CTRL_CAM0_t    mCTRL_CAM0;
    isp_OB_WB1_R_CTRL_CAM0_t  mR_CTRL_CAM0;
    isp_OB_WB1_GR_CTRL_CAM0_t mGR_CTRL_CAM0;
    isp_OB_WB1_GB_CTRL_CAM0_t mGB_CTRL_CAM0;
    isp_OB_WB1_B_CTRL_CAM0_t  mB_CTRL_CAM0;
} isp_OB_WB1_t;

typedef struct
{
    uint32_t  mUnused0 : 2;
    vuint32_t mOBPP : 2;
    uint32_t  mUnused4 : 28;
} isp_OB_WB2_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB2_R_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB2_GR_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB2_GB_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    vuint32_t mGAIN : 16;
} isp_OB_WB2_B_CTRL_CAM0_t;

typedef struct
{
    isp_OB_WB2_CTRL_CAM0_t    mCTRL_CAM0;
    isp_OB_WB2_R_CTRL_CAM0_t  mR_CTRL_CAM0;
    isp_OB_WB2_GR_CTRL_CAM0_t mGR_CTRL_CAM0;
    isp_OB_WB2_GB_CTRL_CAM0_t mGB_CTRL_CAM0;
    isp_OB_WB2_B_CTRL_CAM0_t  mB_CTRL_CAM0;
} isp_OB_WB2_t;

typedef struct
{
    uint32_t  mUnused0 : 2;
    vuint32_t mOBPP : 2;
    vuint32_t mSAFETY_ON : 1;
    uint32_t  mUnused5 : 3;
    vuint32_t mMOTION_FIX_EN : 1;
    vuint32_t mBLEND_3X3 : 1;
    uint32_t  mUnused10 : 6;
    vuint32_t mGAIN0BPP : 2;
    vuint32_t mGAIN1BPP : 2;
    uint32_t  mUnused20 : 11;
    vuint32_t mENABLE : 1;
} isp_HDR_MERGE_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET0 : 16;
    vuint32_t mOFFSET1 : 16;
} isp_HDR_MERGE_GAIN_OFFSET_CAM0_t;

typedef struct
{
    vuint32_t mSCALE0 : 16;
    vuint32_t mSCALE1 : 16;
} isp_HDR_MERGE_GAIN_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT0 : 5;
    uint32_t  mUnused5 : 11;
    vuint32_t mSHIFT1 : 5;
    uint32_t  mUnused21 : 11;
} isp_HDR_MERGE_GAIN_SHIFT_CAM0_t;

typedef struct
{
    vuint32_t mTH0 : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_MERGE_LUMA_TH_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 3;
    vuint32_t mTHSHIFT : 5;
    uint32_t  mUnused29 : 3;
} isp_HDR_MERGE_LUMA_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mIMGSCALE0 : 5;
    uint32_t  mUnused5 : 11;
    vuint32_t mIMGSCALE1 : 5;
    uint32_t  mUnused21 : 11;
} isp_HDR_MERGE_DOWNSCALE_CAM0_t;

typedef struct
{
    vuint32_t mIMGSCALE0 : 4;
    uint32_t  mUnused4 : 12;
    vuint32_t mIMGSCALE1 : 4;
    uint32_t  mUnused20 : 12;
} isp_HDR_MERGE_UPSCALE_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 5;
    uint32_t  mUnused5 : 27;
} isp_HDR_MERGE_POST_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET0 : 16;
    vuint32_t mOFFSET1 : 16;
} isp_HDR_MERGE_S_GAIN_OFFSET_CAM0_t;

typedef struct
{
    vuint32_t mSCALE0 : 16;
    vuint32_t mSCALE1 : 16;
} isp_HDR_MERGE_S_GAIN_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT0 : 5;
    uint32_t  mUnused5 : 11;
    vuint32_t mSHIFT1 : 5;
    uint32_t  mUnused21 : 11;
} isp_HDR_MERGE_S_GAIN_SHIFT_CAM0_t;

typedef struct
{
    vuint32_t mTH0 : 16;
    uint32_t  mUnused16 : 16;
} isp_HDR_MERGE_S_LUMA_TH_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 3;
    vuint32_t mTHSHIFT : 5;
    uint32_t  mUnused29 : 3;
} isp_HDR_MERGE_S_LUMA_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mIMGSCALE0 : 5;
    uint32_t  mUnused5 : 11;
    vuint32_t mIMGSCALE1 : 5;
    uint32_t  mUnused21 : 11;
} isp_HDR_MERGE_S_DOWNSCALE_CAM0_t;

typedef struct
{
    vuint32_t mIMGSCALE0 : 4;
    uint32_t  mUnused4 : 12;
    vuint32_t mIMGSCALE1 : 4;
    uint32_t  mUnused20 : 12;
} isp_HDR_MERGE_S_UPSCALE_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 5;
    uint32_t  mUnused5 : 27;
} isp_HDR_MERGE_S_POST_SCALE_CAM0_t;

typedef struct
{
    uint32_t  mUnused0 : 1;
    vuint32_t mLINE : 15;
    uint32_t  mUnused16 : 16;
} isp_HDR_MERGE_S_LINE_NUM_CAM0_t;

typedef struct
{
    isp_HDR_MERGE_CTRL_CAM0_t          mCTRL_CAM0;
    isp_HDR_MERGE_GAIN_OFFSET_CAM0_t   mGAIN_OFFSET_CAM0;
    isp_HDR_MERGE_GAIN_SCALE_CAM0_t    mGAIN_SCALE_CAM0;
    isp_HDR_MERGE_GAIN_SHIFT_CAM0_t    mGAIN_SHIFT_CAM0;
    isp_HDR_MERGE_LUMA_TH_CAM0_t       mLUMA_TH_CAM0;
    isp_HDR_MERGE_LUMA_SCALE_CAM0_t    mLUMA_SCALE_CAM0;
    isp_HDR_MERGE_DOWNSCALE_CAM0_t     mDOWNSCALE_CAM0;
    isp_HDR_MERGE_UPSCALE_CAM0_t       mUPSCALE_CAM0;
    isp_HDR_MERGE_POST_SCALE_CAM0_t    mPOST_SCALE_CAM0;
    isp_HDR_MERGE_S_GAIN_OFFSET_CAM0_t mS_GAIN_OFFSET_CAM0;
    isp_HDR_MERGE_S_GAIN_SCALE_CAM0_t  mS_GAIN_SCALE_CAM0;
    isp_HDR_MERGE_S_GAIN_SHIFT_CAM0_t  mS_GAIN_SHIFT_CAM0;
    isp_HDR_MERGE_S_LUMA_TH_CAM0_t     mS_LUMA_TH_CAM0;
    isp_HDR_MERGE_S_LUMA_SCALE_CAM0_t  mS_LUMA_SCALE_CAM0;
    isp_HDR_MERGE_S_DOWNSCALE_CAM0_t   mS_DOWNSCALE_CAM0;
    isp_HDR_MERGE_S_UPSCALE_CAM0_t     mS_UPSCALE_CAM0;
    isp_HDR_MERGE_S_POST_SCALE_CAM0_t  mS_POST_SCALE_CAM0;
    isp_HDR_MERGE_S_LINE_NUM_CAM0_t    mS_LINE_NUM_CAM0;
} isp_HDR_MERGE_t;

typedef struct
{
    vuint32_t mIBPP : 2;
    uint32_t  mUnused2 : 2;
    vuint32_t mCSCON : 1;
    uint32_t  mUnused5 : 26;
    vuint32_t mENABLE : 1;
} isp_COLOR_TEMP_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_COLOR_TEMP_ROI_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_COLOR_TEMP_ROI_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mMIN : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mMAX : 8;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_REDGAIN_CAM0_t;

typedef struct
{
    vuint32_t mMIN : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mMAX : 8;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_BLUEGAIN_CAM0_t;

typedef struct
{
    vuint32_t mBLUE : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mRED : 8;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_POINT1_CAM0_t;

typedef struct
{
    vuint32_t mBLUE : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mRED : 8;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_POINT2_CAM0_t;

typedef struct
{
    vuint32_t mRIGHT : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mLEFT : 8;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_HOFFSET_CAM0_t;

typedef struct
{
    vuint32_t mUP : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mDOWN : 8;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_VOFFSET_CAM0_t;

typedef struct
{
    vint32_t mSLOPE_L : 16;
    vint32_t mSLOPE_R : 16;
} isp_COLOR_TEMP_POINT1_SLOPE_CAM0_t;

typedef struct
{
    vint32_t mSLOPE_L : 16;
    vint32_t mSLOPE_R : 16;
} isp_COLOR_TEMP_POINT2_SLOPE_CAM0_t;

typedef struct
{
    vuint32_t mTHL : 16;
    vuint32_t mTHH : 16;
} isp_COLOR_TEMP_LUMA_TH_CAM0_t;

typedef struct
{
    vint32_t mR0C0 : 16;
    vint32_t mR0C1 : 16;
} isp_COLOR_TEMP_CSC_MAT0_CAM0_t;

typedef struct
{
    vint32_t mR0C2 : 16;
    vint32_t mR1C0 : 16;
} isp_COLOR_TEMP_CSC_MAT1_CAM0_t;

typedef struct
{
    vint32_t mR1C1 : 16;
    vint32_t mR1C2 : 16;
} isp_COLOR_TEMP_CSC_MAT2_CAM0_t;

typedef struct
{
    vint32_t mR2C0 : 16;
    vint32_t mR2C1 : 16;
} isp_COLOR_TEMP_CSC_MAT3_CAM0_t;

typedef struct
{
    vint32_t mR2C2 : 16;
    uint32_t mUnused16 : 16;
} isp_COLOR_TEMP_CSC_MAT4_CAM0_t;

typedef struct
{
    vint32_t mOFFSET0 : 16;
    vint32_t mOFFSET1 : 16;
} isp_COLOR_TEMP_R_GR_OFFSET_CAM0_t;

typedef struct
{
    vint32_t mOFFSET0 : 16;
    vint32_t mOFFSET1 : 16;
} isp_COLOR_TEMP_GB_B_OFFSET_CAM0_t;

typedef struct
{
    vuint32_t mXSIZE : 16;
    vuint32_t mYSIZE : 16;
} isp_COLOR_TEMP_STAT_BLK_SIZE0_t;

typedef struct
{
    vuint32_t mBLKLNE : 16;
    vuint32_t mBLKROW : 3;
    uint32_t  mUnused19 : 13;
} isp_COLOR_TEMP_STAT_CURR_BLK_Y0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI0_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI0_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI1_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI1_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI2_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI2_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI3_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI3_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI4_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI4_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI5_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI5_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI6_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI6_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI7_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI7_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI8_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI8_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mROVERG_LOW : 8;
    vuint32_t mROVERG_HIGH : 8;
    vuint32_t mBOVERG_LOW : 8;
    vuint32_t mBOVERG_HIGH : 8;
} isp_COLOR_TEMP_CROI9_POS_CAM0_t;

typedef struct
{
    vuint32_t mPIXCNT : 24;
    uint32_t  mUnused24 : 8;
} isp_COLOR_TEMP_CROI9_PIXCNT_CAM0_t;

typedef struct
{
    vuint32_t mGR_AGV : 20;
    uint32_t  mUnused20 : 12;
} isp_COLOR_TEMP_GR_AVG_IN_CAM0_t;

typedef struct
{
    vuint32_t mGB_AGV : 20;
    uint32_t  mUnused20 : 12;
} isp_COLOR_TEMP_GB_AVG_IN_CAM0_t;

typedef struct
{
    isp_COLOR_TEMP_CTRL_CAM0_t         mCTRL_CAM0;
    isp_COLOR_TEMP_ROI_POS_CAM0_t      mROI_POS_CAM0;
    isp_COLOR_TEMP_ROI_SIZE_CAM0_t     mROI_SIZE_CAM0;
    isp_COLOR_TEMP_REDGAIN_CAM0_t      mREDGAIN_CAM0;
    isp_COLOR_TEMP_BLUEGAIN_CAM0_t     mBLUEGAIN_CAM0;
    isp_COLOR_TEMP_POINT1_CAM0_t       mPOINT1_CAM0;
    isp_COLOR_TEMP_POINT2_CAM0_t       mPOINT2_CAM0;
    isp_COLOR_TEMP_HOFFSET_CAM0_t      mHOFFSET_CAM0;
    isp_COLOR_TEMP_VOFFSET_CAM0_t      mVOFFSET_CAM0;
    isp_COLOR_TEMP_POINT1_SLOPE_CAM0_t mPOINT1_SLOPE_CAM0;
    isp_COLOR_TEMP_POINT2_SLOPE_CAM0_t mPOINT2_SLOPE_CAM0;
    isp_COLOR_TEMP_LUMA_TH_CAM0_t      mLUMA_TH_CAM0;
    isp_COLOR_TEMP_CSC_MAT0_CAM0_t     mCSC_MAT0_CAM0;
    isp_COLOR_TEMP_CSC_MAT1_CAM0_t     mCSC_MAT1_CAM0;
    isp_COLOR_TEMP_CSC_MAT2_CAM0_t     mCSC_MAT2_CAM0;
    isp_COLOR_TEMP_CSC_MAT3_CAM0_t     mCSC_MAT3_CAM0;
    isp_COLOR_TEMP_CSC_MAT4_CAM0_t     mCSC_MAT4_CAM0;
    isp_COLOR_TEMP_R_GR_OFFSET_CAM0_t  mR_GR_OFFSET_CAM0;
    isp_COLOR_TEMP_GB_B_OFFSET_CAM0_t  mGB_B_OFFSET_CAM0;
    vuint32_t                          mCNT_WHITE_CAM0;
    vuint32_t                          mSUMRL_CAM0;
    vuint32_t                          mSUMRH_CAM0;
    vuint32_t                          mSUMGL_CAM0;
    vuint32_t                          mSUMGH_CAM0;
    vuint32_t                          mSUMBL_CAM0;
    vuint32_t                          mSUMBH_CAM0;
    vuint32_t                          mSUMRGL_CAM0;
    vuint32_t                          mSUMRGH_CAM0;
    vuint32_t                          mSUMBGL_CAM0;
    vuint32_t                          mSUMBGH_CAM0;
    uint32_t                           mpUnused0[2];
    isp_COLOR_TEMP_STAT_BLK_SIZE0_t    mSTAT_BLK_SIZE0;
    uint32_t                           mpUnused1[1];
    isp_COLOR_TEMP_STAT_CURR_BLK_Y0_t  mSTAT_CURR_BLK_Y0;
    uint32_t                           mpUnused2[1];
    isp_COLOR_TEMP_CROI0_POS_CAM0_t    mCROI0_POS_CAM0;
    uint32_t                           mpUnused3[1];
    isp_COLOR_TEMP_CROI0_PIXCNT_CAM0_t mCROI0_PIXCNT_CAM0;
    vuint32_t                          mCROI0_SUMRED_CAM0;
    vuint32_t                          mCROI0_SUMGREEN_CAM0;
    vuint32_t                          mCROI0_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI1_POS_CAM0_t    mCROI1_POS_CAM0;
    uint32_t                           mpUnused4[1];
    isp_COLOR_TEMP_CROI1_PIXCNT_CAM0_t mCROI1_PIXCNT_CAM0;
    vuint32_t                          mCROI1_SUMRED_CAM0;
    vuint32_t                          mCROI1_SUMGREEN_CAM0;
    vuint32_t                          mCROI1_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI2_POS_CAM0_t    mCROI2_POS_CAM0;
    uint32_t                           mpUnused5[1];
    isp_COLOR_TEMP_CROI2_PIXCNT_CAM0_t mCROI2_PIXCNT_CAM0;
    vuint32_t                          mCROI2_SUMRED_CAM0;
    vuint32_t                          mCROI2_SUMGREEN_CAM0;
    vuint32_t                          mCROI2_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI3_POS_CAM0_t    mCROI3_POS_CAM0;
    uint32_t                           mpUnused6[1];
    isp_COLOR_TEMP_CROI3_PIXCNT_CAM0_t mCROI3_PIXCNT_CAM0;
    vuint32_t                          mCROI3_SUMRED_CAM0;
    vuint32_t                          mCROI3_SUMGREEN_CAM0;
    vuint32_t                          mCROI3_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI4_POS_CAM0_t    mCROI4_POS_CAM0;
    uint32_t                           mpUnused7[1];
    isp_COLOR_TEMP_CROI4_PIXCNT_CAM0_t mCROI4_PIXCNT_CAM0;
    vuint32_t                          mCROI4_SUMRED_CAM0;
    vuint32_t                          mCROI4_SUMGREEN_CAM0;
    vuint32_t                          mCROI4_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI5_POS_CAM0_t    mCROI5_POS_CAM0;
    uint32_t                           mpUnused8[1];
    isp_COLOR_TEMP_CROI5_PIXCNT_CAM0_t mCROI5_PIXCNT_CAM0;
    vuint32_t                          mCROI5_SUMRED_CAM0;
    vuint32_t                          mCROI5_SUMGREEN_CAM0;
    vuint32_t                          mCROI5_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI6_POS_CAM0_t    mCROI6_POS_CAM0;
    uint32_t                           mpUnused9[1];
    isp_COLOR_TEMP_CROI6_PIXCNT_CAM0_t mCROI6_PIXCNT_CAM0;
    vuint32_t                          mCROI6_SUMRED_CAM0;
    vuint32_t                          mCROI6_SUMGREEN_CAM0;
    vuint32_t                          mCROI6_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI7_POS_CAM0_t    mCROI7_POS_CAM0;
    uint32_t                           mpUnused10[1];
    isp_COLOR_TEMP_CROI7_PIXCNT_CAM0_t mCROI7_PIXCNT_CAM0;
    vuint32_t                          mCROI7_SUMRED_CAM0;
    vuint32_t                          mCROI7_SUMGREEN_CAM0;
    vuint32_t                          mCROI7_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI8_POS_CAM0_t    mCROI8_POS_CAM0;
    uint32_t                           mpUnused11[1];
    isp_COLOR_TEMP_CROI8_PIXCNT_CAM0_t mCROI8_PIXCNT_CAM0;
    vuint32_t                          mCROI8_SUMRED_CAM0;
    vuint32_t                          mCROI8_SUMGREEN_CAM0;
    vuint32_t                          mCROI8_SUMBLUE_CAM0;
    isp_COLOR_TEMP_CROI9_POS_CAM0_t    mCROI9_POS_CAM0;
    uint32_t                           mpUnused12[1];
    isp_COLOR_TEMP_CROI9_PIXCNT_CAM0_t mCROI9_PIXCNT_CAM0;
    vuint32_t                          mCROI9_SUMRED_CAM0;
    vuint32_t                          mCROI9_SUMGREEN_CAM0;
    vuint32_t                          mCROI9_SUMBLUE_CAM0;
    uint32_t                           mpUnused13[1];
    isp_COLOR_TEMP_GR_AVG_IN_CAM0_t    mGR_AVG_IN_CAM0;
    isp_COLOR_TEMP_GB_AVG_IN_CAM0_t    mGB_AVG_IN_CAM0;
    vuint32_t                          mGR_GB_CNT_CAM0;
    vint32_t                           mGR_SUM_CAM0;
    vint32_t                           mGB_SUM_CAM0;
    vuint32_t                          mGR2_SUM_CAM0;
    vuint32_t                          mGB2_SUM_CAM0;
    vint32_t                           mGRGB_SUM_CAM0;
} isp_COLOR_TEMP_t;

typedef struct
{
    uint32_t  mUnused0 : 31;
    vuint32_t mENABLE : 1;
} isp_RGBIR_CTRL_CAM0_t;

typedef struct
{
    vint32_t mCCM : 16;
    uint32_t mUnused16 : 16;
} isp_RGBIR_CCM0_CAM0_t;

typedef struct
{
    vint32_t mCCM : 16;
    uint32_t mUnused16 : 16;
} isp_RGBIR_CCM1_CAM0_t;

typedef struct
{
    vint32_t mCCM : 16;
    uint32_t mUnused16 : 16;
} isp_RGBIR_CCM2_CAM0_t;

typedef struct
{
    vuint32_t mTHRESHOLD : 20;
    uint32_t  mUnused20 : 12;
} isp_RGBIR_CCM0_TH_CAM0_t;

typedef struct
{
    vuint32_t mTHRESHOLD : 20;
    uint32_t  mUnused20 : 12;
} isp_RGBIR_CCM1_TH_CAM0_t;

typedef struct
{
    vuint32_t mTHRESHOLD : 20;
    uint32_t  mUnused20 : 12;
} isp_RGBIR_CCM2_TH_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_RGBIR_ROI0_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_RGBIR_ROI0_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_RGBIR_ROI1_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_RGBIR_ROI1_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mLIN_VS_LOG : 1;
    vuint32_t mDIR_VS_DIF : 1;
    vuint32_t mPATTERN : 1;
    uint32_t  mUnused3 : 5;
    vuint32_t mCHANNEL : 4;
    uint32_t  mUnused12 : 4;
    vuint32_t mOFFSET : 16;
} isp_RGBIR_HIST0_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 24;
    uint32_t  mUnused24 : 8;
} isp_RGBIR_HIST0_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mLIN_VS_LOG : 1;
    vuint32_t mDIR_VS_DIF : 1;
    vuint32_t mPATTERN : 1;
    uint32_t  mUnused3 : 5;
    vuint32_t mCHANNEL : 4;
    uint32_t  mUnused12 : 4;
    vuint32_t mOFFSET : 16;
} isp_RGBIR_HIST1_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 24;
    uint32_t  mUnused24 : 8;
} isp_RGBIR_HIST1_SCALE_CAM0_t;

typedef struct
{
    isp_RGBIR_CTRL_CAM0_t        mCTRL_CAM0;
    isp_RGBIR_CCM0_CAM0_t        mCCM0_CAM0;
    isp_RGBIR_CCM1_CAM0_t        mCCM1_CAM0;
    isp_RGBIR_CCM2_CAM0_t        mCCM2_CAM0;
    isp_RGBIR_CCM0_TH_CAM0_t     mCCM0_TH_CAM0;
    isp_RGBIR_CCM1_TH_CAM0_t     mCCM1_TH_CAM0;
    isp_RGBIR_CCM2_TH_CAM0_t     mCCM2_TH_CAM0;
    uint32_t                     mpUnused0[1];
    isp_RGBIR_ROI0_POS_CAM0_t    mROI0_POS_CAM0;
    isp_RGBIR_ROI0_SIZE_CAM0_t   mROI0_SIZE_CAM0;
    isp_RGBIR_ROI1_POS_CAM0_t    mROI1_POS_CAM0;
    isp_RGBIR_ROI1_SIZE_CAM0_t   mROI1_SIZE_CAM0;
    isp_RGBIR_HIST0_CTRL_CAM0_t  mHIST0_CTRL_CAM0;
    isp_RGBIR_HIST0_SCALE_CAM0_t mHIST0_SCALE_CAM0;
    isp_RGBIR_HIST1_CTRL_CAM0_t  mHIST1_CTRL_CAM0;
    isp_RGBIR_HIST1_SCALE_CAM0_t mHIST1_SCALE_CAM0;
} isp_RGBIR_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_STAT_ROI0_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_STAT_ROI0_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_STAT_ROI1_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_STAT_ROI1_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mLIN_VS_LOG : 1;
    vuint32_t mDIR_VS_DIF : 1;
    vuint32_t mPATTERN : 1;
    uint32_t  mUnused3 : 5;
    vuint32_t mCHANNEL : 4;
    uint32_t  mUnused12 : 4;
    vuint32_t mOFFSET : 16;
} isp_STAT_HIST0_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 24;
    uint32_t  mUnused24 : 8;
} isp_STAT_HIST0_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mLIN_VS_LOG : 1;
    vuint32_t mDIR_VS_DIF : 1;
    vuint32_t mPATTERN : 1;
    uint32_t  mUnused3 : 5;
    vuint32_t mCHANNEL : 4;
    uint32_t  mUnused12 : 4;
    vuint32_t mOFFSET : 16;
} isp_STAT_HIST1_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 24;
    uint32_t  mUnused24 : 8;
} isp_STAT_HIST1_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mLIN_VS_LOG : 1;
    vuint32_t mDIR_VS_DIF : 1;
    vuint32_t mPATTERN : 1;
    uint32_t  mUnused3 : 5;
    vuint32_t mCHANNEL : 4;
    uint32_t  mUnused12 : 4;
    vuint32_t mOFFSET : 16;
} isp_STAT_HIST2_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 24;
    uint32_t  mUnused24 : 8;
} isp_STAT_HIST2_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mLIN_VS_LOG : 1;
    vuint32_t mDIR_VS_DIF : 1;
    vuint32_t mPATTERN : 1;
    uint32_t  mUnused3 : 5;
    vuint32_t mCHANNEL : 4;
    uint32_t  mUnused12 : 4;
    vuint32_t mOFFSET : 16;
} isp_STAT_HIST3_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 24;
    uint32_t  mUnused24 : 8;
} isp_STAT_HIST3_SCALE_CAM0_t;

typedef struct
{
    isp_STAT_ROI0_POS_CAM0_t    mROI0_POS_CAM0;
    isp_STAT_ROI0_SIZE_CAM0_t   mROI0_SIZE_CAM0;
    isp_STAT_ROI1_POS_CAM0_t    mROI1_POS_CAM0;
    isp_STAT_ROI1_SIZE_CAM0_t   mROI1_SIZE_CAM0;
    uint32_t                    mpUnused0[4];
    isp_STAT_HIST0_CTRL_CAM0_t  mHIST0_CTRL_CAM0;
    isp_STAT_HIST0_SCALE_CAM0_t mHIST0_SCALE_CAM0;
    isp_STAT_HIST1_CTRL_CAM0_t  mHIST1_CTRL_CAM0;
    isp_STAT_HIST1_SCALE_CAM0_t mHIST1_SCALE_CAM0;
    isp_STAT_HIST2_CTRL_CAM0_t  mHIST2_CTRL_CAM0;
    isp_STAT_HIST2_SCALE_CAM0_t mHIST2_SCALE_CAM0;
    isp_STAT_HIST3_CTRL_CAM0_t  mHIST3_CTRL_CAM0;
    isp_STAT_HIST3_SCALE_CAM0_t mHIST3_SCALE_CAM0;
} isp_STAT_t;

typedef struct
{
    vuint32_t mOBPP : 1;
    uint32_t  mUnused1 : 30;
    vuint32_t mENABLE : 1;
} isp_IR_COMPRESS_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_POINT1_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_POINT2_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_POINT3_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_POINT4_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_OFFSET0_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_OFFSET1_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_OFFSET2_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_OFFSET3_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 20;
    uint32_t  mUnused20 : 12;
} isp_IR_COMPRESS_KNEE_OFFSET4_CAM0_t;

typedef struct
{
    vuint32_t mRATIO0 : 16;
    vuint32_t mRATIO1 : 16;
} isp_IR_COMPRESS_KNEE_RATIO01_CAM0_t;

typedef struct
{
    vuint32_t mRATIO2 : 16;
    vuint32_t mRATIO3 : 16;
} isp_IR_COMPRESS_KNEE_RATIO23_CAM0_t;

typedef struct
{
    vuint32_t mRATIO4 : 16;
    uint32_t  mUnused16 : 16;
} isp_IR_COMPRESS_KNEE_RATIO4_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_IR_COMPRESS_KNEE_NPOINT0_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_IR_COMPRESS_KNEE_NPOINT1_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_IR_COMPRESS_KNEE_NPOINT2_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_IR_COMPRESS_KNEE_NPOINT3_CAM0_t;

typedef struct
{
    vuint32_t mKNEEPOINT : 16;
    uint32_t  mUnused16 : 16;
} isp_IR_COMPRESS_KNEE_NPOINT4_CAM0_t;

typedef struct
{
    isp_IR_COMPRESS_CTRL_CAM0_t         mCTRL_CAM0;
    isp_IR_COMPRESS_KNEE_POINT1_CAM0_t  mKNEE_POINT1_CAM0;
    isp_IR_COMPRESS_KNEE_POINT2_CAM0_t  mKNEE_POINT2_CAM0;
    isp_IR_COMPRESS_KNEE_POINT3_CAM0_t  mKNEE_POINT3_CAM0;
    isp_IR_COMPRESS_KNEE_POINT4_CAM0_t  mKNEE_POINT4_CAM0;
    isp_IR_COMPRESS_KNEE_OFFSET0_CAM0_t mKNEE_OFFSET0_CAM0;
    isp_IR_COMPRESS_KNEE_OFFSET1_CAM0_t mKNEE_OFFSET1_CAM0;
    isp_IR_COMPRESS_KNEE_OFFSET2_CAM0_t mKNEE_OFFSET2_CAM0;
    isp_IR_COMPRESS_KNEE_OFFSET3_CAM0_t mKNEE_OFFSET3_CAM0;
    isp_IR_COMPRESS_KNEE_OFFSET4_CAM0_t mKNEE_OFFSET4_CAM0;
    isp_IR_COMPRESS_KNEE_RATIO01_CAM0_t mKNEE_RATIO01_CAM0;
    isp_IR_COMPRESS_KNEE_RATIO23_CAM0_t mKNEE_RATIO23_CAM0;
    isp_IR_COMPRESS_KNEE_RATIO4_CAM0_t  mKNEE_RATIO4_CAM0;
    isp_IR_COMPRESS_KNEE_NPOINT0_CAM0_t mKNEE_NPOINT0_CAM0;
    isp_IR_COMPRESS_KNEE_NPOINT1_CAM0_t mKNEE_NPOINT1_CAM0;
    isp_IR_COMPRESS_KNEE_NPOINT2_CAM0_t mKNEE_NPOINT2_CAM0;
    isp_IR_COMPRESS_KNEE_NPOINT3_CAM0_t mKNEE_NPOINT3_CAM0;
    isp_IR_COMPRESS_KNEE_NPOINT4_CAM0_t mKNEE_NPOINT4_CAM0;
} isp_IR_COMPRESS_t;

typedef struct
{
    uint32_t  mUnused0 : 2;
    vuint32_t mOBPP : 2;
    uint32_t  mUnused4 : 4;
    vuint32_t mDEBUG : 3;
    uint32_t  mUnused11 : 5;
    vuint32_t mNHOOD : 1;
    uint32_t  mUnused17 : 14;
    vuint32_t mENABLE : 1;
} isp_BNR_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mPEAK_LOW : 12;
    uint32_t  mUnused12 : 2;
    vuint32_t mPEAK_SEL : 2;
    vuint32_t mPEAK_HIGH : 12;
    uint32_t  mUnused28 : 3;
    vuint32_t mPEAK_OUTSEL : 1;
} isp_BNR_YPEAK_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_TH0 : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_YEDGE_TH0_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_YEDGE_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_TH0 : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_YEDGES_TH0_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_YEDGES_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_TH0 : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_YEDGEA_TH0_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_YEDGEA_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mTH : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_YLUMA_X_TH0_CAM0_t;

typedef struct
{
    vuint32_t mLUMA_Y_TH0 : 10;
    uint32_t  mUnused10 : 6;
    vuint32_t mLUMA_Y_TH1 : 10;
    uint32_t  mUnused26 : 6;
} isp_BNR_YLUMA_Y_TH_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_YLUMA_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mGAIN : 16;
    vuint32_t mOFFSET : 16;
} isp_BNR_YALPHA_GAIN_CAM0_t;

typedef struct
{
    vuint32_t mPEAK_LOW : 12;
    uint32_t  mUnused12 : 2;
    vuint32_t mPEAK_SEL : 2;
    vuint32_t mPEAK_HIGH : 12;
    uint32_t  mUnused28 : 3;
    vuint32_t mPEAK_OUTSEL : 1;
} isp_BNR_CPEAK_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_TH0 : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_CEDGE_TH0_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_CEDGE_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_TH0 : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_CEDGES_TH0_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_CEDGES_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_TH0 : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_CEDGEA_TH0_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_CEDGEA_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mTH : 20;
    uint32_t  mUnused20 : 12;
} isp_BNR_CLUMA_X_TH0_CAM0_t;

typedef struct
{
    vuint32_t mLUMA_Y_TH0 : 10;
    uint32_t  mUnused10 : 6;
    vuint32_t mLUMA_Y_TH1 : 10;
    uint32_t  mUnused26 : 6;
} isp_BNR_CLUMA_Y_TH_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused21 : 11;
} isp_BNR_CLUMA_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mGAIN : 16;
    vuint32_t mOFFSET : 16;
} isp_BNR_CALPHA_GAIN_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_PIXELS : 24;
    uint32_t  mUnused24 : 8;
} isp_BNR_EDGE_STAT_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_PIXELS : 24;
    uint32_t  mUnused24 : 8;
} isp_BNR_EDGES_STAT_CAM0_t;

typedef struct
{
    vuint32_t mGAIN : 16;
    uint32_t  mUnused16 : 16;
} isp_BNR_STRETCH_CAM0_t;

typedef struct
{
    isp_BNR_CTRL_CAM0_t         mCTRL_CAM0;
    isp_BNR_YPEAK_CAM0_t        mYPEAK_CAM0;
    isp_BNR_YEDGE_TH0_CAM0_t    mYEDGE_TH0_CAM0;
    isp_BNR_YEDGE_SCALE_CAM0_t  mYEDGE_SCALE_CAM0;
    isp_BNR_YEDGES_TH0_CAM0_t   mYEDGES_TH0_CAM0;
    isp_BNR_YEDGES_SCALE_CAM0_t mYEDGES_SCALE_CAM0;
    isp_BNR_YEDGEA_TH0_CAM0_t   mYEDGEA_TH0_CAM0;
    isp_BNR_YEDGEA_SCALE_CAM0_t mYEDGEA_SCALE_CAM0;
    isp_BNR_YLUMA_X_TH0_CAM0_t  mYLUMA_X_TH0_CAM0;
    isp_BNR_YLUMA_Y_TH_CAM0_t   mYLUMA_Y_TH_CAM0;
    isp_BNR_YLUMA_SCALE_CAM0_t  mYLUMA_SCALE_CAM0;
    isp_BNR_YALPHA_GAIN_CAM0_t  mYALPHA_GAIN_CAM0;
    isp_BNR_CPEAK_CAM0_t        mCPEAK_CAM0;
    isp_BNR_CEDGE_TH0_CAM0_t    mCEDGE_TH0_CAM0;
    isp_BNR_CEDGE_SCALE_CAM0_t  mCEDGE_SCALE_CAM0;
    isp_BNR_CEDGES_TH0_CAM0_t   mCEDGES_TH0_CAM0;
    isp_BNR_CEDGES_SCALE_CAM0_t mCEDGES_SCALE_CAM0;
    isp_BNR_CEDGEA_TH0_CAM0_t   mCEDGEA_TH0_CAM0;
    isp_BNR_CEDGEA_SCALE_CAM0_t mCEDGEA_SCALE_CAM0;
    isp_BNR_CLUMA_X_TH0_CAM0_t  mCLUMA_X_TH0_CAM0;
    isp_BNR_CLUMA_Y_TH_CAM0_t   mCLUMA_Y_TH_CAM0;
    isp_BNR_CLUMA_SCALE_CAM0_t  mCLUMA_SCALE_CAM0;
    isp_BNR_CALPHA_GAIN_CAM0_t  mCALPHA_GAIN_CAM0;
    isp_BNR_EDGE_STAT_CAM0_t    mEDGE_STAT_CAM0;
    isp_BNR_EDGES_STAT_CAM0_t   mEDGES_STAT_CAM0;
    isp_BNR_STRETCH_CAM0_t      mSTRETCH_CAM0;
} isp_BNR_t;

typedef struct
{
    uint32_t  mUnused0 : 31;
    vuint32_t mENABLE : 1;
} isp_VIGNETTING_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mCOLS : 8;
    uint32_t  mUnused8 : 8;
    vuint32_t mROWS : 8;
    uint32_t  mUnused24 : 8;
} isp_VIGNETTING_BLK_CONF_CAM0_t;

typedef struct
{
    vuint32_t mXSIZE : 16;
    vuint32_t mYSIZE : 16;
} isp_VIGNETTING_BLK_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mSTEP : 16;
    uint32_t  mUnused16 : 16;
} isp_VIGNETTING_BLK_STEPY_CAM0_t;

typedef struct
{
    vuint32_t mSTEP : 16;
    uint32_t  mUnused16 : 16;
} isp_VIGNETTING_BLK_STEPX_CAM0_t;

typedef struct
{
    vuint32_t mLINE : 16;
    uint32_t  mUnused16 : 16;
} isp_VIGNETTING_BLK_C_LINE_CAM0_t;

typedef struct
{
    vuint32_t mBLKROW : 8;
    uint32_t  mUnused8 : 24;
} isp_VIGNETTING_BLK_C_ROW_CAM0_t;

typedef struct
{
    isp_VIGNETTING_CTRL_CAM0_t       mCTRL_CAM0;
    isp_VIGNETTING_BLK_CONF_CAM0_t   mBLK_CONF_CAM0;
    isp_VIGNETTING_BLK_SIZE_CAM0_t   mBLK_SIZE_CAM0;
    isp_VIGNETTING_BLK_STEPY_CAM0_t  mBLK_STEPY_CAM0;
    isp_VIGNETTING_BLK_STEPX_CAM0_t  mBLK_STEPX_CAM0;
    uint32_t                         mpUnused0[3];
    isp_VIGNETTING_BLK_C_LINE_CAM0_t mBLK_C_LINE_CAM0;
    isp_VIGNETTING_BLK_C_ROW_CAM0_t  mBLK_C_ROW_CAM0;
    vuint32_t                        mBLK_C_FRACY_CAM0;
} isp_VIGNETTING_t;

typedef struct
{
    vuint32_t mLINE_NUM : 17;
    uint32_t  mUnused17 : 15;
} isp_IDBG1_LINE_NUM_t;

typedef struct
{
    vuint32_t mCURR_LINE_NUM : 17;
    uint32_t  mUnused17 : 14;
    vuint32_t mDBG_HIT : 1;
} isp_IDBG1_CURR_LINE_NUM_t;

typedef struct
{
    vuint32_t mADDR : 12;
    uint32_t  mUnused12 : 4;
    vuint32_t mNAME : 5;
    uint32_t  mUnused21 : 7;
    vuint32_t mRDWF : 2;
    vuint32_t mWDWF : 2;
} isp_IDBG1_IMA_t;

typedef struct
{
    vuint32_t mVIG : 1;
    vuint32_t mIRCOMP : 1;
    vuint32_t mHDRMERGE : 1;
    vuint32_t mBNR0 : 1;
    vuint32_t mSTAT : 1;
    vuint32_t mCTEMP : 1;
    vuint32_t mOB_WB2 : 1;
    vuint32_t mOBWB1 : 1;
    vuint32_t mOBWB0 : 1;
    vuint32_t mHDRDECOMP1 : 1;
    vuint32_t mHDRDECOMP0 : 1;
    vuint32_t mHC : 1;
    vuint32_t mRGBIR : 1;
    uint32_t  mUnused13 : 19;
} isp_IDBG1_DONE_STAT_t;

typedef struct
{
    isp_IDBG1_LINE_NUM_t      mLINE_NUM;
    isp_IDBG1_CURR_LINE_NUM_t mCURR_LINE_NUM;
    isp_IDBG1_IMA_t           mIMA;
    vuint32_t                 mIMD;
    isp_IDBG1_DONE_STAT_t     mDONE_STAT;
} isp_IDBG1_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mFMT : 2;
    uint32_t  mUnused6 : 26;
} isp_DEMOSAIC_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mALPHA : 9;
    uint32_t  mUnused9 : 7;
    vuint32_t mACT_RATIO : 16;
} isp_DEMOSAIC_ACTIVITY_CTL_CAM0_t;

typedef struct
{
    vuint32_t mSTRENGTHG : 16;
    vuint32_t mSTRENGTHC : 16;
} isp_DEMOSAIC_DYNAMICS_CTL0_CAM0_t;

typedef struct
{
    vuint32_t mMAX_IMPACT : 16;
    uint32_t  mUnused16 : 16;
} isp_DEMOSAIC_DYNAMICS_CTL2_CAM0_t;

typedef struct
{
    isp_DEMOSAIC_CTRL_CAM0_t          mCTRL_CAM0;
    isp_DEMOSAIC_ACTIVITY_CTL_CAM0_t  mACTIVITY_CTL_CAM0;
    isp_DEMOSAIC_DYNAMICS_CTL0_CAM0_t mDYNAMICS_CTL0_CAM0;
    isp_DEMOSAIC_DYNAMICS_CTL2_CAM0_t mDYNAMICS_CTL2_CAM0;
} isp_DEMOSAIC_t;

typedef struct
{
    vuint32_t mRGAIN : 16;
    vuint32_t mBGAIN : 16;
} isp_RGB_to_YUV_GAIN_CTRL_CAM0_t;

typedef struct
{
    vint32_t mR0C0 : 16;
    vint32_t mR0C1 : 16;
} isp_RGB_to_YUV_MAT0_CAM0_t;

typedef struct
{
    vint32_t mR0C2 : 16;
    uint32_t mUnused16 : 16;
} isp_RGB_to_YUV_MAT1_CAM0_t;

typedef struct
{
    vint32_t mR1C0 : 16;
    vint32_t mR1C1 : 16;
} isp_RGB_to_YUV_MAT2_CAM0_t;

typedef struct
{
    vint32_t mR1C2 : 16;
    uint32_t mUnused16 : 16;
} isp_RGB_to_YUV_MAT3_CAM0_t;

typedef struct
{
    vint32_t mR2C0 : 16;
    vint32_t mR2C1 : 16;
} isp_RGB_to_YUV_MAT4_CAM0_t;

typedef struct
{
    vint32_t mR2C2 : 16;
    uint32_t mUnused16 : 16;
} isp_RGB_to_YUV_MAT5_CAM0_t;

typedef struct
{
    vint32_t mOFFSET : 21;
    uint32_t mUnused21 : 11;
} isp_RGB_to_YUV_OFFSET0_CAM0_t;

typedef struct
{
    vint32_t mOFFSET : 21;
    uint32_t mUnused21 : 11;
} isp_RGB_to_YUV_OFFSET1_CAM0_t;

typedef struct
{
    vint32_t mOFFSET : 21;
    uint32_t mUnused21 : 11;
} isp_RGB_to_YUV_OFFSET2_CAM0_t;

typedef struct
{
    isp_RGB_to_YUV_GAIN_CTRL_CAM0_t mGAIN_CTRL_CAM0;
    isp_RGB_to_YUV_MAT0_CAM0_t      mMAT0_CAM0;
    isp_RGB_to_YUV_MAT1_CAM0_t      mMAT1_CAM0;
    isp_RGB_to_YUV_MAT2_CAM0_t      mMAT2_CAM0;
    isp_RGB_to_YUV_MAT3_CAM0_t      mMAT3_CAM0;
    isp_RGB_to_YUV_MAT4_CAM0_t      mMAT4_CAM0;
    isp_RGB_to_YUV_MAT5_CAM0_t      mMAT5_CAM0;
    uint32_t                        mpUnused0[1];
    isp_RGB_to_YUV_OFFSET0_CAM0_t   mOFFSET0_CAM0;
    isp_RGB_to_YUV_OFFSET1_CAM0_t   mOFFSET1_CAM0;
    isp_RGB_to_YUV_OFFSET2_CAM0_t   mOFFSET2_CAM0;
} isp_RGB_to_YUV_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_DRC_ROI0_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_DRC_ROI0_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_DRC_ROI1_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_DRC_ROI1_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT0 : 5;
    uint32_t  mUnused5 : 11;
    vuint32_t mSHIFT1 : 5;
    uint32_t  mUnused21 : 11;
} isp_DRC_GROI_SUM_SHIFT_CAM0_t;

typedef struct
{
    vuint32_t mGAIN : 16;
    uint32_t  mUnused16 : 16;
} isp_DRC_GBL_GAIN_CAM0_t;

typedef struct
{
    vuint32_t mXSIZE : 16;
    vuint32_t mYSIZE : 16;
} isp_DRC_LCL_BLK_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mSTRETCH : 16;
    vuint32_t mOFFSET : 16;
} isp_DRC_LCL_STRETCH_CAM0_t;

typedef struct
{
    vuint32_t mSTEP : 16;
    uint32_t  mUnused16 : 16;
} isp_DRC_LCL_BLK_STEPY_CAM0_t;

typedef struct
{
    vuint32_t mSTEP : 16;
    uint32_t  mUnused16 : 16;
} isp_DRC_LCL_BLK_STEPX_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused5 : 27;
} isp_DRC_LCL_SUM_SHIFT_CAM0_t;

typedef struct
{
    vuint32_t mALPHA : 9;
    uint32_t  mUnused9 : 23;
} isp_DRC_ALPHA_CAM0_t;

typedef struct
{
    vuint32_t mBLKLNE : 16;
    vuint32_t mBLKROW : 8;
    uint32_t  mUnused24 : 8;
} isp_DRC_STAT_BLK_Y_CAM0_t;

typedef struct
{
    isp_DRC_ROI0_POS_CAM0_t       mROI0_POS_CAM0;
    isp_DRC_ROI0_SIZE_CAM0_t      mROI0_SIZE_CAM0;
    isp_DRC_ROI1_POS_CAM0_t       mROI1_POS_CAM0;
    isp_DRC_ROI1_SIZE_CAM0_t      mROI1_SIZE_CAM0;
    isp_DRC_GROI_SUM_SHIFT_CAM0_t mGROI_SUM_SHIFT_CAM0;
    isp_DRC_GBL_GAIN_CAM0_t       mGBL_GAIN_CAM0;
    uint32_t                      mpUnused0[2];
    isp_DRC_LCL_BLK_SIZE_CAM0_t   mLCL_BLK_SIZE_CAM0;
    isp_DRC_LCL_STRETCH_CAM0_t    mLCL_STRETCH_CAM0;
    isp_DRC_LCL_BLK_STEPY_CAM0_t  mLCL_BLK_STEPY_CAM0;
    isp_DRC_LCL_BLK_STEPX_CAM0_t  mLCL_BLK_STEPX_CAM0;
    isp_DRC_LCL_SUM_SHIFT_CAM0_t  mLCL_SUM_SHIFT_CAM0;
    isp_DRC_ALPHA_CAM0_t          mALPHA_CAM0;
    uint32_t                      mpUnused1[2];
    vuint32_t                     mGROI0_SUM_CAM0;
    vuint32_t                     mGROI1_SUM_CAM0;
    uint32_t                      mpUnused2[2];
    isp_DRC_STAT_BLK_Y_CAM0_t     mSTAT_BLK_Y_CAM0;
    vuint32_t                     mCURR_YFRACT_CAM0;
} isp_DRC_t;

typedef struct
{
    uint32_t  mUnused0 : 8;
    vuint32_t mDEBUG : 2;
    uint32_t  mUnused10 : 21;
    vuint32_t mENABLE : 1;
} isp_NR_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 8;
    vuint32_t mGAIN : 8;
} isp_NR_BLEND_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mTH : 20;
    uint32_t  mUnused20 : 12;
} isp_NR_BLEND_TH0_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_PIXELS : 24;
    uint32_t  mUnused24 : 8;
} isp_NR_EDGECNT_CAM0_t;

typedef struct
{
    isp_NR_CTRL_CAM0_t        mCTRL_CAM0;
    isp_NR_BLEND_SCALE_CAM0_t mBLEND_SCALE_CAM0;
    isp_NR_BLEND_TH0_CAM0_t   mBLEND_TH0_CAM0;
    uint32_t                  mpUnused0[1];
    isp_NR_EDGECNT_CAM0_t     mEDGECNT_CAM0;
} isp_NR_t;

typedef struct
{
    uint32_t  mUnused0 : 8;
    vuint32_t mDEBUG : 3;
    uint32_t  mUnused11 : 20;
    vuint32_t mENABLE : 1;
} isp_DF_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mSCALE : 20;
    uint32_t  mUnused20 : 12;
} isp_DF_TH_SCALE_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT : 6;
    uint32_t  mUnused6 : 26;
} isp_DF_BLEND_SHIFT_CAM0_t;

typedef struct
{
    vuint32_t mTH : 20;
    uint32_t  mUnused20 : 12;
} isp_DF_BLEND_TH0_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_PIXELS : 24;
    uint32_t  mUnused24 : 8;
} isp_DF_EDGECNT_CAM0_t;

typedef struct
{
    isp_DF_CTRL_CAM0_t        mCTRL_CAM0;
    isp_DF_TH_SCALE_CAM0_t    mTH_SCALE_CAM0;
    isp_DF_BLEND_SHIFT_CAM0_t mBLEND_SHIFT_CAM0;
    isp_DF_BLEND_TH0_CAM0_t   mBLEND_TH0_CAM0;
    isp_DF_EDGECNT_CAM0_t     mEDGECNT_CAM0;
} isp_DF_t;

typedef struct
{
    uint32_t  mUnused0 : 8;
    vuint32_t mDEBUG : 2;
    uint32_t  mUnused10 : 21;
    vuint32_t mENABLE : 1;
} isp_EE_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mCORING : 20;
    uint32_t  mUnused20 : 12;
} isp_EE_CORING_CAM0_t;

typedef struct
{
    vuint32_t mCLIP : 20;
    uint32_t  mUnused20 : 12;
} isp_EE_CLIP_CAM0_t;

typedef struct
{
    vuint32_t mGAIN : 8;
    uint32_t  mUnused8 : 24;
} isp_EE_MASKGAIN_CAM0_t;

typedef struct
{
    vuint32_t mEDGE_PIXELS : 24;
    uint32_t  mUnused24 : 8;
} isp_EE_EDGECNT_CAM0_t;

typedef struct
{
    isp_EE_CTRL_CAM0_t     mCTRL_CAM0;
    isp_EE_CORING_CAM0_t   mCORING_CAM0;
    isp_EE_CLIP_CAM0_t     mCLIP_CAM0;
    isp_EE_MASKGAIN_CAM0_t mMASKGAIN_CAM0;
    isp_EE_EDGECNT_CAM0_t  mEDGECNT_CAM0;
} isp_EE_t;

typedef struct
{
    uint32_t  mUnused0 : 4;
    vuint32_t mFLT : 2;
    uint32_t  mUnused6 : 26;
} isp_CCONVMED_CTRL_CAM0_t;

typedef struct
{
    isp_CCONVMED_CTRL_CAM0_t mCTRL_CAM0;
} isp_CCONVMED_t;

typedef struct
{
    vuint32_t mSCALE : 16;
    vuint32_t mSHIFT : 8;
    uint32_t  mUnused24 : 8;
} isp_CAS_GAIN_CAM0_t;

typedef struct
{
    vuint32_t mCORR : 16;
    uint32_t  mUnused16 : 16;
} isp_CAS_CORR_CAM0_t;

typedef struct
{
    vuint32_t mOFFSET : 16;
    uint32_t  mUnused16 : 16;
} isp_CAS_OFFSET_CAM0_t;

typedef struct
{
    uint32_t              mpUnused0[1];
    isp_CAS_GAIN_CAM0_t   mGAIN_CAM0;
    isp_CAS_CORR_CAM0_t   mCORR_CAM0;
    isp_CAS_OFFSET_CAM0_t mOFFSET_CAM0;
} isp_CAS_t;

typedef struct
{
    vuint32_t mOBPP : 4;
    uint32_t  mUnused4 : 4;
    vuint32_t mRSA : 3;
    uint32_t  mUnused11 : 1;
    vuint32_t mLSA : 3;
    uint32_t  mUnused15 : 17;
} isp_Packetizer_CH0_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mOBPP : 4;
    uint32_t  mUnused4 : 4;
    vuint32_t mRSA : 3;
    uint32_t  mUnused11 : 1;
    vuint32_t mLSA : 3;
    uint32_t  mUnused15 : 1;
    vuint32_t mSUBSAMPLE : 2;
    uint32_t  mUnused18 : 14;
} isp_Packetizer_CH12_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mTYPE : 1;
    uint32_t  mUnused1 : 7;
    vuint32_t mORDER0 : 2;
    vuint32_t mORDER1 : 2;
    vuint32_t mORDER2 : 2;
    uint32_t  mUnused14 : 2;
    vuint32_t mA0S : 4;
    uint32_t  mUnused20 : 12;
} isp_Packetizer_PACK_CTRL_CAM0_t;

typedef struct
{
    isp_Packetizer_CH0_CTRL_CAM0_t  mCH0_CTRL_CAM0;
    isp_Packetizer_CH12_CTRL_CAM0_t mCH12_CTRL_CAM0;
    isp_Packetizer_PACK_CTRL_CAM0_t mPACK_CTRL_CAM0;
} isp_Packetizer_t;

typedef struct
{
    vint32_t mR0C0 : 16;
    vint32_t mR0C1 : 16;
} isp_GCM_IMAT0_CAM0_t;

typedef struct
{
    vint32_t mR0C2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_IMAT1_CAM0_t;

typedef struct
{
    vint32_t mR1C0 : 16;
    vint32_t mR1C1 : 16;
} isp_GCM_IMAT2_CAM0_t;

typedef struct
{
    vint32_t mR1C2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_IMAT3_CAM0_t;

typedef struct
{
    vint32_t mR2C0 : 16;
    vint32_t mR2C1 : 16;
} isp_GCM_IMAT4_CAM0_t;

typedef struct
{
    vint32_t mR2C2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_IMAT5_CAM0_t;

typedef struct
{
    vint32_t mOFFSET0 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_IOFFSET0_CAM0_t;

typedef struct
{
    vint32_t mOFFSET1 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_IOFFSET1_CAM0_t;

typedef struct
{
    vint32_t mOFFSET2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_IOFFSET2_CAM0_t;

typedef struct
{
    vint32_t mR0C0 : 16;
    vint32_t mR0C1 : 16;
} isp_GCM_OMAT0_CAM0_t;

typedef struct
{
    vint32_t mR0C2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_OMAT1_CAM0_t;

typedef struct
{
    vint32_t mR1C0 : 16;
    vint32_t mR1C1 : 16;
} isp_GCM_OMAT2_CAM0_t;

typedef struct
{
    vint32_t mR1C2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_OMAT3_CAM0_t;

typedef struct
{
    vint32_t mR2C0 : 16;
    vint32_t mR2C1 : 16;
} isp_GCM_OMAT4_CAM0_t;

typedef struct
{
    vint32_t mR2C2 : 16;
    uint32_t mUnused16 : 16;
} isp_GCM_OMAT5_CAM0_t;

typedef struct
{
    vint32_t mOFFSET0 : 13;
    uint32_t mUnused13 : 19;
} isp_GCM_OOFFSET0_CAM0_t;

typedef struct
{
    vint32_t mOFFSET1 : 13;
    uint32_t mUnused13 : 19;
} isp_GCM_OOFFSET1_CAM0_t;

typedef struct
{
    vint32_t mOFFSET2 : 13;
    uint32_t mUnused13 : 19;
} isp_GCM_OOFFSET2_CAM0_t;

typedef struct
{
    vuint32_t mGAMMA0 : 9;
    uint32_t  mUnused9 : 7;
    vuint32_t mOFFSET0 : 12;
    uint32_t  mUnused28 : 4;
} isp_GCM_GAMMA0_CAM0_t;

typedef struct
{
    vuint32_t mGAMMA1 : 9;
    uint32_t  mUnused9 : 7;
    vuint32_t mOFFSET1 : 12;
    uint32_t  mUnused28 : 4;
} isp_GCM_GAMMA1_CAM0_t;

typedef struct
{
    vuint32_t mGAMMA2 : 9;
    uint32_t  mUnused9 : 7;
    vuint32_t mOFFSET2 : 12;
    uint32_t  mUnused28 : 4;
} isp_GCM_GAMMA2_CAM0_t;

typedef struct
{
    vint32_t  mOFFSET0 : 16;
    vuint32_t mGAIN0 : 16;
} isp_GCM_BLKLVL0_CTRL_CAM0_t;

typedef struct
{
    vint32_t  mOFFSET1 : 16;
    vuint32_t mGAIN1 : 16;
} isp_GCM_BLKLVL1_CTRL_CAM0_t;

typedef struct
{
    vint32_t  mOFFSET2 : 16;
    vuint32_t mGAIN2 : 16;
} isp_GCM_BLKLVL2_CTRL_CAM0_t;

typedef struct
{
    vuint32_t mTHRESHOLD0 : 16;
    vuint32_t mTHRESHOLD1 : 16;
} isp_GCM_LOWTH_CTRL01_CAM0_t;

typedef struct
{
    vuint32_t mTHRESHOLD2 : 16;
    uint32_t  mUnused16 : 16;
} isp_GCM_LOWTH_CTRL2_CAM0_t;

typedef struct
{
    vuint32_t mSIGN_CONFG : 1;
    uint32_t  mUnused1 : 31;
} isp_GCM_MAT_CONFG_CAM0_t;

typedef struct
{
    isp_GCM_IMAT0_CAM0_t        mIMAT0_CAM0;
    isp_GCM_IMAT1_CAM0_t        mIMAT1_CAM0;
    uint32_t                    mpUnused0[1];
    isp_GCM_IMAT2_CAM0_t        mIMAT2_CAM0;
    isp_GCM_IMAT3_CAM0_t        mIMAT3_CAM0;
    uint32_t                    mpUnused1[1];
    isp_GCM_IMAT4_CAM0_t        mIMAT4_CAM0;
    isp_GCM_IMAT5_CAM0_t        mIMAT5_CAM0;
    isp_GCM_IOFFSET0_CAM0_t     mIOFFSET0_CAM0;
    isp_GCM_IOFFSET1_CAM0_t     mIOFFSET1_CAM0;
    isp_GCM_IOFFSET2_CAM0_t     mIOFFSET2_CAM0;
    uint32_t                    mpUnused2[1];
    isp_GCM_OMAT0_CAM0_t        mOMAT0_CAM0;
    isp_GCM_OMAT1_CAM0_t        mOMAT1_CAM0;
    isp_GCM_OMAT2_CAM0_t        mOMAT2_CAM0;
    isp_GCM_OMAT3_CAM0_t        mOMAT3_CAM0;
    isp_GCM_OMAT4_CAM0_t        mOMAT4_CAM0;
    isp_GCM_OMAT5_CAM0_t        mOMAT5_CAM0;
    isp_GCM_OOFFSET0_CAM0_t     mOOFFSET0_CAM0;
    isp_GCM_OOFFSET1_CAM0_t     mOOFFSET1_CAM0;
    isp_GCM_OOFFSET2_CAM0_t     mOOFFSET2_CAM0;
    uint32_t                    mpUnused3[3];
    isp_GCM_GAMMA0_CAM0_t       mGAMMA0_CAM0;
    isp_GCM_GAMMA1_CAM0_t       mGAMMA1_CAM0;
    isp_GCM_GAMMA2_CAM0_t       mGAMMA2_CAM0;
    isp_GCM_BLKLVL0_CTRL_CAM0_t mBLKLVL0_CTRL_CAM0;
    isp_GCM_BLKLVL1_CTRL_CAM0_t mBLKLVL1_CTRL_CAM0;
    isp_GCM_BLKLVL2_CTRL_CAM0_t mBLKLVL2_CTRL_CAM0;
    isp_GCM_LOWTH_CTRL01_CAM0_t mLOWTH_CTRL01_CAM0;
    isp_GCM_LOWTH_CTRL2_CAM0_t  mLOWTH_CTRL2_CAM0;
    isp_GCM_MAT_CONFG_CAM0_t    mMAT_CONFG_CAM0;
} isp_GCM_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI0_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI0_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI1_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI1_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI2_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI2_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI3_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI3_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI4_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI4_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI5_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI5_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI6_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI6_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI7_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI7_SIZE_CAM0_t;

typedef struct
{
    vuint32_t mXPOS : 16;
    vuint32_t mYPOS : 16;
} isp_AUTOFOCUS_ROI8_POS_CAM0_t;

typedef struct
{
    vuint32_t mWIDTH : 16;
    vuint32_t mHEIGHT : 16;
} isp_AUTOFOCUS_ROI8_SIZE_CAM0_t;

typedef struct
{
    vint32_t mCOEFF0 : 8;
    vint32_t mCOEFF1 : 8;
    vint32_t mCOEFF2 : 8;
    vint32_t mCOEFF3 : 8;
} isp_AUTOFOCUS_FIL0_COEFFS0_CAM0_t;

typedef struct
{
    vint32_t mCOEFF4 : 8;
    vint32_t mCOEFF5 : 8;
    vint32_t mCOEFF6 : 8;
    vint32_t mCOEFF7 : 8;
} isp_AUTOFOCUS_FIL0_COEFFS1_CAM0_t;

typedef struct
{
    vint32_t mCOEFF8 : 8;
    uint32_t mUnused8 : 24;
} isp_AUTOFOCUS_FIL0_COEFFS2_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused5 : 27;
} isp_AUTOFOCUS_FIL0_SHIFT_CAM0_t;

typedef struct
{
    vint32_t mCOEFF0 : 8;
    vint32_t mCOEFF1 : 8;
    vint32_t mCOEFF2 : 8;
    vint32_t mCOEFF3 : 8;
} isp_AUTOFOCUS_FIL1_COEFFS0_CAM0_t;

typedef struct
{
    vint32_t mCOEFF4 : 8;
    vint32_t mCOEFF5 : 8;
    vint32_t mCOEFF6 : 8;
    vint32_t mCOEFF7 : 8;
} isp_AUTOFOCUS_FIL1_COEFFS1_CAM0_t;

typedef struct
{
    vint32_t mCOEFF8 : 8;
    uint32_t mUnused8 : 24;
} isp_AUTOFOCUS_FIL1_COEFFS2_CAM0_t;

typedef struct
{
    vuint32_t mSHIFT : 5;
    uint32_t  mUnused5 : 27;
} isp_AUTOFOCUS_FIL1_SHIFT_CAM0_t;

typedef struct
{
    isp_AUTOFOCUS_ROI0_POS_CAM0_t     mROI0_POS_CAM0;
    isp_AUTOFOCUS_ROI0_SIZE_CAM0_t    mROI0_SIZE_CAM0;
    isp_AUTOFOCUS_ROI1_POS_CAM0_t     mROI1_POS_CAM0;
    isp_AUTOFOCUS_ROI1_SIZE_CAM0_t    mROI1_SIZE_CAM0;
    isp_AUTOFOCUS_ROI2_POS_CAM0_t     mROI2_POS_CAM0;
    isp_AUTOFOCUS_ROI2_SIZE_CAM0_t    mROI2_SIZE_CAM0;
    isp_AUTOFOCUS_ROI3_POS_CAM0_t     mROI3_POS_CAM0;
    isp_AUTOFOCUS_ROI3_SIZE_CAM0_t    mROI3_SIZE_CAM0;
    isp_AUTOFOCUS_ROI4_POS_CAM0_t     mROI4_POS_CAM0;
    isp_AUTOFOCUS_ROI4_SIZE_CAM0_t    mROI4_SIZE_CAM0;
    isp_AUTOFOCUS_ROI5_POS_CAM0_t     mROI5_POS_CAM0;
    isp_AUTOFOCUS_ROI5_SIZE_CAM0_t    mROI5_SIZE_CAM0;
    isp_AUTOFOCUS_ROI6_POS_CAM0_t     mROI6_POS_CAM0;
    isp_AUTOFOCUS_ROI6_SIZE_CAM0_t    mROI6_SIZE_CAM0;
    isp_AUTOFOCUS_ROI7_POS_CAM0_t     mROI7_POS_CAM0;
    isp_AUTOFOCUS_ROI7_SIZE_CAM0_t    mROI7_SIZE_CAM0;
    isp_AUTOFOCUS_ROI8_POS_CAM0_t     mROI8_POS_CAM0;
    isp_AUTOFOCUS_ROI8_SIZE_CAM0_t    mROI8_SIZE_CAM0;
    uint32_t                          mpUnused0[2];
    isp_AUTOFOCUS_FIL0_COEFFS0_CAM0_t mFIL0_COEFFS0_CAM0;
    isp_AUTOFOCUS_FIL0_COEFFS1_CAM0_t mFIL0_COEFFS1_CAM0;
    isp_AUTOFOCUS_FIL0_COEFFS2_CAM0_t mFIL0_COEFFS2_CAM0;
    isp_AUTOFOCUS_FIL0_SHIFT_CAM0_t   mFIL0_SHIFT_CAM0;
    isp_AUTOFOCUS_FIL1_COEFFS0_CAM0_t mFIL1_COEFFS0_CAM0;
    isp_AUTOFOCUS_FIL1_COEFFS1_CAM0_t mFIL1_COEFFS1_CAM0;
    isp_AUTOFOCUS_FIL1_COEFFS2_CAM0_t mFIL1_COEFFS2_CAM0;
    isp_AUTOFOCUS_FIL1_SHIFT_CAM0_t   mFIL1_SHIFT_CAM0;
    vuint32_t                         mROI0_SUM0_CAM0;
    vuint32_t                         mROI0_SUM1_CAM0;
    vuint32_t                         mROI1_SUM0_CAM0;
    vuint32_t                         mROI1_SUM1_CAM0;
    vuint32_t                         mROI2_SUM0_CAM0;
    vuint32_t                         mROI2_SUM1_CAM0;
    vuint32_t                         mROI3_SUM0_CAM0;
    vuint32_t                         mROI3_SUM1_CAM0;
    vuint32_t                         mROI4_SUM0_CAM0;
    vuint32_t                         mROI4_SUM1_CAM0;
    vuint32_t                         mROI5_SUM0_CAM0;
    vuint32_t                         mROI5_SUM1_CAM0;
    vuint32_t                         mROI6_SUM0_CAM0;
    vuint32_t                         mROI6_SUM1_CAM0;
    vuint32_t                         mROI7_SUM0_CAM0;
    vuint32_t                         mROI7_SUM1_CAM0;
    vuint32_t                         mROI8_SUM0_CAM0;
    vuint32_t                         mROI8_SUM1_CAM0;
} isp_AUTOFOCUS_t;

typedef struct
{
    vuint32_t mLINE_NUM : 17;
    uint32_t  mUnused17 : 15;
} isp_IDBG2_LINE_NUM_t;

typedef struct
{
    vuint32_t mCURR_LINE_NUM : 17;
    uint32_t  mUnused17 : 14;
    vuint32_t mDBG_HIT : 1;
} isp_IDBG2_CURR_LINE_NUM_t;

typedef struct
{
    vuint32_t mADDR : 12;
    uint32_t  mUnused12 : 4;
    vuint32_t mNAME : 6;
    uint32_t  mUnused22 : 6;
    vuint32_t mRDWF : 2;
    vuint32_t mWDWF : 2;
} isp_IDBG2_IMA_t;

typedef struct
{
    vuint32_t mAF : 1;
    vuint32_t mGCM : 1;
    vuint32_t mCAS : 1;
    vuint32_t mCCONVMED : 1;
    vuint32_t mEE : 1;
    vuint32_t mDF : 1;
    vuint32_t mDMAP : 1;
    vuint32_t mNR : 1;
    vuint32_t mDEMOSAIC : 1;
    vuint32_t mCSC : 1;
    vuint32_t mDRC : 1;
    vuint32_t mPKT : 1;
    uint32_t  mUnused12 : 20;
} isp_IDBG2_DONE_STAT_t;

typedef struct
{
    isp_IDBG2_LINE_NUM_t      mLINE_NUM;
    isp_IDBG2_CURR_LINE_NUM_t mCURR_LINE_NUM;
    isp_IDBG2_IMA_t           mIMA;
    vuint32_t                 mIMD;
    isp_IDBG2_DONE_STAT_t     mDONE_STAT;
} isp_IDBG2_t;

typedef struct
{
    vuint32_t                          mCTEMP_CNT_WHITE_CAM0;          // address 8192 = 0x2000;
    vuint32_t                          mCTEMP_SUMRL_CAM0;              // address 8196 = 0x2004;
    vuint32_t                          mCTEMP_SUMRH_CAM0;              // address 8200 = 0x2008;
    vuint32_t                          mCTEMP_SUMGL_CAM0;              // address 8204 = 0x200C;
    vuint32_t                          mCTEMP_SUMGH_CAM0;              // address 8208 = 0x2010;
    vuint32_t                          mCTEMP_SUMBL_CAM0;              // address 8212 = 0x2014;
    vuint32_t                          mCTEMP_SUMBH_CAM0;              // address 8216 = 0x2018;
    vuint32_t                          mCTEMP_SUMRGL_CAM0;             // address 8220 = 0x201C;
    vuint32_t                          mCTEMP_SUMRGH_CAM0;             // address 8224 = 0x2020 ;
    vuint32_t                          mCTEMP_SUMBGL_CAM0;             // address 8228 = 0x2024;
    vuint32_t                          mCTEMP_SUMBGH_CAM0;             // address 8232 = 0x2028;
    isp_COLOR_TEMP_CROI0_PIXCNT_CAM0_t mCTEMP_CROI0_PIXCNT_CAM0;       // address 8236 = 0x202C;
    vuint32_t                          mCTEMP_CROI0_SUMRED_CAM0;       // address 8240 = 0x2030;
    vuint32_t                          mCTEMP_CROI0_SUMGREEN_CAM0;     // address 8244 = 0x2034;
    vuint32_t                          mCTEMP_CROI0_SUMBLUE_CAM0;      // address 8248 = 0x2038;
    isp_COLOR_TEMP_CROI1_PIXCNT_CAM0_t mCTEMP_CROI1_PIXCNT_CAM0;       // address 8252 = 0x203C;
    vuint32_t                          mCTEMP_CROI1_SUMRED_CAM0;       // address 8256 = 0x2040;
    vuint32_t                          mCTEMP_CROI1_SUMGREEN_CAM0;     // address 8260 = 0x2044;
    vuint32_t                          mCTEMP_CROI1_SUMBLUE_CAM0;      // address 8264 = 0x2048;
    isp_COLOR_TEMP_CROI2_PIXCNT_CAM0_t mCTEMP_CROI2_PIXCNT_CAM0;       // address 8268 = 0x204C;
    vuint32_t                          mCTEMP_CROI2_SUMRED_CAM0;       // address 8272 = 0x2050;
    vuint32_t                          mCTEMP_CROI2_SUMGREEN_CAM0;     // address 8276 = 0x2054;
    vuint32_t                          mCTEMP_CROI2_SUMBLUE_CAM0;      // address 8280 = 0x2058;
    isp_COLOR_TEMP_CROI3_PIXCNT_CAM0_t mCTEMP_CROI3_PIXCNT_CAM0;       // address 8284 = 0x205C;
    vuint32_t                          mCTEMP_CROI3_SUMRED_CAM0;       // address 8288 = 0x2060;
    vuint32_t                          mCTEMP_CROI3_SUMGREEN_CAM0;     // address 8292 = 0x2064;
    vuint32_t                          mCTEMP_CROI3_SUMBLUE_CAM0;      // address 8296 = 0x2068;
    isp_COLOR_TEMP_CROI4_PIXCNT_CAM0_t mCTEMP_CROI4_PIXCNT_CAM0;       // address 8300 = 0x206C;
    vuint32_t                          mCTEMP_CROI4_SUMRED_CAM0;       // address 8304 = 0x2070;
    vuint32_t                          mCTEMP_CROI4_SUMGREEN_CAM0;     // address 8308 = 0x2074;
    vuint32_t                          mCTEMP_CROI4_SUMBLUE_CAM0;      // address 8312 = 0x2078;
    isp_COLOR_TEMP_CROI5_PIXCNT_CAM0_t mCTEMP_CROI5_PIXCNT_CAM0;       // address 8316 = 0x207C;
    vuint32_t                          mCTEMP_CROI5_SUMRED_CAM0;       // address 8320 = 0x2080;
    vuint32_t                          mCTEMP_CROI5_SUMGREEN_CAM0;     // address 8324 = 0x2084;
    vuint32_t                          mCTEMP_CROI5_SUMBLUE_CAM0;      // address 8328 = 0x2088;
    isp_COLOR_TEMP_CROI6_PIXCNT_CAM0_t mCTEMP_CROI6_PIXCNT_CAM0;       // address 8332 = 0x208C;
    vuint32_t                          mCTEMP_CROI6_SUMRED_CAM0;       // address 8226 = 0x2090;
    vuint32_t                          mCTEMP_CROI6_SUMGREEN_CAM0;     // address 8230 = 0x2094;
    vuint32_t                          mCTEMP_CROI6_SUMBLUE_CAM0;      // address 8234 = 0x2098;
    isp_COLOR_TEMP_CROI7_PIXCNT_CAM0_t mCTEMP_CROI7_PIXCNT_CAM0;       // address 8348 = 0x209C;
    vuint32_t                          mCTEMP_CROI7_SUMRED_CAM0;       // address 8352 = 0x20A0;
    vuint32_t                          mCTEMP_CROI7_SUMGREEN_CAM0;     // address 8356 = 0x20A4;
    vuint32_t                          mCTEMP_CROI7_SUMBLUE_CAM0;      // address 8360 = 0x20A8;
    isp_COLOR_TEMP_CROI8_PIXCNT_CAM0_t mCTEMP_CROI8_PIXCNT_CAM0;       // address 8364 = 0x20AC;
    vuint32_t                          mCTEMP_CROI8_SUMRED_CAM0;       // address 8368 = 0x20B0;
    vuint32_t                          mCTEMP_CROI8_SUMGREEN_CAM0;     // address 8372 = 0x20B4;
    vuint32_t                          mCTEMP_CROI8_SUMBLUE_CAM0;      // address 8376 = 0x20B8;
    isp_COLOR_TEMP_CROI9_PIXCNT_CAM0_t mCTEMP_CROI9_PIXCNT_CAM0;       // address 8380 = 0x20BC;
    vuint32_t                          mCTEMP_CROI9_SUMRED_CAM0;       // address 8384 = 0x20C0;
    vuint32_t                          mCTEMP_CROI9_SUMGREEN_CAM0;     // address 8488 = 0x20C4;
    vuint32_t                          mCTEMP_CROI9_SUMBLUE_CAM0;      // address 8492 = 0x20C8;
    vuint32_t                          mCTEMP_GR_GB_CNT_CAM0;          // address 8396 = 0x20CC;
    vint32_t                           mCTEMP_GR_SUM_CAM0;             // address 8400 = 0x20D0;
    vint32_t                           mCTEMP_GB_SUM_CAM0;             // address 8404 = 0x20D4;
    vuint32_t                          mCTEMP_GR2_SUM_CAM0;            // address 8408 = 0x20D8;
    vuint32_t                          mCTEMP_GB2_SUM_CAM0;            // address 8412 = 0x20DC;
    isp_COLOR_TEMP_CTRL_CAM0_t         mCTRL_CAM0;                     // address 8416 = 0x20E0, dont use;
    isp_COLOR_TEMP_CTRL_CAM0_t         mCTRL_CAM02ND;                  // address 8420 = 0x20E4, dont use;
    vint32_t                           mCTEMP_GRGB_SUM_CAM0;           // address 8424 = 0x20E8;
    vuint32_t                          mDRC_GROI0_SUM_CAM0;            // address 8428 = 0x20EC;
    vuint32_t                          mDRC_GROI1_SUM_CAM0;            // address 8432 = 0x20F0;
    vuint32_t                          mAF_ROI0_SUM0_CAM0;             // address 8436 = 0x20F4;
    vuint32_t                          mAF_ROI0_SUM1_CAM0;             // address 8440 = 0x20F8;
    vuint32_t                          mAF_ROI1_SUM0_CAM0;             // address 8444 = 0x20FC;
    vuint32_t                          mAF_ROI1_SUM1_CAM0;             // address 8448 = 0x2100;
    vuint32_t                          mAF_ROI2_SUM0_CAM0;             // address 8452 = 0x2104;
    vuint32_t                          mAF_ROI2_SUM1_CAM0;             // address 8456 = 0x2108;
    vuint32_t                          mAF_ROI3_SUM0_CAM0;             // address 8460 = 0x210C;
    vuint32_t                          mAF_ROI3_SUM1_CAM0;             // address 8464 = 0x2110;
    vuint32_t                          mAF_ROI4_SUM0_CAM0;             // address 8468 = 0x2114;
    vuint32_t                          mAF_ROI4_SUM1_CAM0;             // address 8472 = 0x2118;
    vuint32_t                          mAF_ROI5_SUM0_CAM0;             // address 8476 = 0x211C;
    vuint32_t                          mAF_ROI5_SUM1_CAM0;             // address 8480 = 0x2120;
    vuint32_t                          mAF_ROI6_SUM0_CAM0;             // address 8484 = 0x2124;
    vuint32_t                          mAF_ROI6_SUM1_CAM0;             // address 8488 = 0x2128;
    vuint32_t                          mAF_ROI7_SUM0_CAM0;             // address 8492 = 0x212C;
    vuint32_t                          mAF_ROI7_SUM1_CAM0;             // address 8496 = 0x2130;
    vuint32_t                          mAF_ROI8_SUM0_CAM0;             // address 8500 = 0x2134;
    vuint32_t                          mAF_ROI8_SUM1_CAM0;             // address 8504 = 0x2138;
    isp_BNR_EDGE_STAT_CAM0_t           mBNR_EDGE_STAT_EDGE_PIXEL_CAM0; // address 8508 = 0x213C;
    isp_BNR_EDGES_STAT_CAM0_t          mBNR_EDGES_STAT_EDGE_PIXE_CAM0; // address 8512 = 0x2140;
    isp_NR_EDGECNT_CAM0_t              mNR_EDGECNT_VAL_CAM0;           // address 8516 = 0x2144;
    isp_EE_EDGECNT_CAM0_t              mEE_EDGECNT_VAL_CAM0;           // address 8520 = 0x2148;
    isp_DF_EDGECNT_CAM0_t              mDF_EDGECNT_VAL_CAM0;           // address 8524 = 0x214C;
} isp_ALIAS_t;

// SoC address space
typedef struct
{
    isp_PIPE_CONF_t       mPIPE_CONF;       // 0,         0x68
    uint32_t              mpUnused0[22];    // 0x68,      0x58
    isp_HC_t              mHC;              // 0xC0,      0x4
    uint32_t              mpUnused1[15];    // 0xC4,      0x3C
    isp_HDR_Decompress0_t mHDR_Decompress0; // 0x100,     0x48
    uint32_t              mpUnused2[14];    // 0x148,     0x38
    isp_HDR_Decompress1_t mHDR_Decompress1; // 0x180,     0x48
    uint32_t              mpUnused3[14];    // 0x1C8,     0x38
    isp_OB_WB0_t          mOB_WB0;          // 0x200,     0x14
    uint32_t              mpUnused4[11];    // 0x214,     0x2C
    isp_OB_WB1_t          mOB_WB1;          // 0x240,     0x14
    uint32_t              mpUnused5[11];    // 0x254,     0x2C
    isp_OB_WB2_t          mOB_WB2;          // 0x280,     0x14
    uint32_t              mpUnused6[27];    // 0x294,     0x6C
    isp_HDR_MERGE_t       mHDR_MERGE;       // 0x300,     0x48
    uint32_t              mpUnused7[46];    // 0x348,     0xB8
    isp_COLOR_TEMP_t      mCOLOR_TEMP;      // 0x400,     0x1A4
    uint32_t              mpUnused8[23];    // 0x5A4,     0x5C
    isp_RGBIR_t           mRGBIR;           // 0x600,     0x40
    uint32_t              mpUnused9[48];    // 0x640,     0xC0
    isp_STAT_t            mSTAT;            // 0x700,     0x40
    uint32_t              mpUnused10[16];   // 0x740,     0x40
    isp_IR_COMPRESS_t     mIR_COMPRESS;     // 0x780,     0x48
    uint32_t              mpUnused11[14];   // 0x7C8,     0x38
    isp_BNR_t             mBNR;             // 0x800,     0x68
    uint32_t              mpUnused12[38];   // 0x868,     0x98
    isp_VIGNETTING_t      mVIGNETTING;      // 0x900,     0x2C
    uint32_t              mpUnused13[421];  // 0x92C,     0x694
    isp_IDBG1_t           mIDBG1;           // 0xFC0,     0x14
    uint32_t              mpUnused14[107];  // 0xFD4,     0x1AC
    isp_DEMOSAIC_t        mDEMOSAIC;        // 0x1180,    0x10
    uint32_t              mpUnused15[12];   // 0x1190,    0x30
    isp_RGB_to_YUV_t      mRGB_to_YUV;      // 0x11C0,    0x2C
    uint32_t              mpUnused16[69];   // 0x11EC,    0x114
    isp_DRC_t             mDRC;             // 0x1300,    0x58
    uint32_t              mpUnused17[42];   // 0x1358,    0xA8
    isp_NR_t              mNR;              // 0x1400,    0x14
    uint32_t              mpUnused18[11];   // 0x1414,    0x2C
    isp_DF_t              mDF;              // 0x1440,    0x14
    uint32_t              mpUnused19[11];   // 0x1454,    0x2C
    isp_EE_t              mEE;              // 0x1480,    0x14
    uint32_t              mpUnused20[11];   // 0x1494,    0x2C
    isp_CCONVMED_t        mCCONVMED;        // 0x14C0,    0x4
    uint32_t              mpUnused21[15];   // 0x14C4,    0x3C
    isp_CAS_t             mCAS;             // 0x1500,    0x10
    uint32_t              mpUnused22[28];   // 0x1510,    0x70
    isp_Packetizer_t      mPacketizer;      // 0x1580,    0xC
    uint32_t              mpUnused23[29];   // 0x158C,    0x74
    isp_GCM_t             mGCM;             // 0x1600,    0x84
    uint32_t              mpUnused24[31];   // 0x1684,    0x7C
    isp_AUTOFOCUS_t       mAUTOFOCUS;       // 0x1700,    0xB8
    uint32_t              mpUnused25[514];  // 0x17B8,    0x808
    isp_IDBG2_t           mIDBG2;           // 0x1FC0,    0x14
    uint32_t              mpUnused26[11];   // 0x1FD4,    0x2C
    isp_ALIAS_t           mALIAS;           // 0x2000,    0x150
} imx9x_Regs_t;

#endif // __include_RegConfig_h
