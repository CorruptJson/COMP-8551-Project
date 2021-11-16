#pragma once
#include "IObserver.h"
#include "Event.h"
#include "EntityQuery.h"
#include "EntityCoordinator.h"
class SpawnSystem: public IObserver
{
public:
    SpawnSystem();
    EntityCoordinator* ec;

    void Receive(Event e, void* args) override;

    void SpawnStar();

    void SpawnEnemy();
};

