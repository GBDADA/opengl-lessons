#include "terrain.h"

#include <glad/gl.h>
#include "stb_image.h"

void Terrain::updateLocalToGlobal() {
    localToGlobal.x = localToGlobal.z = (
        (float)size / (float)heightMapSize
    );
    localToGlobal.y = (float)maxHeight / 255.0f;
}

Terrain::Terrain(CVector position) {
    this->position = position;
}

int Terrain::getHeightFromMap(int x, int z) const {
    if (heightMap.empty()) return 0;

    x = x % heightMapSize;
    z = z % heightMapSize;
    return heightMap[x + z * heightMapSize];
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

void Terrain::render() const {
    if (heightMap.empty()) return;

    glPushMatrix();

    glColor4f(1.0, 0.0, 0.0, 1.0);

    glTranslatef(position.x, position.y, position.z);
    glScalef(localToGlobal.x, localToGlobal.y, localToGlobal.z);
    glTranslatef(-(heightMapSize / 2.0f), 0, -(heightMapSize / 2.0f));

    glBegin( GL_QUADS );
    for (int z = 0; z < heightMapSize - 1; ++z) {
        for (int x = 0; x < heightMapSize - 1; ++x) {
            glVertex3f(x, getHeightFromMap(x, z), z);
            glVertex3f(x, getHeightFromMap(x, z + 1), z + 1);
            glVertex3f(x + 1, getHeightFromMap(x + 1, z + 1), z + 1);
            glVertex3f(x + 1, getHeightFromMap(x + 1, z), z);
        }
    }
    glEnd();
    glPopMatrix();
}

int Terrain::loadFromFile(const char *heightMapFile, double size, double maxHeight) {
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

    this->size = size;
    this->maxHeight = maxHeight;
    updateLocalToGlobal();

    return 0;
}

void Terrain::setTexture(TextureImage &texture) {
    this->texture = texture;
}