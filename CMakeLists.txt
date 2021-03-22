cmake_minimum_required(VERSION 3.10)

project(nereProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14 -DVK_PROTOTYPES" )

file(GLOB src_files "*.hpp" "*.cpp")

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(src_files_imgui
    ${IMGUI_DIR}/imgui.cpp

    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp

    ${IMGUI_DIR}/imgui_impl_vulkan.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    )

source_group("ImGui" FILES ${src_files_imgui})
include_directories(${IMGUI_DIR})

if (VULKAN_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${src_files} ${src_files_imgui})
    target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES} glfw)
endif (VULKAN_FOUND)