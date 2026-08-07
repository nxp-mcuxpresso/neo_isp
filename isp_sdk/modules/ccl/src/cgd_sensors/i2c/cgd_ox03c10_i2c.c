/*******************************************************************************
 * Copyright 2024 - 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

/******************************************************************************/
/**
 * @file  cgd_ox03c10_i2c.c
 * @brief IMX95 registry configuration class
 *******************************************************************************/
#include "cgd_ox03c10_i2c.h"

#if defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))

#include "cgd_types.h"
#include "cgd_config.h"

/*==============================================================================
*                                LOCAL MACROS
==============================================================================*/

#define DESER_WAIT 2U // Typical wait time for deserializer operations (channel switching)
#define CGD_OX03C10_DESERIALIZER_REPLICATE_OUTPUT 0U // Replicate deserializer output to both ports
#define CGD_OX03C10_DESERIALIZER_I2C_PRIMARY_PORT 0U // Primary port for deserializer i2c input, 0 or 1

/******************************************************************************/
cgd_Cmd_t sgCgdOvx3cInitializationArr[] =
    {
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                            {90}}, /* wait for i2c bus start-up after gpio reset; t_WAKEUP is 2.25ms per datasheet, 5ms gives margin */

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0x00}}, /* enable FOUR chanels at once */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                    {DESER_WAIT}},

        /* Wait for GMSL2 link lock before broadcasting the serializer reset, so the reset does
           not rely on the link auto-locking within the initial delay window (timing sensitive). */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                             {0x02U, 0x00, 0x1A}}, // 0x1A, Wait for Link Lock
        {OX03C_I2C_DESERIALIZER_IDX,  CGD_CMD_WAIT,                         {0x01U, 20U, 5U, 0x01U}}, // wait 20 x 5ms for bit 0 to be set
        {OX03C_I2C_DESERIALIZER_IDX,   CGD_CMD_CON,                                         {0x01U}}, // mask

        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x10, 0x80}}, /* Serializer reset, automatically cleared */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x13, 0x40}}, /* Deserializer reset, automatically cleared */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                             {5}}, /* wait for the Deserializer reset done */

        /**
         * Deserializer first initialization
         */

        /* 0x8A2 register solve problems with CSI initialization - waiting for PHY STOP state*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x08, 0xA2, 0x04}}, /* Disable MIPI DPHY, timing 106.7ns */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0xFF}}, /* disable all remote control channels */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x06, 0xF0}}, /* disable link A-D; GMSL2 for links A-D */
