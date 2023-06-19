
#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "Scene.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
class Scene; // Forward declaration
struct Font {
    unsigned char** font;
    int w, h;
    int wchar, hchar;
};
extern Font* font;
extern Scene* currentScene;
enum MESH {
	CUBE
};


void setLightEnabled(bool enabled, int lightSource);
void changeLightPosition(Vector3f position, int lightSource);
SceneObject* instantiate(SceneObject* instance, Vector3f position, Vector3f rotation, Vector3f scale);

#endif