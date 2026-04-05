#!/bin/bash

#Скрипт для быстрого обновления projectsList в текущем каталоге
source "$(dirname "$0")/common_functions.sh"

AUTO_YES=false
for arg in "$@"; do
    if [ "$arg" = "-y" ]; then
        AUTO_YES=true
        break
    fi
done

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
while IFS= read -u 3 -r project; do
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
done 3<<< "$current_list"

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