#include "MovementComponent.h"


void MovementComponent::addForce(float x, float y) {
    b2Vec2 velocityVec(x, y);
    body->ApplyForceToCenter(velocityVec, true);
}

void MovementComponent::limitMaxSpeed() {
    //limit the velocity of the object to MAX_SPEED
    float MAX_SPEED = 5.0f;
    float MAX_JUMP_SPEED = 200.0f;
    if (body->GetLinearVelocity().x < -MAX_SPEED)
    {
        body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
    }
    else if (body->GetLinearVelocity().x > MAX_SPEED)
    {
        body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
    }
    else if (body->GetLinearVelocity().y > MAX_JUMP_SPEED)
    {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, MAX_JUMP_SPEED));
    }
}

void MovementComponent::setVelocity(float x, float y) {
    b2Vec2 velocityVec(x, y);
    body->SetLinearVelocity(velocityVec);
}

b2Vec2 MovementComponent::getVelocity() {
    return body->GetLinearVelocity();
}

void MovementComponent::setPhysicsBody(b2Body* newBody) {
    body = newBody;
}

b2Body* MovementComponent::getPhysicsBody() {
    return body;
}

void MovementComponent::update() {

    if (body && body->GetType() != b2_staticBody) {
        // TODO: tag checking. if it is player, limit its speed
        limitMaxSpeed();
    }
}
