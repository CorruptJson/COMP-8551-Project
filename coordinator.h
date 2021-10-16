#pragma once

#include "system_manager.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "renderComponent.h"
class Coordinator
{
public:
    SystemManager systemManager;
    EntityManager entityManager;
    ComponentManager componentManager;
    EntityID testingEntity;
    void runSystemUpdates();
    void initializeCoordinator();
};

