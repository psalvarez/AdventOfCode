cmake_minimum_required(VERSION 3.25)
project(AdventOfCode_2015_7 C)

add_executable(main main.c)

if (MSVC)
    target_compile_options(main PRIVATE /Wall /WX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -Werror)
endif()