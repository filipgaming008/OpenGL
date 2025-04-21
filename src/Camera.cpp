#include "Camera.hpp"

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, -2.0f);
    direction = glm::vec3(0.0f, 0.0f, 1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    forward = glm::vec3(0.0, 0.0, 1.0);
    lastMousePos = glm::vec2(0.0f, 0.0f);
}

void Camera::mouse(glm::vec2 mousePos) {
    glm::vec2 delta = mousePos - lastMousePos;
    float sensitivity = 0.1f; // Adjust sensitivity as needed
    delta *= sensitivity;

    Quaternion q1 = Quaternion(up, delta.x);
    Quaternion q2 = Quaternion(right, delta.y);

    direction = q1 * direction;
    direction = glm::normalize(direction);
    direction = q2.iQuaternion() * direction;
    direction = glm::vec3(direction.x, glm::clamp(direction.y, -0.8f, 0.8f), direction.z);
    direction = glm::normalize(direction);
    right = glm::normalize(glm::cross(direction, glm::normalize(q2 * up)));

    lastMousePos = mousePos;

    // Debug output
    // std::cout << "Camera Direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
    // std::cout << "Camera Up: " << up.x << ", " << up.y << ", " << up.z << std::endl;
    // std::cout << "Camera Right: " << right.x << ", " << right.y << ", " << right.z << std::endl;
}

void Camera::moveF(float x){
    position += direction * x;
}
void Camera::moveR(float x){
    position += right * x;
}
void Camera::moveU(float x){
    glm::vec3 tup = glm::normalize(glm::cross(direction, right));
    position += tup * x;
}