#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using std::string;

typedef enum {
    WINDOWED,
    FULLSCREEN_EXCLUSIVE,
    FULLSCREEN_BORDERLESS
} DisplayMode;

typedef struct {
    DisplayMode mode; // windowed fullscreen borderless
    int positionX, positionY;       // Положение окна (в логических координатах)
    int windowWidth, windowHeight;  // Логические
    int fbWidth, fbHeight;          // Физические
    float scaleX, scaleY;           // Масштаб

    bool floating;                  // always-on-top
    bool resizable;
    bool decorated;
} DisplayContext;

GLFWwindow* window;

void setDisplayMode(GLFWwindow* window, DisplayMode disMode) {
    DisplayContext* ctx = reinterpret_cast<DisplayContext*>(glfwGetWindowUserPointer(window));
    ctx->mode = disMode;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    switch (disMode) {
        case WINDOWED:
            glfwSetWindowAttrib(window, GLFW_AUTO_ICONIFY, GLFW_TRUE);
            glfwSetWindowMonitor(window, nullptr, ctx->positionX, ctx->positionY, ctx->windowWidth, ctx->windowHeight, GLFW_DONT_CARE);
            glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
            glfwSetWindowAspectRatio(window, 16, 9);

            glfwSetWindowAttrib(window, GLFW_DECORATED, ctx->decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(window, GLFW_FLOATING, ctx->floating ? GLFW_TRUE : GLFW_FALSE);
            break;
        case FULLSCREEN_EXCLUSIVE:
            glfwSetWindowAttrib(window, GLFW_AUTO_ICONIFY, GLFW_FALSE);
            glfwSetWindowMonitor(window, monitor, 0, 0, 1920, 1080, GLFW_DONT_CARE);
            break;
        case FULLSCREEN_BORDERLESS:
            glfwSetWindowAttrib(window, GLFW_AUTO_ICONIFY, GLFW_FALSE);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            break;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    DisplayContext* ctx = reinterpret_cast<DisplayContext*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        glfwRestoreWindow(window);
    } else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        glfwIconifyWindow(window);
    } else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        glfwMaximizeWindow(window);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        if (ctx->mode == WINDOWED) setDisplayMode(window, FULLSCREEN_EXCLUSIVE);
        else setDisplayMode(window, WINDOWED);
    } else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        if (ctx->mode == WINDOWED) setDisplayMode(window, FULLSCREEN_BORDERLESS);
        else setDisplayMode(window, WINDOWED);
    }
}

void window_size_callback(GLFWwindow* window, int logicalWidth, int logicalHeight) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    int windowX, windowY;
    glfwGetWindowPos(window, &windowX, &windowY);     // Где окно в виртуальном пространстве

    DisplayContext* ctx = reinterpret_cast<DisplayContext*>(glfwGetWindowUserPointer(window));
    if (ctx->mode == WINDOWED) {
        ctx->windowWidth = logicalWidth;
        ctx->windowHeight = logicalHeight;
        ctx->fbWidth = fbWidth;
        ctx->fbHeight = fbHeight;
        ctx->scaleX = (float)fbWidth / logicalWidth;
        ctx->scaleY = (float)fbHeight / logicalHeight;
        ctx->positionX = windowX;
        ctx->positionY = windowY;
    }

    glViewport(0, 0, fbWidth, fbHeight);
}

