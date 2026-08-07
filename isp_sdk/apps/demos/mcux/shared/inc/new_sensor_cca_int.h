/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef NEW_SENSOR_CCA_INT_H
#define NEW_SENSOR_CCA_INT_H

/******************************************************************************/
/**
 * @file new_sensor_cca_int.h
 * @brief Internal declarations for the sensor driver in CCA
 *******************************************************************************/

/*==============================================================================
*                                INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

#define Q8_1  (0x100U)
#define Q10_1 (0x400U)
#define Q16_1 (0x10000U)

#define NEW_SENSOR_MIN_AGAIN_LONG_F    (1.0f) /* 1.0 is minimum analog gain for the Long frame */
#define NEW_SENSOR_MIN_DGAIN_LONG_F    (1.0f) /* 1.0 is minimum digital gain for the Long frame */
#define NEW_SENSOR_MIN_AGAIN_SHORT_F   (1.0f) /* 1.0 is minimum analog gain for the Short frame */
#define NEW_SENSOR_MIN_DGAIN_SHORT_F   (1.0f) /* 1.0 is minimum digital gain for Short frame */
#define NEW_SENSOR_MIN_AGAIN_VS_F      (1.0f) /* 1.0 is minimum analog gain for the VS frame */
#define NEW_SENSOR_MIN_DGAIN_VS_F      (1.0f) /* 1.0 is minimum digital gain for VS frame */
#define NEW_SENSOR_MIN_AGAIN_SPD_F     (4.5f) /* 4.5 is minimum analog gain for the SPD frame */
#define NEW_SENSOR_MIN_DGAIN_SPD_F     (1.0f) /* 1.0 is minimum digital gain for SPD frame */

/* applies to 3exposures only */
#define NEW_SENSOR_3EXP_MAX_AGAIN_LONG_F    (15.5f)   /* 15.5 is maximum analog gain for the Long frame */
#define NEW_SENSOR_3EXP_MAX_DGAIN_LONG_F    (15.999f) /* 15.999 is maximum digital gain for the Long frame */
#define NEW_SENSOR_3EXP_MAX_AGAIN_SHORT_F   (15.5f)   /* 15.5 is maximum analog gain for the Short frame */
#define NEW_SENSOR_3EXP_MAX_DGAIN_SHORT_F   (15.999f) /* 15.999 is maximum digital gain for Short frame */
#define NEW_SENSOR_3EXP_MAX_AGAIN_VS_F      (15.5f)   /* 15.5 is maximum analog gain for the VS frame */
#define NEW_SENSOR_3EXP_MAX_DGAIN_VS_F      (15.999f) /* 15.999 is maximum digital gain for VS frame */

/* applies to 4exposures only */
#define NEW_SENSOR_4EXP_MAX_AGAIN_LONG_F    (15.0f) /* 15.0 is maximum analog gain for the Long frame */
#define NEW_SENSOR_4EXP_MAX_DGAIN_LONG_F    (15.0f) /* 15.0 is maximum digital gain for the Long frame */
#define NEW_SENSOR_4EXP_MAX_AGAIN_SHORT_F   (15.0f) /* 15.0 is maximum analog gain for the Short frame */
#define NEW_SENSOR_4EXP_MAX_DGAIN_SHORT_F   (15.0f) /* 15.0 is maximum digital gain for Short frame */
#define NEW_SENSOR_4EXP_MAX_AGAIN_VS_F      (15.0f) /* 15.0 is maximum analog gain for the VS frame */
#define NEW_SENSOR_4EXP_MAX_DGAIN_VS_F      (15.0f) /* 15.0 is maximum digital gain for VS frame */
#define NEW_SENSOR_4EXP_MAX_AGAIN_SPD_F     (15.0f) /* 15.0 is maximum analog gain for the SPD frame */
#define NEW_SENSOR_4EXP_MAX_DGAIN_SPD_F     (15.0f) /* 15.0 is maximum digital gain for SPD frame */

