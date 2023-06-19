// Etapa 4
#include "Source.h"
#include "Vector3f.h"
#include <list>
#include "SceneObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "CharacterController.h"
#include "Time.h"
#include "GlobalVariables.h"
#include "InteractionController.h"
#include "LightInteractionHandler.h"
#include "NoteInteractionHandler.h"
#include "PlanetController.h"
#include "RobotArmController.h"
#include "AudioManager.h"

float currentWidth = 0, currentHeight = 0;

bool inWindow = true;

int selectedCamera = 1;

void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (selectedCamera == 1) {
		gluLookAt(currentScene->camera->position.x, currentScene->camera->position.y, currentScene->camera->position.z,
			currentScene->camera->rotation.x, currentScene->camera->rotation.y, currentScene->camera->rotation.z,
			currentScene->camera->upVector.x, currentScene->camera->upVector.y, currentScene->camera->upVector.z);
	}
	else {
		gluLookAt(16,-2,21, -60, -60, 90, 0, 1, 0);
	}
	

	for (auto elem : currentScene->objects) {
		if (elem->isActive) {

			MeshRenderer* meshRenderer = static_cast<MeshRenderer*>(elem->getComponent("MeshRenderer"));
			drawObj(meshRenderer, elem, 0);

			if (elem->tag._Equal("Candle") && ((LightInteractionHandler*)elem->getComponent("InteractionHandler"))->state==true) {
				glPushMatrix();
				glColor4f(1, 1, 0,1);
				glTranslatef(elem->position.x, elem->position.y, elem->position.z -0.1f);
				glRotatef(-90, 1, 0, 0);
				glutSolidCone(0.05f, 0.2, 6, 6);
				glPopMatrix();
			}

			// draw box colliders
#if DEBUG_MODE
			BoxCollider* boxCollider = static_cast<BoxCollider*>(elem->getComponent("BoxCollider"));
			if (boxCollider != nullptr) {
				// draw the mesh
				drawCube(boxCollider->xi, boxCollider->xf, boxCollider->yi, boxCollider->yf, boxCollider->zi, boxCollider->zf);
			}
			for (SceneObject* child : elem->getChilds()) {
				BoxCollider* boxCollider = static_cast<BoxCollider*>(child->getComponent("BoxCollider"));
				if (boxCollider != nullptr) {
					// draw the mesh
					drawCube(boxCollider->xi, boxCollider->xf, boxCollider->yi, boxCollider->yf, boxCollider->zi, boxCollider->zf);
				}
			}
#endif
		}

	}
	if (currentScene->currentUITexture != -1) {
		drawUI();
		glutSetCursor(GLUT_CURSOR_NONE);
		
	}
	else if (inWindow) {
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}
	else {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}

#if DEBUG_MODE
	drawAxis();
#endif

	glFlush();
	glutSwapBuffers();
}


void drawUI() {

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, currentScene->currentUITexture);
	// Translate and rotations to center the quad in the front of the screen
	glTranslatef(currentScene->camera->position.x,
		currentScene->camera->position.y,
		currentScene->camera->position.z);

	glRotatef(-currentScene->camera->yaw + 90, 0, 1, 0);
	glRotatef(-currentScene->camera->pitch, 1, 0, 0);

	glTranslatef(0, 0, -1);

	glScalef(1.25, 1.75f, 1.25);

	glBegin(GL_QUADS);

	glColor4f(1, 1,1, 1);

	glTexCoord2f(0,0.01f);
	glVertex3f(-0.5f, -0.47f, 0.0f);

	glTexCoord2f(0.80f,0.01f);
	glVertex3f(0.5f, -0.47f, 0.0f);

	glTexCoord2f(0.80f, 1);
	glVertex3f(0.5f, 0.47f, 0.0f);

	glTexCoord2f(0, 1);
	glVertex3f(-0.5f, 0.47f, 0.0f);
	
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();

}

