#include "Camera.hpp"

#include "OpenGL.hpp"
#include "glm/gtc/quaternion.hpp"
#include <iostream>

Camera::Camera() 
    : projectionMatrix(glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)),
    lastMousePos(glm::vec2(0.0f))
    {   
        Reset();
    }

void Camera::MouseUpdate(glm::vec2 &mousePos){
    glm::vec2 delta = mousePos - lastMousePos;
    delta *= sensitivity * (float)getDeltaTime();

    glm::quat q = glm::angleAxis(-delta.x, up) * glm::angleAxis(delta.y, right);

    // Prevents gimbal lock
    front = glm::normalize(q * front);
    right = glm::normalize(q * right);
    up = glm::normalize(q * up);

    // With gimbal lock
    // front = glm::normalize(glm::vec3(front.x, glm::clamp(front.y, -0.9f, 0.9f), front.z));
    // right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    // up = glm::normalize(glm::cross(front, right));

    lastMousePos = mousePos;
}

void Camera::Update(){
    viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::Reset(){
    position = glm::vec3(0.0f, 0.0f, -2.0f);
    LookAt(glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::LookAt(const glm::vec3 &target){
    viewMatrix = glm::lookAt(position, target, up);
    front = glm::normalize(target - position);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(front, right));
}
glm::mat4 Camera::GetViewMatrix() const {
    return viewMatrix;
}
glm::mat4 Camera::GetProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::PositionUpdate(enum CameraMovement direction){
    float velocity = speed * (float)getDeltaTime();
    switch (direction){
        case FORWARD: position += front * velocity; break;
        case BACKWARD: position -= front * velocity; break;
        case LEFT: position += right * velocity; break;
        case RIGHT: position -= right * velocity; break;
        case UP: position += up * velocity; break;
        case DOWN: position -= up * velocity; break;
    }
}

void Camera::Tilt(float _angle){
    float angle = _angle * (float)getDeltaTime();
    glm::quat q = glm::angleAxis(glm::radians(angle), front);
    front = glm::normalize(q * front);
    right = glm::normalize(q * right);
    up = glm::normalize(q * up);
}