/* choose if 3exp or 4exp mode */
#ifdef CCA_NEW_SENSOR_3EXP
    #define NEW_SENSOR_MAX_AGAIN_LONG_F    NEW_SENSOR_3EXP_MAX_AGAIN_LONG_F
    #define NEW_SENSOR_MAX_DGAIN_LONG_F    NEW_SENSOR_3EXP_MAX_DGAIN_LONG_F
    #define NEW_SENSOR_MAX_AGAIN_SHORT_F   NEW_SENSOR_3EXP_MAX_AGAIN_SHORT_F
    #define NEW_SENSOR_MAX_DGAIN_SHORT_F   NEW_SENSOR_3EXP_MAX_DGAIN_SHORT_F
    #define NEW_SENSOR_MAX_AGAIN_VS_F      NEW_SENSOR_3EXP_MAX_AGAIN_VS_F
    #define NEW_SENSOR_MAX_DGAIN_VS_F      NEW_SENSOR_3EXP_MAX_DGAIN_VS_F
    #define NEW_SENSOR_MAX_AGAIN_SPD_F     NEW_SENSOR_MIN_AGAIN_SPD_F
    #define NEW_SENSOR_MAX_DGAIN_SPD_F     NEW_SENSOR_MIN_DGAIN_SPD_F
#else // from ifdef CCA_NEW_SENSOR_3EXP
    #define NEW_SENSOR_MAX_AGAIN_LONG_F    NEW_SENSOR_4EXP_MAX_AGAIN_LONG_F
    #define NEW_SENSOR_MAX_DGAIN_LONG_F    NEW_SENSOR_4EXP_MAX_DGAIN_LONG_F
    #define NEW_SENSOR_MAX_AGAIN_SHORT_F   NEW_SENSOR_4EXP_MAX_AGAIN_SHORT_F
    #define NEW_SENSOR_MAX_DGAIN_SHORT_F   NEW_SENSOR_4EXP_MAX_DGAIN_SHORT_F
    #define NEW_SENSOR_MAX_AGAIN_VS_F      NEW_SENSOR_4EXP_MAX_AGAIN_VS_F
    #define NEW_SENSOR_MAX_DGAIN_VS_F      NEW_SENSOR_4EXP_MAX_DGAIN_VS_F
    #define NEW_SENSOR_MAX_AGAIN_SPD_F     NEW_SENSOR_4EXP_MAX_AGAIN_SPD_F
    #define NEW_SENSOR_MAX_DGAIN_SPD_F     NEW_SENSOR_4EXP_MAX_DGAIN_SPD_F
#endif // else from ifdef CCA_NEW_SENSOR_3EXP

#define NEW_SENSOR_MIN_AGAIN_LONG      (NEW_SENSOR_MIN_AGAIN_LONG_F  * (float)Q16_1)
#define NEW_SENSOR_MIN_DGAIN_LONG      (NEW_SENSOR_MIN_DGAIN_LONG_F  * (float)Q16_1)
#define NEW_SENSOR_MIN_AGAIN_SHORT     (NEW_SENSOR_MIN_AGAIN_SHORT_F * (float)Q16_1)
#define NEW_SENSOR_MIN_DGAIN_SHORT     (NEW_SENSOR_MIN_DGAIN_SHORT_F * (float)Q16_1)
#define NEW_SENSOR_MIN_AGAIN_VS        (NEW_SENSOR_MIN_AGAIN_VS_F    * (float)Q16_1)
#define NEW_SENSOR_MIN_DGAIN_VS        (NEW_SENSOR_MIN_DGAIN_VS_F    * (float)Q16_1)
#define NEW_SENSOR_MIN_AGAIN_SPD       (NEW_SENSOR_MIN_AGAIN_SPD_F   * (float)Q16_1)
#define NEW_SENSOR_MIN_DGAIN_SPD       (NEW_SENSOR_MIN_DGAIN_SPD_F   * (float)Q16_1)

