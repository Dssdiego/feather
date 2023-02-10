include_directories(mbedtls)
include_directories(sodium)
include_directories(netcode)
include_directories(reliable)
include_directories(tlsf)

file(GLOB_RECURSE SOURCE_FILES
        "*.h"
        "*.c"
        "*.cpp"
        )

add_library(yojimbo
        ${SOURCE_FILES}
)