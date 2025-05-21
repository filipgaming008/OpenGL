#pragma once

#include "Shader.hpp"
#include "Buffers.hpp"

#include "Camera.hpp"


//-------------------------------------------
// Materials and light
struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shine;
};

struct Light{
    glm::vec3 pos;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

//-------------------------------------------
// Model
enum ModelType{
    CUBE = 1,
    PLANE,
    SPHERE
};

class Model{
private:
    VertexLayout *layout;
    VertexArray *VAO;
    VertexBuffer *VBO;
    IndexBuffer *IBO;

    glm::mat4 model;

    void Cube();
    void Plane();
    void Sphere();
public:
    Model(enum ModelType type);
    ~Model();

    void Draw(Shader &shader, const Camera &camera);
    inline void SetModelMatrix(const glm::mat4 &modelMatrix) { model = modelMatrix; }
    inline glm::mat4 GetModelMatrix() const { return model; }
};