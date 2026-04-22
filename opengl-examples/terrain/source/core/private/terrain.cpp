#include "terrain.h"
#include "glad/gl.h"

#include <iostream>
#include <stb_image.h>

void Terrain::draw() {
    if (!isCreated) return;

    float TexSize = 1.0 / (heightMapSize - 1);
    float step = static_cast<float>(size) / heightMapSize;
    float stepHeight = maxHeight / 255.0;

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, textureId);
    for (int i = 0; i < heightMapSize-1; ++i) {
        glBegin(GL_TRIANGLE_STRIP);

        for (int j = 0; j < heightMapSize; ++j) {
            glTexCoord2f(j * TexSize, i * TexSize);         glVertex3f(i*step, heightMap[i*heightMapSize+j] * stepHeight, j*step);
            glTexCoord2f(j * TexSize, (i + 1.0) * TexSize); glVertex3f((i+1)*step, heightMap[(i+1)*heightMapSize+j] * stepHeight, j*step);
        }

        glEnd();
    }

    glPopMatrix();
}

int Terrain::loadHeightMap(const char *heightMapFile) {
    int width, height, channels;
    unsigned char* data = stbi_load(heightMapFile, &width, &height, &channels, 0);
    if (data == nullptr) return 1;

    this->heightMapSize = width;
    this->heightMap.clear();

    int currentPixel = 0;

    for (int i = 0; i < heightMapSize * heightMapSize; ++i) {
        this->heightMap.push_back(data[currentPixel]);
        currentPixel += channels;
    }

    stbi_image_free(data);

    return 0;
}

int Terrain::loadTexture(const char *textureFile) {
    int width, height, channels;
    unsigned char* data = stbi_load(textureFile, &width, &height, &channels, 0);
    if (data == nullptr) return 1;

    glGenTextures(1, &(this->textureId));
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);

    return 0;
}

Terrain::Terrain(const char *heightMapFile, const char *textureFile, double size, double maxHeight) {
    if (loadHeightMap(heightMapFile)) {
        std::cout << "Heightmap load fail: " << heightMapFile << std::endl;
        return;
    }

    if (loadTexture(textureFile)) {
        std::cout << "Texture load fail: " << textureFile << std::endl;
        return;
    }

    this->size = size;
    this->maxHeight = maxHeight;
    isCreated = true;

    std::cout << this->size << " " << this->heightMapSize << " " << isCreated << std::endl;
}