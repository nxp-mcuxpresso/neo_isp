# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.utils.gpio)
    mcux_component_version(0.1.8)

    mcux_add_source(SOURCES ../src/freertos/utils_gpio.c ../inc/utils_gpio.h)

    mcux_add_include(INCLUDES ../inc)

endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.utils.i2c)
    mcux_component_version(0.1.8)

    mcux_add_source(SOURCES ../src/freertos/utils_i2c.c ../inc/utils_i2c.h)

    mcux_add_include(INCLUDES ../inc)

endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.utils.edma)
    mcux_component_version(0.1.8)

    mcux_add_source(SOURCES ../src/freertos/utils_edma.c ../inc/freertos/utils_edma.h)

    mcux_add_include(INCLUDES ../inc/freertos)

endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.utils.misc)
    mcux_component_version(0.1.8)

    mcux_add_source(SOURCES ../src/endians.c ../inc/endians.h ../src/utils_crc32c.c ../inc/utils_crc32c.h)

    mcux_add_include(INCLUDES ../inc)

endif()



