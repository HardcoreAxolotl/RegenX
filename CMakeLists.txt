cmake_minimum_required(VERSION 4.0)
project(RegenX)

set(CMAKE_CXX_STANDARD 20)

find_package(Lua REQUIRED)

add_executable(RegenX src/main.cpp
        src/RegenX/Functions/RegenXIO.cpp
        src/RegenX/Script/Lua/RegenXLua.cpp
        src/RegenX/Functions/RegenXLog.cpp
        src/RegenX/RegenXPlatform.cpp
        src/RegenX/RegenXPlatform.hpp
)

target_include_directories(RegenX PRIVATE
        ${LUA_INCLUDE_DIR}
)

target_link_libraries(RegenX PRIVATE
        ${LUA_LIBRARIES}
)