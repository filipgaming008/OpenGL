#include "Quaternion.hpp"

Quaternion::Quaternion(float w, float x, float y, float z){
    this->w = w;
    v.x = x;
    v.y = y;
    v.z = z;
}

Quaternion::Quaternion(const glm::vec3 &n, float a){

    a = a /360 * glm::pi<float>() * 2.0;

    w = cos(a/2.0f);
    v.x = n.x * sin(a/2.0f);
    v.y = n.y * sin(a/2.0f);
    v.z = n.z * sin(a/2.0f);
}

const Quaternion Quaternion::iQuaternion() const{
    return Quaternion(w, -v.x, -v.y, -v.z);
}

const Quaternion Quaternion::operator*(const Quaternion &q) const{
    Quaternion r;
    
    r.w = w*q.w + glm::dot(v, q.v);
    r.v = v*q.w + q.v*w + glm::cross(v, q.v);

    return r;
}

const glm::vec3 Quaternion::operator*(const glm::vec3 &V) const{
    Quaternion p;
    p.w = 0.0f;
    p.v = V;

    glm::vec3 vcV = glm::cross(v, V);

    return V + vcV*(2.0f*w) + glm::cross(v, vcV)*2.0f;
}