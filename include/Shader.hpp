#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    enum SHADER_TYPE {VERTEX, FRAGMENT, GEOMETRY};

    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);

    // constructor using std::string
    // ------------------------------------------------------------------------
    Shader(const std::string vertexPath, const std::string fragmentPath);

    // activate the shader
    // ------------------------------------------------------------------------
    void use();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const;

    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const;

    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const;

    // ------------------------------------------------------------------------
    void setVec2f(const std::string &name, float x, float y) const;

    // ------------------------------------------------------------------------
    void setVec3f(const std::string &name, float x, float y, float z) const;

    // ------------------------------------------------------------------------
    void setMat4f(const std::string &name, const float *value) const;

    // ------------------------------------------------------------------------
    void setMVPMatrices(const float *model, const float *view, const float *projection) const;

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);

    void readShader(char const * const, Shader::SHADER_TYPE);

    void compileShader();


    std::string vertexShader;
    std::string fragmentShader;

};