#include "GlobalVariables.h"

// Definir la variable global
Scene* currentScene = nullptr;
Font* font;

void setLightEnabled(bool enabled, int lightSource) {
	if (enabled) {
		glEnable(GL_LIGHT0 + lightSource);
	}
	else {
		glDisable(GL_LIGHT0 + lightSource);
	}
}

SceneObject* instantiate(SceneObject* instance, Vector3f position, Vector3f rotation, Vector3f scale)
{
	FLAG* destFlag = new FLAG[instance->nFlags];
	memcpy(destFlag, instance->flags, sizeof(FLAG)*instance->nFlags);
	SceneObject* newInstance = new SceneObject(position, rotation, scale, instance->pivot, instance->tag, destFlag, instance->nFlags);

	// Copy all childs
	std::list<SceneObject*> childs = instance->getChilds();
	for (SceneObject* child : childs) {
		SceneObject* copy = instantiate(child, child->position, child->rotation, child->scale);
		newInstance->addChild(copy);
	}

	// Copy all components
	std::list<ObjectComponent*> components = instance->getComponents();
	for (ObjectComponent* component : components) {
		ObjectComponent* copy = component->clone(newInstance);
		newInstance->addComponent(copy);
	}
	return newInstance;
}

void changeLightPosition(Vector3f position, int lightSource) {
	GLfloat lightPosition[] = { position.x, position.y, position.z, 1.0f};
	glLightfv(GL_LIGHT0 + lightSource, GL_POSITION, lightPosition);
}

void loadTextRendering() {

}

void RenderText(std::string text, Vector3f position) {

}
