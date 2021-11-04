#pragma once

#include "EntityUserData.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"

class PlayerControlSystem {

public:
    PlayerControlSystem();
    ~PlayerControlSystem();
    void processEntity(EntityID id, PhysicsWorld* physicsWorld);
    //void Update(EntityCoordinator* coordinator);

private:

};
