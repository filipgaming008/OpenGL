#include "Texture.hpp"
#include "Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>


// Texture Parameters
// -------------------------------------------------
TextureParameters::~TextureParameters(){
    parameters.clear();
}

template<>
void TextureParameters::addParameter<GLint>(GLenum pname, GLint param){
    parameters.push_back({pname, param});
}
template<>
void TextureParameters::addParameter<GLfloat>(GLenum pname, GLfloat param){
    parameters.push_back({pname, param});
}
template<>
void TextureParameters::addParameter<GLuint>(GLenum pname, GLuint param){
    parameters.push_back({pname, param});
}

// Texture Class
// -------------------------------------------------
Texture::Texture(){
    GLCall(glGenTextures(1, &texture_ID));
}

Texture::~Texture(){
    GLCall(glDeleteTextures(1 , &texture_ID));
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

    GLenum format;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
        stbi_image_free(data);
        return;
    }

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(data);
}

void Texture::bindTexture(unsigned int slot){
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_ID));
}

void Texture::addTextureParameters(const TextureParameters &param){
    for (const auto& p : param.getParameters()) {
        if (std::holds_alternative<GLint>(p.param)) {
            GLCall(glTexParameteri(GL_TEXTURE_2D, p.pname, std::get<GLint>(p.param)));
        } else if (std::holds_alternative<GLfloat>(p.param)) {
            GLCall(glTexParameterf(GL_TEXTURE_2D, p.pname, std::get<GLfloat>(p.param)));
        }else {
            std::cerr << "Unsupported parameter type for texture: " << p.pname << std::endl;
        }
    }
}

void Texture::genTextureHDR(int width, int height){
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_ID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ID, 0));
}