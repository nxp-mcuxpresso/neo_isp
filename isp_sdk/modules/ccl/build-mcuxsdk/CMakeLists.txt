# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccl)
    mcux_component_version(0.1.8)

    if (${board} STREQUAL "imx95lpd5evk19")
        mcux_add_source(SOURCES
            ../src/platform/ccl_neob.c
        )
    endif()

    mcux_add_source(SOURCES
            ../src/capture/ccl_cap_cgd.c
            ../src/cgd_sensors/cgd_ox03c10.c
            ../src/platform/freertos/ccl_time.c
            ../src/platform/freertos/ccl_semaphore.c
            ../src/platform/freertos/ccl_mutex.c
            ../src/platform/freertos/ccl_thread.c
            ../src/ccl_frame_queue.c
            ../src/ccl_cca.c
            ../src/ccl_isp.c
            ../src/ccl.c
            ../src/capture/ccl_cap_memcpy.c
            ../src/cgd_sensors/cgd_ox03c10.c
            ../inc/cgd_sensors/cgd_ox03c10.h
            ../src/cgd_sensors/i2c/cgd_ox03c10_i2c.c
            ../inc/cgd_sensors/i2c/cgd_ox03c10_i2c.h
    )

    mcux_add_include(INCLUDES
            ../inc
            ../inc/platform/freertos
            ../inc/internal
            ../inc/internal/capture
            ../inc/platform
            ../inc/platform/freertos
            ../inc/cgd_sensors/
            ../inc/cgd_sensors/i2c
    )

endif()



