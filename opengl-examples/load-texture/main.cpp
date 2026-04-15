#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <filesystem>
#include <string>

int textureId;

void loadTexture(GLuint *textureId, const char *textureFile) {
    int width, height, channels;
    unsigned char* data = stbi_load(textureFile, &width, &height, &channels, 0);

    std::cout << width << " " << height << " " <<channels << std::endl;

    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
}

int main(int argc, char* argv[]) {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 800, "Мое GLFW окно", nullptr, nullptr);

    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Делаем контекст текущим
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Ошибка инициализации OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Загружаем текстуру
    glEnable(GL_TEXTURE_2D);

    // Проверка существования файла
    std::string texturePath = argv[0];

    size_t last_slash = texturePath.find_last_of("/\\");

    if (last_slash != std::string::npos) {
        texturePath.erase(last_slash + 1); // Удаляем всё после последнего слеша
    }

    texturePath += "data/triangle.png";
    if (FILE *file = fopen(texturePath.c_str(), "r")) {
        std::cout << "Texture file " << texturePath << " found!" << std::endl;
        fclose(file);
    } else {
        std::cout << "Texture file " << texturePath << " not found!"  << std::endl;
    }

    loadTexture((GLuint*)&textureId, texturePath.c_str());

    std::cout << "texture Id: " << textureId << std::endl;

    glClearColor(0.0, 0.0, 0.0, 1.0);

    float angle = 0.0f;
    double lastTime = glfwGetTime();
    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        angle += 50.0f * deltaTime;

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // Здесь можно добавить код отрисовки
        glPushMatrix();
        glRotatef(angle, 0.0f, 1.0f, 0.0f);

        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBegin(GL_QUADS);
            glTexCoord2d(0.0, 1.0); glVertex3f(-0.87, -0.87, 0.0);
            glTexCoord2d(1.0, 1.0); glVertex3f(0.87, -0.87, 0.0);
            glTexCoord2d(1.0, 0.0); glVertex3f(0.87, 0.87, 0.0);
            glTexCoord2d(0.0, 0.0); glVertex3f(-0.87, 0.87, 0.0);
        glEnd();

        glPopMatrix();
        // Смена буферов
        glfwSwapBuffers(window);

        // Обработка событий
        glfwPollEvents();
    }

    // Очистка ресурсов
    glDeleteTextures(1, (GLuint*)&textureId);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
