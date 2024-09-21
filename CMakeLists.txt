cmake_minimum_required(VERSION 3.27)
project(S0_P0 C)

set(CMAKE_C_STANDARD 99)

add_executable(S0_P0 main.c shell_list.h shell_list.c Commands.c
        test.c
        Commands.h)
