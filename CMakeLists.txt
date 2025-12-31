cmake_minimum_required(VERSION 3.26)
project(RegenX LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(REGENX_BUILD_EDITOR "Build RegenX Editor" ON)

# --- System deps ---
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Freetype REQUIRED)
find_package(Lua REQUIRED)

# OpenGL is required for the ImGui OpenGL backend test app
find_package(OpenGL REQUIRED)

# Python is editor-only
if (REGENX_BUILD_EDITOR)
    find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
endif()

# --- Third-party ---
add_subdirectory(third_party)

# --- Engine ---
add_subdirectory(src/regenx)

# --- Editor ---
if (REGENX_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

# --- App ---
add_executable(RegenXApp src/main.cpp)
target_link_libraries(RegenXApp PRIVATE regenx_engine OpenGL::GL)