#define NEW_SENSOR_MAX_AGAIN_LONG      (NEW_SENSOR_MAX_AGAIN_LONG_F  * (float)Q16_1)
#define NEW_SENSOR_MAX_DGAIN_LONG      (NEW_SENSOR_MAX_DGAIN_LONG_F  * (float)Q16_1)
#define NEW_SENSOR_MAX_AGAIN_SHORT     (NEW_SENSOR_MAX_AGAIN_SHORT_F * (float)Q16_1)
#define NEW_SENSOR_MAX_DGAIN_SHORT     (NEW_SENSOR_MAX_DGAIN_SHORT_F * (float)Q16_1)
#define NEW_SENSOR_MAX_AGAIN_VS        (NEW_SENSOR_MAX_AGAIN_VS_F    * (float)Q16_1)
#define NEW_SENSOR_MAX_DGAIN_VS        (NEW_SENSOR_MAX_DGAIN_VS_F    * (float)Q16_1)
#define NEW_SENSOR_MAX_AGAIN_SPD       (NEW_SENSOR_MAX_AGAIN_SPD_F   * (float)Q16_1)
#define NEW_SENSOR_MAX_DGAIN_SPD       (NEW_SENSOR_MAX_DGAIN_SPD_F   * (float)Q16_1)

#define NEW_SENSOR_AGAIN_MASK_RANGE_1  (0xFF000U) /* full fraction range */
#define NEW_SENSOR_AGAIN_MASK_RANGE_2  (0xFE000U) /* 1 bit loss in fraction */
#define NEW_SENSOR_AGAIN_MASK_RANGE_3  (0xFC000U) /* 2 bit loss in fraction */
#define NEW_SENSOR_AGAIN_MASK_RANGE_4  (0xF8000U) /* 3 bit loss in fraction */

#define NEW_SENSOR_DGAIN_MASK          (0xFFFC0U) /* as maximum for register mapping */

#define AGAIN_RANGE_1_MIN       (0x10000U) /* 1.0 gain is minimum for register mapping */
#define AGAIN_RANGE_1_MAX       (0x20000U)
#define AGAIN_RANGE_2_MIN       (AGAIN_RANGE_1_MAX)
#define AGAIN_RANGE_2_MAX       (0x40000U)
#define AGAIN_RANGE_3_MIN       (AGAIN_RANGE_2_MAX)
#define AGAIN_RANGE_3_MAX       (0x80000U)

#define PREC_MULT               (1000U)
#define MULT256                 (256U)

#define SCLK                    (62.0f) //62MHz

#define HTS_DCG                 (0x5e2U) //1506
#define HTS_SPD                 (0x2F1U) //753
#define HTS_VS                  (0x2F1U) //753
#ifdef CCA_NEW_SENSOR_3EXP
    #define HTS                     (HTS_DCG + HTS_VS) //2259
    #define VTS                     (0x2AEU)
#else
    #define HTS                     (HTS_DCG + HTS_VS + HTS_SPD) //3012
    #define VTS                     (0x2A3U) //675
#endif

#define MIN_VS_EXP_LINES        (0U) //A value of 0 is confirmed by OVT for 3exp mode and we assume it is usable for 4exp mode
#ifdef CCA_NEW_SENSOR_3EXP
    #define MAX_VS_EXP_LINES        (16U)
    #define MIN_EXP_LINES           (4U)
    #define MAX_EXP_LINES           (VTS - MAX_VS_EXP_LINES - 12U - 1U) //639 => max exp_time= MAX_EXP_LINES* ROW_TIME_NS  = 639 * 36.435usec = 23.281 miliseconds
    #define ROW_TIME_NS             ((uint32_t)((HTS * 1000U) / ((uint32_t) SCLK))) // 36435ns
