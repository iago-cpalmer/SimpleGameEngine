#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "InteractionHandler.h"

InteractionHandler::InteractionHandler(SceneObject* object) :ObjectComponent(object) {
	this->name = "InteractionHandler";
	this->interactionKey = InputManager::Interact;
}

void InteractionHandler::update() {

}

void InteractionHandler::start() {

}

InteractionHandler* InteractionHandler::clone(SceneObject* parent) const
{
	return nullptr;
}
