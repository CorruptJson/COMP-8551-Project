#pragma once

#include <iostream>
#include "Libraries/include/box2d/box2d.h"
#include "EntityUserData.h"

class ContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact) {
        std::cout << "Contact made!" << std::endl;

        //PhysicsComponent* physComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        //printf("Contact bodyA X-Pos: %0.2f Y-Pos %0.2f\n", physComponentA->x, physComponentA->y);
        //printf("Contact bodyA X-Pos: %0.2f Y-Pos %0.2f\n", contact->GetFixtureA()->GetBody()->GetPosition().x, contact->GetFixtureA()->GetBody()->GetPosition().y);
        //printf("Contact bodyB X-Pos: %0.2f Y-Pos %0.2f\n", contact->GetFixtureA()->GetBody()->GetPosition().x, contact->GetFixtureB()->GetBody()->GetPosition().y);

        //EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        //EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
        
        EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);


        std::cout << entUserDataA->tag << std::endl;
        printf("Contact bodyA X-Pos: %0.2f Y-Pos %0.2f\n", entUserDataA->physComponent.x, entUserDataA->physComponent.y);
        std::cout << entUserDataB->tag << std::endl;
        printf("Contact bodyB X-Pos: %0.2f Y-Pos %0.2f\n", entUserDataB->physComponent.x, entUserDataB->physComponent.y);



    };
    void EndContact(b2Contact* contact) {};
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
        //b2WorldManifold worldManifold;
        //contact->GetWorldManifold(&worldManifold);

        //b2PointState state1[2], state2[2];
        //b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
    };
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};
};
