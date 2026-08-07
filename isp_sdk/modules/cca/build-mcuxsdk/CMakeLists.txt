# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.cca)
    mcux_component_version(0.1.8)

    if (${board} STREQUAL "imx95lpd5evk19")
        mcux_add_source(SOURCES
            ../src/platform/cca_neob.c
        )
        mcux_add_include(INCLUDES
            ../inc/platform/neob
        )
    endif()


    mcux_add_source(SOURCES
        ../src/cca.c
        ../src/cca_ovx3c.c
    )

    mcux_add_include(INCLUDES
        ../inc
        ../inc/itec
    )

    if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.cca.ccfselection.neo_control)
        mcux_component_version(0.1.8)

        mcux_add_source(SOURCES
            ../src/neo-isp-control/cca_neo_isp_control.c
        )

        mcux_add_include(INCLUDES
            ../inc/neo-isp-control
        )
        mcux_add_configuration(
                CC "-DNEOCONTROL"
            )

        if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccf.lt)
            if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccf.lt.transport.usb)
                mcux_add_source(SOURCES
                    ../src/neo-isp-control/freertos/cca_lt_connect_usb.c
                )
            endif()
            if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccf.lt.transport.eth)
                mcux_add_source(SOURCES
                    ../src/neo-isp-control/freertos/cca_lt_connect_eth.c
                )
            endif()
        endif()
    endif()

    if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.cca.ccfselection.ccf_custom)
        mcux_component_version(0.1.8)

        mcux_add_source(SOURCES
            ../src/custom/cca_ccf_custom.c
        )

        mcux_add_include(INCLUDES
            ../inc/custom
        )

        mcux_add_configuration(
                CC "-DCCFCUSTOM"
            )
    endif()

endif()

