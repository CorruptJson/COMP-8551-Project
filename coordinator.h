#pragma once

#include "system_manager.h"
class Coordinator
{
public:
    SystemManager systemManager;
    void runSystemUpdates();
    void initializeCoordinator();
};

