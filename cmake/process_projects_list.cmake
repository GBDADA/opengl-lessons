# Данная функция обрабаотывает projectsList.txt в указанной директории, добавляя проекты в переменную PROJECTS_PATHS - проектов доступных для сборки

function(process_projects_list)
    # Обработка аргументов
    set(prefix ARG)
    set(singleValues CURR_DIR)

    cmake_parse_arguments(${prefix} "" ${singleValues} "" ${ARGN})

    # Проверяем, было ли вообще что-то передано
    if ( NOT ${prefix}_CURR_DIR )
        message(AUTHOR_WARNING "Директория с проектом для обработки projectsList.txt не была передана!")
        return()
    endif()

    # Проверяем есть ли projectsList.txt в указанной директории
    set(projects_list_file "${${prefix}_CURR_DIR}/projectsList.txt")
    if ( NOT EXISTS "${projects_list_file}" )
        message(AUTHOR_WARNING "В директории отсутствует файл projectsList.txt: ${${prefix}_CURR_DIR}")
        return()
    endif()
    file(STRINGS "${projects_list_file}" projects_list)

    # Инициализируем временный список для текущей сессии
    set(current_valid_paths "${PROJECTS_PATHS}")
    set(current_valid_names "${PROJECTS_NAMES}")

    foreach(current_project IN LISTS projects_list)
        set(current_project_path "${${prefix}_CURR_DIR}/${current_project}/")

        # Проверка, что есть лишний проект в файле
        if (NOT ((EXISTS "${current_project_path}") AND (IS_DIRECTORY "${current_project_path}")))
            message(AUTHOR_WARNING "Проект из списка не найден на диске: ${current_project_path}")
            continue()
        endif()

        # Проверяем на наличие повторов
        if ( "${current_project}" IN_LIST current_valid_names )
            list(FIND current_valid_names "${current_project}" repeat_index)
            list(GET current_valid_paths "${repeat_index}" repeat_path)
            message(FATAL_ERROR "Обнаружно повторяющееся название проекта: ${current_project}. Расположение повторов:\n"
                "${current_project_path}\n"
                "${repeat_path}\n"
                "Дальнейшая сборка невозможна")
            return()
        endif()

        # Добавляем в список валидных путей и имён
        list(APPEND current_valid_paths "${current_project_path}")
        list(APPEND current_valid_names "${current_project}")

        # Есть новый файл в папке
        if (NOT ("${current_project_path}" IN_LIST PROJECTS_PATHS))
            message(STATUS "Обнаружен новый проект в файле: ${current_project_path}")
        endif()
    endforeach()

    # Обновляем переменную кэша
    set(PROJECTS_PATHS "${current_valid_paths}" CACHE INTERNAL "Доступные пути проектов" FORCE)
    set(PROJECTS_NAMES "${current_valid_names}" CACHE STRING "Имена доступных проектов" FORCE)
endfunction()