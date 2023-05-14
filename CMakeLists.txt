cmake_minimum_required(VERSION 3.25)
project(The_Sword_In_The_Stone)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(The_Sword_In_The_Stone
        knight2.cpp
        knight2.h
        main.cpp
        main.h)
