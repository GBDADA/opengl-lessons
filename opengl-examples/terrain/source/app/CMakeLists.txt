project(terrainMain)
set(TARGET_NAME "terrain")

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME}
    PRIVATE glfwApplication
    PRIVATE core
)

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME "terrain"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)