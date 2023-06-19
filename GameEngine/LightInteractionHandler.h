#ifndef LIGHTINTERACTIONHANDLER_H
#define LIGHTINTERACTIONHANDLER_H

#include "ObjectComponent.h";
#include "Camera.h"
#include "InputManager.h"
#include "InteractionHandler.h"

class LightInteractionHandler :public InteractionHandler {
private:
	int lightSource;	
public:
	bool state;
	InputManager::Key interactionKey;
	LightInteractionHandler(SceneObject* object, int light);
	void update();
	void start();
	void interact(SceneObject* interactor);
	LightInteractionHandler* clone(SceneObject* parent) const;
};

#endif

