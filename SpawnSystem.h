#pragma once
#include "IObserver.h"
#include "Event.h"
#include "EntityQuery.h"
#include "EntityCoordinator.h"
#include "GameManager.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"

class SpawnSystem: public System, public IObserver
{
public:
    SpawnSystem();
    EntityCoordinator* ec;

    void Receive(Event e, void* args) override;

    void SpawnStar();

    void SpawnEnemy();
};

