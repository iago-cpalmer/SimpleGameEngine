#ifndef PLANETCONTROLLER_H
#define PLANETCONTROLLER_H
#define _USE_MATH_DEFINES
#include "ObjectComponent.h";
#include "Camera.h"
#include "InputManager.h"
#include "InteractionHandler.h"
#include <math.h>

class PlanetController:public ObjectComponent {
public:
	float speed, radius, rotationSpeed;
	float translationAngle, rotationAngle;
	PlanetController(SceneObject* object, float radius, float speed, float rotationSpeed);
	void update();
	void start();
	PlanetController* clone(SceneObject* parent) const;
};

#endif

