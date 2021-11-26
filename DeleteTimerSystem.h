#pragma once
#include "system.h"
#include "EntityCoordinator.h"
#include "DeleteTimer.h"
#include "GameManager.h"

class DeleteTimerSystem : public System
{
    void update() override;
};
