#pragma once

#include "Libraries/include/box2d/box2d.h"
#include <iostream>

class Box2DTutorial {
public:
    Box2DTutorial();
    ~Box2DTutorial();
    
    void Update();
    void GravityExample();

private:
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 6;
    const int positionIterations = 2;

    b2Vec2* gravity;
    b2World* world;
    b2Body* body;
};
