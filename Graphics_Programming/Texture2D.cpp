#include "Texture2D.h"
#include <fstream>
#include <iostream>

Texture2D::Texture2D() : _ID(0), _width(0), _height(0) {}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &_ID);
}

bool Texture2D::Load(char* path, int width, int height)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return false;

    _width = width;
    _height = height;

    char* tempTextureData = new char[width * height * 3];
    file.read(tempTextureData, width * height * 3);
    file.close();

    glGenTextures(1, &_ID);
    glBindTexture(GL_TEXTURE_2D, _ID);

    // If your penguins look blue/orange instead of black/white, change GL_RGB to GL_BGR_EXT here!
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Ensure the texture repeats instead of stretching the edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] tempTextureData;

    return true;
}