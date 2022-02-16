if(BOARD STREQUAL "T31")
    set(BOARD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${BOARD})
    set(BOARD_SRCS
        ${BOARD_SDK_DIR}/samples/libimp-samples/sample-common.c
    )
    set(BOARD_INCS_DIR
        ${BOARD_SDK_DIR}/include/
        ${BOARD_SDK_DIR}/samples/libimp-samples/
    )
    if(USE_MUCLIBC)
        set(BOARD_LIBS_DIR
            ${BOARD_SDK_DIR}/lib/uclibc
        )
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -muclibc")
        set(BOARD_DESTINATION_PLATFORM mips-unknown-linux-uclibc)
    else()
        set(BOARD_LIBS_DIR
            ${BOARD_SDK_DIR}/lib/glibc
        )
        set(BOARD_DESTINATION_PLATFORM mips-unknown-linux-gnu)
    endif()
    set(BOARD_LIBS_SHARED
        imp alog rt m pthread dl
    )
    set(BOARD_LIBS_STATIC
        libimp.a libalog.a librt.a libm.a pthread dl
    )
endif()
