# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccf)
    mcux_component_version(1.0.0)


    if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccf.lt)
        mcux_add_macro(CC LIVE_TUNE)

        mcux_add_library(
            BASE_PATH ../lib
            LIBS liblt.a libengine_lt.a
            CORES cm7f
            TOOLCHAINS armgcc
        )
    else()
        mcux_add_library(
            BASE_PATH ../lib
            LIBS libengine.a
            CORES cm7f
            TOOLCHAINS armgcc
        )
    endif()

    mcux_add_include(INCLUDES
        ../include
        ../include/device
    )

endif()

