#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <math.h>
#define _USE_MATH_DEFINES
#include "ObjectComponent.h";
#include "Vector3f.h"
#include "SceneObject.h"

class BoxCollider :public ObjectComponent {

public:
	BoxCollider(SceneObject* object, Vector3f vo, Vector3f vf, Vector3f scale);
	float xi, xf, yi, yf, zi, zf;
	Vector3f scale;
	void update();
	void start();
	bool isInside(Vector3f posToCheck);
	BoxCollider* clone(SceneObject* parent) const;
};

#endif
