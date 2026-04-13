cmake_minimum_required(VERSION 4.2.3 FATAL_ERROR)

# Подключаем модули cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(add_selected)
include(build_selected)
include(prepare_selected)
include(process_projects_list)

# Задаём название проекта (самый бесполезный комментарий)
set(PROJECT_NAME opengl-lessons)
project(${PROJECT_NAME} LANGUAGES CXX)

# чтобы kate не ругался
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Указываем место сборки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Подключение внешних библиотек
add_subdirectory("${CMAKE_SOURCE_DIR}/external")

# Вывести используемый тип сборки
message(STATUS "Текущая конфигурация сборки '${CMAKE_BUILD_TYPE}'")

# Подключение проектов
set(SELECTED_LESSONS "" CACHE STRING "проекты для сборки")
set(DIRECTORIES_WITH_LESSONS "glfw-examples;tutorials-masandilov;opengl-examples" CACHE STRING "Директории, где ищутся уроки")
prepare_selected(DIR_LESSONS ${DIRECTORIES_WITH_LESSONS})
build_selected()