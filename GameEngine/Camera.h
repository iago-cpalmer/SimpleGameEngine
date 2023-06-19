#ifndef CAMERA_H
#define CAMERA_H

#include "SceneObject.h"
#include "Vector3f.h"
#include "Raycast.h"
#include "GlobalVariables.h"

class Camera :public SceneObject {
private:
	void look();
public:
	Vector3f upVector;
	SceneObject* player;
	float pitch;
	float yaw;
	// The rotation vector will work as the position that the camera is looking at
	Camera(Vector3f position, Vector3f rotation, Vector3f upVector, SceneObject* player);
	void update() override;
	void start() override;
	void initComponents();
	Vector3f getCameraForwardVector();
	Vector3f getCameraRightVector();
};
#endif
