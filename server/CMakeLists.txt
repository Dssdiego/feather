cmake_minimum_required(VERSION 3.20)
project(feather_server)

include_directories(../thirdparty/yojimbo)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES
        "*.h"
        "*.cpp")

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        )

target_link_libraries(
        ${PROJECT_NAME}
        yojimbo
        ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/mbedtls.lib
        ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/mbedx509.lib
        ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/mbedcrypto.lib
        ${CMAKE_SOURCE_DIR}/thirdparty/yojimbo/sodium.lib
)
