file(GLOB_RECURSE SOURCE_FILES
        "*.h"
        "*.cpp")

add_library(cpputest
        ${SOURCE_FILES}
)