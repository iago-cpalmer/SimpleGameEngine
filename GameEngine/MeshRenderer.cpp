#include "MeshRenderer.h"
#include <iostream>


MeshRenderer::MeshRenderer(SceneObject* object, std::string filePath, std::string texturePath) :ObjectComponent(object) {
	this->name = "MeshRenderer";
	this->model = ParseObj(filePath);
	initTextures(&textureId, texturePath);
}

MeshRenderer::MeshRenderer(SceneObject* object, MESH mesh, std::string texturePath, float widthRatio, float heightRatio) :ObjectComponent(object) {
	this->name = "MeshRenderer";

	this->model = getMesh(mesh,
		Vector3f(1,1,1)
		, Vector3f(-1,-1,-1), widthRatio, heightRatio);
	initTextures(&textureId, texturePath);
}

MeshRenderer::MeshRenderer(SceneObject* object, Model* model, int textureId):ObjectComponent(object)
{
	this->name = "MeshRenderer";
	this->model = model;
	this->object = object;
	this->textureId = textureId;
}

void MeshRenderer::update() {
}

void MeshRenderer::start() {
}

MeshRenderer* MeshRenderer::clone(SceneObject* parent) const
{
	return new MeshRenderer(parent, this->model, this->textureId);
}
