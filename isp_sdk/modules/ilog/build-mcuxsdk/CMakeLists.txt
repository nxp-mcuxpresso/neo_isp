# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.ilog)
    mcux_component_version(1.0.0)

    mcux_add_source(SOURCES ../src/ilog_func.c ../inc/ilog.h)

    mcux_add_include(INCLUDES ../inc)

endif()



