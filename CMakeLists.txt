cmake_minimum_required(VERSION 4.0)
project(RegenX)

set(CMAKE_CXX_STANDARD 20)

find_package(Lua REQUIRED)

add_executable(RegenX src/main.cpp
        src/RegenX/RegenXIO.cpp
        src/RegenX/RegenXIO.h)

target_include_directories(RegenX PRIVATE
        ${LUA_INCLUDE_DIR}
)

target_link_libraries(RegenX PRIVATE
        ${LUA_LIBRARIES}
)