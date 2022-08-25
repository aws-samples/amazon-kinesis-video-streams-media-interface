cmake_minimum_required(VERSION 3.12)
project(kvswebrtcmaster VERSION 1.0.0 LANGUAGES C CXX)

get_target_property(EMBEDDED_MEDIA_INCLUDES_DIR embedded-media-shared INCLUDE_DIRECTORIES)
get_target_property(EMBEDDED_MEDIA_LINK_DIR embedded-media-shared LINK_DIRECTORIES)

message(STATUS "SAMPLES_DIR - ${SAMPLES_DIR}")
message(STATUS "AWS_DEPENDENCIES_DIR - ${AWS_DEPENDENCIES_DIR}")
message(STATUS "BOARD_LIBS_DIR - ${BOARD_LIBS_DIR}")
message(STATUS "BOARD_LIBS_SHARED - ${BOARD_LIBS_SHARED}")
message(STATUS "BOARD_LIBS_STATIC - ${BOARD_LIBS_STATIC}")
message(STATUS "EMBEDDED_MEDIA_INCLUDES_DIR - ${EMBEDDED_MEDIA_INCLUDES_DIR}")
message(STATUS "EMBEDDED_MEDIA_LINK_DIR - ${EMBEDDED_MEDIA_LINK_DIR}")
message(STATUS "USE_MUCLIBC - ${USE_MUCLIBC}")
message(STATUS "BOARD_DESTINATION_PLATFORM - ${BOARD_DESTINATION_PLATFORM}")

set(WEBRTC_CMAKE_C_FLAGS "-fPIC -std=gnu99")

include(ExternalProject)
ExternalProject_Add(kvs-webrtc
  GIT_REPOSITORY    https://github.com/awslabs/amazon-kinesis-video-streams-webrtc-sdk-c.git
  GIT_TAG           v1.7.3
  CMAKE_ARGS        -DADD_MUCLIBC=${USE_MUCLIBC}
                    -DUSE_OPENSSL=OFF -DUSE_MBEDTLS=ON -DBUILD_STATIC_LIBS=ON -DBUILD_SAMPLE=OFF -DBUILD_LIBSRTP_DESTINATION_PLATFORM=${BOARD_DESTINATION_PLATFORM} -DCMAKE_INSTALL_PREFIX=${AWS_DEPENDENCIES_DIR}/webrtc/
                    -DCMAKE_BUILD_TYPE=Release -DOPEN_SRC_INSTALL_PREFIX=${AWS_DEPENDENCIES_DIR}/webrtc/
                    -DCMAKE_C_FLAGS=${WEBRTC_CMAKE_C_FLAGS}
  PATCH_COMMAND     ${CMAKE_CURRENT_LIST_DIR}/patches/webrtc-patch-manager.sh
  BUILD_ALWAYS      TRUE
  GIT_PROGRESS      TRUE
  TEST_COMMAND      ""
)

set(WEBRTC_SAMPLE_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/source/kvsWebRTCClientMaster.c
    ${CMAKE_CURRENT_LIST_DIR}/source/Common.c)

set(WEBRTC_SDK_LIBS_SHARED
    kvsWebrtcClient
    kvsWebrtcSignalingClient
    kvsCommonLws
    kvspicUtils
    kvspicState
    websockets
    srtp2
    usrsctp
    mbedtls
    mbedx509
    mbedcrypto
    pthread
    m)

set(WEBRTC_SDK_LIBS_STATIC
    libkvsWebrtcClient.a
    libkvsWebrtcSignalingClient.a
    libkvsCommonLws.a
    libkvspicUtils.a
    libkvspicState.a
    libwebsockets.a
    libsrtp2.a
    libusrsctp.a
    libmbedtls.a
    libmbedx509.a
    libmbedcrypto.a
    pthread
    m)

add_executable(kvswebrtcmaster-shared ${WEBRTC_SAMPLE_SRCS})
add_dependencies(kvswebrtcmaster-shared kvs-webrtc embedded-media-shared)
target_include_directories(kvswebrtcmaster-shared PRIVATE ${AWS_DEPENDENCIES_DIR}/webrtc/include/ ${EMBEDDED_MEDIA_INCLUDES_DIR})
target_link_directories(kvswebrtcmaster-shared PRIVATE ${AWS_DEPENDENCIES_DIR}/webrtc/lib/ ${EMBEDDED_MEDIA_LINK_DIR})
target_link_libraries(kvswebrtcmaster-shared embedded-media-shared ${WEBRTC_SDK_LIBS_SHARED} ${BOARD_LIBS_SHARED})

add_executable(kvswebrtcmaster-static ${WEBRTC_SAMPLE_SRCS})
add_dependencies(kvswebrtcmaster-static kvs-webrtc embedded-media-static)
target_include_directories(kvswebrtcmaster-static PRIVATE ${AWS_DEPENDENCIES_DIR}/webrtc/include/ ${EMBEDDED_MEDIA_INCLUDES_DIR})
target_link_directories(kvswebrtcmaster-static PRIVATE ${AWS_DEPENDENCIES_DIR}/webrtc/lib/ ${EMBEDDED_MEDIA_LINK_DIR})
target_link_libraries(kvswebrtcmaster-static embedded-media-static ${WEBRTC_SDK_LIBS_STATIC} ${BOARD_LIBS_STATIC})
