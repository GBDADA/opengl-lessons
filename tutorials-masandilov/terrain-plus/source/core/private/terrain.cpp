#include "terrain.h"

#include <glad/gl.h>
#include "stb_image.h"
#include <assert.h>

void Terrain::updateLocalToGlobal() {
    localToGlobal.x = localToGlobal.z = (
        (float)size / (float)heightMapSize
    );
    localToGlobal.y = (float)maxHeight / 255.0f;
}

Terrain::Terrain(CVector position) {
    this->position = position;

    this->indices.clear();
    this->heightMap.clear();
    this->texCoords.clear();

    this->texture = {
        .width = 0,
        .height = 0,
        .texID = 0
    };
}

int Terrain::getHeightFromMap(int x, int z) const {
    if (heightMap.empty()) return 0;

    x = x % heightMapSize;
    z = z % heightMapSize;
    return heightMap[x + z * heightMapSize].y;
}

// Линейная интерполяция между где y(0)=y1, y(1)=y2, x [0,1]
double lerp(double y1, double y2, double x) {
    return x * (y2 - y1) + y1;
}

double Terrain::getHeight(double x, double z) const {
    if (heightMap.empty()) return 0;

    double cellSide = (double)size / (heightMapSize);

    x = (x - position.x) / localToGlobal.x + (heightMapSize / 2.0f); // Переводим координаты относительно террейна
    z = (z - position.z) / localToGlobal.z + (heightMapSize / 2.0f);

    int xi = static_cast<int>(x); // Переводим в координаты плитки
    int zi = static_cast<int>(z);
    x -= xi; z -= zi;

    // Проверка границ, чтобы не вылететь за массив
    if (xi < 0 || xi >= heightMapSize - 1 || zi < 0 || zi >= heightMapSize - 1) {
        return position.y;
    }

    return lerp(
        lerp(getHeightFromMap(xi, zi),      getHeightFromMap(xi+1, zi), x),
        lerp(getHeightFromMap(xi, zi+1),    getHeightFromMap(xi+1, zi+1), x),
        z
    ) * localToGlobal.y + position.y;
}

void Terrain::renderWithSplatting() const {
    if (heightMap.empty() || maskR.texID == 0) return;

    glPushMatrix();

    glTranslatef(position.x, position.y, position.z);
    glScalef(localToGlobal.x, localToGlobal.y, localToGlobal.z);
    glTranslatef(-(heightMapSize / 2.0f), 0, -(heightMapSize / 2.0f));

    // Включаем что надо
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, heightMap.data());

    for (int i = 0; i < 7; ++i) {
        glClientActiveTexture(GL_TEXTURE0 + i);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, (i % 2 == 0) ? texCoords.data() :maskCoords.data());
    }

    // С начала работаем с масками
    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE1 + 2 * i);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i == 0) ? maskR.texID : (i == 1) ? maskG.texID : maskB.texID);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }

    // Закладываем базовую текстуру
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBack.texID);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Потом последовательно смешиваем текстуры
    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE2 + 2 * i);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i == 0) ? textureR.texID : (i == 1) ? textureG.texID : textureB.texID);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);

        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE1 + 2 * i);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
    }

    // Рисуем
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    // Выключаем
    for (int i = 6; i >= 0; --i) {
        glClientActiveTexture(GL_TEXTURE0 + i);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

void Terrain::render() const {
    if (heightMap.empty()) return;

    if (texture.texID != 0) glBindTexture(GL_TEXTURE_2D, texture.texID);

    glPushMatrix();

    glTranslatef(position.x, position.y, position.z);
    glScalef(localToGlobal.x, localToGlobal.y, localToGlobal.z);
    glTranslatef(-(heightMapSize / 2.0f), 0, -(heightMapSize / 2.0f));

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, heightMap.data());
    glTexCoordPointer(2, GL_FLOAT, 0, maskCoords.data());
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

int Terrain::loadFromFile(const char *heightMapFile, double size, double maxHeight) {
    int width, height, channels;
    unsigned char* data = stbi_load(heightMapFile, &width, &height, &channels, 0);
    if (data == nullptr) return 1;

    this->heightMapSize = width;
    this->heightMap.clear();
    this->indices.clear();
    this->texCoords.clear();
    this->maskCoords.clear();

    int currentPixel = 0;
    float texScale = 1.0 / (heightMapSize - 1.0) * 10;
    float maskScale = 1.0 / (heightMapSize - 1.0);

    for (int z = 0; z < heightMapSize; ++z) {
        for (int x = 0; x <heightMapSize; ++x) {
            heightMap.push_back(CVector(x, data[currentPixel], z));
            texCoords.push_back({x * texScale, z * texScale});
            maskCoords.push_back({x * maskScale, z * maskScale});
            currentPixel += channels;
        }
    }

    for (int z = 0; z < heightMapSize - 1; ++z) {
        int offset = z *heightMapSize;
        for (int x = 0; x < heightMapSize - 1; ++x) {
            indices.push_back(offset);
            indices.push_back(offset + 1);
            indices.push_back(offset + 1 + heightMapSize);

            indices.push_back(offset + 1 + heightMapSize);
            indices.push_back(offset + heightMapSize);
            indices.push_back(offset);

            ++offset;
        }
    }

    stbi_image_free(data);

    this->size = size;
    this->maxHeight = maxHeight;
    updateLocalToGlobal();

    return 0;
}

void Terrain::generateMask(TextureImage *mask, float* data) {
    glGenTextures(1, &mask->texID);
    glBindTexture(GL_TEXTURE_2D, mask->texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, heightMapSize, heightMapSize, 0, GL_ALPHA, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

int Terrain::loadSplattingMask(const char *maskFile, const TextureImage &textureBack,
                      const TextureImage &textureR, const TextureImage &textureG, const TextureImage &textureB) {
    int width, height, channels;
    unsigned char* data = stbi_load(maskFile, &width, &height, &channels, 0);
    if (data == nullptr) return 1;

    if (!(width == heightMapSize && height == heightMapSize)) {
        stbi_image_free(data);
        return 2;
    }

    int currentPixel = 0;
    float w1, w2, w3, sum; // Коэффициенты при каждой текстуре w0 - фон = 1 - sum
    std::vector<float> dataR, dataG, dataB;

    for (int z = 0; z < heightMapSize; ++z) {
        for (int x = 0; x <heightMapSize; ++x) {
            w1 = data[currentPixel] / 255.0f;
            w2 = data[currentPixel + 1] / 255.0f;
            w3 = data[currentPixel + 2] / 255.0f;
            if ((sum = w1 + w2 + w3) > 1) {
                w1 /= sum;
                w2 /= sum;
                w3 /= sum;
            }

            // Поскольку мы используем GL_INTERPOLATE то меняем коэффициенты для корректности
            dataR.push_back(w1 / (1.0f - w2 - w3));
            dataG.push_back(w2 / (1.0f - w3));
            dataB.push_back(w3);

            currentPixel += channels;
        }
    }

    maskR = maskG = maskB = {0, 0, 0};
    generateMask(&maskR, dataR.data());
    generateMask(&maskG, dataG.data());
    generateMask(&maskB, dataB.data());

    this->textureBack = textureBack;
    this->textureR = textureR;
    this->textureG = textureG;
    this->textureB = textureB;

    stbi_image_free(data);
    return 0;
}

void Terrain::setTexture(TextureImage &texture) {
    this->texture = texture;
}