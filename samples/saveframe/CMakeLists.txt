cmake_minimum_required(VERSION 3.12)
project(saveframe VERSION 1.0.0 LANGUAGES C CXX)

get_target_property(EMBEDDED_MEDIA_INCLUDES_DIR embedded-media-shared INCLUDE_DIRECTORIES)
get_target_property(EMBEDDED_MEDIA_LINK_DIR embedded-media-shared LINK_DIRECTORIES)

message(STATUS "SAMPLES_DIR - ${SAMPLES_DIR}")
message(STATUS "BOARD_LIBS_DIR - ${BOARD_LIBS_DIR}")
message(STATUS "BOARD_LIBS - ${BOARD_LIBS}")
message(STATUS "EMBEDDED_MEDIA_INCLUDES_DIR - ${EMBEDDED_MEDIA_INCLUDES_DIR}")
message(STATUS "EMBEDDED_MEDIA_LINK_DIR - ${EMBEDDED_MEDIA_LINK_DIR}")

set(SAVE_FRAME_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/source/saveframe.c)

add_executable(saveframe-shared ${SAVE_FRAME_SRCS})
add_dependencies(saveframe-shared embedded-media-shared)
target_include_directories(saveframe-shared PRIVATE ${EMBEDDED_MEDIA_INCLUDES_DIR})
target_link_directories(saveframe-shared PRIVATE ${EMBEDDED_MEDIA_LINK_DIR} ${BOARD_LIBS_DIR})
target_link_libraries(saveframe-shared embedded-media-shared ${BOARD_LIBS_SHARED})

add_executable(saveframe-static ${SAVE_FRAME_SRCS})
add_dependencies(saveframe-static embedded-media-static)
target_include_directories(saveframe-static PRIVATE ${EMBEDDED_MEDIA_INCLUDES_DIR})
target_link_directories(saveframe-static PRIVATE ${EMBEDDED_MEDIA_LINK_DIR} ${BOARD_LIBS_DIR})
target_link_libraries(saveframe-static embedded-media-static ${BOARD_LIBS_STATIC})

include(GNUInstallDirs)

install(TARGETS saveframe-shared
    RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

install(TARGETS saveframe-static
    RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
