function(build_selected)

    # Используем переменную из кэша для проверки
    if(NOT SELECTED_LESSONS)
        message(STATUS "Проекты для сборки не выбраны. Используйте -DSELECTED_LESSONS=\"name1;name2\"")
        return()
    endif()

    foreach(lesson IN LISTS SELECTED_LESSONS)
        list(FIND PROJECTS_NAMES "${lesson}" index)

        if(index EQUAL -1)
            message(FATAL_ERROR "[ERROR]: Урок '${lesson}' отсутствует в списке доступных проектов!")
        endif()

        list(GET PROJECTS_PATHS "${index}" path)
        message(STATUS "Включение проекта в сборку: ${lesson} (${path})")

        # Умное добавление проекта
        add_selected("${lesson}" "${path}")
    endforeach()

endfunction()