/* Reg 0x07 Select I2C Control port of the deserializer board - 0x00 for Port 0, 0xF0 for Port 1 */
#if (CGD_OX03C10_DESERIALIZER_I2C_PRIMARY_PORT == 0)
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x07, 0x00}}, /* Control Channel Port Crossover Selector */
#else
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x07, 0xF0}}, /* Control Channel Port Crossover Selector */
#endif
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x04, 0x00}}, /* disable all video transmit channels (0-3) */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x08, 0xA2, 0xF4}}, /* Enable MIPI DPHY, timing 106.7ns */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x04, 0x0B, 0x00}}, /* CSI disabled; BPP = 0 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0xF4, 0x00}}, /* video pipes disabled; legacy mode */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x06, 0x41, 0x76}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x06, 0x51, 0x76}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x06, 0x61, 0x76}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x06, 0x71, 0x76}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x06, 0xFF}}, /* enable link A-D; GMSL2 for links A-D */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x18, 0x0F}}, /* Release link A-D reset; Reset datapath */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x08, 0xA0, 0x04}}, /* MIPI PHY 2x4 mode enabled */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x08, 0xA4, 0xE4}}, /* phy3_lane_map; phy2_lane_map */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x08, 0xA5, 0x00}}, /* MIPI PHY0  a PHY1 lane polarity settings; DPHY clock lane */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x08, 0xA6, 0x00}}, /* MIPI PHY2  a PHY3 lane polarity settings; */

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                            {90}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                             {0x02U, 0x00, 0x1A}}, // 0x1A, Wait for Link Lock
        {OX03C_I2C_DESERIALIZER_IDX,  CGD_CMD_WAIT,                         {0x01U, 20U, 5U, 0x01U}}, // wait 20 x 5ms for bit 0 to be set
        {OX03C_I2C_DESERIALIZER_IDX,   CGD_CMD_CON,                                         {0x01U}}, // mask
                                                            // end sp_Adr27_Deser_Init

        /**
         * Serializers and Cameras address translation setup, enable all channels at once
         */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0xFC}}, /* switch to channel A*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                    {DESER_WAIT}},
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,     {0x03U, 0x00, 0x42, OX03C_I2C_CAMERA1 << 1}}, /* set adress translator SRC_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,      {0x03U, 0x00, 0x43, OX03C_I2C_CAMERA << 1}}, /* set adress translator DST_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x44, OX03C_I2C_SERIALIZER1 << 1}}, /* set adress translator SRC_B*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,  {0x03U, 0x00, 0x45, OX03C_I2C_SERIALIZER << 1}}, /* set adress translator DST_B*/

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0xF3}}, /* switch to channel B*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                    {DESER_WAIT}},
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,     {0x03U, 0x00, 0x42, OX03C_I2C_CAMERA2 << 1}}, /* set adress translator SRC_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,      {0x03U, 0x00, 0x43, OX03C_I2C_CAMERA << 1}}, /* set adress translator DST_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x44, OX03C_I2C_SERIALIZER2 << 1}}, /* set adress translator SRC_B*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,  {0x03U, 0x00, 0x45, OX03C_I2C_SERIALIZER << 1}}, /* set adress translator DST_B*/

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0xCF}}, /* switch to channel C*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                    {DESER_WAIT}},
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,     {0x03U, 0x00, 0x42, OX03C_I2C_CAMERA3 << 1}}, /* set adress translator SRC_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,      {0x03U, 0x00, 0x43, OX03C_I2C_CAMERA << 1}}, /* set adress translator DST_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x44, OX03C_I2C_SERIALIZER3 << 1}}, /* set adress translator SRC_B*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,  {0x03U, 0x00, 0x45, OX03C_I2C_SERIALIZER << 1}}, /* set adress translator DST_B*/

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0x3F}}, /* switch to channel D*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                    {DESER_WAIT}},
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,     {0x03U, 0x00, 0x42, OX03C_I2C_CAMERA4 << 1}}, /* set adress translator SRC_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,      {0x03U, 0x00, 0x43, OX03C_I2C_CAMERA << 1}}, /* set adress translator DST_A*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x44, OX03C_I2C_SERIALIZER4 << 1}}, /* set adress translator SRC_B*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,  {0x03U, 0x00, 0x45, OX03C_I2C_SERIALIZER << 1}}, /* set adress translator DST_B*/

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x03, 0x00}}, /* enable FOUR chanels at once */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,                                    {DESER_WAIT}},

        /**
         * Serializers first initialization - broadcast
         */
        // sp_Adr40_Ser_InitHw
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x03, 0x02, 0x10}}, /* voltage regulation - 1.1V */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x00, 0x02, 0x03}}, /* Video transmit Pipe Z disabled */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x03, 0x08, 0x44}}, /* Enable sending line start info-frames */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x03, 0xF1, 0x09}}, /* Enabled PCLK output on MFP, MFP pin 3 */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x03, 0xF0, 0x59}}, /* Enable ref. PLL; enable alternative PLL; set & lock PLL */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x05, 0x70, 0x1C}}, /* Slew rate settings */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x02, 0xD7, 0x68}}, /* Buff, pull up/down sel.; Driver = Open drain; GPIO TX ID */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x02, 0xD6, 0x90}}, /* GPIO8 out = 1 - Sensor power enable (SEN_RST) */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x02, 0xBF, 0x60}}, /* Pullup; Driver = push-pull */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE,                       {0x03U, 0x02, 0xBE, 0x84}}, /* GMSL source ena; 1M pull up/down */
        // end sp_Adr40_Ser_InitHw
};

cgd_CmdTable_t sgCgdOvx3cInitialization =
    {
        .ympI2cTable = sgCgdOvx3cInitializationArr,
        .mI2cTableSz = sizeof(sgCgdOvx3cInitializationArr),
};

