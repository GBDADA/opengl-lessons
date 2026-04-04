#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using std::string;

// Размеры render-resolution
static const int VIRTUAL_WIDTH = 1920, VIRTUAL_HEIGHT = 1080;
// Максимальное кол-во UI элементов
static const int MAX_UI_ELEMENTS = 16;

// По сути это не нужно, поскольку я использую letterbox, но почему нет, потом пригодиться
typedef enum {
    ANCHOR_LEFT, ANCHOR_RIGHT, ANCHOR_TOP, ANCHOR_BOTTOM, ANCHOR_CENTER_H, ANCHOR_CENTER_V
} Anchor;

typedef struct {
    int x, y, width, height; // Тут всё в виртуальных координатах (координатах viewport)
    int leftMargin, rightMargin;
    int topMargin, bottomMargin;
    Anchor hAnchor, vAnchor;
    float r, g, b;
} UIElement;

typedef struct {
    float scale;
    int offsetX, offsetY;               // Смещение в логических координатах
    int virtualWidth, virtualHeight;
} Viewport;

typedef struct {
    UIElement elements[MAX_UI_ELEMENTS];
    int count;
} UIManager;

typedef struct {
    float clearColor[4];
    int lastWindowWidth, lastWindowHeight;
    int windowX, windowY;
    bool fullscreen;
    Viewport* viewport;
    UIManager* uiManager;
} AppContext;

void updateUIElement(UIElement* elem, Viewport *vp) {
    switch (elem->hAnchor) {
        case ANCHOR_RIGHT:
            elem->x = vp->virtualWidth - elem->width - elem->rightMargin;
            break;
        case ANCHOR_CENTER_H:
            elem->x = (vp->virtualWidth - elem->width) / 2 + elem->leftMargin - elem->rightMargin;
            break;
        case ANCHOR_LEFT:
        default:
            elem->x = elem->leftMargin;
            break;
    }

    switch (elem->vAnchor) {
        case ANCHOR_BOTTOM:
            elem->y = vp->virtualHeight - elem->height - elem->bottomMargin;
            break;
        case ANCHOR_CENTER_V:
            elem->y = (vp->virtualHeight - elem->height) / 2 + elem->topMargin - elem->bottomMargin;
            break;
        case ANCHOR_TOP:
        default:
            elem->y = elem->topMargin;
            break;
    }
}

void updateUI(UIManager* ui, Viewport *vp) {
    for (int i = 0; i < ui->count; ++i) {
        updateUIElement(&ui->elements[i], vp);
    }
}

void setupUI(UIManager* ui) {
    // Верхняя серая панель
    ui->elements[ui->count++] = (UIElement){
        .width = 0, .height = 60,
        .leftMargin = 0, .rightMargin = 0,
        .topMargin = 0, .bottomMargin = 0,
        .hAnchor = ANCHOR_CENTER_H,
        .vAnchor = ANCHOR_TOP,
        .r = 0.35, .g = 0.35, .b = 0.35,
    };
    // Боковая синяя панель
    ui->elements[ui->count++] = (UIElement){
        .width = 250, .height = -60,
        .leftMargin = 0, .rightMargin = 0,
        .topMargin = 60, .bottomMargin = 0,
        .hAnchor = ANCHOR_LEFT,
        .vAnchor = ANCHOR_TOP,
        .r = 0.12, .g = 0.16, .b = 0.44,
    };
    // Зелёный контекст
    ui->elements[ui->count++] = (UIElement){
        .width = -250, .height = -60,
        .leftMargin = 250, .rightMargin = 0,
        .topMargin = 60, .bottomMargin = 0,
        .hAnchor = ANCHOR_LEFT,
        .vAnchor = ANCHOR_TOP,
        .r = 0.55, .g = 0.65, .b = 0.54,
    };
    // Красная кнопка
    ui->elements[ui->count++] = (UIElement){
        .width = 200, .height = 60,
        .leftMargin = 0, .rightMargin = 0,
        .topMargin = 0, .bottomMargin = 0,
        .hAnchor = ANCHOR_CENTER_H,
        .vAnchor = ANCHOR_CENTER_V,
        .r = 0.9, .g = 0.3, .b = 0.25,
    };
}

