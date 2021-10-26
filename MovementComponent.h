#pragma once

#pragma once
#include <ostream>
#include <string>
#include "Libraries/include/box2d/box2d.h"

//struct MovementComponent
//{
//    float xVelocity;
//    float yVelocity;
//    b2Body* body;
//};
//struct Movement
//{
//    float xVelocity;
//    float yVelocity;
//};
//
class MovementComponent
{
public:
    b2Body* body;
    float xVelocity;
    float yVelocity;
//    MovementComponent();
//    MovementComponent(float xVelocity, float yVelocity);
//    Movement getMovement() const;
//    void setMovement(float xVelocity, float yVelocity);
    void addForce(float xVelocity, float yVelocity);

    void setPhysicsBody(b2Body* body);
    b2Body* getPhysicsBody();
//    b2Vec2 velocityVec;
    void update();
//
//private:
//    Movement movement;
// 
//    b2Body* bodyM;
};
