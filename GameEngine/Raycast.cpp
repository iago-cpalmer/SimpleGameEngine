#include "Raycast.h"
#include <iostream>

SceneObject* castRaycast(Vector3f position, Vector3f direction, float range) {
	position.x += direction.x * 0.1f;
	position.y += direction.y * 0.1f;
	position.z += direction.z * 0.1f;
	
	for (auto it = currentScene->objects.begin(); it != currentScene->objects.end(); ++it) {
		BoxCollider* collider = (BoxCollider *)((*it)->getComponent("BoxCollider"));
		if (collider != nullptr) {
			if (collider->isInside(position)) {
				// collision detected
				return (*it);
			}
		}
	}
	// Not object found that collides with raycast, continue 
	if (range >= 0) {
		return castRaycast(position, direction, range - 0.1f);
	}
	return nullptr;
}