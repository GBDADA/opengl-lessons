#!/bin/bash

#Скрипт для быстрой сборки
[ -z "$ROOT_DIR" ] && { echo "Ошибка: ROOT_DIR не установлена" >&2; exit 1; }
BUILD_DIR="$ROOT_DIR/build"

current_key="NONE"
selected_list=""

debug="FALSE"
config=""

build="FALSE"
clean="FALSE"
launch="FALSE" # Запускает самый новый файл в build/bin
quiet="FALSE"

while [ -n "$1" ]; do
    # Меняем обрабатываемый ключ
    case "$1" in
        -p|--projects) current_key="projects" ;;
        -d|--debug) debug="TRUE"; current_key="NONE" ;;
        -b|--build) build="TRUE"; current_key="build" ;;
        -c|--clean) clean="TRUE"; current_key="NONE" ;;
        -l|--launch) launch="TRUE"; current_key="NONE" ;;
        -q|--quiet) quiet="TRUE"; current_key="NONE" ;;
        --) shift; break ;;
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
                build)
                    arg=$(echo "$1" | tr 'A-Z' 'a-z');
                    if [ "$arg" = "debug" ]; then
                        config="Debug"
                    elif [ "$arg" = "release" ]; then
                        config="Release"
                    else
                        echo "Неизвестный тип сборки" >&2
                        exit 1
                    fi
                    current_key="NONE"
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

[ $# -gt 0 ] && launch="TRUE" # Если есть что передавать, значит нужно запустить

if [ "$quiet" = "TRUE" ]; then
    exec 3>&1
    exec > /dev/null
fi

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

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" \
      -DSELECTED_LESSONS="$selected_list" \
      ${config:+"-DCMAKE_BUILD_TYPE=$config"}

if [ "$build" = "TRUE" ]; then
    cmake --build "$BUILD_DIR" ${config:+"--config $config"}
fi

if [ "$quiet" = "TRUE" ]; then
    exec 1>&3 3>&-
fi

if [ "$launch" = "TRUE" ]; then
    launch_name=$( ls -tl "$BUILD_DIR/bin" | grep -v "^d" | awk 'NR==2 {print $9}' )
    "$BUILD_DIR/bin/$launch_name" "$@"
fi