#!/bin/bash

# Добавляет новый проект
source "$(dirname "$0")/common_functions.sh"

name=""
index=""
copy_of=""

while [ -n "$1" ]; do
    case "$1" in
        -n|--name) name="$2"; shift;;
        -i|--index) index="$2"; shift;;
        -c|--clone) copy_of="$2"; shift;;
        *) ;;
    esac
    shift
done

if [ -z "$name" ]; then
    name="lesson"
    echo -ne "${YELLOW}Имя не было выбрано, использовать имя по умолчанию: $name? [y/n]: ${NC}"
    if ! get_answer; then
        echo -e "${RED}Название не было выбрано, завершение работы скрипта.${NC}" >&2
        exit 1
    fi
fi

if [[ -z "$index" && -f "$name/main.cpp" ]]; then
    echo -e "${YELLOW}Проект с названием $name существует.${NC}"
    max_index=$(ls | grep -oP "^${name}\K\d+" | sort -rn | head -n 1)
    index=$(( ${max_index:-0} + 1 ))

    echo -ne "${YELLOW}Индекс не был выбран, использовать автовыбор: $index? [y/n]: ${NC}"
    if ! get_answer; then
        echo -e "${RED}Индекс не был выбран, завершение работы скрипта.${NC}" >&2
        exit 1
    fi
fi

if [[ (-n "$copy_of") && (! -d "$copy_of") ]]; then
    echo -ne "${YELLOW}Проект $copy_of не существует, использовать проект по умолчанию? [y/n]: ${NC}"
    if ! get_answer; then
        echo -e "${RED}Копирование $copy_of невозможно, завершение работы скрипта.${NC}" >&2
        exit 1
    fi
fi

path="$name$index"
full_name="$path/main.cpp"

if [ -f "$full_name" ]; then
    echo -e "${YELLOW}Файл $full_name уже существует, завершение работы скрипта!${NC}" >&2
    exit 1
fi

mkdir -p "$path"

if [ -n "$copy_of" ]; then
    echo -e "${GREEN}Копирование проекта $copy_of в ${path}.${NC}"
    cp -r "$copy_of/." "$path"
else

touch "$full_name"
echo -e "${GREEN}Создание проекта по умолчанию ${full_name}.${NC}"

cat <<EOF > "$full_name"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 800, "Мое GLFW окно", nullptr, nullptr);

    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Делаем контекст текущим
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Ошибка инициализации OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);

    float angle = 0.0f;
    double lastTime = glfwGetTime();
    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        angle += 50.0f * deltaTime;

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // Здесь можно добавить код отрисовки
        glPushMatrix();
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        glBegin(GL_TRIANGLES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(0.0, 1.0, 0.0);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0.87, -0.5, 0.0);

            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(-0.87, -0.5, 0.0);
        glEnd();

        glPopMatrix();
        // Смена буферов
        glfwSwapBuffers(window);

        // Обработка событий
        glfwPollEvents();
    }

    // Очистка ресурсов
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
EOF

fi