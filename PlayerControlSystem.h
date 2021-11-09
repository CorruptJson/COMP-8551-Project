#pragma once

#include "EntityUserData.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"

class PlayerControlSystem {

public:
    ~PlayerControlSystem();
    void processEntity(EntityID id);
    //void Update(EntityCoordinator* coordinator);

private:
};
