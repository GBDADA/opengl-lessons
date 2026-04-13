#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using std::vector;

const int VIRTUAL_WIDTH = 800;
const int VIRTUAL_HEIGHT = 600;
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 30;
const float CELL_WIDTH = static_cast<float>(VIRTUAL_WIDTH) / MAP_WIDTH;
const float CELL_HEIGHT = static_cast<float>(VIRTUAL_HEIGHT) / MAP_HEIGHT;

typedef struct {
    int x, y;
} tail;

typedef struct {
    vector<tail> player;
    int dx, dy;
    int appleX, appleY;
    bool isFinish;
} Game;

void restart(Game *game) {
    *game = {
        .dx = 1,
        .dy = 0,
        .appleX = 15,
        .appleY = 15,
        .isFinish = false,
    };
    game->player.clear();
    game->player.push_back({4, 5});
    game->player.push_back({3, 5});
    game->player.push_back({2, 5});
    game->player.push_back({1, 5});
}

void renderGame(Game *game) {
    int x, y;

    glColor3f(0.7, 0.7, 1.0);
    glBegin(GL_QUADS);
        for (auto element = game->player.begin(), end = game->player.end(); element != end; ++element) {
            x = element->x;
            y = element->y;
            glVertex2f(x * CELL_WIDTH, y * CELL_HEIGHT);
            glVertex2f((x + 1) * CELL_WIDTH, y * CELL_HEIGHT);
            glVertex2f((x + 1) * CELL_WIDTH, (y + 1) * CELL_HEIGHT);
            glVertex2f(x * CELL_WIDTH, (y + 1) * CELL_HEIGHT);
        }
    glEnd();

    glColor3f(1.0, 0.4, 0.5);
    glBegin(GL_QUADS);
        x = game->appleX;
        y = game->appleY;
        glVertex2f(x * CELL_WIDTH, y * CELL_HEIGHT);
        glVertex2f((x + 1) * CELL_WIDTH, y * CELL_HEIGHT);
        glVertex2f((x + 1) * CELL_WIDTH, (y + 1) * CELL_HEIGHT);
        glVertex2f(x * CELL_WIDTH, (y + 1) * CELL_HEIGHT);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        for (int i = 1; i < MAP_WIDTH; ++i) {
            glVertex2f(i * CELL_WIDTH, 0);
            glVertex2f(i * CELL_WIDTH, VIRTUAL_HEIGHT);
        }
        for (int i = 1; i <MAP_HEIGHT; ++i) {
            glVertex2f(0, i * CELL_HEIGHT);
            glVertex2f(VIRTUAL_WIDTH, i * CELL_HEIGHT);
        }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
}

void updateGame(Game *game) {
    if (game->isFinish) return;

    int len = game->player.size();
    for (int i = len - 1; i > 0; --i) {
        game->player[i] = game->player[i - 1];
    }
    game->player[0].x += game->dx;
    game->player[0].y += game->dy;

    if (game->player[0].x < 0 || game->player[0].x >= MAP_WIDTH ||
        game->player[0].y < 0 || game->player[0].y >= MAP_HEIGHT) {
        game->isFinish = true;
        return;
    }

    for (int i = 1; i < len; ++i) {
        if (game->player[0].x == game->player[i].x && game->player[0].y == game->player[i].y) {
            game->isFinish = true;
            return;
        }
    }

    if (game->player[0].x == game->appleX && game->player[0].y == game->appleY) {
        game->player.push_back(game->player[len-1]);

        bool correctPos = true;
        do {
            correctPos = true;
            game->appleX = rand() % MAP_WIDTH;
            game->appleY = rand() % MAP_HEIGHT;
            for (auto element = game->player.begin(), end = game->player.end(); element != end; ++element) {
                if (element->x == game->appleX && element->y == game->appleY) {
                    correctPos = false;
                    break;
                }
            }
        } while (!correctPos);
    }
}

void viewportResize(int fbWidth, int fbHeight) {
    float xscale = static_cast<float>(fbWidth) / VIRTUAL_WIDTH;
    float yscale = static_cast<float>(fbHeight) / VIRTUAL_HEIGHT;

    float scale = (xscale > yscale) ? yscale : xscale;

    float viewportWidth = VIRTUAL_WIDTH * scale;
    float viewportHeight = VIRTUAL_HEIGHT * scale;;
    float xoffset = (fbWidth - viewportWidth) / 2.0;
    float yoffset = (fbHeight - viewportHeight) / 2.0;

    glViewport(xoffset, yoffset, viewportWidth, viewportHeight);
    glScissor(xoffset, yoffset, viewportWidth, viewportHeight);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void framebuffer_size_callback(GLFWwindow *window, int fbWidth, int fbHeight) {
    viewportResize(fbWidth, fbHeight);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));

    switch (key) {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_W: case GLFW_KEY_UP:
            if (game->dy == 0) {game->dx = 0; game->dy = -1;}
            break;
        case GLFW_KEY_A: case GLFW_KEY_LEFT:
            if (game->dx == 0) {game->dx = -1; game->dy = 0;}
            break;
        case GLFW_KEY_S: case GLFW_KEY_DOWN:
            if (game->dy == 0) {game->dx = 0; game->dy = 1;}
            break;
        case GLFW_KEY_D: case GLFW_KEY_RIGHT:
            if (game->dx == 0) {game->dx = 1; game->dy = 0;}
            break;
        case GLFW_KEY_SPACE:
            if (game->isFinish) {
                restart(game);
            }
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization error" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 0);
    glfwWindowHint(GLFW_STENCIL_BITS, 0);

    GLFWwindow *window = glfwCreateWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Snake-Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window create error" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    Game *game = new Game();
    srand(time(nullptr));
    restart(game);
    glfwSetWindowUserPointer(window, game);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "OpenGL initialization error" << std::endl;
        glfwTerminate();
        return 1;
    }

    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.4f, 0.7f, 0.5f, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    viewportResize(width, height);

    double previousTime = glfwGetTime();
    double lag = 0.0;
    const double MS_PER_UPDATE = 1.0 / 10.0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double elapsed = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsed;

        while (lag >= MS_PER_UPDATE) {
            updateGame(game);
            lag -= MS_PER_UPDATE;

            std::string title;
            if (!game->isFinish)
                title = "Snake - Score: " + std::to_string(game->player.size());
            else title = "Game over! Press space to restart";
            glfwSetWindowTitle(window, title.c_str());
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        renderGame(game);

        glfwSwapBuffers(window);
    }

    delete game;
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