/******************************************************************************/
cgd_Cmd_t sgCgdOvx3cVideoEnabledArr[] =
    {
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x03, 0x00}}, /* enable FOUR chanels at once */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,              {DESER_WAIT}},
        /**
         * Serializers run
         */
        // sp_Adr40_Ser_InitSw
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x32, 0xE0}}, /* MIPI lane 2,3 mapping */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x33, 0x04}}, /* MIPI lane 0,1 mapping */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x34, 0x00}}, /* MIPI lane 2,3 polarity */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x35, 0x00}}, /* MIPI lane 0,1 polarity */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x18, 0x6E}}, /* Filtering video pipeline */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x19, 0x52}}, /* Filtering video pipeline */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x12, 0x04}}, /* send 8b pixel data as 16b */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x1E, 0x30}}, /* software override enable for video pipeline Z */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x0E, 0x00}}, /* Virtual channel filter bits [15:8] */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x0D, 0x01}}, /* Virtual channel filter bits [7:0] - only VC0 is processed*/
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x5B, 0x00}}, /* Stream ID used in packets transmitted from this channel */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x41, 0x76}},
        // end sp_Adr40_Ser_InitSw

        /**
         * Deserializer run
         */
        // sp_Adr27_Deser_VidPipeEna
        /* channel 0 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF0, 0x40}}, /* GMSL2 PHY A -> Pipe 0, GMSL2 PHY B -> Pipe 1 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x04, 0x01}}, /* Video transmit Channel 0 enabled */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF4, 0x01}}, /* Video pipe 0 enabled */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x0C, 0x00}}, /* MIPI_TX12 (0x90C, 0x94C), Mapping enable [15:8] - disabled */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x0B, 0x0F}}, /* MIPI_TX11 (0x90B, 0x94B), Mapping enable [7:0] - map SRC_0~3 to DES_0~3*/
        /* virtual channel mapping to VC=0 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x0D, 0x2E}}, /* Video Pipe Source Mapping Register 0, DataType=2E from VC=0 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x0E, 0x2E}}, /* Video Pipe Destination Mapping Register 0, to VC=0*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x0F, 0x00}}, /* Video Pipe Source Mapping Register 1 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x10, 0x00}}, /* Video Pipe Destination Mapping Register 1 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x11, 0x01}}, /* Video Pipe Source Mapping Register 2 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x12, 0x01}}, /* Video Pipe Destination Mapping Register 2 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x13, 0x12}}, /* Video Pipe Source Mapping Register 3 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x14, 0x12}}, /* Video Pipe Destination Mapping Register 3 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x2D, 0x55}}, /* DPHY and CPHY Mapping destination controller register (CSI2 controller 1) */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x2E, 0x00}}, /* DPHY and CPHY Mapping destination controller register */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x2F, 0x00}}, /* DPHY and CPHY Mapping destination controller register */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x30, 0x00}}, /* DPHY and CPHY Mapping destination controller register */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x43, 0x07}}, /* DPHY Deskew Initial Calibration Control */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x44, 0x01}}, /* DPHY Periodic Deskew Calibration Control */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x08, 0xCA, 0xE4}}, /* Selects target MIPI Controller for video pipe 0..3, 0xE4 is default */
        /* channel 1 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x04, 0x03}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF4, 0x03}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x4C, 0x00}}, /* MIPI_TX12 (0x90C, 0x94C), Mapping enable [15:8] - disabled */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x4B, 0x0F}}, /* MIPI_TX11 (0x90B, 0x94B), Mapping enable [7:0] - Map SRC_0~3 to DES_0~3*/
        /* virtual channel mapping to VC=1 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x4D, 0x2E}}, /* MIPI_TX13 (0x90D, 0x94D), Video Pipe Source Mapping Register 0, DataType=2E from VC=0*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x4E, 0x6E}}, /* MIPI_TX14 (0x90E, 0x94E), Video Pipe Destination Mapping Register 0, to VC=1*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x4F, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x50, 0x40}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x51, 0x01}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x52, 0x41}}, /* MIPI_TX18 (0x912, 0x952), Video Pipe Destination Mapping Register 2*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x53, 0x12}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x54, 0x52}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x6D, 0x55}}, /* DPHY and CPHY Mapping destination controller register  */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x6E, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x6F, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x70, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x43, 0x07}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x44, 0x01}},
        /* channel 2 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF1, 0xC8}}, /* GMSL2 PHY C -> Pipe 2, GMSL2 PHY D -> Pipe 3  */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x04, 0x07}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF4, 0x07}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x8C, 0x00}}, /* MIPI_TX12 (0x98C, 0x9CC) Mapping enable high byte [15:8].*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x8B, 0x0F}}, /* MIPI_TX11 (0x98B, 0x9CB), Mapping enable low byte [7:0]. Map SRC_0~3 to DES_0~3*/
        /* virtual channel mapping to VC=2 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x8D, 0x2E}}, /* Video Pipe Source Mapping Register 0, DataType=2E from VC=0*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x8E, 0xAE}}, /* MIPI_TX14 (0x98E, 0x9CE), Video Pipe Destination Mapping Register 0, to VC=2*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x8F, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x90, 0x80}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x91, 0x01}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x92, 0x81}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x93, 0x12}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x94, 0x92}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xAD, 0x55}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xAE, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xAF, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xB0, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x43, 0x07}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x44, 0x01}},
        /* channel 3 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x04, 0x0F}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF4, 0x0F}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xCC, 0x00}}, /* MIPI_TX12 (0x98C, 0x9CC) Mapping enable high byte [15:8].*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xCB, 0x0F}}, /* MIPI_TX11 (0x98B, 0x9CB), Mapping enable low byte [7:0]. Map SRC_0~3 to DES_0~3*/
        /* virtual channel mapping to VC=3 */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xCD, 0x2E}}, /* MIPI_TX13 (0x98D, 0x9CD), Video Pipe Source Mapping Register 0, DataType=2E from VC=0*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xCE, 0xEE}}, /* MIPI_TX14 (0x98E, 0x9CE), Video Pipe Destination Mapping Register 0, to VC=3*/
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xCF, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xD0, 0xC0}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xD1, 0x01}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xD2, 0xC1}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xD3, 0x12}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xD4, 0xD2}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xED, 0x55}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xEE, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xEF, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0xF0, 0x00}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x43, 0x07}},
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x09, 0x44, 0x01}},

