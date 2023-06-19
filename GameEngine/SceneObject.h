#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

class ObjectComponent;
#include "Vector3f.h"
#include <map>
#include <string>
#include <list>

enum FLAG {
    IS_INTERACTABLE
};

class SceneObject {
public:
    Vector3f position;
    Vector3f rotation;
    Vector3f pivot;
    Vector3f scale;
    FLAG* flags;
    int nFlags;
    std::string tag;
    std::map<std::string, ObjectComponent*> components;
    std::list<SceneObject*> childs;
    bool isActive;

    SceneObject(Vector3f position, Vector3f rotation, Vector3f scale, Vector3f pivot, std::string tag, FLAG* flags, int nFlags);
    virtual void update();
    virtual void start();
    void addComponent(ObjectComponent* oc);
    ObjectComponent* getComponent(std::string name);
    void addChild(SceneObject* o);
    std::list<SceneObject* > getChilds();
    bool hasFlag(FLAG flag);
    std::list<ObjectComponent*> getComponents();
};

#endif