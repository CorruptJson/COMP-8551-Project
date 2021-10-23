#pragma once

#include <iostream>
#include "Libraries/include/box2d/box2d.h"
#include "PhysicsComponent.h"

class ContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact) {
        std::cout << "Contact made!" << std::endl;

        //PhysicsComponent* physComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        //printf("Contact bodyA X-Pos: %0.2f Y-Pos %0.2f\n", physComponentA->x, physComponentA->y);
        //printf("Contact bodyA X-Pos: %0.2f Y-Pos %0.2f\n", contact->GetFixtureA()->GetBody()->GetPosition().x, contact->GetFixtureA()->GetBody()->GetPosition().y);
        //printf("Contact bodyB X-Pos: %0.2f Y-Pos %0.2f\n", contact->GetFixtureA()->GetBody()->GetPosition().x, contact->GetFixtureB()->GetBody()->GetPosition().y);


    };
    void EndContact(b2Contact* contact) {};
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};
};
