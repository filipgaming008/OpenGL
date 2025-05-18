#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <variant>


// Texture Parameters
// -------------------------------------------------
template<typename T>
struct TextureParametersElement{
    GLenum pname;
    T param;
};


// Texture Parameters
// -------------------------------------------------
class TextureParameters{
private:
    std::vector<TextureParametersElement<std::variant<GLint, GLfloat, GLuint>>> parameters;

public:

    TextureParameters() : parameters() {};
    ~TextureParameters();

    template<typename T>
    void addParameter(GLenum pname, T param);

    inline const std::vector<TextureParametersElement<std::variant<GLint, GLfloat, GLuint>>> &getParameters() const { return parameters; }
};


// Texture Class
// -------------------------------------------------
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

    void addTextureParameters(const TextureParameters &param);

    inline unsigned int getTextureID() const { return texture_ID; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline int getNRChannels() const { return nrChannels; }
};