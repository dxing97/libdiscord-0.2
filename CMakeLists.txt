cmake_minimum_required(VERSION 3.8)
project(libdiscordv0_1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c libdiscord.h)
add_executable(libdiscordv0_1 ${SOURCE_FILES})