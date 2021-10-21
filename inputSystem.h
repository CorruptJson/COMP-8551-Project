#pragma once
#include "system.h" 
#include "InputTracker.h"

class InputSystem : public System
{
public:
    InputSystem();
    //InputSystem(EntityCoordinator* coord);
    void preUpdate() override;
    void update() override;
};
