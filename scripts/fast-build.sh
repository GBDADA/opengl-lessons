#!/bin/bash

#Скрипт для быстрой сборки
current_key="NONE"
build_list=""
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
                    if [ -z "$build_list" ]; then
                        build_list="$1"
                    else
                        build_list="$build_list;$1"
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
    echo "build_list = $build_list"
fi

BUILD_DIR="$ROOT_DIR/build"

# Удаляем кэш из корневого каталога, если вдруг он там появился
rm -f "$ROOT_DIR/CMakeCache.txt"
rm -rf "$ROOT_DIR/CMakeFiles"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DBUILD_PROJECTS="$build_list"

if [ "$build" = "TRUE" ]; then
    cmake --build "$BUILD_DIR"
fi