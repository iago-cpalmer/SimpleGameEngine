#ifndef VECTOR3F_H
#define VECTOR3F_H
#include <cmath>
class Vector3f {
public:
    float x;
    float y;
    float z;
    Vector3f();
    Vector3f(float x, float y, float z);
    void normalize();
    Vector3f cross(const Vector3f& other) const;
    float distance(Vector3f other);
};

#endif