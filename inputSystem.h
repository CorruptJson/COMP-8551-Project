#pragma once
#include "system.h" 
#include "InputTracker.h"
#include "GameManager.h"

class InputSystem : public System, public ISubject
{
public:
    InputSystem();
    //InputSystem(EntityCoordinator* coord);
    void preUpdate() override;
    void update() override;
    void Notify(Event e, void* args) override;
};
