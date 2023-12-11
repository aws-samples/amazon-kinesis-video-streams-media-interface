if(BOARD STREQUAL "QCS610")
    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    set(BOARD_SRCS
        ${BOARD_SDK_DIR}/../GSTREAMER/GSTREAMERPort.c
    )
    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}
        ${BOARD_SDK_DIR}/../GSTREAMER/
    )
    set(BOARD_LIBS_DIR
    )
    set(BOARD_DESTINATION_PLATFORM
        aarch64-oe-linux
    )
    set(BOARD_LIBS_SHARED
    )
    set(BOARD_LIBS_STATIC
    )
    add_definitions(-DBOARD=${BOARD})
endif()
