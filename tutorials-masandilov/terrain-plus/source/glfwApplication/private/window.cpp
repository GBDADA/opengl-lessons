#include "window.h"
#include "GLFW/glfw3.h"
#include <iostream>

GLFWwindow *g_window;

GLFWwindow* createMyWindow(const char *capture, int width, int height) {
    if (!glfwInit()) {
        std::cerr << "GLFW init error" << std::endl;
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

    g_window = glfwCreateWindow(width, height, capture, nullptr, nullptr);
    if (!g_window) {
        std::cerr << "Window create error" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(g_window);

    glfwSetKeyCallback(g_window, keyCallback);
    glfwSetFramebufferSizeCallback(g_window, framebufferSizeCallback);
    glfwShowWindow(g_window);

    return g_window;
};

int initializeOpenGL(int width, int height) {
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Load GL error" << std::endl;
        return 1;
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    sizeOpenGLScreen(width, height);

    return 0;
}

void sizeOpenGLScreen(int width, int height) {
    if (height == 0) height = 1;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/height, .5f, 150.0f);

    glMatrixMode(GL_MODELVIEW);
}

void deInit() {
    if (g_window) glfwDestroyWindow(g_window);
    glfwTerminate();
}