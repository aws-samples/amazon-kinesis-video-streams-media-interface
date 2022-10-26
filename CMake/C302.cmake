if(BOARD STREQUAL "C302")
    add_compile_definitions(__LINUX_OS__)
    add_compile_definitions(CONFIG_ARCH_C302)

    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/include/
    )

    set(BOARD_DESTINATION_PLATFORM arm-unknown-linux-glibc)

    link_directories(${BOARD_SDK_DIR}/lib/static ${BOARD_SDK_DIR}/lib/dynamic)

    set(BOARD_LIBS_SHARED
        pthread dl m z fdk-aac
        sns_imx307 sns_imx335 sns_gc4653 sns_imx415
        ae alg awb dewarp mbi mbi_dewarp mbi_isp mbi_venc ppu
    	param video audio mbuffer
    )
    set(BOARD_LIBS_STATIC
	pthread dl m z fdk-aac
        sns_imx307 sns_imx335 sns_gc4653 sns_imx415
	ae alg awb dewarp mbi mbi_dewarp mbi_isp mbi_venc ppu
        libparam.a libvideo.a libaudio.a libmbuffer.a
    )
endif()
