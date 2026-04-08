#include <../include/main.h>

void init(GLFWwindow* window) {
    g_window = window;

    glfwGetFramebufferSize(window, &g_windowWidth, &g_windowHeight);

    initializeOpenGL(g_windowWidth, g_windowHeight);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glfwSwapBuffers(g_window);
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    window = createMyWindow("OpenGL + GLFW", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr)
        return -1;

    init(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        renderScene();
    }

    deInit();

    return 0;
}