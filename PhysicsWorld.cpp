#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() {
    // Initialization
    gravity = new b2Vec2(0.0f, -9.8f);
    world = new b2World(*gravity);
    world->SetAllowSleeping(false);

    //contactListener = new ContactListener();
    //world->SetContactListener(contactListener);
}

void PhysicsWorld::AddObject(EntityCoordinator* coordinator) {
        
    
}

void PhysicsWorld::Update() {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);

    }
}

PhysicsWorld::~PhysicsWorld() {
    if (gravity) delete gravity;
    if (world) delete world;
    //if (contactListener) delete contactListener;
}
