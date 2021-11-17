#pragma once

#include "EntityUserData.h"
#include "GameEntityCreator.h"
#include "PhysicsWorld.h"
#include "GameManager.h"

enum playerState {
    STATE_NORMAL,
    STATE_JUMPING,
    STATE_FALLING,
    STATE_MOVING,
    STATE_HIT,
    STATE_SHOOTING,
    STATE_DIE,
};

class PlayerControlSystem : public IObserver {

public:
    PlayerControlSystem();
    ~PlayerControlSystem();
    void processEntity(EntityID id);
    void jump();
    void shoot();
    void damaged();
    bool isGrounded();
    void checkRespawn();
    bool isDead();

    void Receive(Event e, void* args) override;
    //void Update(EntityCoordinator* coordinator);

private:
    b2Timer* invincibleTimer;
    bool isDamaged;
    const float invincibleLength = 2000; // millisecond
};
