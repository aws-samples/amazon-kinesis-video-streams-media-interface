if(BOARD STREQUAL "AK3918")

    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    set(BOARD_SRCS
        ${BOARD_SDK_DIR}/ipc_main.c
    )

       set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/framework/include/
        ${BOARD_SDK_DIR}/framework/include/common/
        ${BOARD_SDK_DIR}/framework/include/alarm/
        ${BOARD_SDK_DIR}/framework/record/
        ${BOARD_SDK_DIR}/framework/include/record/
        ${BOARD_SDK_DIR}/platform/include/
       
    )

    set(BOARD_DESTINATION_PLATFORM arm-unknown-linux-uclibc)

    link_directories(${BOARD_SDK_DIR}/lib/static ${BOARD_SDK_DIR}/lib/dynamic)

    set(BOARD_LIBS_SHARED
        pthread rt m
        ipc_framework
    )
    set(BOARD_LIBS_STATIC
        
    )
endif()
