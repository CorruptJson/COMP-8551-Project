#pragma once

#include "EntityUserData.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"
#include "GameManager.h"

class PlayerControlSystem : public IObserver {

public:
    ~PlayerControlSystem();
    void processEntity(EntityID id);
    void jump();
    void shoot();
    const int STATE_NORMAL = 0;
    const int STATE_JUMPING = 1;
    const int STATE_FALLING = 2;
    const int STATE_MOVING = 3;
    const int STATE_HIT = 4;
    const int STATE_SHOOTING = 5;

    void Receive(Event e, void* args) override;
    //void Update(EntityCoordinator* coordinator);

private:
};
