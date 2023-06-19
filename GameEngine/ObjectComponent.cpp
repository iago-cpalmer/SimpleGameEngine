#include "ObjectComponent.h"
#include <iostream>

ObjectComponent::ObjectComponent(SceneObject* object) {
	this->object = object;
}

std::string ObjectComponent::getName() {
	return this->name;
}