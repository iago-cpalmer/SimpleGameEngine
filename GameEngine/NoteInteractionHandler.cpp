#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "NoteInteractionHandler.h"
#include "ModelParser.h"

NoteInteractionHandler::NoteInteractionHandler(SceneObject* object, std::string texturePath) :InteractionHandler(object) {
	this->name = "InteractionHandler";
	this->interactionKey = InputManager::Interact;
	initTextures(&this->textureId, texturePath);
}

NoteInteractionHandler::NoteInteractionHandler(SceneObject* object) :InteractionHandler(object) {
	this->name = "InteractionHandler";
	this->interactionKey = InputManager::Interact;
	textureId = 1;
}

void NoteInteractionHandler::update() {

}

void NoteInteractionHandler::start() {

}

void NoteInteractionHandler::interact(SceneObject* interactor) {
	currentScene->currentUITexture = currentScene->currentUITexture = textureId;
}

NoteInteractionHandler* NoteInteractionHandler::clone(SceneObject* parent) const
{
	return new NoteInteractionHandler(parent);
}

void NoteInteractionHandler::changeTexture(std::string path)
{
	initTextures(&textureId, path);
}
