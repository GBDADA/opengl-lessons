cmake_minimum_required(VERSION 4.2.3 FATAL_ERROR)

# Подключаем модули cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(add_selected)
include(build_selected)

# Задаём название проекта (самый бесполезный комментарий)
set(PROJECT_NAME opengl-lessons)
project(${PROJECT_NAME} LANGUAGES CXX)

# чтобы kate не ругался
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Указываем место сборки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Подключение внешних библиотек
add_subdirectory("${CMAKE_SOURCE_DIR}/external")

# Подключение проектов
set(SELECTED_LESSONS "" CACHE STRING "проекты для сборки")
add_subdirectory("${CMAKE_SOURCE_DIR}/glfw-examples")
build_selected()