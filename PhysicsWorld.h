#pragma once

#include "Libraries/include/box2d/box2d.h"
#include "Types.h"
#include "PhysicsWorld.h"
#include "PhysicsComponent.h"
#include "ContactListener.h"
#include "EntityCoordinator.h"

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void Update();
    void AddObject(EntityCoordinator* coordinator);
private:
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 10;
    const int positionIterations = 3;

    b2Vec2* gravity;
    b2World* world;
    //ContactListener* contactListener;
};
