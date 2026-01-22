cmake_minimum_required(VERSION 4.0)

# ===========================================================
# Project
# ===========================================================
project(RegenXApp LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================================================
# Dependencies
# ===========================================================
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)

find_library(ENET_LIBRARY enet)
find_path(ENET_INCLUDE_DIR enet/enet.h)

# Gainput (installed via cmake install)
find_library(GAINPUT_LIBRARY NAMES gainput)
find_path(GAINPUT_INCLUDE_DIR gainput/GainputInputManager.h)

# GLFW (system-installed)
pkg_check_modules(GLFW REQUIRED glfw3)

# ===========================================================
# Paths
# ===========================================================
set(ROOT_DIR        "${CMAKE_SOURCE_DIR}")
set(SRC_PATH        "${ROOT_DIR}/src")
set(INCLUDE_PATH    "${ROOT_DIR}/include")
set(REGENX_INCLUDE  "${INCLUDE_PATH}/RegenX")
set(IMGUI_PATH      "${ROOT_DIR}/thirdparty/imgui")
set(IMGUI_BACKENDS  "${IMGUI_PATH}/backends")
set(IMNODEFLOW_PATH "${ROOT_DIR}/thirdparty/ImNodeFlow")
set(GLAD_PATH       "${ROOT_DIR}/thirdparty/glad")
set(GLAD_INCLUDE    "${GLAD_PATH}/include")
set(GLAD_SRC        "${GLAD_PATH}/src/glad.c")
set(RESOURCE_DIR    "${ROOT_DIR}/resource")
set(RESOURCE_DEST   "${CMAKE_BINARY_DIR}/resource")

# ===========================================================
# Source Files
# ===========================================================
set(MAIN_SOURCES
        "${SRC_PATH}/main.cpp"
        "${GLAD_SRC}"
)

set(REGENX_SOURCES
        "${REGENX_INCLUDE}/RegenXIO.cpp"
        "${REGENX_INCLUDE}/RegenXLog.cpp"
        "${REGENX_INCLUDE}/RegenXMath.cpp"
)

set(RENDERING_SOURCES
        "${INCLUDE_PATH}/rendering/Shader.cpp"
        "${INCLUDE_PATH}/rendering/VAO.cpp"
        "${INCLUDE_PATH}/rendering/VBO.cpp"
        "${INCLUDE_PATH}/rendering/EBO.cpp"
)

set(IMGUI_CORE_SOURCES
        "${IMGUI_PATH}/imgui.cpp"
        "${IMGUI_PATH}/imgui_draw.cpp"
        "${IMGUI_PATH}/imgui_widgets.cpp"
        "${IMGUI_PATH}/imgui_tables.cpp"
)

set(IMGUI_BACKEND_SOURCES
        "${IMGUI_BACKENDS}/imgui_impl_glfw.cpp"
        "${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp"
)

file(GLOB_RECURSE IMNODEFLOW_SOURCES
        "${IMNODEFLOW_PATH}/src/*.cpp"
)

set(ALL_SOURCES
        ${MAIN_SOURCES}
        ${REGENX_SOURCES}
        ${RENDERING_SOURCES}
        ${IMNODEFLOW_SOURCES}
        ${IMGUI_CORE_SOURCES}
        ${IMGUI_BACKEND_SOURCES}
        thirdparty/stb/stb.cpp
)

# ===========================================================
# Executable
# ===========================================================
add_executable(RegenXApp ${ALL_SOURCES})

# ===========================================================
# Include Directories
# ===========================================================
target_include_directories(RegenXApp PRIVATE
        ${INCLUDE_PATH}
        ${REGENX_INCLUDE}
        ${IMGUI_PATH}
        ${IMGUI_BACKENDS}
        ${IMNODEFLOW_PATH}/include
        ${GLAD_INCLUDE}
        ${ENET_INCLUDE_DIR}
        ${GAINPUT_INCLUDE_DIR}
        ${CURL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
)

# ===========================================================
# Link Libraries
# ===========================================================
target_link_libraries(RegenXApp PRIVATE
        ${GLFW_LIBRARIES}
        OpenGL::GL
        ${CURL_LIBRARIES}
        ${ENET_LIBRARY}
        ${GAINPUT_LIBRARY}
        X11
)

# ===========================================================
# Resource Folder Copy
# ===========================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${RESOURCE_DEST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_DEST}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCE_DIR}" "${RESOURCE_DEST}"
        COMMENT "Regenerating resource folder in build directory"
)

# ===========================================================
# IDE Source Grouping
# ===========================================================
source_group("Main"        FILES ${MAIN_SOURCES})
source_group("RegenX"      FILES ${REGENX_SOURCES})
source_group("Rendering"   FILES ${RENDERING_SOURCES})
source_group("ImGui Core"  FILES ${IMGUI_CORE_SOURCES})
source_group("ImGui Backends" FILES ${IMGUI_BACKEND_SOURCES})
source_group("ImNodeFlow"  FILES ${IMNODEFLOW_SOURCES})
source_group("GLAD"        FILES ${GLAD_SRC})
