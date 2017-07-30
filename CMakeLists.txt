cmake_minimum_required(VERSION 3.7)
project(libdiscordv0_1)

set(LD_VERSION_MAJOR 0)
set(LD_VERSION_MINOR 1)
set(LD_VERSION_PATCH 0)

configure_file(
        "${PROJECT_SOURCE_DIR}/libdiscordConfig.h.in"
        "${PROJECT_BINARY_DIR}/libdiscordConfig.h"
)

include_directories("${PROJECT_BINARY_DIR}")

set(LIBS websockets ulfius yder orcania jansson config)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        main.c
        libdiscord.h
        config.h
        config.c
        libdiscordConfig.h.in)
add_executable(libdiscordv0_1 ${SOURCE_FILES})
target_link_libraries(libdiscordv0_1 ${LIBS})