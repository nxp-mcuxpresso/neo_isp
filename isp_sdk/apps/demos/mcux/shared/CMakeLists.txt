# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.display_task)
    mcux_component_version(0.1.8)

    mcux_add_source(
        SOURCES
        ./inc/display_task.h
        ./src/display_task.c
    )

    mcux_add_include(
        INCLUDES
        ./inc
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.usb_cdc_acm)
    mcux_component_version(0.1.8)

    mcux_add_source(
        SOURCES
        ./src/usb_device_descriptor.c
        ./inc/usb_device_descriptor.h
        ./src/virtual_com.c
        ./inc/virtual_com.h
    )

    mcux_add_include(
        INCLUDES
        ./inc
    )

    mcux_add_macro(CC "-DCONFIG_USB_CDC_ACM")
    mcux_add_macro(CC "-DUSB_STACK_FREERTOS")

    if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.ccf.lt.transport.usb AND CONFIG_MCUX_COMPONENT_middleware.ispsdk.ilog.transport.usb)
        message( SEND_ERROR "USB transport configured for both CCF and ILOG.")
    endif()
endif()

if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.tcp_server)
    mcux_add_source(
        SOURCES
        ./src/tcp_server.c
        ./inc/tcp_server.h
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.camera_task)
    mcux_component_version(0.1.8)

    mcux_add_source(
        SOURCES
        ./inc/camera_ctx.h
        ./inc/camera_task.h
        ./src/camera_task.c
        ./src/new_sensor_cca.c
        ./src/new_sensor_ccl_i2c.c
        ./src/new_sensor_ccl.c
    )

    mcux_add_include(
        INCLUDES
        ./inc
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.isp_initial_cfg)
    mcux_component_version(0.1.8)

    mcux_add_source(
        SOURCES
        ./src/camera_cfg.c
    )

    mcux_add_include(
        INCLUDES
        ./inc
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.neo_control.tuning_database)
    mcux_component_version(0.1.8)

    mcux_add_source(
        SOURCES
        ./src/camera_tuning_database.c
    )

    mcux_add_include(
        INCLUDES
        ./inc
    )

endif()

if (CONFIG_MCUX_MISC_middleware.ispsdk.example.camera_selection.ovx3c)
    mcux_add_macro(CC "-DCONFIG_ISP_SDK_CAMERA_TYPE=3")
elseif (CONFIG_MCUX_MISC_middleware.ispsdk.example.camera_selection.new_sensor)
    mcux_add_macro(CC "-DCONFIG_ISP_SDK_CAMERA_TYPE=0")
else()
    message(FATAL_ERROR "Camera selection unknown")
endif()

if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.stdout.usb)
    mcux_add_source(
        SOURCES
        ./src/stdout_usb.c
    )

    mcux_add_macro(CC "-DUSB_STDOUT")
    mcux_add_macro(CC "-DSDK_DEBUGCONSOLE=0")
    target_link_options(${MCUX_SDK_PROJECT_NAME} PRIVATE "-Wl,-u,_printf_float")

    if (CONFIG_MCUX_MISC_middleware.ispsdk.enable_files_usb)
        mcux_add_macro(CC "-DSEND_IMG_VIA_USB")
    endif()
endif()
