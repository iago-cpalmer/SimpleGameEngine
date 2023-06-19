#include "Camera.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>


float radians(float x) {
	return (M_PI * x / 180);
}
void Camera::look() {
	this->rotation.x = this->position.x - cos(radians(pitch)) * cos(radians(yaw));
	this->rotation.y = this->position.y - sin(radians(pitch));
	this->rotation.z = this->position.z - cos(radians(pitch)) * sin(radians(yaw));
}
Camera::Camera(Vector3f position, Vector3f rotation, Vector3f upVector, SceneObject* player) :SceneObject(position, rotation, Vector3f(1,1,1), Vector3f(0.0f,0,0), "Camera", nullptr, 0) {
	this->upVector = upVector;
	this->player = player;
}

void Camera::initComponents() {
}

void Camera::start() {
	SceneObject::start();
}

void Camera::update() {
	SceneObject::update();
	if (player != nullptr) {
		this->position.x = player->position.x;
		this->position.y = player->position.y;
		this->position.z = player->position.z;
	}
	look();
}
Vector3f Camera::getCameraForwardVector() {
	Vector3f forwardVector = { this->rotation.x - this->position.x, this->rotation.y - this->position.y, this->rotation.z - this->position.z };
	return forwardVector;
}

Vector3f Camera::getCameraRightVector() {
	Vector3f forwardVector = getCameraForwardVector();
	Vector3f rightVector = {
	(float)(forwardVector.x * cos(-90) + forwardVector.z * sin(-90)),
	forwardVector.y,
	(float)(-forwardVector.x * sin(-90) + forwardVector.z * cos(-90))
	};
	return rightVector;
}
