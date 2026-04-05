function(prepare_selected)
    # Обработка аргументов
    set(prefix ARG)
    set(multiValues DIR_LESSONS)

    cmake_parse_arguments(${prefix} "" "" ${multiValues} ${ARGN})

    # Проверяем есть ли вообще папки с проектами
    if (NOT ${prefix}_DIR_LESSONS)
        message(AUTHOR_WARNING "Список директорий с проектами не был передан!")
        return()
    endif()

    # Сбрасываем переменные кэша
    set(PROJECTS_PATHS "" CACHE INTERNAL "Доступные пути проектов" FORCE)
    set(PROJECTS_NAMES "" CACHE STRING "Имена доступных проектов" FORCE)

    # Проходимся по всем рабочим папкам
    message(STATUS "Список директорий с проектами: ${${prefix}_DIR_LESSONS}")
    foreach(DIR_NAME IN LISTS ${prefix}_DIR_LESSONS)
        set(CURR_DIR "${CMAKE_SOURCE_DIR}/${DIR_NAME}")
        if (NOT IS_DIRECTORY "${CURR_DIR}")
            message(AUTHOR_WARNING "Директория с проектами не найдена: $CURR_DIR")
            continue()
        endif()

        message(STATUS "Обработка директории: ${CURR_DIR}")
        process_projects_list(CURR_DIR "${CURR_DIR}")
    endforeach()

    # Выводим имена проектов доступных для сборки
    foreach(name IN LISTS PROJECTS_NAMES)
        message(STATUS "Доступен для сборки: ${name}")
    endforeach()
endfunction()