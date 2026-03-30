function(add_selected)
    # Собираем сложный проект
    if(EXISTS "${path}/CMakeLists.txt")
        add_subdirectory("${path}")
    # Собираем просто проект
    elseif(EXISTS "${path}/main.cpp")
        add_executable("${lesson}" "${path}/main.cpp")
        target_link_libraries("${lesson}" external)
        message(STATUS "Авто-проект создан для: ${lesson}")
    # Не удалось собрать проект
    else()
        message(WARNING "В папке ${path} не найден CMakeLists.txt или main.cpp")
    endif()
endfunction()