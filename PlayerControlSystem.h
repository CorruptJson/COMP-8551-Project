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
    void processPlayer();
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
    bool isInvincible;
    bool isInContactWithEnemy;
    bool isRespawning;
    int health;
    const int maxHealth = 3;
    const float invincibleLength = 2000; // millisecond
    const float respawningTime = 1000;

    void updateContactWithEnemy(bool isContacted);
    void respawn();
};
