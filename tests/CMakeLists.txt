project(feather_tests)

include_directories(../thirdparty/cpputest)

file(GLOB_RECURSE SOURCE_FILES
        "*.h"
        "*.cpp"
)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
)

target_link_libraries(
        ${PROJECT_NAME}
        cpputest
)
