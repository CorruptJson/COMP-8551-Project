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

class PlayerControlSystem : public IObserver, public ISubject {

public:
    PlayerControlSystem();
    ~PlayerControlSystem();
    void processPlayer();
    void jump();
    void shoot();
    void damaged();
    bool isGrounded();

    void Receive(Event e, void* args) override;

private:
    b2Timer* invincibleTimer;
    bool isInvincible;
    bool isInContactWithEnemy;
    bool isRespawning;
    int health;
    const int maxHealth = 3;
    const float invincibleLength = 2000; // millisecond
    const float respawningTime = 1000;
    bool isDead;

    void respawn();
};
