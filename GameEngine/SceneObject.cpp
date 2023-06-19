#include "SceneObject.h"
#include "ObjectComponent.h"

#include <iostream>

SceneObject::SceneObject(Vector3f position, Vector3f rotation, Vector3f scale, Vector3f pivot, std::string tag, FLAG* flags, int nFlags)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->pivot = pivot;
	this->tag = tag;
	this->flags = flags;
	this->nFlags = nFlags;
	this->isActive = true;
}

void SceneObject::update() {
	//std::cout << "sceneobject update" << std::endl;
	// Update the childs
	for (auto it = childs.begin(); it != childs.end(); ++it) {
		SceneObject* child = *it;
		child->update();
	}
	// Update the components
	for (auto it = components.begin(); it != components.end(); ++it) {
		it->second->update();
	}
}

void SceneObject::start() {
	// Start the childs
	for (auto it = childs.begin(); it != childs.end(); ++it) {
		SceneObject* child = *it;
		child->start();
	}
	// Start the components
	for (auto it = components.begin(); it != components.end(); ++it) {
		it->second->start();
	}
}

void SceneObject::addComponent(ObjectComponent* oc) {
	components[oc->getName()] = oc;
}

ObjectComponent* SceneObject::getComponent(std::string name) {

	auto it = components.find(name);
	if (it != components.end()) {
		return it->second;
	}
	return nullptr;
}

void SceneObject::addChild(SceneObject* o) {
	this->childs.push_back(o);
}

std::list<SceneObject* > SceneObject::getChilds() {
	return this->childs;
}

bool SceneObject::hasFlag(FLAG flag) {
	for (int i = 0; i < nFlags; i++) {
		if (flags[i] == flag) {
			return true;
		}
	}
	return false;
}

std::list<ObjectComponent*> SceneObject::getComponents()
{
	std::list<ObjectComponent*> componentList; // The resulting list

	// Iterate over the map and insert values into the list
	for (const auto& pair : components) {
		componentList.push_back(pair.second);
	}
	return componentList;
}
