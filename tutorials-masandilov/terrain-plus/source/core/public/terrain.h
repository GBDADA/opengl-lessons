#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "texture2D.h"
#include "vector.h"

class Terrain {
private:
    int heightMapSize;
    std::vector<int> heightMap;
    TextureImage texture;

    int getHeightFromMap(int x, int z) const;

    CVector localToGlobal; // Коэффициенты масштабирования
    double size;
    double maxHeight;

    void updateLocalToGlobal();
public:
    CVector position;

    Terrain(CVector position);

    double getHeight(double x, double z) const;
    void render() const;
    int loadFromFile(const char *heightMapFile, double size, double maxHeight);
    void setTexture(TextureImage &texture);
};

#endif