void renderUI(UIManager* ui, Viewport *vp) {
    glBegin(GL_QUADS);
    for (int i = 0; i < ui->count; ++i) {
        glColor3f(ui->elements[i].r, ui->elements[i].g, ui->elements[i].b);
        float x, y, width, height;
        x = ui->elements[i].x; y = ui->elements[i].y;
        width = ((ui->elements[i].width % vp->virtualWidth) + vp->virtualWidth) % vp->virtualWidth;
        height = ((ui->elements[i].height % vp->virtualHeight) + vp->virtualHeight) % vp->virtualHeight;

        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    }
    glEnd();
}

void clickUI(UIManager* ui, Viewport *vp, double mouseX, double mouseY, int *id) {
    *id = -1;

    for (int i = 0; i < ui->count; ++i) {
        float x, y, width, height;
        x = ui->elements[i].x; y = ui->elements[i].y;
        width = ((ui->elements[i].width % vp->virtualWidth) + vp->virtualWidth) % vp->virtualWidth;
        height = ((ui->elements[i].height % vp->virtualHeight) + vp->virtualHeight) % vp->virtualHeight;

        if (mouseX >= x && mouseX <= (x + width) && mouseY >= y && mouseY <= (y + height)) {
            *id = i;
        }
    }
}

void updateViewport(GLFWwindow *window, Viewport *vp, AppContext* ctx) {
    if (ctx->lastWindowWidth == 0 || ctx->lastWindowHeight == 0) return;

    float scaleX = (float)ctx->lastWindowWidth / vp->virtualWidth;
    float scaleY = (float)ctx->lastWindowHeight / vp->virtualHeight;
    vp->scale = (scaleX > scaleY) ? scaleY : scaleX;

    vp->offsetX = (ctx->lastWindowWidth - vp->virtualWidth * vp->scale) / 2;
    vp->offsetY = (ctx->lastWindowHeight - vp->virtualHeight * vp->scale) / 2;

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    float fbScaleX = (float)fbWidth / ctx->lastWindowWidth;
    float fbScaleY = (float)fbHeight / ctx->lastWindowHeight;

    int fbOffsetX = vp->offsetX * fbScaleX;
    int fbOffsetY = vp->offsetY * fbScaleY;
    int fbWidthScaled = vp->virtualWidth * (vp->scale * fbScaleX);
    int fbHeightScaled = vp->virtualHeight * (vp->scale * fbScaleY);

    glViewport(fbOffsetX, fbOffsetY, fbWidthScaled, fbHeightScaled);
    glScissor(fbOffsetX, fbOffsetY, fbWidthScaled, fbHeightScaled);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, vp->virtualWidth, vp->virtualHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseToVirtual(Viewport *vp,
                    double mouseX, double mouseY, double *virtX, double *virtY) {
    *virtX = (mouseX - vp->offsetX)/vp->scale;
    *virtY = (mouseY - vp->offsetY)/vp->scale;

    if (*virtX < 0) *virtX = 0;
    if (*virtY < 0) *virtY = 0;
    if (*virtX > vp->virtualWidth) *virtX = vp->virtualWidth;
    if (*virtY > vp->virtualHeight) *virtY = vp->virtualHeight;
}

int currentResolution = 0;
int resolutionList[6] = { 1920, 1080, 1280, 720, 2560, 1440};
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        AppContext *ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        if (!ctx->fullscreen) {
            glfwGetWindowPos(window, &ctx->windowX, &ctx->windowY);
            glfwGetWindowSize(window, &ctx->lastWindowWidth, &ctx->lastWindowHeight);

            glfwSetWindowMonitor(window, monitor, 0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, GLFW_DONT_CARE);
        } else {
            glfwSetWindowMonitor(window, nullptr, ctx->windowX, ctx->windowY, ctx->lastWindowWidth, ctx->lastWindowHeight, GLFW_DONT_CARE);
        }

        ctx->fullscreen = !ctx->fullscreen;
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        AppContext *ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));

        currentResolution = (currentResolution + 2) % 6;
        glfwSetWindowMonitor(window, nullptr, ctx->windowX, ctx->windowY, resolutionList[currentResolution], resolutionList[currentResolution + 1], GLFW_DONT_CARE);
    }
}

