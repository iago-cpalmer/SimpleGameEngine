#include "Scene.h"


Scene::Scene(std::list<SceneObject*> objects, Camera* camera) {
	this->objects = objects;
	this->camera = camera;
	this->currentUITexture = -1;
}