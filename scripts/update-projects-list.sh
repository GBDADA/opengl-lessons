#!/bin/bash

#Скрипт для быстрого обновления projectsList в текущем каталоге

# Цвета для терминала
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

AUTO_YES=false
for arg in "$@"; do
    if [ "$arg" = "-y" ]; then
        AUTO_YES=true
        break
    fi
done

function get_answer {
    if [ "$AUTO_YES" = true ]; then
        echo "y (авто)"
        return 0
    fi

    while true; do
        read -n 1 answer
        echo
        case "$answer" in
            [Yy]* ) answer="y"; return 0 ;;
            [Nn]* ) answer="n"; return 1 ;;
            *) echo -ne "${RED}Ошибка ввода, введите [y/n]: ${NC}" ;;
        esac
    done
}

#list_dir=${1:-$(pwd)}
list_dir="."
for arg in "$@"; do
    if [ "$arg" != "-y" ]; then
        list_dir="$arg"
        break
    fi
done
list_file="$list_dir/projectsList.txt"

if [ ! -f "$list_file" ]; then
    echo -ne "${YELLOW}Файл projectsList.txt отсутствует в каталоге $list_dir, добавить? [y/n]: ${NC}"
    if get_answer; then
        touch "$list_file"
    else
        echo -e "${RED}Файл projectsList.txt не был добавлен, завершение работы скрипта.${NC}"
        exit 1
    fi

fi

current_list=$(cat "$list_dir/projectsList.txt")
new_list=""

# Проходимся по старому списку удаляя дупликаты, а также удалённые проекты
while IFS= read -r project; do
    [ -z "$project" ] && continue

    if echo "$new_list" | grep -qFx "$project"; then
        echo -e "${YELLOW}Удалён дупликат:${NC} $project"
        continue
    fi

    if [ ! -d "$list_dir/$project" ]; then
        echo -ne "${RED}Каталог с проектом $project был удалён, удалить из списка? [y/n]: ${NC}"
        if get_answer; then
            continue
        fi
    fi

    # В противном случае добавляем проект в новый список
    if [ -z "$new_list" ]; then new_list="$project"; else new_list+=$'\n'"$project"; fi
done <<< "$current_list"

# Проходимся по директориям в поисках новых проектов
for dir_path in "$list_dir"/*; do
    [[ ! -d "$dir_path" || "$name" == .* ]] && continue

    name=$( basename "$dir_path" )

    if echo "$new_list" | grep -qFx "$name"; then
        # Проект $name уже есть в списке и был обработан ранее
        continue
    fi

    echo -ne "${GREEN}Обнаружен новый проект: $name. Добавить? [y/n]: ${NC}"
    if get_answer; then
        if [ -z "$new_list" ]; then new_list="$name"; else new_list+=$'\n'"$name"; fi
    fi
done

# Записываем результат обратно в файл
echo "$new_list" > "$list_file"
echo -e "${BLUE}Список обновлен.${NC}"