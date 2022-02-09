cmake_minimum_required(VERSION 3.12)
project(embedded-media VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(USE_MUCLIBC "Use muclibc" OFF)

# Board Specific Parts Start
if(NOT BOARD OR BOARD STREQUAL "")
    message(STATUS "No board was selected. Use FILE by default.")
    set(BOARD "FILE")
elseif(BOARD STREQUAL "FILE")
    message(STATUS "Selected board FILE")
elseif(BOARD STREQUAL "V4L2")
    message(STATUS "Selected board V4L2")
elseif(BOARD STREQUAL "T31")
    message(STATUS "Selected board T31")
else()
    message(FATAL "${BOARD} is not implemented yet.")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/${BOARD}.cmake)
# Board Specific Parts End

option(BUILD_WEBRTC_SAMPLES "Build webrtc samples" ON)
option(BUILD_KVS_SAMPLES "Build KVS Producer samples" OFF)
option(BUILD_SAVE_FRAME_SAMPLES "Build save frame samples" OFF)

set(INCS_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)
set(INCS
    ${INCS_DIR}/com/amazonaws/kinesis/video/capability/AudioCapability.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/capability/VideoCapability.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/capturer/VideoCapturer.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/capturer/AudioCapturer.h
    ${INCS_DIR}/com/amazonaws/kinesis/video/player/AudioPlayer.h
)
set(SRCS
    ${CMAKE_CURRENT_LIST_DIR}/source/${BOARD}/${BOARD}VideoCapturer.c
    ${CMAKE_CURRENT_LIST_DIR}/source/${BOARD}/${BOARD}AudioCapturer.c
    ${CMAKE_CURRENT_LIST_DIR}/source/${BOARD}/${BOARD}AudioPlayer.c
)

add_library(embedded-media-static STATIC ${SRCS} ${BOARD_SRCS})
if(BOARD_BUILD_DEPENDS AND NOT (BOARD_BUILD_DEPENDS STREQUAL ""))
    add_dependencies(embedded-media-static ${BOARD_BUILD_DEPENDS})
endif()
target_include_directories(embedded-media-static PRIVATE ${BOARD_INCS_DIR})
target_include_directories(embedded-media-static PUBLIC ${INCS_DIR})
target_link_directories(embedded-media-static PRIVATE ${BOARD_LIBS_DIR})
target_link_libraries(embedded-media-static PRIVATE ${BOARD_LIBS_STATIC})
set_target_properties(embedded-media-static PROPERTIES PUBLIC_HEADER "${INCS}")
set_target_properties(embedded-media-static PROPERTIES OUTPUT_NAME embedded-media)

add_library(embedded-media-shared SHARED ${SRCS} ${BOARD_SRCS})
if(BOARD_BUILD_DEPENDS AND NOT (BOARD_BUILD_DEPENDS STREQUAL ""))
    add_dependencies(embedded-media-shared ${BOARD_BUILD_DEPENDS})
endif()
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
set(AWS_DEPENDENCIES_DIR ${CMAKE_BINARY_DIR}/aws-dependencies/)

if(BUILD_WEBRTC_SAMPLES)
    add_subdirectory(${SAMPLES_DIR}/webrtc)
endif()
if(BUILD_KVS_SAMPLES)
    add_subdirectory(${SAMPLES_DIR}/kvs)
endif()
if(BUILD_SAVE_FRAME_SAMPLES)
    add_subdirectory(${SAMPLES_DIR}/saveframe)
endif()
