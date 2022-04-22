if(BOARD STREQUAL "FH8626V100")
    add_compile_definitions(__LINUX_OS__)
    add_compile_definitions(CONFIG_ARCH_FH8626V100)

    if(NOT SENSOR OR SENSOR STREQUAL "")
        message(STATUS "No SENSOR was selected. Use jxf37_mipi by default.")
        set(SENSOR "jxf37_mipi")
    endif()

    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    set(BOARD_SRCS
        ${BOARD_SDK_DIR}/common/components/libpes/src/libpes.c
        ${BOARD_SDK_DIR}/common/sample_common/isp/src/start_sensor/start_sensor_v1.c
        ${BOARD_SDK_DIR}/common/sample_common/isp/src/start_sensor/start_sensor_v2.c
    )

    aux_source_directory(${BOARD_SDK_DIR}/common/sample_common/isp/src BOARD_SRCS)
    aux_source_directory(${BOARD_SDK_DIR}/common/sample_common/dsp/src BOARD_SRCS)
    aux_source_directory(${BOARD_SDK_DIR}/common/sample_common/bgm/src BOARD_SRCS)
    aux_source_directory(${BOARD_SDK_DIR}/common/components/libdbi_over_tcp/src BOARD_SRCS)
    aux_source_directory(${BOARD_SDK_DIR}/common/components/libmisc/src BOARD_SRCS)
    aux_source_directory(${BOARD_SDK_DIR}/common/vlcview BOARD_SRCS)

    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/include/
        ${BOARD_SDK_DIR}/include/dsp/
        ${BOARD_SDK_DIR}/include/dsp_ext/
        ${BOARD_SDK_DIR}/include/isp/
        ${BOARD_SDK_DIR}/include/isp_ext/
        ${BOARD_SDK_DIR}/include/linux/
        ${BOARD_SDK_DIR}/include/types/
        ${BOARD_SDK_DIR}/common/
        ${BOARD_SDK_DIR}/common/components/
        ${BOARD_SDK_DIR}/common/components/libdbi_over_tcp/include/
        ${BOARD_SDK_DIR}/common/sample_common/
        ${BOARD_SDK_DIR}/common/vlcview/
    )

    set(USE_MUCLIBC ON)
    set(BOARD_DESTINATION_PLATFORM arm-unknown-linux-uclibc)

    link_directories(${BOARD_SDK_DIR}/lib/static ${BOARD_SDK_DIR}/lib/dynamic)

    set(BOARD_LIBS_SHARED
        pthread rt m
        advapi dsp isp ispcore dbi dci ${SENSOR}
        vmm vlcview mipi
        acw_mpi audio_codec
    )
    set(BOARD_LIBS_STATIC
        pthread rt m
        libadvapi.a libdsp.a libisp.a libispcore.a libdbi.a libdci.a lib${SENSOR}.a
        libvmm.a libvlcview.a libmipi.a 
        libacw_mpi.a libaudio_codec.a
    )
endif()
