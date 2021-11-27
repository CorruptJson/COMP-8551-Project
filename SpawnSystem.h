#pragma once
#include "IObserver.h"
#include "Event.h"
#include "EntityQuery.h"
#include "EntityCoordinator.h"
#include "GameManager.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"
#include "vector"

class SpawnSystem: public System, public IObserver
{
public:
    SpawnSystem();
    EntityCoordinator* ec;
    GameEntityCreator* gameEntCreator;
    bool hasActiveStar = false;


    void Receive(Event e, void* args) override;
    void SpawnStar();
    void SpawnEnemy();
};

