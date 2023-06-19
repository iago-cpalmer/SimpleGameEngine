#include "BoxCollider.h"
#include <iostream>
void rotatePointX(Vector3f& point, float angle) {
    float radian = angle * M_PI / 180.0f;
    float cosVal = cos(radian);
    float sinVal = sin(radian);

    float newY = point.y * cosVal + point.z * sinVal;
    float newZ = -point.y * sinVal + point.z * cosVal;

    point.y = newY;
    point.z = newZ;
}

void rotatePointY(Vector3f& point, float angle) {
    float radian = angle * M_PI / 180.0f;
    float cosVal = cos(radian);
    float sinVal = sin(radian);

    float newX = point.x * cosVal - point.z * sinVal;
    float newZ = point.x * sinVal + point.z * cosVal;

    point.x = newX;
    point.z = newZ;
}

void rotatePointZ(Vector3f& point, float angle) {
    float radian = angle * M_PI / 180.0f;
    float cosVal = cos(radian);
    float sinVal = sin(radian);

    float newX = point.x * cosVal + point.y * sinVal;
    float newY = -point.x * sinVal + point.y * cosVal;

    point.x = newX;
    point.y = newY;
}



BoxCollider::BoxCollider(SceneObject* object, Vector3f vo, Vector3f vf, Vector3f scale) : ObjectComponent(object) {
    this->name = "BoxCollider";
    this->scale = scale;
    Vector3f v1 = Vector3f(((vo.x - object->pivot.x) * scale.x) - scale.x / 2, ((vo.y - object->pivot.y) * scale.y) - scale.y / 2, ((vo.z - object->pivot.z) * scale.z) - scale.z / 2);
    Vector3f v2 = Vector3f(((vf.x - object->pivot.x) * scale.x) - scale.x / 2, ((vf.y - object->pivot.y) * scale.y) - scale.y / 2, ((vf.z - object->pivot.z) * scale.z) - scale.z / 2);

    
    rotatePointX(v1, -object->rotation.x);
    rotatePointX(v2, -object->rotation.x);

    rotatePointY(v1, -object->rotation.y);
    rotatePointY(v2, -object->rotation.y);

    rotatePointZ(v1, -object->rotation.z);
    rotatePointZ(v2, -object->rotation.z);

    this->xi = v1.x + this->object->position.x;
    this->xf = v2.x + this->object->position.x;
                                              
    this->yi = v1.y + this->object->position.y;
    this->yf = v2.y + this->object->position.y;
                                              
    this->zi = v1.z + this->object->position.z;
    this->zf = v2.z + this->object->position.z;
}

void BoxCollider::update() {
}

void BoxCollider::start() {
}

bool BoxCollider::isInside(Vector3f posToCheck) {
    if (((posToCheck.x >= xf && posToCheck.x <= xi) || (posToCheck.x >= xi && posToCheck.x <= xf)) &&
        ((posToCheck.y >= yf && posToCheck.y <= yi) || (posToCheck.y >= yi && posToCheck.y <= yf)) &&
        ((posToCheck.z >= zf && posToCheck.z <= zi) || (posToCheck.z >= zi && posToCheck.z <= zf))) {
        return true;
    }
	return false;
}

BoxCollider* BoxCollider::clone(SceneObject* parent) const
{
    return new BoxCollider(parent, Vector3f(0,0,0), Vector3f(1,1,1),  parent->scale);
}
