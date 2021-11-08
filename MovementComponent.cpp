#include "MovementComponent.h"


void MovementComponent::addForce(float x, float y) {
    b2Vec2 velocityVec(x, y);
    physComponent->box2dBody->ApplyForceToCenter(velocityVec, true);
}

void MovementComponent::limitMaxSpeed() {
    //limit the velocity of the object to MAX_SPEED
    float MAX_SPEED = 8.0f;
    float MAX_JUMP_SPEED = 300.0f;
    if (physComponent->box2dBody->GetLinearVelocity().x < -MAX_SPEED)
    {
        physComponent->box2dBody->SetLinearVelocity(b2Vec2(-MAX_SPEED, physComponent->box2dBody->GetLinearVelocity().y));
    }
    else if (physComponent->box2dBody->GetLinearVelocity().x > MAX_SPEED)
    {
        physComponent->box2dBody->SetLinearVelocity(b2Vec2(MAX_SPEED, physComponent->box2dBody->GetLinearVelocity().y));
    }
    else if (physComponent->box2dBody->GetLinearVelocity().y > MAX_JUMP_SPEED)
    {
        physComponent->box2dBody->SetLinearVelocity(b2Vec2(physComponent->box2dBody->GetLinearVelocity().x, MAX_JUMP_SPEED));
    }
}

void MovementComponent::setVelocity(float x, float y) {
    b2Vec2 velocityVec(x, y);
    physComponent->box2dBody->SetLinearVelocity(velocityVec);
}

b2Vec2 MovementComponent::getVelocity() {
    return physComponent->box2dBody->GetLinearVelocity();
}

b2Body* MovementComponent::getPhysicsBody() {
    return physComponent->box2dBody;
}

void MovementComponent::update() {

    if (physComponent->box2dBody->IsBullet()) {
        physComponent->box2dBody->SetGravityScale(0);
        physComponent->box2dBody->SetLinearVelocity(b2Vec2(physComponent->box2dBody->GetLinearVelocity().x, 0));
    } 
    else if (physComponent->box2dBody && physComponent->box2dBody->GetType() != b2_staticBody) {
        // TODO: tag checking. if it is player, limit its speed
        limitMaxSpeed();
    }
}
