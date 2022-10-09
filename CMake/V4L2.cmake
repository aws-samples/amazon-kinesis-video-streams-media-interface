if(BOARD STREQUAL "V4L2")
    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})

    include(ExternalProject)
    ExternalProject_Add(v4l2capturer
    GIT_REPOSITORY    https://github.com/codingspirit/V4l2Capturer.git
    GIT_TAG           master
    CMAKE_ARGS        -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${BOARD_SDK_DIR}
    BUILD_ALWAYS      TRUE
    GIT_PROGRESS      TRUE
    TEST_COMMAND      ""
    )
    set(BOARD_BUILD_DEPENDS v4l2capturer)

    set(BOARD_SRCS
        ${BOARD_SDK_DIR}/V4L2Port.c
    )
    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}
        ${BOARD_SDK_DIR}/include
    )
    set(BOARD_LIBS_DIR
        ${BOARD_SDK_DIR}/lib
    )
    set(BOARD_LIBS_SHARED
        libv4l2capturer.a
    )
    set(BOARD_LIBS_STATIC
        libv4l2capturer.a
    )
endif()