#else
    #define MAX_VS_EXP_LINES        (31U)
    #define MIN_EXP_LINES           (2U)
    #define MAX_EXP_LINES           (VTS - MAX_VS_EXP_LINES - 12U - 1U) //631 => max exp=30653uS
    #define ROW_TIME_NS             ((uint32_t)((HTS * 1000U) / ((uint32_t) SCLK))) //48580nS
#endif

/* Setup for 60fps version running with TI ser/deser */
#define SCLK_60 (90.0f) // 90MHz

#define HTS_DCG_60 (0x447U) // 1095
#define HTS_SPD_60 (0x20DU) // 525
#define HTS_VS_60 (0x22BU) // 555

#define HTS_60 (HTS_DCG_60 + HTS_VS_60 + HTS_SPD_60) // 2175
#define VTS_60 (0x2AEU) // 686

#define MAX_EXP_LINES_60 (VTS_60 - MAX_VS_EXP_LINES - 12U - 1U) // 642 => max exp=15514uS
#define ROW_TIME_NS_60 ((uint32_t)((HTS_60 * 1000U) / ((uint32_t)SCLK_60))) // 24166nS
/* end Setup for 60fps version running with TI ser/deser */

#define NEW_SENSOR_DEFAULT_CG_RATIO      (7.32f * (float)Q16_1)
#define NEW_SENSOR_DEFAULT_LPD_SPD_RATIO (108.5f * (float)Q16_1)

#ifdef USE_OFFSET_M
    #define OFFSET_M            (0.232621227534758f)
#else
    #define OFFSET_M            (0.0f)
#endif

#ifdef CCA_NEW_SENSOR_3EXP
    #define OFFSET_VS           (0.729738894540522f)
#else
    #define OFFSET_VS           (0.6f)
#endif

#define USE_FIXED_EXP_RATIO

#ifdef USE_FIXED_EXP_RATIO
    #ifdef CCA_NEW_SENSOR_3EXP
        #define NEW_SENSOR_RATIO_LONG_TO_SHORT      (32U * Q16_1)  /* in Q16 */
        #define NEW_SENSOR_RATIO_LONG_TO_SPD        (64U * Q16_1)   /* in Q16 */
        #define NEW_SENSOR_RATIO_LONG_TO_VS         (1024U * Q16_1) /* in Q16 */
    #else
        #define NEW_SENSOR_RATIO_LONG_TO_SHORT (16U * Q16_1)      /* in Q16 */
        #define NEW_SENSOR_RATIO_LONG_TO_SPD   (1024U * Q16_1)   /* in Q16 */
        #define NEW_SENSOR_RATIO_LONG_TO_VS    (128U * Q16_1)    /* in Q16 */
    #endif
#endif


#define NEW_SENSOR_GROUP_HOLD_REG          (0x3208U)
#define NEW_SENSOR_GROUP_LAUNCH_REG        (0x3211U)

#define NEW_SENSOR_DCG_EXP_REG             (0x3501U)

#define NEW_SENSOR_SPD_EXP_REG             (0x3541U)

#define NEW_SENSOR_VS_EXP_REG              (0x35C1U)

#define NEW_SENSOR_HCG_REAL_GAIN_REG       (0x3508U)
#define NEW_SENSOR_HCG_DIG_GAIN_REG        (0x350AU)

#define NEW_SENSOR_SPD_REAL_GAIN_REG       (0x3548U)
#define NEW_SENSOR_SPD_DIG_GAIN_REG        (0x354AU)

#define NEW_SENSOR_VS_REAL_GAIN_REG        (0x35C8U)
#define NEW_SENSOR_VS_DIG_GAIN_REG         (0x35CAU)

#define NEW_SENSOR_LCG_REAL_GAIN_REG       (0x3588U)
#define NEW_SENSOR_LCG_DIG_GAIN_REG        (0x358AU)

#define NEW_SENSOR_FRAME_COUNT_REG         (0x483EU)

