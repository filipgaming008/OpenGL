#include "Camera.hpp"

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, -2.0f);
    direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
    up = glm::normalize(glm::cross(direction, right));
    lastMousePos = glm::vec2(0.0f, 0.0f);
}

void Camera::mouse(glm::vec2 mousePos) {
    glm::vec2 delta = mousePos - lastMousePos;
    float sensitivity = 0.1f;
    delta *= sensitivity;

    Quaternion q1 = Quaternion(up, -delta.x);
    Quaternion q2 = Quaternion(right, -delta.y);
    Quaternion q = q1 * q2;

    direction = glm::normalize(q * direction);
    direction = glm::normalize(glm::vec3(direction.x, glm::clamp(direction.y, -0.85f, 0.85f), direction.z));
    right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(direction, right));

    lastMousePos = mousePos;
}

void Camera::moveF(float x){
    position += direction * x;
}
void Camera::moveR(float x){
    position += right * x;
}
void Camera::moveU(float x){
    position += up * x;
}