#pragma once

#pragma once
#include <ostream>
#include <string>
#include "Libraries/include/box2d/box2d.h"
#include "EntityCoordinator.h"
#include "PhysicsComponent.h"

class MovementComponent
{
public:
    PhysicsComponent* physComponent;
    float xVelocity;
    float yVelocity;

    void addForce(float xVelocity, float yVelocity);
    void setVelocity(float xVelocity, float yVelocity);
    b2Vec2 getVelocity();
    void limitMaxSpeed();

    b2Body* getPhysicsBody();
    void update();
};
