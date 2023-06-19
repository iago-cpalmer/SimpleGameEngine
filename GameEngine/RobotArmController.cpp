#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "RobotArmController.h"

RobotArmController::RobotArmController(SceneObject* object, Vector3f speed) :ObjectComponent(object) {
	this->name = "RobotArmController";
	this->speed = speed;
	this->rangeMin = Vector3f(0,0,0);
	this->rangeMax = Vector3f(90,90,90);
}

RobotArmController::RobotArmController(SceneObject* object, Vector3f speed, Vector3f rangeMin, Vector3f rangeMax) : ObjectComponent(object) {
	this->name = "RobotArmController";
	this->speed = speed;
	this->rangeMin = rangeMin;
	this->rangeMax = rangeMax;
}


void RobotArmController::update() {
	// Rotation
	if (object->rotation.x > rangeMax.x) {
		speed.x = -abs(speed.x);
	}
	else if(object->rotation.x < rangeMin.x){
		speed.x = abs(speed.x);
	}

	if (object->rotation.y > rangeMax.y) {
		speed.y = -abs(speed.y);
	}
	else if (object->rotation.x < rangeMin.y) {
		speed.y = abs(speed.y);
	}

	if (object->rotation.z > rangeMax.z) {
		speed.z = -abs(speed.z);
	}
	else if (object->rotation.z < rangeMin.z) {
		speed.z = abs(speed.z);
	}

	this->object->rotation.x += speed.x * Time::deltaTime;
	this->object->rotation.y += speed.y * Time::deltaTime;
	this->object->rotation.y += speed.z * Time::deltaTime;
}

void RobotArmController::start() {

}

RobotArmController* RobotArmController::clone(SceneObject* parent) const
{
	return new RobotArmController(parent, this->speed);
}
