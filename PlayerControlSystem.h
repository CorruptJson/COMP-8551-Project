#pragma once

#include "EntityUserData.h"

class PlayerControlSystem {

public:
    PlayerControlSystem();
    ~PlayerControlSystem();
    void processEntity(EntityID id);
    //void Update(EntityCoordinator* coordinator);

private:

};
