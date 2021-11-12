cmake_minimum_required(VERSION 3.12)
project(kvsproducer VERSION 1.0.0 LANGUAGES C CXX)

get_target_property(EMBEDDED_MEDIA_INCLUDES_DIR embedded-media-shared INCLUDE_DIRECTORIES)
get_target_property(EMBEDDED_MEDIA_LINK_DIR embedded-media-shared LINK_DIRECTORIES)

message(STATUS "SAMPLES_DIR - ${SAMPLES_DIR}")
message(STATUS "AWS_SDK_DIR - ${AWS_SDK_DIR}")
message(STATUS "BOARD_LIBS_DIR - ${BOARD_LIBS_DIR}")
message(STATUS "BOARD_LIBS - ${BOARD_LIBS}")
message(STATUS "EMBEDDED_MEDIA_INCLUDES_DIR - ${EMBEDDED_MEDIA_INCLUDES_DIR}")
message(STATUS "EMBEDDED_MEDIA_LINK_DIR - ${EMBEDDED_MEDIA_LINK_DIR}")

include(ExternalProject)
ExternalProject_Add(kvs-producer
  GIT_REPOSITORY    https://github.com/aws-samples/amazon-kinesis-video-streams-producer-embedded-c.git
  GIT_TAG           cmake_fix
  CMAKE_ARGS        -DBUILD_SHARED_SAMPLES=ON -DBUILD_STATIC_SAMPLES=ON -DCMAKE_INSTALL_PREFIX=${AWS_SDK_DIR} -DCMAKE_BUILD_TYPE=Release
  BUILD_ALWAYS      TRUE
  GIT_PROGRESS      TRUE
  TEST_COMMAND      ""
)

set(KVS_SAMPLE_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/source/kvsappcli.c
    ${CMAKE_CURRENT_LIST_DIR}/source/option_configuration.c)

set(KVS_SDK_LIBS_SHARED
    kvsapp
    kvs-embedded-c
    mbedtls
    mbedcrypto
    mbedx509
    llhttp
    parson
    aziotsharedutil
    tlsf)

set(KVS_SDK_LIBS_STATIC
    libkvsapp.a
    libkvs-embedded-c.a
    libmbedtls.a
    libmbedx509.a
    libmbedcrypto.a
    libllhttp.a
    libparson.a
    libaziotsharedutil.a
    libtlsf.a)

add_executable(kvsproducer-shared ${KVS_SAMPLE_SRCS})
add_dependencies(kvsproducer-shared kvs-producer embedded-media-shared)
target_include_directories(kvsproducer-shared PRIVATE ${AWS_SDK_DIR}/include/ ${EMBEDDED_MEDIA_INCLUDES_DIR})
target_link_directories(kvsproducer-shared PRIVATE ${AWS_SDK_DIR}/lib/ ${EMBEDDED_MEDIA_LINK_DIR})
target_link_libraries(kvsproducer-shared embedded-media-shared ${KVS_SDK_LIBS_SHARED} ${BOARD_LIBS_SHARED})

add_executable(kvsproducer-static ${KVS_SAMPLE_SRCS})
add_dependencies(kvsproducer-static kvs-producer embedded-media-static)
target_include_directories(kvsproducer-static PRIVATE ${AWS_SDK_DIR}/include/ ${EMBEDDED_MEDIA_INCLUDES_DIR})
target_link_directories(kvsproducer-static PRIVATE ${AWS_SDK_DIR}/lib/ ${EMBEDDED_MEDIA_LINK_DIR})
target_link_libraries(kvsproducer-static embedded-media-static ${KVS_SDK_LIBS_STATIC} ${BOARD_LIBS_STATIC})
