#!/bin/bash

#Скрипт для быстрой сборки
BUILD_DIR="$ROOT_DIR/build"

current_key="NONE"
selected_list=""
debug="FALSE"
build="FALSE"
clean="FALSE"

while [ -n "$1" ]; do
    # Меняем обрабатываемый ключ
    case "$1" in
        -p|--projects) current_key=projects ;;
        -d|--debug) debug="TRUE"; current_key="NONE" ;;
        -b|--build) build="TRUE"; current_key="NONE" ;;
        -c|--clean) clean="TRUE"; current_key="NONE" ;;
        # Передаём аргумент в один из ключей
        *)
            case "$current_key" in
                projects)
                    if [ -z "$selected_list" ]; then
                        selected_list="$1"
                    else
                        selected_list="$selected_list;$1"
                    fi
                    ;;
                *)
                    echo "Некорректный ввод" >&2
                    exit 1
                    ;;
            esac
            ;;
    esac
    shift
done

# Удаляем папку build если нужно
if [ "$clean" = "TRUE" ]; then
    [ "$debug" = "TRUE" ] && echo "Очистка папки сборки"
    rm -rf "$BUILD_DIR"
fi

# Создаем папку build, если её нет (после rm -rf она исчезнет)
mkdir -p "$BUILD_DIR"

if [ "$debug" = "TRUE" ]; then
    echo "selected_list = $selected_list"
fi

# Удаляем кэш из корневого каталога, если вдруг он там появился
rm -f "$ROOT_DIR/CMakeCache.txt"
rm -rf "$ROOT_DIR/CMakeFiles"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DSELECTED_LESSONS="$selected_list"

if [ "$build" = "TRUE" ]; then
    cmake --build "$BUILD_DIR"
fi