#if (CGD_OX03C10_DESERIALIZER_REPLICATE_OUTPUT == 1)
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x08, 0xA9, 0xC8}}, /* PHY copy 0, replicates data from source PHY to destination PHY */
#endif

        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x08, 0xA2, 0xF4}}, /* MIPI PHY Enable, DPHY timing 106.7ns */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x0B, 0x02}}, /* Enables MIPI CSI output */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0xA7, 0x0C}}, /* High byte of number of PLCK cycles in FSYNC period */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0xA6, 0xB7}}, /* Middle byte of number of PLCK cycles in FSYNC period */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0xA5, 0x35}}, /* Low byte of number of PLCK cycles in FSYNC period */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0xA0, 0x04}}, /* Frame Sync manual; GPIO is used as FSYNC output and drives a slave device */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0xB1, 0x00}}, /* Frame sync error reporting threshold = 1; GPIO ID used for FSYNC */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0xA2, 0x00}}, /* frame sync margin = 0,85us; positive; Master link for frame sync = Video 0 */
        // end sp_Adr27_Deser_VidPipeEna

        // sp_Adr40_Ser_VideoOn
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x08, 0x64}}, /* Line start info frames enabled; CSI enabled; */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x11, 0x40}}, /* Video pipe Z from CSI port started */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x02, 0x43}}, /* Video transmit Pipe Z enabled */
        // end sp_Adr40_Ser_VideoOn
};

cgd_CmdTable_t sgCgdOvx3cVideoEnabled =
    {
        .ympI2cTable = sgCgdOvx3cVideoEnabledArr,
        .mI2cTableSz = sizeof(sgCgdOvx3cVideoEnabledArr),
};

/******************************************************************************/
cgd_Cmd_t sgCgdOvx3cVideoEnableOnAllCamerasArr[] =
    {
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x00}}, /* streaming disabled */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_DELAY,                     {33U}}, /* wait */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x01}}, /* streaming enabled */
};

cgd_CmdTable_t sgCgdOvx3cVideoEnableOnAllCameras =
    {
        .ympI2cTable = sgCgdOvx3cVideoEnableOnAllCamerasArr,
        .mI2cTableSz = sizeof(sgCgdOvx3cVideoEnableOnAllCamerasArr),
};

