#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "LightInteractionHandler.h"

LightInteractionHandler::LightInteractionHandler(SceneObject* object, int light) :InteractionHandler(object) {
	this->name = "InteractionHandler";
	this->interactionKey = InputManager::Interact;
	this->lightSource = light;
	this->state = true;
	changeLightPosition(object->position, this->lightSource);
}

void LightInteractionHandler::update() {
}

void LightInteractionHandler::start() {

}

void LightInteractionHandler::interact(SceneObject* interactor){
	state = !state;
	setLightEnabled(state, this->lightSource);
}

LightInteractionHandler* LightInteractionHandler::clone(SceneObject* parent) const
{
	return new LightInteractionHandler(parent, (lightSource + 1)%8);
}
