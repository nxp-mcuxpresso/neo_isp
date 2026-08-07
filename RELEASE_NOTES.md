<!--
/*******************************************************************************
 * Copyright 2026 NXP
 ******************************************************************************/
-->

# i.MX 95 ISP SDK Release Notes EAR 0.1.8

**Document identifier:** IMX95-ISPSDK-RN

**Revision:** 1.0

**Date:** 21 Aug 2026

**Company:** NXP Semiconductors

## Introduction

This document provides release notes for the i.MX 95 ISP SDK integrated with MCUX SDK.

## General

* Build environment
  * Linux host PC
  * Compiler version: [arm-gnu-toolchain-14.3.rel1-x86_64-arm-none-eabi](https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/14.3.rel1/arm-gnu-toolchain-14.3.rel1-aarch64-arm-none-eabi.tar.xz)
  * MCUX SDK 26.09

* Target environment
  * i.MX 95 LPD5 EVK-19 EVK board, B0 silicon, M7 core
  * Quad OX03C10 camera hardware setup with Maxim deserializer
  * IMX-LVDS-HDMI display adapter
  * MCUX SDK 26.09

## Change log

* Initial ISP SDK version for MCUX SDK
* Components
  * ISP driver
  * CGD (Camera Generic Driver)
  * Neo ISP Control 3A library, version 0.4.1
  * Camera Control Abstraction (CCA) library
  * Camera Control Library (CCL)
  * Utilities library
  * Various helper functions for other components and demos
  * `ilog` logging library
  * `isp_ccl` demo application
    * Uses Ox03C10 camera inputs ranging from 1 to 4
    * Live display of ISP output image over HDMI adapter
* Data
  * Ox03C10 camera
    * TDS (Tuning Data Set) for Neo ISP Control library
    * Default ISP configuration for ox03c10 camera processing
* Documents
  * [i.MX 95 ISP SDK User Guide](isp_sdk/docs/IMX95_ISPSDK_UG_MCUX.md#imx-95-isp-sdk-user-guide)
  * [Sensor Porting User Guide](isp_sdk/docs/IMX95_ISPSDK_SPG.md#imx-95-isp-sdk-sensor-porting-guide)

## Test results overview

* Minimal integration testing

* Longevity testing
  * `isp_ccl` with four Ox03C10 cameras, passed 5-hour run

## Limitations

The MIPI_DSICSI combo port is not supported for camera data input.

## Revision history

| Document ID | Release Data | Description |
|-------------|--------------|-------------|
| IMX95-ISPSDK-RN_v1.0  | 20 August 2026 | Initial content for the first MCUX SDK public release isp_sdk_ear_0.1.8 |
