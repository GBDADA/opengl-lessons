# Данная функция создаёт цели для каждого проекта в переданной директории и подключает main.cpp (это временно)
# Также если нужно собрать определённые файлы, то она их отсекает
# Пока что оставлю код, он не используется, но как пример оставлю временно, в одном из следующих коммитов удалю

function(add_more_projects)
    set(prefix "ARG")
    set(singleValues DIRECTORY)
    set(multiValues "LIBRARIES;BUILD")

    cmake_parse_arguments(
        ${prefix}
        ""
        "${singleValues}"
        "${multiValues}"
        ${ARGN}
    )

    # Ищем имена каталогов с проектами
    file(GLOB SOURCE_DIRECTORIES CONFIGURE_DEPENDS "${${prefix}_DIRECTORY}/*")

    # Делаем вспомогательную переменную поскольку иначе видители не будет работать IN_LIST
    set(BUILD_LIST "${${prefix}_BUILD}")

    foreach(CURRENT_DIR IN LISTS SOURCE_DIRECTORIES)
        # Пропускаем всё что не является директорией и если нету main.cpp
        if ((NOT (IS_DIRECTORY ${CURRENT_DIR})) OR (NOT (EXISTS "${CURRENT_DIR}/main.cpp")))
            continue()
        endif()

        # Имя папки с проектом, далее будем использовать в качестве названия цели
        get_filename_component(FILE_NAME ${CURRENT_DIR} NAME)

        # Если данного проекта нету в требуемых, то также пропускаем
        if ((BUILD_LIST) AND (NOT (${FILE_NAME} IN_LIST BUILD_LIST)))
            continue()
        endif()

        add_executable(${FILE_NAME} "${CURRENT_DIR}/main.cpp")
        target_link_libraries(${FILE_NAME} ${${prefix}_LIBRARIES})

        # Меняем место сборки
        set_target_properties(${FILE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
    endforeach()
endfunction()