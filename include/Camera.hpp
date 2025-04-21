#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Quaternion.hpp"

#include <iostream>

class Camera{
public:
    // camera Attributes
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;
    glm::vec2 lastMousePos;

public:
    Camera();

    void mouse(const glm::vec2 mousePos);
    void moveF(float x);
    void moveR(float x);
    void moveU(float x);
};