/******************************************************************************/
cgd_Cmd_t sgCgdOvx3cVideoDisableArr[] =
    {
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x03, 0x00}}, /* enable FOUR chanels at once */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,              {DESER_WAIT}},

        // sp_Adr40_Ser_VideoOff
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x08, 0x44}}, /* Line start info frames enabled; CSI disabled; */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x11, 0x00}}, /* Video pipe Z from CSI port not started */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x02, 0x03}}, /* Video transmit Pipe Z disabled */
        // end sp_Adr40_Ser_VideoOff

        // sp_Adr36_CamOvx3c_StreamOff
        {      OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0x00}}, /* GH start*/
        {      OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x00}}, /* Streaming bit clear */
        {      OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0x10}}, /* GH end*/
        {      OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0xA0}}, /* GH run*/
        {      OX03C_I2C_CAMERA_IDX, CGD_CMD_DELAY,                     {34U}}, /* Wait a maximum of 1 time frame. Worst case is 33.33ms. */
        // end sp_Adr36_CamOvx3c_StreamOff

        // sp_Adr27_Deser_VidPipeDis
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x08, 0xA2, 0x04}}, /* Put MIPI DPHY into standby mode */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x0B, 0x00}}, /* CSI output disabled; Pipe 0 BPP software-override */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0xF4, 0x00}}, /* Video Pipe Disabled; Legacy (MAX96712) mode */
        // end sp_Adr27_Deser_VidPipeDis
};

