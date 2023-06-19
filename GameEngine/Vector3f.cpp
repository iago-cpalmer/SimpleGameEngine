#include "Vector3f.h"
Vector3f::Vector3f() {

}
Vector3f::Vector3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
void Vector3f::normalize() {
    float length = std::sqrt(x * x + y * y + z * z);
    if (length != 0.0f) {
        x /= length;
        y /= length;
        z /= length;
    }
}

Vector3f Vector3f::cross(const Vector3f& other) const {
    return Vector3f(
        (y * other.z) - (z * other.y),
        (z * other.x) - (x * other.z),
        (x * other.y) - (y * other.x)
    );
}

float Vector3f::distance(Vector3f other) {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
}