#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "ObjectComponent.h";
#include "Camera.h"
class CharacterController :public ObjectComponent {
private:
	void handleMovement();
	Camera* camera;
	double speed;
	double jumpForce;
	bool wasGrounded;
public:
	CharacterController(SceneObject* object);
	void update();
	void start();
	bool isGrounded();
	CharacterController* clone(SceneObject* parent) const;
};


#endif
