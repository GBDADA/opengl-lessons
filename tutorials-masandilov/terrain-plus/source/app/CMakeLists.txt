project(terrainPlus)
set(TARGET_NAME "terrain-plus")

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME}
    PRIVATE glfwApplication
    PRIVATE core
)

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME "terrain-plus"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)