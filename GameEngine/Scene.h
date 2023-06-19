#include "SceneObject.h"
#include <list>
#include "Camera.h"
#ifndef SCENE_H
#define SCENE_H
class Camera;
class Scene {
public:
	unsigned int currentUITexture;
	std::list<SceneObject*> objects;
	Camera* camera;
	Scene(std::list<SceneObject*> objects, Camera* camera);
};
#endif