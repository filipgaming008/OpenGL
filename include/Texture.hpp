#pragma once

#include <glad/glad.h>
#include <string>


class Texture{
private:
    unsigned int texture_ID;
    int width;
    int height;
    int nrChannels;
    unsigned char *data;

public:
    Texture();
    ~Texture();

    void loadTexture(const std::string path);
    void bindTexture(unsigned int slot = 0);

    inline unsigned int getTextureID() const { return texture_ID; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline int getNRChannels() const { return nrChannels; }
};