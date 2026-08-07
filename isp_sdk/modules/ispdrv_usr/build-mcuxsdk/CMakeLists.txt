# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.ispdrv_usr)
    mcux_component_version(0.1.8)

    if (${board} STREQUAL "imx95lpd5evk19")
        mcux_add_source(SOURCES
            ../src/freertos/imx95m7/ispdrv_irq.c
            ../src/freertos/imx95m7/ispdrv_usr.c
            ../../ism/imx9xm0p_drivers/Source/edma.c
            ../../ism/imx9xm0p_drivers/Source/isp.c
        )

        mcux_add_include(INCLUDES
            ../../ism/imx9xm0p_drivers/Include
        )

    endif()

    mcux_add_source(SOURCES
            ../src/freertos/common/mshr.c
            ../src/freertos/common/ispdrv_ismevt.c

            ../inc/freertos/ispdrv_usr_internal.h

            ../../ism/stubs/arm/_edma_trigger.c
            ../../ism/stubs/arm/_isp_trigger.c
            ../../ism/middleware/ctx_mngr/ctx_mngr.c
            ../../ism/middleware/event_handlers/ehandlers.c
            ../../ism/middleware/ismanager/ismanager.c
            ../../ism/middleware/timestamps/timestamps.c

            ../../ism/port/freertos_mcux/qadapter.c
            ../../ism/port/freertos_mcux/evtbroker.c
            ../../ism/port/freertos_mcux/clock.c
            ../../ism/port/irq_handlers.c
    )

    mcux_add_include(INCLUDES
            ../inc
            ../inc/freertos
            ../../isp_platform/inc/
            ../../isp_platform/inc/mx95-headers/mx95_CM33/Include

            ../../ism
            ../../ism/stubs

            ../../ism/port/
            ../../ism/port/freertos_mcux
            ../../ism/middleware/ctx_mngr/
            ../../ism/middleware/comm/
            ../../ism/middleware/event_handlers/
            ../../ism/middleware/ismanager/
            ../../ism/middleware/timestamps/
    )

endif()

