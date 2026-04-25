#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>
#include "window.h"
#include "terrain.h"
#include <string>

std::string texturePath;
Terrain *myTerrain;
double a = 1;

void init(GLFWwindow *window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    initializeOpenGL(width, height);

    myTerrain = new Terrain(CVector(0,0,0));
    if (myTerrain->loadFromFile((texturePath + "data/heightMap.png").c_str(), 100, 15)) {
        std::cerr << "terrain load error" << std::endl;
    }
}

void renderScene(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 50, 100,  // Позиция камеры
            0, 0, 0,      // Куда смотрим (в центр террейна)
            0, 1, 0);

    myTerrain->render();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {

}

void framebufferSizeCallback(GLFWwindow *window, int fbWidth, int fbHeight) {
    sizeOpenGLScreen(fbWidth, fbHeight);
}

int main(int argc, char *argv[]) {
    GLFWwindow *window = createMyWindow("terrain-plus", 800, 600);
    if (window == nullptr) {
        std::cerr << "Create my window error" << std::endl;
        return 1;
    }

    // Загрузим террейн
    texturePath = argv[0];
    size_t last_slash = texturePath.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        texturePath.erase(last_slash + 1); // Удаляем всё после последнего слеша
    }

    init(window);
    myTerrain->position.z -= 0;
    myTerrain->position.y -= 0;

    std::cout << myTerrain->position.x << " " << myTerrain->position.y << " " << myTerrain->position.z << std::endl;
    std::cout <<myTerrain->getHeight(0, 0) << std::endl;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        renderScene(window);

        glfwSwapBuffers(window);
    }

    delete myTerrain;
    deInit();
}