#ifndef RAYCAST_H
#define RAYCAST_H

#include "SceneObject.h"
#include <list>
#include "BoxCollider.h"
#include "GlobalVariables.h"

SceneObject* castRaycast(Vector3f position, Vector3f direction, float range);

#endif