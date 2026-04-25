cmake_minimum_required(VERSION 4.3.1)
project(terrainPlus)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source/app")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source/core")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source/glfwApplication")

add_custom_target(copy-resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/data"
    "${CMAKE_BINARY_DIR}/bin/data"
)