#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/gl.h>
#include <vector>

class Terrain {
private:
    int heightMapSize;
    GLuint textureId;
    std::vector<int> heightMap; // [i * cols + j] i - строка, j - столбец, а то путаю
    bool isCreated = false;

    int loadHeightMap(const char *heightMapFile);
    int loadTexture(const char *textureFile);
public:
    double size;
    double maxHeight;

    Terrain(const char *heightMapFile, const char *textureFile, double size, double maxHeight);
    void draw();
};

#endif