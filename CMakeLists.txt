# ==============================
# File: CMakeLists.txt (ROOT)
# ==============================
cmake_minimum_required(VERSION 3.26)
project(RegenX LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(REGENX_BUILD_EDITOR "Build RegenX Editor" ON)

# ---------- System deps (Arch packages) ----------
find_package(Threads REQUIRED)

find_package(SDL2 REQUIRED)      # SDL2::SDL2
find_package(OpenAL REQUIRED)    # OpenAL::OpenAL
find_package(Freetype REQUIRED)  # Freetype::Freetype
find_package(Lua REQUIRED)       # Lua::Lua

# Python is editor-only (do NOT find_package here unless editor is ON)
if (REGENX_BUILD_EDITOR)
    find_package(Python3 REQUIRED COMPONENTS Interpreter Development) # Python3::Python
endif()

# ---------- Third-party deps in ./third_party ----------
add_subdirectory(third_party)

# ---------- Engine (runtime library) ----------
add_subdirectory(src/regenx)

# ---------- Editor (tooling, optional) ----------
if (REGENX_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

# ---------- App / sandbox executable ----------
add_executable(RegenXApp src/main.cpp)
target_link_libraries(RegenXApp PRIVATE regenx_engine)
