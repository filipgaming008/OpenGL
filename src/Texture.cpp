#include "Texture.hpp"
#include "Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

Texture::Texture(){
    GLCall(glGenTextures(1, &texture_ID));
}


void Texture::loadTexture(const std::string path){   
    
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_ID));

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    // Load image data
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(data);
}

void Texture::bindTexture(unsigned int slot){
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_ID));
}

Texture::~Texture(){
    GLCall(glDeleteTextures(1 , &texture_ID));
}