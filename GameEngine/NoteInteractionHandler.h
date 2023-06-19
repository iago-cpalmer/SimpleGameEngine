#ifndef NOTEINTERACTIONAHNDLER_H
#define NOTEINTERACTIONAHNDLER_H

#include "ObjectComponent.h";
#include "Camera.h"
#include "InputManager.h"
#include "InteractionHandler.h"

class NoteInteractionHandler :public InteractionHandler {
public:
	unsigned int textureId;
	InputManager::Key interactionKey;
	NoteInteractionHandler(SceneObject* object, std::string texturePath);
	NoteInteractionHandler(SceneObject* object);
	void update();
	void start();
	void interact(SceneObject* interactor);
	NoteInteractionHandler* clone(SceneObject* parent) const;
	void changeTexture(std::string path);
};

#endif

