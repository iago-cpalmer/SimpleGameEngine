#ifndef INTERACTIONCONTROLLER_H
#define INTERACTIONCONTROLLER_H

#include "ObjectComponent.h";
#include "Camera.h"
class InteractionController :public ObjectComponent {
private:
	Camera* camera;
	double speed;
public:
	unsigned int tid;
	InteractionController(SceneObject* object);
	void update();
	void start();
	InteractionController* clone(SceneObject* parent) const;
};

#endif