void drawObj(MeshRenderer* mesh, SceneObject* base, int depth) {
	glPushMatrix();
	// Apply translation
	glTranslatef(-(base->pivot.x) * base->scale.x, -(base->pivot.y) * base->scale.y,
		-base->pivot.z * base->scale.z);

	glTranslatef(base->position.x, base->position.y,
		base->position.z);

	// Apply rotation
	glTranslatef((base->pivot.x) * base->scale.x, (base->pivot.y) * base->scale.y,
		base->pivot.z * base->scale.z);

	glRotatef(base->rotation.z, 0.0f, 0.0f, 1.0f);
	glRotatef(base->rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(base->rotation.x, 1.0f, 0.0f, 0.0f);

	glTranslatef(-(base->pivot.x) * base->scale.x, -(base->pivot.y) * base->scale.y,
		-base->pivot.z * base->scale.z);

	if (mesh != nullptr) {
		// draw the mesh
		drawMesh(mesh, base);
	}

	for (SceneObject* child : base->getChilds()) {
		MeshRenderer* childMesh = static_cast<MeshRenderer*>(child->getComponent("MeshRenderer"));
		drawObj(childMesh, child, depth + 1);
	}
	glPopMatrix();
}

void drawCube(float xi, float xf, float yi, float yf, float zi, float zf) {
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	glColor4f(0,1,0, 1);
	// Face 1
	glVertex3f(xi, yi, zi);
	glVertex3f(xi, yf, zi);
	glVertex3f(xf, yf, zi);
	glVertex3f(xf, yi, zi);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//Face 2
	glVertex3f(xi, yi, zf);
	glVertex3f(xi, yf, zf);
	glVertex3f(xi, yf, zi);
	glVertex3f(xi, yi, zi);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//Face 3
	glVertex3f(xi, yf, zf);
	glVertex3f(xf, yf, zf);
	glVertex3f(xf, yf, zi);
	glVertex3f(xi, yf, zi);
	glEnd();

	glBegin(GL_LINE_LOOP);
	// Face 4
	glVertex3f(xi, yi, zf);
	glVertex3f(xf, yi, zf);
	glVertex3f(xf, yi, zi);
	glVertex3f(xi, yi, zi);
	glEnd();

	glBegin(GL_LINE_LOOP);
	// Face 5
	glVertex3f(xi, yi, zf);
	glVertex3f(xi, yf, zf);
	glVertex3f(xf, yf, zf);
	glVertex3f(xf, yi, zf);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//Face 6
	glVertex3f(xf, yi, zf);
	glVertex3f(xf, yf, zf);
	glVertex3f(xf, yf, zi);
	glVertex3f(xf, yi, zi);
	glEnd();

	glPopMatrix();
}
void drawMesh(MeshRenderer* meshRenderer, SceneObject* obj) {
	Model* model = meshRenderer->model;
	// Set the material properties for the mesh
	GLfloat materialAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat materialDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialShininess = 32.0f;


	for (int i = 0; i < model->n_faces; i++)
	{
		Vector3Int vertices = model->faces[i][0];
		Vector3Int textures = model->faces[i][1];
		Vector3Int normals = model->faces[i][2];

		Vector3f v1n = model->normals[normals.x - 1];
		Vector3f v2n = model->normals[normals.y - 1];
		Vector3f v3n = model->normals[normals.z - 1];

		Vector3f v1 = model->vertexs[vertices.x - 1];
		Vector3f v2 = model->vertexs[vertices.y - 1];
		Vector3f v3 = model->vertexs[vertices.z - 1];

		Vector3f v1t = model->textures[textures.x - 1];
		Vector3f v2t = model->textures[textures.y - 1];
		Vector3f v3t = model->textures[textures.z - 1];
		
#if DEBUG_MODE
		glPushMatrix();
		glScalef(obj->scale.x / 2, obj->scale.y / 2, obj->scale.z / 2);
		glBegin(GL_LINES);
		{

			// Vertex 1
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v1.x + v1n.x, v1.y + v1n.y, v1.z + v1n.z);

			// Vertex 2
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v2.x + v2n.x, v2.y + v2n.y, v2.z + v2n.z);

			// Vertex 3
			glVertex3f(v3.x, v3.y, v3.z);
			glVertex3f(v3.x + v3n.x, v3.y + v3n.y, v3.z + v3n.z);
		}
		glEnd();

		glPopMatrix();
#endif		

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, meshRenderer->textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glScalef(obj->scale.x / 2, obj->scale.y / 2, obj->scale.z / 2);
		glBegin(GL_TRIANGLES);
		glColor4f(1, 1, 1, 1);


		GLfloat materialAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat materialDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat materialShininess = 32.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glNormal3f(v1n.x, v1n.y, v1n.z);
		glTexCoord2f(v1t.x, v1t.y);
		glVertex3f((GLfloat)v1.x, (GLfloat)v1.y, (GLfloat)v1.z);


		glNormal3f(v2n.x, v2n.y, v2n.z);
		glTexCoord2f(v2t.x, v2t.y);
		glVertex3f((GLfloat)v2.x, (GLfloat)v2.y, (GLfloat)v2.z);



		glNormal3f(v3n.x, v3n.y, v3n.z);
		glTexCoord2f(v3t.x, v3t.y);
		glVertex3f((GLfloat)v3.x, (GLfloat)v3.y, (GLfloat)v3.z);


		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);


	}
}
void drawAxis() {
	glPushMatrix();
	// Eje x
	glBegin(GL_LINES);
	glColor4f(0, 0, 1,1);


	glVertex3f(10, 0, 0);
	glVertex3f(-10, 0, 0);

	glEnd();

	// Eje y
	glBegin(GL_LINES);
	glColor4f(0, 1, 0,1);

	glVertex3f(0, 10, 0);
	glVertex3f(0, -10, 0);

	glEnd();

	// Eje z
	glBegin(GL_LINES);
	glColor4f(1, 0, 0, 1);

	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, -10);

	glEnd();

	glPopMatrix();
}