cgd_CmdTable_t sgCgdOvx3cVideoDisable =
    {
        .ympI2cTable = sgCgdOvx3cVideoDisableArr,
        .mI2cTableSz = sizeof(sgCgdOvx3cVideoDisableArr),
};
/******************************************************************************/
cgd_Cmd_t sgCgdOX03C10Config4ExposuresArr[] =
    {
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x03, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x07, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_DELAY,                      {5U}}, //  reset camera

        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4d, 0x5a, 0x1c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x09, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x20, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x23, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x62, 0x8a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x63, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x28, 0x03, 0xfe}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x28, 0x0c, 0x79}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x30, 0x05, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x30, 0x07, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x30, 0x08, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x30, 0x20, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x00, 0x28}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x01, 0x15}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x02, 0x19}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x03, 0x23}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x04, 0x0a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x06, 0x3e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x07, 0x0d}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x08, 0x50}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x09, 0x5a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x0b, 0x96}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x11, 0x11}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x2c, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x38, 0x36}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x39, 0x36}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x3a, 0x25}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x3b, 0x25}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x47, 0x28}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x48, 0x28}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x49, 0x19}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x55, 0x1a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x56, 0x0a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x57, 0x1c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x65, 0x19}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x66, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x67, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x68, 0x13}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x78, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x7c, 0xc8}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x81, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0x83, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xc0, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xc4, 0x3e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xc5, 0x3e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xc6, 0x2a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xc7, 0x28}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xc9, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xcb, 0x29}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xcd, 0x29}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xd3, 0x73}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xd7, 0x6b}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xdf, 0x54}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xf9, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x37, 0xfb, 0x19}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x03, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x04, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x06, 0x21}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x08, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x09, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x0a, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x0b, 0x21}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x13, 0x21}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x14, 0x82}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x16, 0x13}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x22, 0xf3}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x37, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x38, 0x31}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x3c, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x3d, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x44, 0x16}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x5c, 0x8a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x5f, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x6f, 0x2b}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x70, 0x5f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x71, 0x2c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x72, 0x2c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x73, 0x2c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3c, 0x76, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x31, 0x82, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x32, 0x19, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x06, 0x30}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x86, 0x60}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x49, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0xc6, 0xa0}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x05, 0x16}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x09, 0xf0}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x0a, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x0f, 0x10}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x10, 0x70}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x11, 0x3a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x12, 0x28}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x1a, 0x29}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x1b, 0x6c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x1c, 0x0b}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x1d, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x4d, 0x0f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x4e, 0x18}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x4f, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x53, 0x1c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x55, 0x1f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x56, 0x1f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x57, 0x0c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x58, 0x0a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x59, 0x14}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x5a, 0x18}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x5b, 0x14}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x5c, 0x10}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x5e, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x74, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x77, 0x3a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x78, 0x3a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x36, 0x79, 0x19}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x20, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x32, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x34, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x40, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x41, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x43, 0x90}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x44, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x45, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x46, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x47, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x48, 0x19}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x49, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x4a, 0x16}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x4b, 0x2e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x87, 0x34}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x89, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x8a, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x8b, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x8d, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x92, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x93, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x95, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3b, 0x9e, 0x09}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3d, 0x82, 0x73}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3d, 0x85, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3d, 0x9a, 0x9f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3d, 0x9c, 0xa0}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3d, 0xa4, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x3d, 0xa7, 0x50}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x42, 0x1f, 0x45}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x01, 0xff}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x0a, 0x13}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x0d, 0x93}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x0e, 0x14}}, /* bottom emb DT not as image DT */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x0f, 0x17}}, /* disable output statistics */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x17, 0x28}}, /* enable top and disable bottom emb lines */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x19, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x1f, 0x30}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x83, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x84, 0x6a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x85, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x86, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x87, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x88, 0x73}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x89, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x8a, 0x1f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x8b, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x8c, 0xdc}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x8d, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x8e, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x97, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x98, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x99, 0x0e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x9a, 0x0e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x9b, 0xfb}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x45, 0x9c, 0xf3}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x48, 0x0a, 0x22}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4d, 0x15, 0x7d}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4d, 0x30, 0x0a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4d, 0x31, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4d, 0x34, 0x7d}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4d, 0x3c, 0x7d}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x50, 0x02, 0x60}}, /* PWL and retiming enable, all statistics blocks disabled */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x07, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x08, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x09, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x0b, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x0c, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x0d, 0x88}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x27, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x28, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x29, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x2b, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x2c, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x2d, 0x88}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x47, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x48, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x49, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x4b, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x4c, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x4d, 0x88}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x67, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x68, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x69, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x6b, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x6c, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x6d, 0x88}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x87, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x88, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x89, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x8b, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x8c, 0x07}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x60, 0x8d, 0x88}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x01, 0x0f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x02, 0x0f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x03, 0x10}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x04, 0x11}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x05, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x06, 0x13}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x07, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x08, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x09, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x0a, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x0b, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x0c, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x0d, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x0e, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x0f, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x10, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x11, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x12, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x13, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x14, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x15, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x16, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x17, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x18, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x19, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x1a, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x1b, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x1c, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x1d, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x1e, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x1f, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x20, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x21, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x23, 0x7f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x24, 0xff}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x26, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x29, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x2c, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x2d, 0x92}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x2f, 0x09}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x30, 0x25}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x32, 0x12}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x33, 0x49}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x35, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x38, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x3b, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x3e, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x41, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x44, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x47, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x4a, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x4d, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x4f, 0x7f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x50, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x53, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x56, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x59, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x5c, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x5e, 0x7f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x5f, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x62, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x65, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x68, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x6d, 0x7f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x6e, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x71, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x74, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x77, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x7a, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x7d, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x80, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x83, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5e, 0x84, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x08, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x09, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x22, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x23, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x82, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x83, 0x53}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x84, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x85, 0x2b}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x86, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x40, 0x87, 0xb3}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x46, 0x41, 0x11}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x46, 0x42, 0x0e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x46, 0x43, 0xee}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x46, 0x46, 0x0f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x50, 0x03, 0x7a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x80, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x00, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x80, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x8e, 0x60}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x92, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x97, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x9a, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x9b, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x9c, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0x9f, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xa0, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xa1, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xa3, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xa4, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xa5, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xae, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xaf, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xb0, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xb1, 0xc0}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xb2, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xb3, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x30, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x31, 0xc0}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x32, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x9d, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xa5, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xe7, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd2, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd4, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd5, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd6, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd7, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd8, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xd9, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xda, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xdb, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xdc, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xdd, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xde, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xdf, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5b, 0xe0, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x4d, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x51, 0x60}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x52, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x55, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x56, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x57, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x59, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x5a, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x5b, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x5c, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x5d, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x5e, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x5f, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0x60, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xd5, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xd6, 0x60}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xd9, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xda, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xdb, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xdd, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xde, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xdf, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xe2, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xe3, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5c, 0xe4, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0xc9, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0xca, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0xcb, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0xcd, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0xce, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0xcf, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0xc9, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0xca, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0xcb, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0xcd, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0xce, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0xcf, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0xc9, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0xca, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0xcb, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0xcd, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0xce, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0xcf, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0xc9, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0xca, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0xcb, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0xcd, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0xce, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0xcf, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x15, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x16, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x17, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x09, 0xb6}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x0b, 0xb6}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x19, 0xb6}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x62, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x40, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x63, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x65, 0xff}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x59, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x5b, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x5e, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x5f, 0xb6}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x60, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x61, 0xb6}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x4a, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x4b, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x4c, 0x10}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x4d, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x4e, 0x10}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x4f, 0x40}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x50, 0x18}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x51, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x52, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x53, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x54, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x55, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x47, 0x20}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x49, 0x60}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x5d, 0x66, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x50, 0x04, 0x1e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x42, 0x21, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x01, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x02, 0xc8}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x41, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x42, 0xc8}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0xc2, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x42, 0x0e, 0x54}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x42, 0x0f, 0xa0}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x42, 0x10, 0xca}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x42, 0x11, 0xf2}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x50, 0x7a, 0x5f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x50, 0x7b, 0x46}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4f, 0x00, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4f, 0x01, 0x00}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4f, 0x02, 0x80}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4f, 0x03, 0x2c}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4f, 0x04, 0xf8}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x07, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x48, 0x37, 0x1a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x0d, 0xed}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x08, 0x70}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x09, 0x62}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x0a, 0x2d}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x0b, 0x09}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x24, 0x01}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x25, 0x36}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x29, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x2a, 0x05}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x2b, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x2c, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x27, 0x09}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x03, 0x26, 0x0e}},

        /* 4 EXP changes*/
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x0c, 0x05}}, //  HTS1
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x0d, 0xe2}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x4c, 0x02}}, //  HTS2
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x4d, 0xf1}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x8c, 0x02}}, //  HTS3
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x8d, 0xf1}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x0e, 0x02}}, //  VTS
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x0f, 0xa3}},

        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x4d, 0xf1}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x04, 0x09}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x05, 0x2b}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x06, 0x8d}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x8d, 0xf1}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x00, 0x70}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x01, 0x7f}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x04, 0x03, 0x2d}},

        /* Fsync */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x30, 0x15, 0x0A}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x30, 0x09, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x22, 0x24}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x23, 0x50}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x3e, 0x81}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x81, 0x34}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x82, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x83, 0x8a}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x38, 0x92, 0x44}}, /* Declaring the registers to be included in the embedded data */
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0x04}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x0e, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x14, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x18, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x4e, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x54, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x58, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x94, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0x98, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0xce, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0xd4, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x35, 0xd8, 0x03}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x48, 0x3E, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x4D, 0x2A, 0x02}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x52, 0x80, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x54, 0x80, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x56, 0x80, 0x08}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x58, 0x80, 0x0A}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x32, 0x08, 0x14}},

        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x43, 0x1c, 0x6e}},
        {OX03C_I2C_CAMERA_IDX, CGD_CMD_WRITE, {0x03U, 0x01, 0x00, 0x00}},
};

