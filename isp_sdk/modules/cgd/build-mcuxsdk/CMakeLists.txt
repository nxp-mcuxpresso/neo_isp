# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.cgd)
    mcux_component_version(0.1.8)

    mcux_add_source(SOURCES
        ../src/freertos/cgd_freertos_config.c
        ../src/freertos/cgd_freertos_hal.c
        ../src/cgd.c
        ../src/cgd_i2c.c
    )

    mcux_add_include(INCLUDES
        ../inc
        ../inc/freertos
        ../inc/sensors
    )

    mcux_add_macro(
            CC "-DBOARD_USE_PCAL6408=1"
    )
endif()



