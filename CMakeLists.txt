cmake_minimum_required(VERSION 4.0)
project(RegenX)

set(CMAKE_CXX_STANDARD 20)

add_executable(RegenX main.cpp
        RegenXIO.cpp
        RegenXIO.h)
