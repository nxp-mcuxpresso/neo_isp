# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.platform)
    mcux_component_version(0.1.8)

    mcux_add_source(SOURCES
        ../inc/isp_task.h
        ../inc/isp_platform.h
    )

    mcux_add_include(INCLUDES ../inc)

endif()



