#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "ObjectComponent.h";
#include "Vector3Int.h"
#include "ModelParser.h"
#include "GlobalVariables.h"

class MeshRenderer:public ObjectComponent {
	
public:
	MeshRenderer(SceneObject* object, std::string filePath, std::string texturePath);
	MeshRenderer(SceneObject* object, MESH mesh, std::string texturePath, float widthRatio, float heightRatio);
	MeshRenderer(SceneObject* object, Model* model, int textureId);
	Model* model;
	unsigned int textureId;
	void update();
	void start();
	MeshRenderer* clone(SceneObject* parent) const;
};

#endif
