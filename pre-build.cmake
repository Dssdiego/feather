message("Compiling shaders...")
file(GLOB SHADERS ${CMAKE_CURRENT_LIST_DIR}/src/engine/assets/shaders/*.frag ${CMAKE_CURRENT_LIST_DIR}/src/engine/assets/shaders/*.vert)
foreach(shader ${SHADERS})
    execute_process(COMMAND glslc -c ${shader} -o ${shader}.spv)
endforeach()

message("Copying assets to binary folder...")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message("Copying config files to binary folder...")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/configs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/configs/feather.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/configs/imgui.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/configs/steam_appid.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