// Funciï¿½n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME) - lastFrameTime;
	updateDeltaTime += elapsedTime;
	frameDeltaTime += elapsedTime;

	lastFrameTime = glutGet(GLUT_ELAPSED_TIME);

	if (updateDeltaTime > MIN_UPDATE_TIME) {
		for (auto elem : currentScene->objects) {
			if (!elem->tag._Equal("ScenarioElement") && elem->position.distance(currentScene->camera->position) >= 70) {
				elem->isActive = false;
			}
			else {
				elem->isActive = true;
				elem->update();
			}
		}
		Time::deltaTime = elapsedTime * 0.001; // Convert to seconds
		//std::cout << 1 / Time::deltaTime << std::endl;
		updateDeltaTime -= MIN_UPDATE_TIME;
		InputManager::resetKeys();
	}
	if (frameDeltaTime > MIN_FRAME_TIME) {
		glutPostRedisplay();
		frameDeltaTime -= MIN_FRAME_TIME;
	}
}



void PassiveMotionHandler(int x, int y) {
	if (inWindow) {
		// Keep the mouse in the center of the screen
		glutWarpPointer(currentWidth / 2, currentHeight / 2);

		// Calculate the distance between the center of the screen and the mouse position
		float deltaX = (int)((x - currentWidth / 2)) * sensitivity;
		float deltaY = (int)(y - currentHeight / 2) * sensitivity;
		// Add it to the yaw and pitch
		currentScene->camera->yaw += deltaX; // rotation in y-axis
		currentScene->camera->pitch += deltaY; // rotation in x-axis

		// Clamp the pitch to prevent flipping the camera when looking up or down
		if (currentScene->camera->pitch > 89.0f) {
			currentScene->camera->pitch = 89.0f;
		}
		else if (currentScene->camera->pitch < -89.0f) {
			currentScene->camera->pitch = -89.0f;
		}
	}
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	currentHeight = (float)height;
	currentWidth = (float)width;
	// Calculate the aspect ratio of the window
	float aspect_ratio = (float)width / (float)height;
	float desired_aspect_ratio = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (aspect_ratio < desired_aspect_ratio)
	{
		fovY = 2.0f * atan(tan(fovY * 0.5f * M_PI / 180.0f) / aspect_ratio) * 180.0f / M_PI;
	}

	gluPerspective(fovY, aspect_ratio, zNear, zFar);

	// Return to the model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void createWalls(std::list<SceneObject*>* sceneObjects) {
	SceneObject* wallPrefab = new SceneObject(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(35, 20, 1), Vector3f(0.5f, 0.5f, 0.5f), "ScenarioElement", nullptr, 0);
	wallPrefab->addComponent(new MeshRenderer(wallPrefab, CUBE, "res/textures/wall-texture.png", 0.96f, 0.54f));
	wallPrefab->addComponent(new BoxCollider(wallPrefab, Vector3f(0, 0, 0), Vector3f(1, 1, 1), wallPrefab->scale));
	
	// Walls
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(45, 0, 35), Vector3f(0, 0, 0), Vector3f(1, 20, 35)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(35, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(45, 0, 0), Vector3f(0, 0, 0), Vector3f(45, 20, 1)));

	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(-35, 0, 20), Vector3f(0, 0, 0), Vector3f(1, 20, 20)));

	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(45, 0, 75), Vector3f(0, 0, 0), Vector3f(1, 20, 40)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(45, 0, 110), Vector3f(0, 0, 0), Vector3f(1, 20, 35)));

	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(0, 0, 20), Vector3f(0, 0, 0), Vector3f(35, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(20, 0, 20), Vector3f(0, 0, 0), Vector3f(20, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(20, 0, 55), Vector3f(0, 0, 0), Vector3f(1, 20, 35)));

	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(-35, 0, 55), Vector3f(0, 0, 0), Vector3f(1, 20, 35)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(-35, 0, 90), Vector3f(0, 0, 0), Vector3f(1, 20, 35)));

	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(-25, 0, 90), Vector3f(0, 0, 0), Vector3f(10, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(45, 0, 110), Vector3f(0, 0, 0), Vector3f(35, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(-25, 0, 90), Vector3f(0, 90, 0), Vector3f(20, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(10, 0, 110), Vector3f(0, 0, 0), Vector3f(35, 20, 1)));
	sceneObjects->push_back(instantiate(wallPrefab, Vector3f(20, 0, 90), Vector3f(0, 0, 0), Vector3f(1, 20, 35)));

	// Ceiling
	SceneObject* ceiling = new SceneObject(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(90, 1, 110), Vector3f(0, 0, -0.5f), "ScenarioElement", nullptr, 0);
	ceiling->addComponent(new MeshRenderer(ceiling, CUBE, "res/textures/ceiling-texture.png", 1, 1));
	ceiling->addComponent(new BoxCollider(ceiling, Vector3f(0, 0, 0), Vector3f(1, 1, 1), ceiling->scale));
	sceneObjects->push_back(ceiling);
	// Floor
	SceneObject* floor = new SceneObject(Vector3f(0, -20, 0), Vector3f(0, 0, 0), Vector3f(90, 1, 110), Vector3f(0, 0, -0.5f), "ScenarioElement", nullptr, 0);
	floor->addComponent(new MeshRenderer(floor, CUBE, "res/textures/floor-texture.png", 1, 1));
	floor->addComponent(new BoxCollider(floor, Vector3f(0, 0, 0), Vector3f(1, 1, 1), floor->scale));
	sceneObjects->push_back(floor);
}

void initTable(int id, std::list<SceneObject*>* sceneObjects, float x, float y, float z, int yRotation) {
	std::string texPath = "res/textures/characters/" + std::to_string(id);
	texPath.append(".png");
	std::string notePath = "res/textures/characters/notes/" + std::to_string(id);
	notePath.append(".png");
	// Table
	SceneObject* tablePrefab = new SceneObject(Vector3f(x, y, z), Vector3f(0, yRotation, 0), Vector3f(6, 6, 5), Vector3f(0, -0.5f, 0), "Table", new FLAG(IS_INTERACTABLE), 1);
	tablePrefab->addComponent(new MeshRenderer(tablePrefab, "res/models/table.obj", "res/textures/wood.png"));
	tablePrefab->addComponent(new BoxCollider(tablePrefab, Vector3f(0, 0, 0), Vector3f(1, 1, 1), tablePrefab->scale));
	tablePrefab->addComponent(new NoteInteractionHandler(tablePrefab, notePath));
	// Note
	SceneObject* notePrefab = new SceneObject(Vector3f(0, 3.1f, -0.75f), Vector3f(0, 0, 0), Vector3f(2, 0.1f, 2.5f), Vector3f(0, 0, 0), "Note", new FLAG(IS_INTERACTABLE), 1);
	notePrefab->addComponent(new MeshRenderer(notePrefab, "res/models/PaperNote.obj", "res/textures/PaperNote.png"));
	notePrefab->addComponent(new BoxCollider(notePrefab, Vector3f(0, 0, 0), Vector3f(1, 1, 1), notePrefab->scale));
	tablePrefab->addChild(notePrefab);

	//Frame
	SceneObject* framePrefab = new SceneObject(Vector3f(0, 9, 2.5f), Vector3f(0, 0, 0), Vector3f(5, 7, 0.1f), Vector3f(0, 0, 0), "Frame", new FLAG(IS_INTERACTABLE), 1);
	framePrefab->addComponent(new MeshRenderer(framePrefab, "res/models/frame.obj", texPath));
	framePrefab->addComponent(new BoxCollider(framePrefab, Vector3f(0, 0, 0), Vector3f(1, 1, 1), framePrefab->scale));
	tablePrefab->addChild(framePrefab);

	sceneObjects->push_back(tablePrefab);
}

