#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture
{
public:
    static unsigned int LoadTexture(const char* path);
};

#endif