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
    //void Receive(Event e, void* args);
private:



    void handleWallCollision(EntityID id);
    void switchDirection(EntityID id);
};

