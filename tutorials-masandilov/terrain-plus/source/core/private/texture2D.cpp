#include "texture2D.h"

#include <glad/gl.h>
#include <stb_image.h>
#include <assert.h>

TextureManager::TextureManager() {};
TextureManager::~TextureManager() {};

void TextureManager::LoadTexture(const char *fileName, TextureImage *texture) {
    int channels;
    unsigned char* data = stbi_load(fileName, &texture->width, &texture->height, &channels, 0);

    assert(data != nullptr);

    glGenTextures(1, &texture->texID);
    glBindTexture(GL_TEXTURE_2D, texture->texID);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
}

void TextureManager::freeTexture(TextureImage *texture) {

}