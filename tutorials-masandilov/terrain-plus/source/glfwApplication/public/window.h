#ifndef WINDOW_H
#define WINDOW_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

extern GLFWwindow *g_window;

GLFWwindow* createMyWindow(const char *capture, int width, int height);
void sizeOpenGLScreen(int width, int height);
int initializeOpenGL(int width, int height);

void init(GLFWwindow *window);
void deInit();
void renderScene(GLFWwindow *window);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebufferSizeCallback(GLFWwindow *window, int bfWidth, int bfHeight);

#endif