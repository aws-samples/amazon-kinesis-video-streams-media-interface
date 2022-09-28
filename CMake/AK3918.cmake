if(BOARD STREQUAL "AK3918")
    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    set(BOARD_SRCS
        ${BOARD_SDK_DIR}/ipc_main.c
    )

    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/framework/include/
        ${BOARD_SDK_DIR}/framework/include/common/
        ${BOARD_SDK_DIR}/framework/include/alarm/
        ${BOARD_SDK_DIR}/framework/include/record/
        ${BOARD_SDK_DIR}/platform/include/
    )

    set(USE_MUCLIBC ON)
    set(BOARD_DESTINATION_PLATFORM arm-unknown-linux-uclibc)

    link_directories(${BOARD_SDK_DIR}/lib/static ${BOARD_SDK_DIR}/lib/dynamic)

    set(BOARD_LIBS_SHARED
        pthread rt m
        ipc_framework plat_common plat_log mpi_mux mpi_demux akmedia plat_mem
        plat_thread plat_osal mpi_venc plat_vi plat_isp_sdk plat_vpss plat_dbg
        mpi_adec plat_vqe plat_ao akaudiofilter akaudiocodec ak_mrd plat_ai
        mpi_aenc app_osd_ex mpi_osd mpi_svp app_video akv_cnn app_mem_ex iconv
    )
    set(BOARD_LIBS_STATIC
        pthread rt m
        ipc_framework plat_common plat_log mpi_mux mpi_demux akmedia plat_mem
        plat_thread plat_osal mpi_venc plat_vi plat_isp_sdk plat_vpss plat_dbg
        mpi_adec plat_vqe plat_ao akaudiofilter akaudiocodec ak_mrd plat_ai
        mpi_aenc app_osd_ex mpi_osd mpi_svp app_video akv_cnn app_mem_ex iconv
    )
endif()
