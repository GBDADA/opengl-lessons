set(APP_NAME "start-initialization")

add_executable("${APP_NAME}"
    src/main.cpp
    src/init.cpp
    include/main.h)

target_include_directories("${APP_NAME}" PRIVATE include)
target_link_libraries("${APP_NAME}" PRIVATE external)