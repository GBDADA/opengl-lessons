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
