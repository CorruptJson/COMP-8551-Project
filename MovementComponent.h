#pragma once

#pragma once
#include <ostream>
#include <string>
#include "Libraries/include/box2d/box2d.h"

class MovementComponent
{
public:
    b2Body* body;
    float xVelocity;
    float yVelocity;

    void addForce(float xVelocity, float yVelocity);
    void setVelocity(float xVelocity, float yVelocity);
    b2Vec2 getVelocity();
    void limitMaxSpeed();

    void setPhysicsBody(b2Body* body);
    b2Body* getPhysicsBody();
    void update();
};
