#include "../include/main.h"
#include "GLFW/glfw3.h"

GLFWwindow* g_window = nullptr;
int g_windowWidth = SCREEN_WIDTH;
int g_windowHeight = SCREEN_HEIGHT;

GLFWwindow*  createMyWindow(const char* capture, int width, int height) {
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации OpenGL" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_REFRESH_RATE, 0);

    GLFWwindow* window = glfwCreateWindow(width, height, capture, nullptr, nullptr);
    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    g_window = window;
    g_windowWidth = width;
    g_windowHeight = height;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwShowWindow(window);

    return window;
}

void sizeOpenGLScreen(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/height, .5f, 150.0f);


    glMatrixMode(GL_MODELVIEW);
}

int initializeOpenGL(int width, int height) {
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Ошибка инициализации glad/OpenGL" << std::endl;
        return -1;
    }

    glClearColor(0.0, 0.0 ,0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    sizeOpenGLScreen(width, height);

    return 0;
}

void deInit()
{
    if (g_window)
    {
        glfwDestroyWindow(g_window);
    }

    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    g_windowWidth = width;
    g_windowHeight = height;
    sizeOpenGLScreen(width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}






















