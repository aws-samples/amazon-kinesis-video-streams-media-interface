if(BOARD STREQUAL "C302")
    add_compile_definitions(__LINUX_OS__)
    add_compile_definitions(CONFIG_ARCH_C302)

    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/include/
    )

    # User MUST copy the sysroot folder provided by amlogic into ${BOARD_SDK_DIR}
    set(CMAKE_SYSROOT ${BOARD_SDK_DIR}/sysroot/)

    set(BOARD_DESTINATION_PLATFORM arm-unknown-linux-gnu)

    link_directories(${BOARD_SDK_DIR}/lib)

    set(BOARD_LIBS_SHARED
        pthread dl m z fdk-aac
        sns_imx290 sns_imx307 sns_imx335 sns_gc4653 sns_imx415
        ae alg awb dewarp mbi mbi_dewarp mbi_isp mbi_venc ppu asp AudioSignalProcess
        libparam.a libvideo.a libaudio.a libmbuffer.a
    )
    set(BOARD_LIBS_STATIC
        pthread dl m z fdk-aac
        sns_imx290 sns_imx307 sns_imx335 sns_gc4653 sns_imx415
        ae alg awb dewarp mbi mbi_dewarp mbi_isp mbi_venc ppu asp AudioSignalProcess
        libparam.a libvideo.a libaudio.a libmbuffer.a
    )
endif()