#define NEW_SENSOR_AVERAGE_TEMPERATURE_REG (0x4D2AU)

#define NEW_SENSOR_WB_GAIN_B_HCG_REG       (0x5280U)
#define NEW_SENSOR_WB_GAIN_GB_HCG_REG      (0x5282U)
#define NEW_SENSOR_WB_GAIN_GR_HCG_REG      (0x5284U)
#define NEW_SENSOR_WB_GAIN_R_HCG_REG       (0x5286U)

#define NEW_SENSOR_WB_GAIN_B_LCG_REG       (0x5480U)
#define NEW_SENSOR_WB_GAIN_GB_LCG_REG      (0x5482U)
#define NEW_SENSOR_WB_GAIN_GR_LCG_REG      (0x5484U)
#define NEW_SENSOR_WB_GAIN_R_LCG_REG       (0x5486U)

#define NEW_SENSOR_WB_GAIN_B_SPD_REG       (0x5680U)
#define NEW_SENSOR_WB_GAIN_GB_SPD_REG      (0x5682U)
#define NEW_SENSOR_WB_GAIN_GR_SPD_REG      (0x5684U)
#define NEW_SENSOR_WB_GAIN_R_SPD_REG       (0x5686U)

#define NEW_SENSOR_WB_GAIN_B_VS_REG        (0x5880U)
#define NEW_SENSOR_WB_GAIN_GB_VS_REG       (0x5882U)
#define NEW_SENSOR_WB_GAIN_GR_VS_REG       (0x5884U)
#define NEW_SENSOR_WB_GAIN_R_VS_REG        (0x5886U)

#define NEW_SENSOR_VTS_REG                 (0x380EU)

#define NEW_SENSOR_OTP_SENS_HCG_REG        (0x7057U)
#define NEW_SENSOR_OTP_SENS_LCG_REG        (0x705BU)
#define NEW_SENSOR_OTP_SENS_SPD_REG        (0x705FU)

typedef enum
{
    DCG_EXP_H,
    DCG_EXP_L,
    HCG_REAL_GAIN_H,
    HCG_REAL_GAIN_L,
    HCG_DIG_GAIN_H,
    HCG_DIG_GAIN_M,
    HCG_DIG_GAIN_L,
    SPD_EXP_H,
    SPD_EXP_L,
    SPD_REAL_GAIN_H,
    SPD_REAL_GAIN_L,
    SPD_DIG_GAIN_H,
    SPD_DIG_GAIN_M,
    SPD_DIG_GAIN_L,
    LCG_REAL_GAIN_H,
    LCG_REAL_GAIN_L,
    LCG_DIG_GAIN_H,
    LCG_DIG_GAIN_M,
    LCG_DIG_GAIN_L,
    VS_EXP_H,
    VS_EXP_L,
    VS_REAL_GAIN_H,
    VS_REAL_GAIN_L,
    VS_DIG_GAIN_H,
    VS_DIG_GAIN_M,
    VS_DIG_GAIN_L,
    FRAME_COUNT_H,
    FRAME_COUNT_L,
    TEMPERATURE_SENSOR_H,
    TEMPERATURE_SENSOR_L,
    WB_GAIN_B_HCG_H,
    WB_GAIN_B_HCG_L,
    WB_GAIN_GB_HCG_H,
    WB_GAIN_GB_HCG_L,
    WB_GAIN_GR_HCG_H,
    WB_GAIN_GR_HCG_L,
    WB_GAIN_R_HCG_H,
    WB_GAIN_R_HCG_L,
    WB_GAIN_B_LCG_H,
    WB_GAIN_B_LCG_L,
    WB_GAIN_GB_LCG_H,
    WB_GAIN_GB_LCG_L,
    WB_GAIN_GR_LCG_H,
    WB_GAIN_GR_LCG_L,
    WB_GAIN_R_LCG_H,
    WB_GAIN_R_LCG_L,
    WB_GAIN_B_SPD_H,
    WB_GAIN_B_SPD_L,
    WB_GAIN_GB_SPD_H,
    WB_GAIN_GB_SPD_L,
    WB_GAIN_GR_SPD_H,
    WB_GAIN_GR_SPD_L,
    WB_GAIN_R_SPD_H,
    WB_GAIN_R_SPD_L,
    WB_GAIN_B_VS_H,
    WB_GAIN_B_VS_L,
    WB_GAIN_GB_VS_H,
    WB_GAIN_GB_VS_L,
    WB_GAIN_GR_VS_H,
    WB_GAIN_GR_VS_L,
    WB_GAIN_R_VS_H,
    WB_GAIN_R_VS_L,
    EMB_PADDING_1,
    EMB_PADDING_2,
    MCRC32_B0_LSB,
    MCRC32_B1,
    MCRC32_B2,
    MCRC32_B3_MSB,
    CRC32_B0_LSB,
    CRC32_B1,
    CRC32_B2,
    CRC32_B3_MSB,
    NEW_SENSOR_EMB_DATA_SIZE
} NEW_SENSORSenEmbDataRegMap;

