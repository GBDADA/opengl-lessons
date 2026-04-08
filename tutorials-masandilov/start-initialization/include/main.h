#ifndef MAIN_H
#define MAIN_H

#include <glad/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// Размеры экрана
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Глобальные переменные (доступны из других файлов)
extern GLFWwindow* g_window;
extern int g_windowWidth;
extern int g_windowHeight;

struct CVertex3{
    float x,y,z;
};

int main(int argc, char* argv[]);

GLFWwindow* createMyWindow(const char* capture, int width, int height);

void sizeOpenGLScreen(int width, int height);

int  initializeOpenGL(int width, int height);

void init(GLFWwindow* window);

void renderScene();

void deInit();

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif