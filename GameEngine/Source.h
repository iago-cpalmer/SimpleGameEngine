#ifndef SOURCE_H
#define SOURCE_H



#define _USE_MATH_DEFINES

#define DEBUG_MODE 0


#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <map>
#include "ModelParser.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
//these two headers are already included in the <Windows.h> header

#define W_WIDTH 800
#define W_HEIGHT 600

double fovY = 80;

float sensitivity = 0.5f;

double zNear = 0.1f;
double zFar = 120;


double updateDeltaTime = 0;
double frameDeltaTime = 0;

int const FPS = 120;
int const UPS = 120;

double const MIN_UPDATE_TIME = 1000 / UPS;
double const MIN_FRAME_TIME = 1000 / FPS;

int lastFrameTime;
void drawObj(MeshRenderer* mesh, SceneObject* base, int depth);
void drawUI();
void drawCube(float xi, float xf, float yi, float yf, float zi, float zf);
void drawMesh(MeshRenderer* meshRenderer, SceneObject* obj);
void Display(void);
void drawAxis();
void drawCube();
void drawTeapot();
void Idle(void);
float radians(float x);
void PassiveMotionHandler(int x, int y);
void Reshape(int width, int height);
void KeyboardUpHandler(unsigned char key, int x, int y);
void MouseHandler(int button, int state, int x, int y);

#endif // !SOURCE_H