#ifdef NEW_SENSOR_PWL_12BITS
#define NEW_SENSOR_EMB_LINE_DATA_SHIFT            (4U)
#else
#define NEW_SENSOR_EMB_LINE_DATA_SHIFT            (8U)
#endif

#define NEW_SENSOR_EMB_LINE_DCG_EXP_H_IDX         ((uint32_t)DCG_EXP_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_DCG_EXP_L_IDX         ((uint32_t)DCG_EXP_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_SPD_EXP_H_IDX         ((uint32_t)SPD_EXP_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_SPD_EXP_L_IDX         ((uint32_t)SPD_EXP_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_VS_EXP_H_IDX          ((uint32_t)VS_EXP_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_VS_EXP_L_IDX          ((uint32_t)VS_EXP_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_HCG_REAL_GAIN_H_IDX   ((uint32_t)HCG_REAL_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_HCG_REAL_GAIN_L_IDX   ((uint32_t)HCG_REAL_GAIN_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_HCG_DIG_GAIN_H_IDX    ((uint32_t)HCG_DIG_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_HCG_DIG_GAIN_M_IDX    ((uint32_t)HCG_DIG_GAIN_M*2U+1U)
#define NEW_SENSOR_EMB_LINE_HCG_DIG_GAIN_L_IDX    ((uint32_t)HCG_DIG_GAIN_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_LCG_REAL_GAIN_H_IDX   ((uint32_t)LCG_REAL_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_LCG_REAL_GAIN_L_IDX   ((uint32_t)LCG_REAL_GAIN_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_LCG_DIG_GAIN_H_IDX    ((uint32_t)LCG_DIG_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_LCG_DIG_GAIN_M_IDX    ((uint32_t)LCG_DIG_GAIN_M*2U+1U)
#define NEW_SENSOR_EMB_LINE_LCG_DIG_GAIN_L_IDX    ((uint32_t)LCG_DIG_GAIN_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_SPD_REAL_GAIN_H_IDX   ((uint32_t)SPD_REAL_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_SPD_REAL_GAIN_L_IDX   ((uint32_t)SPD_REAL_GAIN_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_SPD_DIG_GAIN_H_IDX    ((uint32_t)SPD_DIG_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_SPD_DIG_GAIN_M_IDX    ((uint32_t)SPD_DIG_GAIN_M*2U+1U)
#define NEW_SENSOR_EMB_LINE_SPD_DIG_GAIN_L_IDX    ((uint32_t)SPD_DIG_GAIN_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_VS_REAL_GAIN_H_IDX    ((uint32_t)VS_REAL_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_VS_REAL_GAIN_L_IDX    ((uint32_t)VS_REAL_GAIN_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_VS_DIG_GAIN_H_IDX     ((uint32_t)VS_DIG_GAIN_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_VS_DIG_GAIN_M_IDX     ((uint32_t)VS_DIG_GAIN_M*2U+1U)
#define NEW_SENSOR_EMB_LINE_VS_DIG_GAIN_L_IDX     ((uint32_t)VS_DIG_GAIN_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_FRAME_COUNT_H_IDX     ((uint32_t)FRAME_COUNT_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_FRAME_COUNT_L_IDX     ((uint32_t)FRAME_COUNT_L*2U+1U)

