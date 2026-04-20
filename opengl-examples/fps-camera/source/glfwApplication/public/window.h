#ifndef WINDOW_H
#define WINDOW_H

#include <glad/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

extern GLFWwindow *g_window;
extern int g_windowWidth;
extern int g_windowHeight;

GLFWwindow* createWindow(const char* caption, int width, int height);

void init();

void deInit();

void sizeOpenGLScreen(int width, int height);

#endif