void window_size_callback(GLFWwindow* window, int winWidth, int winHeight) {
    AppContext *ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));

    ctx->lastWindowWidth = winWidth;
    ctx->lastWindowHeight = winHeight;
}

void framebuffer_size_callback(GLFWwindow* window, int fbWidth, int fbHeight) {
    //glViewport(0, 0, fbWidth, fbHeight);
    AppContext *ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));

    glfwGetWindowSize(window, &ctx->lastWindowWidth, &ctx->lastWindowHeight);
    updateViewport(window, ctx->viewport, ctx);
}

void window_close_callback(GLFWwindow* window) {
    std::cout << "window close" << std::endl;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    AppContext *ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));

    double mouseX, mouseY, virtX, virtY;
    int id;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseToVirtual(ctx->viewport, mouseX, mouseY, &virtX, &virtY);

        clickUI(ctx->uiManager, ctx->viewport, virtX, virtY, &id);
        // Нажали на красную кнопку
        if (id == 3) {
            std::cout << "Play clicked!" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    AppContext *ctx = new AppContext();
    *ctx = {
        .clearColor = {0.2, 0.3, 0.4, 1.0},
        .lastWindowWidth = VIRTUAL_WIDTH, .lastWindowHeight = VIRTUAL_HEIGHT,
        .windowX = 100, .windowY = 100,
        .fullscreen = false,
        .viewport = new Viewport,
        .uiManager = new UIManager,
    };
    *(ctx->viewport) = {0};
    ctx->viewport->virtualWidth = VIRTUAL_WIDTH;
    ctx->viewport->virtualHeight = VIRTUAL_HEIGHT;

    *(ctx->uiManager) = {0};
    setupUI(ctx->uiManager);
    updateUI(ctx->uiManager, ctx->viewport);

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg.find("--width=") == 0) {
            ctx->lastWindowWidth = std::stoi(arg.substr(8));
        } else if (arg.find("--height=") == 0) {
            ctx->lastWindowHeight = std::stoi(arg.substr(9));
        } else if (arg.find("--fullscreen") == 0) {
            ctx->fullscreen = true;
        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 0);         // Для 2д графики это не нужно
    glfwWindowHint(GLFW_STENCIL_BITS, 0);

    // Создание окна
    GLFWwindow* window;
    if (!ctx->fullscreen)
        window = glfwCreateWindow(ctx->lastWindowWidth, ctx->lastWindowHeight, "Мое GLFW окно", nullptr, nullptr);
    else
        window = glfwCreateWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Мое GLFW окно", glfwGetPrimaryMonitor(), nullptr);

    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        delete ctx->uiManager;
        delete ctx->viewport;
        delete ctx;
        glfwTerminate();
        return -1;
    }

    string title = "Resolution " + std::to_string(VIRTUAL_WIDTH) + "x" + std::to_string(VIRTUAL_HEIGHT);
    glfwSetWindowTitle(window, title.c_str());

    glfwSetWindowUserPointer(window, ctx);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Делаем контекст текущим
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Ошибка инициализации OpenGL" << std::endl;
        delete ctx->uiManager;
        delete ctx->viewport;
        delete ctx;
        glfwTerminate();
        return -1;
    }
    glEnable(GL_SCISSOR_TEST);

    updateViewport(window, ctx->viewport, ctx);

    glClearColor(ctx->clearColor[0], ctx->clearColor[1], ctx->clearColor[2], ctx->clearColor[3]);

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        // Обработка событий
        glfwPollEvents();

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        renderUI(ctx->uiManager, ctx->viewport);

        // Смена буферов
        glfwSwapBuffers(window);
    }

    // Очистка ресурсов
    delete ctx->uiManager;
    delete ctx->viewport;
    delete ctx;
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
