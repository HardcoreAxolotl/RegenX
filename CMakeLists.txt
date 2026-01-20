cmake_minimum_required(VERSION 3.26)
project(RegenX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================================================
# Dependencies
# ===========================================================
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)

find_path(ENET_INCLUDE_DIR enet/enet.h)
find_library(ENET_LIBRARY enet)

# ===========================================================
# Paths
# ===========================================================
set(IMGUI_PATH        "${CMAKE_SOURCE_DIR}/thirdparty/imgui")
set(IMGUI_BACKENDS    "${IMGUI_PATH}/backends")
set(REGENX_PATH       "${CMAKE_SOURCE_DIR}/RegenX")
set(INCLUDE_PATH      "${CMAKE_SOURCE_DIR}/include")
set(SRC_PATH          "${CMAKE_SOURCE_DIR}/src")
set(IMNODEFLOW_PATH   "${CMAKE_SOURCE_DIR}/thirdparty/ImNodeFlow")
set(RESOURCE_DIR      "${CMAKE_SOURCE_DIR}/resource")
set(RESOURCE_DEST     "${CMAKE_BINARY_DIR}/resource")

# ===========================================================
# Source Files
# ===========================================================
set(MAIN_SOURCES
        "${SRC_PATH}/main.cpp"
        "${SRC_PATH}/glad.c"
)

set(REGENX_SOURCES
        "${REGENX_PATH}/RegenXIO.cpp"
        "${REGENX_PATH}/RegenXLog.cpp"
)

set(INCLUDE_SOURCES
        "${INCLUDE_PATH}/rendering/Shader.cpp"
        "${INCLUDE_PATH}/rendering/VAO.cpp"
        "${INCLUDE_PATH}/rendering/VBO.cpp"
        "${INCLUDE_PATH}/rendering/EBO.cpp"
)

set(IMGUI_CORE
        "${IMGUI_PATH}/imgui.cpp"
        "${IMGUI_PATH}/imgui_draw.cpp"
        "${IMGUI_PATH}/imgui_widgets.cpp"
        "${IMGUI_PATH}/imgui_tables.cpp"
)

set(IMGUI_BACKEND_SOURCES
        "${IMGUI_BACKENDS}/imgui_impl_sdl2.cpp"
        "${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp"
)

file(GLOB_RECURSE IMNODEFLOW_SOURCES
        "${IMNODEFLOW_PATH}/src/*.cpp"
)

set(ALL_SOURCES
        ${MAIN_SOURCES}
        ${REGENX_SOURCES}
        ${INCLUDE_SOURCES}
        ${IMNODEFLOW_SOURCES}
        ${IMGUI_CORE}
        ${IMGUI_BACKEND_SOURCES}
        include/stb/stb_image.h
)

# ===========================================================
# Executable
# ===========================================================
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# ===========================================================
# Include Directories
# ===========================================================
target_include_directories(${PROJECT_NAME} PRIVATE
        ${IMGUI_PATH}
        ${IMGUI_BACKENDS}
        ${REGENX_PATH}
        ${IMNODEFLOW_PATH}/include
        ${CURL_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${ENET_INCLUDE_DIRS}
        include
)

# ===========================================================
# Link Libraries
# ===========================================================
target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        OpenGL::GL
        ${CURL_LIBRARIES}
        ${ENET_LIBRARIES}
)

# ===========================================================
# Copy / regenerate resources folder
# ===========================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${RESOURCE_DEST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_DEST}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCE_DIR}" "${RESOURCE_DEST}"
        COMMENT "Regenerating resource folder in build directory"
)

# ===========================================================
# IDE Source Grouping (optional)
# ===========================================================
source_group("ImGui Core"         FILES ${IMGUI_CORE})
source_group("ImGui Backends"     FILES ${IMGUI_BACKEND_SOURCES})
source_group("RegenX Modules"     FILES ${REGENX_SOURCES})
source_group("ImNodeFlow Modules" FILES ${IMNODEFLOW_SOURCES})
source_group("Main Source"        FILES ${MAIN_SOURCES})
