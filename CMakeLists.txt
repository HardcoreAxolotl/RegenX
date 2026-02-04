cmake_minimum_required(VERSION 4.0)
project(RegenXApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================================================
# Paths
# ===========================================================
set(ROOT_DIR        "${CMAKE_SOURCE_DIR}")
set(SRC_PATH        "${ROOT_DIR}/src")
set(INCLUDE_PATH    "${ROOT_DIR}/include")
set(THIRDPARTY_PATH "${ROOT_DIR}/thirdparty")
set(RESOURCE_DIR    "${ROOT_DIR}/resource")
set(RESOURCE_DEST   "${CMAKE_BINARY_DIR}/resource")

set(IMGUI_PATH      "${THIRDPARTY_PATH}/imgui")
set(IMGUI_BACKENDS  "${IMGUI_PATH}/backends")
set(IMNODEFLOW_PATH "${THIRDPARTY_PATH}/ImNodeFlow")
set(GLAD_INCLUDE    "${THIRDPARTY_PATH}/glad/include")
set(GLAD_SRC        "${THIRDPARTY_PATH}/glad/src/glad.c")

# ===========================================================
# Dependencies
# ===========================================================
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

find_library(ENET_LIBRARY enet)
find_path(ENET_INCLUDE_DIR enet/enet.h)

# ===========================================================
# Source Files (automatic)
# ===========================================================
# RegenX internal source
file(GLOB_RECURSE REGENX_SOURCES
        "${SRC_PATH}/Core/*.cpp"
        "${SRC_PATH}/Networking/*.cpp"
        "${SRC_PATH}/Renderer/*.cpp"
        "${SRC_PATH}/Window/*.cpp"
        "${SRC_PATH}/Shapes/*.cpp"
)

# Main app
set(MAIN_SOURCES "${SRC_PATH}/main.cpp")

# ImGui core sources (only what is needed)
set(IMGUI_CORE_SOURCES
        "${IMGUI_PATH}/imgui.cpp"
        "${IMGUI_PATH}/imgui_draw.cpp"
        "${IMGUI_PATH}/imgui_widgets.cpp"
        "${IMGUI_PATH}/imgui_tables.cpp"
)

# Only include the GLFW + OpenGL3 backends for ImGui
set(IMGUI_BACKEND_SOURCES
        "${IMGUI_BACKENDS}/imgui_impl_glfw.cpp"
        "${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp"
)

# ImNodeFlow sources
file(GLOB_RECURSE IMNODEFLOW_SOURCES
        "${IMNODEFLOW_PATH}/src/*.cpp"
)

set(ALL_SOURCES
        ${MAIN_SOURCES}
        ${REGENX_SOURCES}
        ${IMGUI_CORE_SOURCES}
        ${IMGUI_BACKEND_SOURCES}
        ${IMNODEFLOW_SOURCES}
        ${GLAD_SRC}
        "${THIRDPARTY_PATH}/stb/stb.cpp"
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
        ${THIRDPARTY_PATH}
        ${IMGUI_PATH}
        ${IMGUI_BACKENDS}
        ${IMNODEFLOW_PATH}/include
        ${GLAD_INCLUDE}
        ${ENET_INCLUDE_DIR}
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
        X11
)

# ===========================================================
# Resource Folder Copy
# ===========================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${RESOURCE_DEST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_DEST}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCE_DIR}" "${RESOURCE_DEST}"
        COMMENT "Copying resource files"
)
