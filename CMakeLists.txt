cmake_minimum_required(VERSION 3.12)
project(embedded-media VERSION 1.0.0 LANGUAGES C CXX)

if(NOT BOARD OR BOARD STREQUAL "")
    message(STATUS "No board was selected. Use T31 by default.")
    set(BOARD "T31")
elseif(BOARD STREQUAL "T31")
    message(STATUS "Selected board T31")
elseif(BOARD STREQUAL "SSC325")
    message(FATAL "SSC325 is not implemented yet.")
elseif(BOARD STREQUAL "AK3919")
    message(FATAL "AK3919 is not implemented yet.")
endif()

if(BOARD STREQUAL "T31")
    set(T31_SDK_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdparty/t31/)
    set(BOARD_SRCS
        ${T31_SDK_DIR}/samples/libimp-samples/sample-common.c
    )
    set(BOARD_INCS_DIR
        ${T31_SDK_DIR}/include/
        ${T31_SDK_DIR}/samples/libimp-samples/
    )
    set(BOARD_LIBS_DIR
        ${T31_SDK_DIR}/lib/uclibc
    )
    set(BOARD_LIBS_SHARED
        imp alog rt m pthread dl
    )
    set(BOARD_LIBS_STATIC
        libimp.a libalog.a librt.a libm.a pthread dl
    )
endif()

option(BUILD_WEBRTC_SAMPLES "Build webrtc samples" ON)
option(BUILD_KVS_SAMPLES "Build KVS Producer samples" OFF)

set(INCS_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)
set(INCS
    ${INCS_DIR}/com/amazonaws/kinesis/video/capability/AudioCapability.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/capability/VideoCapability.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/capturer/VideoCapturer.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/capturer/AudioCapturer.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/player/AudioPlayer.h
)
set(SRCS
    ${CMAKE_CURRENT_LIST_DIR}/source/${BOARD}VideoCapturer.c
    ${CMAKE_CURRENT_LIST_DIR}/source/${BOARD}AudioCapturer.c
    ${CMAKE_CURRENT_LIST_DIR}/source/${BOARD}AudioPlayer.c
)

add_library(embedded-media-static STATIC ${SRCS} ${BOARD_SRCS})
target_include_directories(embedded-media-static PRIVATE ${BOARD_INCS_DIR})
target_include_directories(embedded-media-static PUBLIC ${INCS_DIR})
target_link_directories(embedded-media-static PRIVATE ${BOARD_LIBS_DIR})
target_link_libraries(embedded-media-static PRIVATE ${BOARD_LIBS_STATIC})
set_target_properties(embedded-media-static PROPERTIES PUBLIC_HEADER "${INCS}")
set_target_properties(embedded-media-static PROPERTIES OUTPUT_NAME embedded-media)

add_library(embedded-media-shared SHARED ${SRCS} ${BOARD_SRCS})
target_include_directories(embedded-media-shared PRIVATE ${BOARD_INCS_DIR})
target_include_directories(embedded-media-shared PUBLIC ${INCS_DIR})
target_link_directories(embedded-media-shared PRIVATE ${BOARD_LIBS_DIR})
target_link_libraries(embedded-media-shared PRIVATE ${BOARD_LIBS_SHARED})
set_target_properties(embedded-media-shared PROPERTIES PUBLIC_HEADER "${INCS}")
set_target_properties(embedded-media-shared PROPERTIES OUTPUT_NAME embedded-media)

include(GNUInstallDirs)

install(TARGETS embedded-media-static
        LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

install(TARGETS embedded-media-shared
        LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

install(DIRECTORY ${INCS_DIR}
        DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})

set(SAMPLES_DIR ${CMAKE_CURRENT_LIST_DIR}/samples/)
set(AWS_SDK_DIR ${SAMPLES_DIR}/aws-sdk/)
if(BUILD_WEBRTC_SAMPLES)
    add_subdirectory(${SAMPLES_DIR}/webrtc)
endif()
if(BUILD_KVS_SAMPLES)
    add_subdirectory(${SAMPLES_DIR}/kvs)
endif()
