# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# This file acts as manifest for McuX-SDK, it specifies all mcux "packages" that are included in this repository
# all packages are optional and must be enabled in one of .prj files as is standard procedure for mcux-sdk

if (CONFIG_MCUX_MISC_middleware.ispsdk)
    set(IspSdkRoot "${SdkRootDirPath}/middleware/neo_isp/isp_sdk")

    message(STATUS "ISP SDK includded from: ${IspSdkRoot}")

    mcux_add_cmakelists(${IspSdkRoot}/apps/demos/mcux OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/utils/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/ilog/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/isp_platform/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/ispdrv_usr/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/cca/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/ccf/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/ccl/build-mcuxsdk OPTIONAL)
    mcux_add_cmakelists(${IspSdkRoot}/modules/cgd/build-mcuxsdk OPTIONAL)
endif()
