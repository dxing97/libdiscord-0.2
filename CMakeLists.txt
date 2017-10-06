cmake_minimum_required(VERSION 3.7)
project(libdiscordv0_2)

set(LD_VERSION_MAJOR 0)
set(LD_VERSION_MINOR 1)
set(LD_VERSION_PATCH 0)

set(LD_API_VERSION 6)
set(LD_GATEWAY_ENCODING "json")

#number of seconds between connection attempts to the gateway (min allowed by Discord is 5)
set(LD_GATEWAY_CONNECTION_LIMIT 6)
#number of payloads that can be sent in a minute (max allowed by Discord is 120)
set(LD_GATEWAY_SEND_LIMIT 100)
#number of times that a game status update payload can be sent in one minute (max allowed by Discord is 5)
set(LD_GATEWAY_GAME_UPDATE_SEND_LIMIT 4)

set(LD_META libdiscord)

configure_file(
        "${PROJECT_SOURCE_DIR}/libdiscordConfig.h.in"
        "${PROJECT_BINARY_DIR}/libdiscordConfig.h"
)

include_directories("${PROJECT_BINARY_DIR}")

set(LIBS websockets ulfius yder orcania jansson config)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        json.h json.c
        libdiscord_private.h
        main.c
        libdiscord.h
        config.h
        config.c
        libdiscordConfig.h.in libdiscord.c  http.h http.c logging.h logging.c
        libdiscord_private.c ratelimit.c ratelimit.h websocket.c websocket.h)
add_executable(libdiscordv0_1 ${SOURCE_FILES} )

target_compile_options(libdiscordv0_1 PRIVATE "-Wall")
target_compile_options(libdiscordv0_1 PRIVATE "-Werror")
target_link_libraries(libdiscordv0_1 ${LIBS})