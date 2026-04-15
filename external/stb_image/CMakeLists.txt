cmake_minimum_required(VERSION 4.2.3 FATAL_ERROR)
project(stb_image)

add_library(stb_image STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stb_impl.cpp"
)

target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")