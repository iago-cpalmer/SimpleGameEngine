#ifndef MODELPARSER_H
#define MODELPARSER_H
#include "Vector3f.h"
#include "Vector3Int.h"
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "GlobalVariables.h"


struct Model {
    Vector3f* vertexs;
    Vector3f* textures;
    Vector3f* normals;
    Vector3Int** faces;
    unsigned int n_vertices;
    unsigned int n_textures;
    unsigned int n_normals;
    unsigned int n_faces;
};

extern Model cube;

Model* ParseObj(std::string filePath);
void loadFont(std::string fontPath, Font* font, int charSize);
void loadText(std::string text, GLuint* tid, Font* font, int startingX, int startingY);
void initTextures(GLuint* tid, std::string texturePath);
Model* getMesh(MESH mesh, Vector3f vo, Vector3f vf, float widthRatio, float heightRatio);
#endif // !MODELPARSER_H