cgd_CmdTable_t sgCgdOX03C10Config =
    {
        .ympI2cTable = sgCgdOX03C10Config4ExposuresArr,
        .mI2cTableSz = sizeof(sgCgdOX03C10Config4ExposuresArr),
};

/******************************************************************************/

cgd_Cmd_t sgCgdOvx3cCloseArr[] =
    {
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x03, 0x00}}, /* enable FOUR chanels at once */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_DELAY,              {DESER_WAIT}},

        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x02, 0xD6, 0x80}}, /* Sensor power disable */
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_DELAY,                      {1U}},
        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x02, 0xD6, 0x90}}, /* Sensor power enable */

        {  OX03C_I2C_SERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x10, 0x80}}, /* Reset all, automatically cleared */
        {OX03C_I2C_DESERIALIZER_IDX, CGD_CMD_WRITE, {0x03U, 0x00, 0x13, 0x40}}, /* Reset all, automatically cleared */
};

cgd_CmdTable_t sgCgdOvx3cClose =
    {
        .ympI2cTable = sgCgdOvx3cCloseArr,
        .mI2cTableSz = sizeof(sgCgdOvx3cCloseArr),
};

/******************************************************************************/

#endif // else from defined(IMX95) && (defined(LINUX_EMB) || defined(SDK_OS_FREE_RTOS) || defined(GHS))
