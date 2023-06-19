#include "CharacterController.h"
#include <iostream>
#include "InputManager.h"
#include "Vector3f.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "Raycast.h"
#include "AudioManager.h"

CharacterController::CharacterController(SceneObject* object) :ObjectComponent(object) {
	this->name = "CharacterController";
	this->camera = currentScene->camera;
	this->speed = 7;
	this->jumpForce = 0;
	this->wasGrounded = true;
	openSound(L"step.mp3");
}

void CharacterController::update() {
	handleMovement();
}

void CharacterController::start() {
	playSound();
}

CharacterController* CharacterController::clone(SceneObject* parent) const
{
	return new CharacterController(parent);
}
bool CharacterController::isGrounded() {
	Vector3f v = object->position;
	SceneObject* collision = castRaycast(v, Vector3f(0, -1, 0), 9);
	if (collision != nullptr) {
		return true;
	}
	return false;
}

void CharacterController::handleMovement() {
	Vector3f direction = { 0,0,0 };

	if (InputManager::isKeyPressed(InputManager::Forward)) {
		direction.z += 1;
	}
	if (InputManager::isKeyPressed(InputManager::Backward)) {
		direction.z -= 1;
	}
	if (InputManager::isKeyPressed(InputManager::Left)) {
		direction.x -= 1;
	}
	if (InputManager::isKeyPressed(InputManager::Right)) {
		direction.x += 1;
	}

	// Calculate the camera's forward and right vectors
	Vector3f cameraForward = camera->getCameraForwardVector();
	Vector3f cameraRight = camera->getCameraRightVector();

	// Calculate the movement direction vector based on the camera's forward and right vectors
	Vector3f movementDirection = {
		cameraForward.x * direction.z + cameraRight.x * direction.x,
		cameraForward.y * direction.z + direction.y,
		cameraForward.z * direction.z + cameraRight.z * direction.x
	};

	// Normalize the movement direction vector to ensure a consistent movement speed regardless of the direction
	float magnitude = sqrt(movementDirection.x * movementDirection.x + movementDirection.z * movementDirection.z);
	if (magnitude > 0) {
		movementDirection.x /= magnitude;
		movementDirection.z /= magnitude;
		resumeSound();
	}
	else {
		pauseSound();
	}
	// Check collisions at front
	Vector3f v = object->position;
	SceneObject* collision;
	bool colisioning = false;
	for (int i = 0; i > -3; i--) {
		v.y -= 1;
		collision = castRaycast(v, movementDirection, 0.1f);
		if (collision != nullptr) {
			colisioning = true;
			break;
		}
	}

	if (!colisioning) {
		object->position.x += movementDirection.x * Time::deltaTime * speed;
		object->position.z += movementDirection.z * Time::deltaTime * speed;
	}

	// Manage jump and gravity
	bool _isGrounded = isGrounded();
	collision = castRaycast(object->position, Vector3f(0, 1, 0), 0.5f);
	if (collision != nullptr) {
		jumpForce = 0;
	}
	if (InputManager::isKeyPressed(InputManager::Up) && _isGrounded) {
		jumpForce = 30;
	}
	else if (_isGrounded) {
		jumpForce = 0;
	}
	else {
		jumpForce -= 2;
		wasGrounded = false;
	}

	if (!wasGrounded && _isGrounded) {
		wasGrounded = true;
	}
	
	object->position.y += jumpForce * Time::deltaTime;

	if (object->position.y <= -20) {
		object->position.y = -5;
	}
}