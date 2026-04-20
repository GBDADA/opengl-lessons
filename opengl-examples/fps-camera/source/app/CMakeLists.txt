project(fpsCameraMain)
set(TARGET_NAME "fps-camera")

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME}
    PRIVATE glfwApplication
    PRIVATE core
)

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME "fps-camera"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)