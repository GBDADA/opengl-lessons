cmake_minimum_required(VERSION 4.2.3 FATAL_ERROR)
project(Glad)

add_library(glad STATIC include/glad/gl.h src/gl.c)
target_include_directories(glad PUBLIC include/)