#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "texture2D.h"
#include "vector.h"

typedef struct {
    float u, v;
} uvCoord;

typedef struct{
    float r, g, b;
} color;

class Terrain {
private:
    int heightMapSize;
    std::vector<CVector> heightMap;
    std::vector<unsigned int> indices;

    // Можем либо использовать текстурные координаты, либо использовать splatting texture
    std::vector<uvCoord> texCoords;
    std::vector<uvCoord> maskCoords;
    // Для splatting texturing используем дохренелиард текстур, но это причуда статического конвейера, с шейдерами будет лучше
    TextureImage maskR, maskG, maskB;
    TextureImage textureBack, textureR, textureG, textureB;

    TextureImage texture;

    int getHeightFromMap(int x, int z) const;

    CVector localToGlobal; // Коэффициенты масштабирования
    double size;
    double maxHeight;

    void updateLocalToGlobal();
    void generateMask(TextureImage *mask, float* data);
public:
    CVector position;

    Terrain(CVector position);

    double getHeight(double x, double z) const;

    void render() const;
    void renderWithSplatting() const;

    int loadFromFile(const char *heightMapFile, double size, double maxHeight);
    void setTexture(TextureImage &texture);
    int loadSplattingMask(const char *maskFile, const TextureImage &textureBack,
        const TextureImage &textureR, const TextureImage &textureG, const TextureImage &textureB);
};

#endif