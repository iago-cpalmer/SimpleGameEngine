#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "PlanetController.h"

PlanetController::PlanetController(SceneObject* object, float radius, float speed, float rotationSpeed) :ObjectComponent(object) {
	this->name = "PlanetController";
	this->radius = radius;
	this->speed = speed * 3;
	this->rotationSpeed = rotationSpeed/50;
}

void PlanetController::update() {
	// Translation
	float radTranAngle = translationAngle * M_PI / 180.0f;
	float x = radius * sin(radTranAngle);
	float z = radius * cos(radTranAngle);
	translationAngle += speed * Time::deltaTime;
	if (translationAngle >= 360) {
		translationAngle = 0;
	}
	this->object->position.x = x;
	this->object->position.z = z;

	// Rotation
	rotationAngle += rotationSpeed * Time::deltaTime;
	if (rotationAngle >= 360) {
		rotationAngle = 0;
	}
	this->object->rotation.y = rotationAngle;
}

void PlanetController::start() {

}

PlanetController* PlanetController::clone(SceneObject* parent) const
{
	return new PlanetController(parent, this->radius, this->speed, this->rotationSpeed);
}
