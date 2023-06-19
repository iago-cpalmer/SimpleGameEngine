#include "InteractionController.h"
#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "Raycast.h"
#include "InteractionHandler.h"
#include "LightInteractionHandler.h"
#include "ModelParser.h"

const float rangeOfInteraction = 7;

InteractionController::InteractionController(SceneObject* object) :ObjectComponent(object) {
	this->name = "InteractionController";
	this->camera = currentScene->camera;
	loadText("  Presiona 'E'\npara interactuar", &this->tid, &font[0], 100, 800);
}

void InteractionController::update() {
	SceneObject* collision = castRaycast(camera->position, camera->getCameraForwardVector(), rangeOfInteraction);
	if (collision != nullptr) {
		if (collision->hasFlag(IS_INTERACTABLE)) {
			// Player can interact with the object
			InteractionHandler* interactionHandler = (InteractionHandler*)(collision->getComponent("InteractionHandler"));
			if (interactionHandler != nullptr && InputManager::isKeyUp(interactionHandler->interactionKey)) {
				interactionHandler->interact(this->object);
			}
			else if (interactionHandler != nullptr && currentScene->currentUITexture == -1) {
				currentScene->currentUITexture = this->tid;
			}
		}
	}
	else if(currentScene->currentUITexture == tid) {
		currentScene->currentUITexture = -1;
	}
}

void InteractionController::start() {
	
}

InteractionController* InteractionController::clone(SceneObject* parent) const
{
	return new InteractionController(parent);
}