int main(int argc, char* argv[]) {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    // Создание режима отображения
    DisplayContext ctx;
    ctx.mode = WINDOWED;
    ctx.resizable = true;
    ctx.floating = false;
    ctx.decorated = true;
    ctx.positionX = ctx.positionY = 100;
    ctx.windowWidth = 1280; ctx.windowHeight = 720;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--mode=") == 0) {
            //ctx.mode = arg.substr(7);
            std::string str_mode = arg.substr(7);
            if (str_mode == "windowed") ctx.mode = WINDOWED;
            else if (str_mode == "fullscreen") ctx.mode = FULLSCREEN_EXCLUSIVE;
            else if (str_mode == "borderless") ctx.mode = FULLSCREEN_BORDERLESS;
            else {
                std::cout << "Incorrect mode " << str_mode << ". Use windowed mode." << std::endl;
                ctx.mode = WINDOWED;
            }

        } else if (arg.find("--floating=") == 0) {
            ctx.floating = (arg.substr(11) == "TRUE") ? true : false; // По умолчанию и в случае ошибки false
        } else if (arg.find("--resizable=") == 0) {
            ctx.resizable = (arg.substr(12) == "FALSE") ? false : true; // По умолчанию и в случае ошибки true
        } else if (arg.find("--decorated=") == 0) {
            ctx.decorated = (arg.substr(12) == "FALSE") ? false : true; // По умолчанию и в случае ошибки true
        }
    }

    // Создание окна
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_FLOATING, (ctx.floating) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, (ctx.resizable) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, (ctx.decorated) ? GLFW_TRUE : GLFW_FALSE);

    // Запрашиваем OpenGL 3.3 (или любую другую версию)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Для macOS обязательно

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Для macOS
    #endif

    switch (ctx.mode) {
        case WINDOWED:
            window = glfwCreateWindow(1280, 720, "Мое GLFW окно", nullptr, nullptr);
            break;
        case FULLSCREEN_EXCLUSIVE:
            window = glfwCreateWindow(1920, 1080, "Мое GLFW окно", monitor, nullptr);
            break;
        case FULLSCREEN_BORDERLESS:
            window = glfwCreateWindow(mode->width, mode->height, "Мое GLFW окно", monitor, nullptr);
            break;
        default:
            std::cout << "Incorrect mode. Use windowed mode." << std::endl;
            ctx.mode = WINDOWED;
            window = glfwCreateWindow(1280, 720, "Мое GLFW окно", nullptr, nullptr);
            break;
    }
    glfwSetWindowUserPointer(window, &ctx);

    std::cout << "Window creation hints set:" << std::endl;
    std::cout << "- Version: 3.3 Core Profile (hard constraint)" << std::endl;
    std::cout << "- Resizable: " << (ctx.resizable ? "true" : "false") << std::endl;
    std::cout << "- Decorated: " << (ctx.decorated ? "true" : "false") << std::endl;
    std::cout << "- Floating: " << (ctx.floating ? "true" : "false") << std::endl;

    std::cout << "Creating window in mode: "
    << (ctx.mode == FULLSCREEN_BORDERLESS ? "borderless" : ctx.mode == FULLSCREEN_EXCLUSIVE ? "fullscreen" : "windowed" ) << std::endl;

    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Сразу вызываем его вручную, чтобы заполнить структуру ctx
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    ctx.windowWidth = w;
    ctx.windowHeight = h;
    ctx.fbWidth = fbWidth;
    ctx.fbHeight = fbHeight;
    ctx.scaleX = (float)fbWidth / ctx.windowWidth;
    ctx.scaleY = (float)fbHeight / ctx.windowHeight;
    if (ctx.mode != WINDOWED) {
        setDisplayMode(window, ctx.mode);
    } else {
        glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwSetWindowAspectRatio(window, 16, 9);
    }

    // Делаем контекст текущим
    glfwMakeContextCurrent(window);

    // Загружаем OpenGL функции
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Ошибка инициализации OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    std::cout << "Window created successfully" << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Window size: " << ctx.windowWidth << "x" << ctx.windowHeight << std::endl;
    std::cout << "Framebuffer size: " << ctx.fbWidth << "x" << ctx.fbHeight << std::endl;
    std::cout << "Attributes: " <<
        "decorated=" << glfwGetWindowAttrib(window, GLFW_DECORATED) <<
        ", resizable=" << glfwGetWindowAttrib(window, GLFW_RESIZABLE) <<
        ", fullscreen=" << ((glfwGetWindowMonitor(window) != nullptr) ? 1 : 0) <<
        ", maximized=" << glfwGetWindowAttrib(window, GLFW_MAXIMIZED) <<
        ", iconified=" << glfwGetWindowAttrib(window, GLFW_ICONIFIED);

    std::cout << "Available controls:\n"
    "F - toggle fullscreen\n"
    "B - toggle borderless fullscreen\n"
    "I - iconify\n"
    "R - restore\n"
    "ESC - exit\n\n"
    "Waiting for input..." << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Очистка ресурсов
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
