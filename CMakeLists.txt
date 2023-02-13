cmake_minimum_required(VERSION 3.20)
project(feather)

set(CMAKE_CXX_STANDARD 17)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# subdirectories
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/lua)
add_subdirectory(thirdparty/imgui)
add_subdirectory(thirdparty/tracy)
add_subdirectory(thirdparty/cpputest)
add_subdirectory(thirdparty/yojimbo)

add_subdirectory(tests)
add_subdirectory(server)

# Vulkan
find_package(Vulkan REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# FMOD
set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/fmod/inc")

if(WIN32)
    set(FMOD_LIBRARY
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmod_vc.lib" # for production use
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudio_vc.lib" # for production use
        #            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudioL_vc.lib" # for debug use
    )
    file(COPY
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmod.dll" # for production use
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudio.dll" # for production use
        #            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudioL.dll" # for debug use
        DESTINATION ${CMAKE_BINARY_DIR})
elseif(APPLE)
    set(FMOD_LIBRARY
            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/libfmod.dylib" # for production use
            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/libfmodstudio.dylib" # for production use
            )
else()
    set(FMOD_LIBRARY
            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/libfmod.so" # for production use
            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/libfmodstudio.so" # for production use
            #            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudioL_vc.lib" # for debug use
            )
endif()

# Geforce NOW
set(GEFORCE_NOW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/geforce_now/include")
#set(GEFORCE_NOW_LIBRARY
#        "${CMAKE_SOURCE_DIR}/thirdparty/geforce_now/lib/x64/GfnRuntimeSdk.dll"
#        )
file(COPY
        "${CMAKE_SOURCE_DIR}/thirdparty/geforce_now/lib/x64/GfnRuntimeSdk.dll"
        DESTINATION ${CMAKE_BINARY_DIR})
#add_library(GEFORCE_NOW_LIBRARY SHARED thirdparty/geforce_now/lib/x64/GfnRuntimeSdk.dll)
#set_target_properties(GEFORCE_NOW_LIBRARY PROPERTIES LINKER_LANGUAGE C)

# Steam SDK
set(STEAM_SDK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/steam/inc")
if(WIN32)
    set(STEAM_SDK_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/steam/lib/win64/steam_api64.lib" )
    file(COPY "${CMAKE_SOURCE_DIR}/thirdparty/steam/lib/win64/steam_api64.dll" DESTINATION ${CMAKE_BINARY_DIR})
elseif(APPLE)
    set(STEAM_SDK_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/steam/lib/osx/libsteam_api.dylib" )
    file(COPY "${CMAKE_SOURCE_DIR}/thirdparty/steam/lib/osx/libsteam_api.dylib" DESTINATION ${CMAKE_BINARY_DIR})
endif()

# YAML-CPP
set(YAML_CPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/yaml-cpp/inc")
if(WIN32)
    set(YAML_CPP_LIBRARY 
        debug "${CMAKE_SOURCE_DIR}/thirdparty/yaml-cpp/lib/yaml-cppd.lib"
        optimized "${CMAKE_SOURCE_DIR}/thirdparty/yaml-cpp/lib/yaml-cpp.lib"
    )
    file(COPY "${CMAKE_SOURCE_DIR}/thirdparty/yaml-cpp/lib/yaml-cppd.dll" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${CMAKE_SOURCE_DIR}/thirdparty/yaml-cpp/lib/yaml-cpp.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Messages
message("Libraries:")
message(VULKAN_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS})
message(VULKAN_LIB: ${Vulkan_LIBRARIES})
message(OPENGL_LIBRARIES: ${OPENGL_LIBRARIES})
message(FMOD_LIBRARY: ${FMOD_LIBRARY})
message(GEFORCE_NOW_LIBRARY: ${GEFORCE_NOW_LIBRARY})
message(STEAM_SDK_LIBRARY: ${STEAM_SDK_LIBRARY})
message(YAML_CPP_LIBRARY: ${YAML_CPP_LIBRARY})

# include directories
include_directories(
        thirdparty
        thirdparty/entt/single_include
        thirdparty/glm
        thirdparty/json/single_include
        ${FMOD_INCLUDE_DIR}
        ${GEFORCE_NOW_INCLUDE_DIR}
        ${STEAM_SDK_INCLUDE_DIR}
        ${YAML_CPP_INCLUDE_DIR}
)

# load source and header files
file(GLOB_RECURSE SOURCE_FILES
        "src/*.h"
        "src/*.cpp")

# Package project with CPACK
SET(CPACK_GENERATOR "ZIP")
SET(CPACK_PACKAGE_NAME "Feather - v0.0.1")
include(CPack)

# tests
#option(COMPILE_TESTS "Compile the tests" OFF)
#if(COMPILE_TESTS)
#    add_subdirectory(tests)
#endif(COMPILE_TESTS)

# application icon
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

# creating executable
if(WIN32)
    add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        thirdparty/glad/glad.c
        ${APP_ICON_RESOURCE_WINDOWS}
        )
else()
    add_executable(${PROJECT_NAME}
            ${SOURCE_FILES}
            thirdparty/glad/glad.c
            ${APP_ICON_RESOURCE_WINDOWS}
            )
endif()

if(WIN32)
    target_link_libraries(
            ${PROJECT_NAME}
            glfw
            Vulkan::Vulkan
            d3d12
            OpenGL::GL
            lua_lib
            imgui
            TracyClient
            ${FMOD_LIBRARY}
            ${STEAM_SDK_LIBRARY}
            yojimbo
            ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/mbedtls.lib
            ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/mbedx509.lib
            ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/mbedcrypto.lib
            ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/sodium.lib
            ${YAML_CPP_LIBRARY}
    )
else()
    target_link_libraries(
            ${PROJECT_NAME}
            glfw
            Vulkan::Vulkan
            OpenGL::GL
            lua_lib
            imgui
            TracyClient
            ${FMOD_LIBRARY}
            ${STEAM_SDK_LIBRARY}
    )
endif()

# defining final executable name
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Feather - v0.0.1")

# pre-build stuff (shader compilation and asset copying to binary folder)
add_custom_target(pre_build
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/pre-build.cmake
        )
add_dependencies(${PROJECT_NAME} pre_build)

# TODO: Use rcedit after cmake build to change windows executable details
# TODO: Package project with CPACK