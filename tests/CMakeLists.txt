project(feather_tests)

include_directories(../thirdparty/cpputest)
include_directories(../thirdparty/glm)

file(GLOB_RECURSE SOURCE_FILES
        "*.h"
        "*.cpp"
)

set(ENGINE_FILES
    "../src/engine/common/Rect.cpp"
)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${ENGINE_FILES}
)

target_link_libraries(
        ${PROJECT_NAME}
        cpputest
)