#define NEW_SENSOR_EMB_TEMPERATURE_SENSOR_H_IDX   ((uint32_t)TEMPERATURE_SENSOR_H*2U+1U)
#define NEW_SENSOR_EMB_TEMPERATURE_SENSOR_L_IDX   ((uint32_t)TEMPERATURE_SENSOR_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_HCG_H_IDX   ((uint32_t)WB_GAIN_B_HCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_HCG_L_IDX   ((uint32_t)WB_GAIN_B_HCG_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_HCG_H_IDX  ((uint32_t)WB_GAIN_GB_HCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_HCG_L_IDX  ((uint32_t)WB_GAIN_GB_HCG_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_HCG_H_IDX  ((uint32_t)WB_GAIN_GR_HCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_HCG_L_IDX  ((uint32_t)WB_GAIN_GR_HCG_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_HCG_H_IDX   ((uint32_t)WB_GAIN_R_HCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_HCG_L_IDX   ((uint32_t)WB_GAIN_R_HCG_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_LCG_H_IDX   ((uint32_t)WB_GAIN_B_LCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_LCG_L_IDX   ((uint32_t)WB_GAIN_B_LCG_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_LCG_H_IDX  ((uint32_t)WB_GAIN_GB_LCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_LCG_L_IDX  ((uint32_t)WB_GAIN_GB_LCG_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_LCG_H_IDX  ((uint32_t)WB_GAIN_GR_LCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_LCG_L_IDX  ((uint32_t)WB_GAIN_GR_LCG_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_LCG_H_IDX   ((uint32_t)WB_GAIN_R_LCG_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_LCG_L_IDX   ((uint32_t)WB_GAIN_R_LCG_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_SPD_H_IDX   ((uint32_t)WB_GAIN_B_SPD_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_SPD_L_IDX   ((uint32_t)WB_GAIN_B_SPD_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_SPD_H_IDX  ((uint32_t)WB_GAIN_GB_SPD_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_SPD_L_IDX  ((uint32_t)WB_GAIN_GB_SPD_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_SPD_H_IDX  ((uint32_t)WB_GAIN_GR_SPD_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_SPD_L_IDX  ((uint32_t)WB_GAIN_GR_SPD_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_SPD_H_IDX   ((uint32_t)WB_GAIN_R_SPD_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_SPD_L_IDX   ((uint32_t)WB_GAIN_R_SPD_L*2U+1U)

#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_VS_H_IDX   ((uint32_t)WB_GAIN_B_VS_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_B_VS_L_IDX   ((uint32_t)WB_GAIN_B_VS_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_VS_H_IDX  ((uint32_t)WB_GAIN_GB_VS_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GB_VS_L_IDX  ((uint32_t)WB_GAIN_GB_VS_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_VS_H_IDX  ((uint32_t)WB_GAIN_GR_VS_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_GR_VS_L_IDX  ((uint32_t)WB_GAIN_GR_VS_L*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_VS_H_IDX   ((uint32_t)WB_GAIN_R_VS_H*2U+1U)
#define NEW_SENSOR_EMB_LINE_WB_GAIN_R_VS_L_IDX   ((uint32_t)WB_GAIN_R_VS_L*2U+1U)

/*==============================================================================
*                                    ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/*==============================================================================
*                         GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                             FUNCTION PROTOTYPES
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef NEW_SENSOR_CCA_INT_H */
