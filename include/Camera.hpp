#pragma once

#include <glm/glm.hpp>

enum CameraMovement{
    FORWARD = 1,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera{
    private:
    float sensitivity = 0.1f;
    float speed = 1.0f;
    glm::vec2 lastMousePos;
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
public:
    Camera();
    ~Camera(){};

    void Update();
    void LookAt(const glm::vec3 &target);
    void MouseUpdate(glm::vec2 &mousePos);
    void PositionUpdate(enum CameraMovement direction);
    void Tilt(float _angle);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
};