# Добавляет проекты из projectsList.txt в переменную PROJECTS_PATHS - проектов доступных для сборки

file(STRINGS "projectsList.txt" projects_list)

# Инициализируем временный список для текущей сессии
set(current_valid_paths "")
set(current_valid_names "")

foreach(current_project IN LISTS projects_list)
    set(current_project_path "${CMAKE_CURRENT_SOURCE_DIR}/${current_project}/")

    # Проверка, что есть лишний проект в файле
    if (NOT ((EXISTS "${current_project_path}") AND (IS_DIRECTORY "${current_project_path}")))
        message(AUTHOR_WARNING "Проект из списка не найден на диске: ${current_project_path}")
        continue()
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

# Выводим имена проектов доступных для сборки
foreach(name IN LISTS PROJECTS_NAMES)
    message(STATUS "Доступен для сборки: ${name}")
endforeach()