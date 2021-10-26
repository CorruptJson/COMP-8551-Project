#include "MovementComponent.h"

//
//// public 
//MovementComponent::MovementComponent() : MovementComponent(0, 0) { }
//MovementComponent::MovementComponent(float x = 0, float y = 0) {
//    movement = {
//        x,
//        y
//    };
//}
//
//Movement MovementComponent::getMovement() const {
//    return movement;
//}
//
//void MovementComponent::setMovement(float x, float y) {
//    movement = {
//        x, y
//    };
//    //changed = true;
//}
//
void MovementComponent::addForce(float x, float y) {
    b2Vec2 velocityVec(x, y);
    //b2Vec2 linearVelocity = body->GetPosition();
    //body->SetTransform(b2Vec2(position.x, position.y), 0);
    //setMovement(velocityVec.x, velocityVec.y);
    body->ApplyForceToCenter(velocityVec, true);

    ////limit the velocity of the object
    //float MAX_SPEED = 100.0f;
    //if (body->GetLinearVelocity().x < (-MAX_SPEED))
    //{
    //    body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
    //}
    //else if (body->GetLinearVelocity().x > MAX_SPEED)
    //{
    //    body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
   
}

void MovementComponent::setPhysicsBody(b2Body* newBody) {
    body = newBody;
}

b2Body* MovementComponent::getPhysicsBody() {
    return body;
}

void MovementComponent::update() {

    if (body && body->GetType() != b2_staticBody) {
        b2Vec2 velocityVec = body->GetLinearVelocity();
        //setMovement(velocityVec.x, velocityVec.y);
        addForce(xVelocity, yVelocity);
        
        
    }
}
