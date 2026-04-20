#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "camera.h"

// Размеры экрана
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef enum {
    MY_KEY_W = 0,
    MY_KEY_A = 1,
    MY_KEY_S = 2,
    MY_KEY_D = 3,
    MY_KEY_SPACE = 4,
    MY_KEY_SHIFT = 5
} MY_KEYS;
const int MY_KEYS_CNT = 6;

typedef struct {
    Camera *camera;
    bool keys[MY_KEYS_CNT]; // WASD + space + shift + esc
    int lastX, lastY;       // Нужно для мыши
    bool firstMouse;
    bool fpsEnable;
} AppContext;

void drawSolidCube(int x, int y, int z) {
    glPushMatrix();

    glTranslatef(x, y, z);

    glBegin(GL_QUADS);
        // Передняя грань (Z = 0.5)
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);

        // Задняя грань (Z = -0.5)
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);

        // Верхняя грань (Y = 0.5)
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);

        // Нижняя грань (Y = -0.5)
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);

        // Правая грань (X = 0.5)
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);

        // Левая грань (X = -0.5)
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();

    glPopMatrix();
}

void render(Camera *camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera->look();

    // Рисуем кучу кубов
    for (int i = -5; i <= 5; ++i) {
        for (int j = -5; j <= 5; ++j) {
            glColor3f((i+5.0)/10.0, (j+5.0)/10.0, 1 - (i+5.0)*(j+5.0)/100.0);
            drawSolidCube(2*i, -2, 2*j);
        }
    }
}

void cursorCallback(GLFWwindow *window, double xpos, double ypos) {
    AppContext *cnt = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));

    if (!cnt->fpsEnable) return;

    if (cnt->firstMouse) {
        cnt->lastX = xpos;
        cnt->lastY = ypos;
        cnt->firstMouse = false;
    }

    float dx = xpos - cnt->lastX;
    float dy = ypos - cnt->lastY;

    cnt->lastX = xpos;
    cnt->lastY = ypos;

    cnt->camera->setViewByMouse(-dx, -dy);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

    AppContext *cnt = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));
    bool isPress = (action == GLFW_PRESS);

    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (isPress) {
                cnt->fpsEnable = !cnt->fpsEnable;
                glfwSetInputMode(window, GLFW_CURSOR, cnt->fpsEnable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                cnt->firstMouse = true;
            }
            break;
        case GLFW_KEY_W:        cnt->keys[MY_KEY_W] = isPress;
            break;
        case GLFW_KEY_A:        cnt->keys[MY_KEY_A] = isPress;
            break;
        case GLFW_KEY_S:        cnt->keys[MY_KEY_S] = isPress;
            break;
        case GLFW_KEY_D:        cnt->keys[MY_KEY_D] = isPress;
            break;
        case GLFW_KEY_SPACE:    cnt->keys[MY_KEY_SPACE] = isPress;
            break;
        case GLFW_KEY_LEFT_SHIFT: cnt->keys[MY_KEY_SHIFT] = isPress;
            break;
        default:
            break;
    }
}

void updateGame(AppContext *cnt, double deltaTime) {
    Camera *camera = cnt->camera;

    if (cnt->keys[MY_KEY_W])        camera->move_camera(deltaTime *  5.0);
    if (cnt->keys[MY_KEY_A])        camera->strafe(deltaTime * -5.0);
    if (cnt->keys[MY_KEY_S])        camera->move_camera(deltaTime * -5.0);
    if (cnt->keys[MY_KEY_D])        camera->strafe(deltaTime *  5.0);
    if (cnt->keys[MY_KEY_SPACE])    camera->upDown(deltaTime *  5.0);
    if (cnt->keys[MY_KEY_SHIFT])    camera->upDown(deltaTime * -5.0);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    g_windowWidth = width;
    g_windowHeight = height;
    sizeOpenGLScreen(width, height);
}

int main(int argc, char *argv[]) {
    GLFWwindow *window = nullptr;

    window = createWindow("FPS - camera", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr) {
        std::cerr << "Create window error" << std::endl;
        return 1;
    }

    init();

    // Создадим контекст приложения
    AppContext *cnt = new AppContext;
    cnt->firstMouse = true;
    cnt->fpsEnable = true;

    cnt->camera = new Camera(0, 0, 0, 0, 0, -1);
    glfwSetWindowUserPointer(window, cnt);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    // Цепляем колбеки
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    double previousTime = glfwGetTime();
    double lag = 0.0;
    const double MS_PER_UPDATE = 1 / 60.0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double elapsed = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsed;
        if (lag >= MS_PER_UPDATE) {
            updateGame(cnt, MS_PER_UPDATE);
            lag -= MS_PER_UPDATE;
        }

        render(cnt->camera);

        glfwSwapBuffers(window);
    }

    delete cnt->camera;
    delete cnt;
    deInit();

    return 0;
}