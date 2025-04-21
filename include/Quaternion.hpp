#pragma once

#include <glm/glm.hpp>
//glm constants
#include <glm/gtc/constants.hpp>

class Quaternion{
private:
    float w;
    glm::vec3 v;
public:

    Quaternion(){};
    Quaternion(float w, float x, float y, float z);
    Quaternion(const glm::vec3 &n, float a);

    const Quaternion iQuaternion() const;
    const Quaternion operator*(const Quaternion &q) const;
    const glm::vec3 operator*(const glm::vec3 &v) const;
};