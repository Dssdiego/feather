cmake_minimum_required(VERSION 3.7)

project(imgui)

include_directories(../glfw/include)

set (IMGUI_RUNTIME_SOURCES
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_glfw.cpp
        imgui_impl_glfw.h
        imgui_impl_vulkan.cpp
        imgui_impl_vulkan.h
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        ImGuizmo.h
        ImGuizmo.cpp
)

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} ${IMGUI_RUNTIME_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})