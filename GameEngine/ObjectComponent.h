#ifndef OBJECTCOMPONENT_H
#define OBJECTCOMPONENT_H

class SceneObject;
#include <string>

class ObjectComponent {
public:
    std::string name;
    SceneObject* object;
    ObjectComponent(SceneObject* object);
    virtual void update() = 0;
    virtual void start() = 0;
    virtual ObjectComponent* clone(SceneObject* parent) const = 0;
    std::string getName();
};

#endif
