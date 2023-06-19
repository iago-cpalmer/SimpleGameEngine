#ifndef INTERACTIONHANDER_H
#define INTERACTIONHANDER_H

#include "ObjectComponent.h";
#include "Camera.h"
#include "InputManager.h"
class InteractionHandler :public ObjectComponent {
public:
	InputManager::Key interactionKey;
	InteractionHandler(SceneObject* object);
	void update();
	void start();
	virtual void interact(SceneObject* interactor) = 0;
	InteractionHandler* clone(SceneObject* parent) const;
};

#endif
