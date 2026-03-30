#!/bin/bash

#Скрипт для быстрой сборки
current_key="NONE"
selected_list=""
debug="FALSE"
build="FALSE"

while [ -n "$1" ]; do
    # Меняем обрабатываемый ключ
    case "$1" in
        -projects) current_key=projects ;;
        -debug) debug="TRUE"; current_key="NONE" ;;
        -build) build="TRUE"; current_key="NONE" ;;
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

if [ "$debug" = "TRUE" ]; then
    echo "selected_list = $selected_list"
fi

BUILD_DIR="$ROOT_DIR/build"

# Удаляем кэш из корневого каталога, если вдруг он там появился
rm -f "$ROOT_DIR/CMakeCache.txt"
rm -rf "$ROOT_DIR/CMakeFiles"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DSELECTED_LESSONS="$selected_list"

if [ "$build" = "TRUE" ]; then
    cmake --build "$BUILD_DIR"
fi