void initSolarSystem(std::list<SceneObject*>* sceneObjects) {
	SceneObject* spherePrefab = new SceneObject(Vector3f(-10, -14, 55), Vector3f(0, 0, 0), Vector3f(5, 5, 5), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	spherePrefab->addComponent(new MeshRenderer(spherePrefab, "res/models/sphere.obj", "res/textures/sun.png"));
	spherePrefab->addComponent(new LightInteractionHandler(spherePrefab, 3));
	sceneObjects->push_back(spherePrefab);

	SceneObject* planet = new SceneObject(Vector3f(2.88, 0, 2.88), Vector3f(0, 0, 0), Vector3f(0.2, 0.2, 0.2), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/mercury.png"));
	planet->addComponent(new PlanetController(planet, 2.5f + 0.38f, 47, 10.83));
	spherePrefab->addChild(planet);

	planet = new SceneObject(Vector3f(4, 0, 4), Vector3f(0, 0, 0), Vector3f(0.45, 0.45, 0.45), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/venus.png"));
	planet->addComponent(new PlanetController(planet, 4, 35, 6.52));
	spherePrefab->addChild(planet);

	planet = new SceneObject(Vector3f(5, 0, 5), Vector3f(0, 0, 0), Vector3f(0.5,0.5,0.5), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/earth.png"));
	planet->addComponent(new PlanetController(planet, 5, 29, 1574));
	spherePrefab->addChild(planet);
	// Moon:
	SceneObject* moon = new SceneObject(Vector3f(1,0,1), Vector3f(0, 0, 0), Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	moon->addComponent(new MeshRenderer(moon, "res/models/sphere.obj", "res/textures/moon.png"));
	moon->addComponent(new PlanetController(moon, 1, 29, 1574));
	planet->addChild(moon);

	planet = new SceneObject(Vector3f(6,0,6), Vector3f(0, 0, 0), Vector3f(0.25, 0.25, 0.25), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/mars.png"));
	planet->addComponent(new PlanetController(planet, 6, 24, 866));
	spherePrefab->addChild(planet);

	planet = new SceneObject(Vector3f(9, 0, 9), Vector3f(0, 0, 0), Vector3f(3,3,3), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/jupyter.png"));
	planet->addComponent(new PlanetController(planet, 9, 13, 4500));
	spherePrefab->addChild(planet);
	moon = new SceneObject(Vector3f(1.6, 0, 1.6), Vector3f(0, 0, 0), Vector3f(0.25, 0.25, 0.25), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	moon->addComponent(new MeshRenderer(moon, "res/models/african_head.obj", "res/textures/african_head2.png"));
	moon->addComponent(new PlanetController(moon, 1.75, 29, 1574));
	planet->addChild(moon);

	planet = new SceneObject(Vector3f(14,0, 14), Vector3f(0, 0, 0), Vector3f(2.2, 2.2, 2.2), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/saturn.png"));
	planet->addComponent(new PlanetController(planet, 14, 9.69, 3600));
	spherePrefab->addChild(planet);
	SceneObject* ring = new SceneObject(Vector3f(0,0,0), Vector3f(0, 0, 0), Vector3f(4, 0.1f, 4), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	ring->addComponent(new MeshRenderer(ring, "res/models/ring.obj", "res/textures/saturn-ring.png"));
	ring->addComponent(new PlanetController(ring, 0, 29, 1574));
	planet->addChild(ring);

	planet = new SceneObject(Vector3f(17,0,17), Vector3f(0, 0, 0), Vector3f(1.25, 1.25, 1.25), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/uranus.png"));
	planet->addComponent(new PlanetController(planet, 17, 6.81, 1400));
	spherePrefab->addChild(planet);


	planet = new SceneObject(Vector3f(19,0,19), Vector3f(0, 0, 0), Vector3f(1,1,1), Vector3f(0, 0, 0), "Sphere", nullptr, 0);
	planet->addComponent(new MeshRenderer(planet, "res/models/sphere.obj", "res/textures/neptune.png"));
	planet->addComponent(new PlanetController(planet, 19, 5.43, 970));
	spherePrefab->addChild(planet);
}

void initTables(std::list<SceneObject*>* sceneObjects) {

	initTable(13, sceneObjects, -7, -19.5f, 16.5, 0);
	initTable(14, sceneObjects, -7, -19.5f, 2.5, 180);

	initTable(1, sceneObjects, 13, -19.5f, 16.5, 0);
	initTable(2, sceneObjects, 3, -19.5f, 16.5, 0);
	initTable(3, sceneObjects, 13, -19.5f, 2.5, 180);
	initTable(4, sceneObjects, 3, -19.5f, 2.5, 180);

	initTable(5, sceneObjects, 41.5, -19.5f, 10, 90);
	initTable(6, sceneObjects, 41.5, -19.5f, 20, 90);
	initTable(7, sceneObjects, 41.5, -19.5f, 30, 90);
	initTable(8, sceneObjects, 41.5, -19.5f, 40, 90);
	initTable(9, sceneObjects, 41.5, -19.5f, 50, 90);
	initTable(10, sceneObjects, 41.5, -19.5f, 60, 90);
	initTable(11, sceneObjects, 41.5, -19.5f, 70, 90);
	initTable(12, sceneObjects, 41.5, -19.5f, 80, 90);


	initTable(15, sceneObjects, 22.5, -19.5f, 25, -90);
	initTable(16, sceneObjects, 22.5, -19.5f, 35, -90);
	initTable(17, sceneObjects, 22.5, -19.5f, 45, -90);
	initTable(18, sceneObjects, 22.5, -19.5f, 55, -90);
	initTable(19, sceneObjects, 22.5, -19.5f, 65, -90);
	initTable(20, sceneObjects, 22.5, -19.5f, 75, -90);
}

void initScene() {
	std::list<SceneObject*> sceneObjects;
	// Player position, rotation, scale, pivot,"Player", nullptr
	SceneObject* obj1 = new SceneObject(Vector3f(-12,-5, 10), Vector3f(0, 0, 0), Vector3f(1, 1, 1), Vector3f(0, -0.5f, 0), "Player", nullptr, 0);
	//Init Camera
	//									Position		LookAt dir		 UpVector
	Camera* camera = new Camera(Vector3f(0,0,0), Vector3f(0, 0, 0), Vector3f(0, 1, 0), obj1);
	sceneObjects.push_back(camera);
	currentScene = new Scene(sceneObjects, camera);
	obj1->addComponent(new InteractionController(obj1));
	obj1->addComponent(new CharacterController(obj1));
	currentScene->objects.push_back(obj1);

	createWalls(&currentScene->objects);
	
	SceneObject* qrFrame = new SceneObject(Vector3f(-34.75, -10, 10), Vector3f(90, 0, -90), Vector3f(10, 10, 0.1), Vector3f(0, 0, 0), "Frame", nullptr, 0);
	qrFrame->addComponent(new MeshRenderer(qrFrame, "res/models/frame.obj", "res/textures/qr.png"));
	currentScene->objects.push_back(qrFrame);
	
	// Candles
	SceneObject* lampPrefab = new SceneObject(Vector3f(11.5, -10.5, 18.5), Vector3f(0, 90, 0), Vector3f(1,3,1), Vector3f(0,0.5f,0), "Candle", new FLAG(IS_INTERACTABLE), 1);
	lampPrefab->addComponent(new MeshRenderer(lampPrefab, "res/models/candle.obj", "res/textures/candle.png"));
	lampPrefab->addComponent(new BoxCollider(lampPrefab, Vector3f(0, 0, 0), Vector3f(1, 1, 1), lampPrefab->scale));
	lampPrefab->addComponent(new LightInteractionHandler(lampPrefab, 0)); 
	currentScene->objects.push_back(lampPrefab);
	currentScene->objects.push_back(instantiate(lampPrefab, Vector3f(43.5,-10.5,8.5f), Vector3f(0,90,0), Vector3f(1,3,1)));
	currentScene->objects.push_back(instantiate(lampPrefab, Vector3f(21.5, -10.5f, 43.5f), Vector3f(0,90,0), Vector3f(1,3,1)));
	
	// Tables
	initTables(&currentScene->objects);

	// Solar system
	initSolarSystem(&currentScene->objects);
	// Barrier
	
	SceneObject* barrier = new SceneObject(Vector3f(8.5, -19.5, 75), Vector3f(0, 0, 0), Vector3f(20, 7, 1), Vector3f(0, -0.5f, 0), "Barrier", nullptr, 0);
	barrier->addComponent(new MeshRenderer(barrier, "res/models/barrier.obj", "res/textures/barrier.png"));
	barrier->addComponent(new BoxCollider(barrier, Vector3f(0, 0, 0), Vector3f(1, 1, 1), barrier->scale));
	currentScene->objects.push_back(barrier);
	currentScene->objects.push_back(instantiate(barrier, Vector3f(-20.5, -19.5, 75), Vector3f(0, 0, 0), Vector3f(20, 7, 1)));
	currentScene->objects.push_back(instantiate(barrier, Vector3f(-5.5, -19.5, 78), Vector3f(0, 0, 0), Vector3f(20, 7, 1)));
	
	// Robot arm
	SceneObject* baseRobotArm = new SceneObject(Vector3f(-17, -19.5, 100), Vector3f(0, -90, 0), Vector3f(4, 1, 4), Vector3f(0, -0.5f, 0), "RobotArm", nullptr, 0);
	baseRobotArm->addComponent(new MeshRenderer(baseRobotArm, CUBE, "res/textures/metal.png", 1, 1));
	currentScene->objects.push_back(baseRobotArm);

	SceneObject* arm = new SceneObject(Vector3f(0, 0.5f, 0), Vector3f(0, 0, 0), Vector3f(2, 4, 2), Vector3f(0, -0.5f, 0), "RobotArm", nullptr, 0);
	arm->addComponent(new MeshRenderer(arm, CUBE, "res/textures/metal.png", 1, 1));
	arm->addComponent(new RobotArmController(arm, Vector3f(10,10,10), Vector3f(-45, -45, -45), Vector3f(45, 45, 45)));
	baseRobotArm->addChild(arm);
	
	SceneObject* forearm = new SceneObject(Vector3f(0, 2, 0), Vector3f(0,0, 0), Vector3f(1, 4, 1), Vector3f(0, -0.5f, 0), "RobotArm", nullptr, 0);
	forearm->addComponent(new MeshRenderer(forearm, CUBE, "res/textures/metal.png", 1, 1));
	forearm->addComponent(new RobotArmController(forearm, Vector3f(20, 20, 20), Vector3f(-45, -45, -45), Vector3f(45, 45, 45)));
	arm->addChild(forearm);

	SceneObject* hand = new SceneObject(Vector3f(0, 2, 0), Vector3f(0, 0, 0), Vector3f(2, 2, 0.5f), Vector3f(0, -0.5f, 0), "RobotArm", nullptr, 0);
	hand->addComponent(new MeshRenderer(hand, CUBE, "res/textures/metal.png", 1, 1));
	hand->addComponent(new RobotArmController(hand, Vector3f(30, 30, 30), Vector3f(-90,-90,-90), Vector3f(90,90,90)));
	forearm->addChild(hand);

	SceneObject* fingerPrefab = new SceneObject(Vector3f(0.5f, 1, 0), Vector3f(0, 0, 0), Vector3f(0.5f, 1, 0.5f), Vector3f(0, -0.5f, 0), "RobotArm", nullptr, 0);
	fingerPrefab->addComponent(new MeshRenderer(fingerPrefab, CUBE, "res/textures/metal.png", 1, 1));
	fingerPrefab->addComponent(new RobotArmController(fingerPrefab, Vector3f(50, 0, 0)));
	hand->addChild(fingerPrefab);
	
	SceneObject* fingerTop = new SceneObject(Vector3f(0, 0.5f, 0), Vector3f(0, 0, 0), Vector3f(0.5f, 1, 0.5f), Vector3f(0, -0.5f, 0), "RobotArm", nullptr, 0);
	fingerTop->addComponent(new MeshRenderer(fingerTop, CUBE, "res/textures/metal.png", 1, 1));
	fingerTop->addComponent(new RobotArmController(fingerTop, Vector3f(70, 0, 0)));
	fingerPrefab->addChild(fingerTop);
	
	hand->addChild(instantiate(fingerPrefab, Vector3f(1, 0, 0), Vector3f(0,0,-45), Vector3f(0.5f, 1, 0.5f)));
	hand->addChild(instantiate(fingerPrefab, Vector3f(-1, 0, 0), Vector3f(0,0,45), Vector3f(0.5f, 1, 0.5f)));
	hand->addChild(instantiate(fingerPrefab, Vector3f(-0.5f, 1, 0), Vector3f(0, 0, 0), Vector3f(0.5f, 1, 0.5f)));
	
	SceneObject* glass = new SceneObject(Vector3f(-20, -19.5, 99.5f), Vector3f(0, 0, 0), Vector3f(20, 20, 20), Vector3f(0, -0.5f, 0), "Glass", nullptr, 0);
	glass->addComponent(new MeshRenderer(glass, CUBE, "res/textures/glass.png", 1, 1));
	glass->addComponent(new BoxCollider(glass, Vector3f(0, 0, 0), Vector3f(1, 1, 1), glass->scale));
	currentScene->objects.push_back(glass);
	
}

void start() {
	// Call their starts methods
	for (auto elem : currentScene->objects) {
		elem->start();
	}
}

void KeyboardUpHandler(unsigned char key, int x, int y) {
	InputManager::Key _key = static_cast<InputManager::Key>(key - 0);
	if (InputManager::keyExists(_key)) {
		InputManager::setKeyState(_key, UP);
	}

	int nkey = key - 48;
	if (nkey == 1 || nkey == 2) {
		selectedCamera = nkey;
	}

	if (key - 0 == 27) {
		if (currentScene->currentUITexture != -1) {
			currentScene->currentUITexture = -1;
		}
		else {
			inWindow = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}

	}
}

void KeyboardHandler(unsigned char key, int x, int y) {
	std::cout;
	InputManager::Key _key = static_cast<InputManager::Key>(key - 0);
	if (InputManager::keyExists(_key)) {
		InputManager::setKeyState(_key, PRESSED);
	}
}

void MouseHandler(int button, int state, int x, int y) {
	if (!inWindow && button == GLUT_LEFT_BUTTON && state == GLUT_UP && x > 0 && x < currentWidth && y>0 && y < currentHeight) {
		inWindow = true;
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}
}

void initLights() {
	// Attributes for each light to simulate lamp lighting
	GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	GLfloat lightDiffuse[] = { 0.9f, 0.8f, 0.4f, 1.0f };
	GLfloat lightSpecular[] = { 0.9f, 0.8f, 0.4f, 1.0f };
	GLfloat lightInitPosition[] = { -30, -30, -30, 1.0f };

	for (int i = 0; i < 4; i++) {
		glEnable(GL_LIGHT0 + i);

		glLightfv(GL_LIGHT0+i, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, lightSpecular);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, lightInitPosition);

		glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, 0.05f);
		glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 0.001f);
	}
}

void initFonts() {
	// Load fonts (possible upgrade to do: load .xml files to get the info of each font)
	font = new Font[1];
	loadFont("res/fonts/font.png", &font[0], 32);
}

void initFog() {
	GLfloat fogColor[] = { 0,0,0, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.5f);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 70.0f);
}

void init() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE); 
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	initLights();
	// Fog
	glEnable(GL_FOG);
	initFog();
}

int main(int argc, char** argv) {
	initFonts();
	// Inicializamos la librerï¿½a GLUT
	glutInit(&argc, argv);


	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);

	// Creamos la nueva ventana
	glutCreateWindow("Etapa 6");

	init();
	initScene();
	start();
	
	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);

	glutPassiveMotionFunc(PassiveMotionHandler);
	glutKeyboardUpFunc(KeyboardUpHandler);
	glutKeyboardFunc(KeyboardHandler);
	glutMouseFunc(MouseHandler);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gluPerspective(1, 1, zNear, zFar);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	std::cout << "main loop starts" << std::endl;
	// Comienza la ejecuciï¿½n del core de GLUT
	glutMainLoop();
	return 0;
}