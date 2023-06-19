#ifndef ROBOTARMCONTROLLER_H
#define ROBOTARMCONTROLLER_H
#define _USE_MATH_DEFINES
#include "ObjectComponent.h";
#include "Camera.h"
#include "InputManager.h"
#include "InteractionHandler.h"
#include <math.h>

class RobotArmController :public ObjectComponent {
public:
	Vector3f speed, rangeMin, rangeMax;
	RobotArmController(SceneObject* object, Vector3f speed);
	RobotArmController(SceneObject* object, Vector3f speed, Vector3f rangeMin, Vector3f rangeMax);
	void update();
	void start();
	RobotArmController* clone(SceneObject* parent) const;
};

#endif

