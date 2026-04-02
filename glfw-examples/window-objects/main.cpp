#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using std::string;

typedef struct {
    string mode; // windowed fullscreen borderless
    int windowWidth, windowHeight;  // Логические
    int fbWidth, fbHeight;          // Физические
    float scaleX, scaleY;           // Масштаб
    bool floating;                  // always-on-top
} DisplayContext;

GLFWwindow* window;

void key_callback(GLFWwindow* wondow, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    // Размер в логических координатах
    DisplayContext* ctx = reinterpret_cast<DisplayContext*>(glfwGetWindowUserPointer(window));
    ctx->windowWidth = width;
    ctx->windowHeight = height;
}

int main(int argc, char* argv[]) {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    // Создание режима отображения
    ctx.mode = "windowed";
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--mode=") == 0) {
            ctx.mode = arg.substr(7);
        } else if (arg.find("--floating") == 0) {
            ctx.floating = true;
        }
    }

    // Создание окна
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    DisplayContext ctx;
    if (ctx.mode == "windowed") {
        window = glfwCreateWindow(800, 800, "Мое GLFW окно", nullptr, nullptr);
    } else if (ctx.mode == "fullscreen") {
        window = glfwCreateWindow(1920, 1080, "Мое GLFW окно", monitor, nullptr);
    } else if (ctx.mode == "borderless") {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        window = glfwCreateWindow(mode->width, mode->height, "Мое GLFW окно", monitor, nullptr);
    } else {
        std::cout << "Incorrect mode " << ctx.mode << std::endl;
        window = glfwCreateWindow(800, 800, "Мое GLFW окно", nullptr, nullptr);
    }
    glfwSetWindowUserPointer(window, &ctx);

    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        ctx.mode = "windowed";
        glfwTerminate();
        return -1;
    }

    // Делаем контекст текущим
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Очистка ресурсов
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
