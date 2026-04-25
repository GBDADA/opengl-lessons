#ifndef TEXTURE2D_H
#define TEXTURE2D_H

typedef struct {
    int width, height;
    unsigned int texID;
} TextureImage;

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    void LoadTexture(const char *, TextureImage *);
    void freeTexture(TextureImage *);
};

#endif