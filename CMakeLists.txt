cmake_minimum_required(VERSION 4.0)
project(RegenX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)

# --- Boost (header‑only or only includes available parts) ---
# Unlike older Boost versions, boost_system is header‑only starting with 1.69,
# and recent Boost config files on some distros (e.g., Arch Linux) do *not*
# provide a separate boost_systemConfig.cmake. To avoid CMake trying to
# locate a non‑existent config file, we use classic FindBoost without COMPONENTS.
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.65 REQUIRED)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
else()
    message(STATUS "Using Boost includes: ${Boost_INCLUDE_DIRS}")
endif()

# --- Paths ---
set(IMGUI_PATH        "${CMAKE_SOURCE_DIR}/imgui")
set(IMGUI_BACKENDS    "${IMGUI_PATH}/backends")
set(REGENX_PATH       "${CMAKE_SOURCE_DIR}/RegenX")
set(SRC_PATH          "${CMAKE_SOURCE_DIR}/src")
set(IMNODEFLOW_PATH   "${CMAKE_SOURCE_DIR}/ImNodeFlow")

# --- Source Files ---
# RegenX .cpp files
file(GLOB_RECURSE REGENX_SOURCES "${REGENX_PATH}/*.cpp")

# ImGui core
file(GLOB IMGUI_CORE
        "${IMGUI_PATH}/imgui.cpp"
        "${IMGUI_PATH}/imgui_draw.cpp"
        "${IMGUI_PATH}/imgui_widgets.cpp"
        "${IMGUI_PATH}/imgui_tables.cpp"
)

# ImGui SDL2 + OpenGL3 backends (only what we need)
set(IMGUI_BACKEND_SOURCES
        "${IMGUI_BACKENDS}/imgui_impl_sdl2.cpp"
        "${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp"
)

# ImNodeFlow implementation files
file(GLOB_RECURSE IMNODEFLOW_SOURCES "${IMNODEFLOW_PATH}/src/*.cpp")

# Main app source
set(MAIN_SOURCES
        "${SRC_PATH}/main.cpp"
        "${SRC_PATH}/glad.c"
)

# Combine everything
set(ALL_SOURCES
        ${IMGUI_CORE}
        ${IMGUI_BACKEND_SOURCES}
        ${IMNODEFLOW_SOURCES}
        ${REGENX_SOURCES}
        ${MAIN_SOURCES}
)

# --- Executable ---
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# --- Include Directories ---
target_include_directories(${PROJECT_NAME} PRIVATE
        ${IMGUI_PATH}
        ${IMGUI_BACKENDS}
        ${REGENX_PATH}
        ${IMNODEFLOW_PATH}/include
        ${Boost_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIRS}
        include
)

# --- Link Libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        OpenGL::GL
        ${CURL_LIBRARIES}
)

# --- IDE Grouping (optional) ---
source_group("ImGui Core"         FILES ${IMGUI_CORE})
source_group("ImGui Backends"     FILES ${IMGUI_BACKEND_SOURCES})
source_group("RegenX Modules"     FILES ${REGENX_SOURCES})
source_group("ImNodeFlow Modules" FILES ${IMNODEFLOW_SOURCES})
source_group("Main Source"        FILES ${MAIN_SOURCES})
