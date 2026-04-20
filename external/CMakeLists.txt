# Подключение внешних библиотек

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw")

# Glad
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glad")

# stb_image
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/stb_image")

# Для glu
find_package(OpenGL REQUIRED)

# Создаем интерфейсную цель "opengl_deps", которая объединяет всё
add_library(external INTERFACE)

target_include_directories(external INTERFACE
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(external INTERFACE
    glfw
    glad
    stb_image
    GLU
)