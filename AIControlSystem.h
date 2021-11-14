#pragma once

#include "Animation.h"
#include "Renderer.h"
#include "EntityUserData.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"

class AIControlSystem
{
public:
    ~AIControlSystem();
    void processEntity(EntityID id);
    void handleWallCollision(EntityID id);
    //void Receive(Event e, void* args)
private:
    float speed = 2.5f;
    bool collided;

    void switchDirection(EntityID id);
};

