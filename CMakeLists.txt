cmake_minimum_required(VERSION 4.2.3 FATAL_ERROR)

# Подключаем модули cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(add_more_projects)

set(PROJECT_NAME opengl-lessons)
project(${PROJECT_NAME})

# чтобы kate не ругался
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Создание списка библиотек --------------------------------------------

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
list(APPEND LIBRARIES_LIST "glfw")

# Glad
add_subdirectory(external/glad)
list(APPEND LIBRARIES_LIST "glad")

# Подключение проектов
set(BUILD_PROJECTS "" CACHE STRING "хранит список названий проектов, которые должны быть собраны (если пустой, то будут собраны все файлы в small-projects)")

# Директория с проектами без общем тематики
add_more_projects(DIRECTORY small-projects BUILD "${BUILD_PROJECTS}" LIBRARIES "${LIBRARIES_LIST}")