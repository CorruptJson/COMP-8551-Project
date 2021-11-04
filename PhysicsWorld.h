#pragma once

#include "ContactListener.h"
#include "EntityUserData.h"

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    static PhysicsWorld& getInstance();
    void AddObject(EntityID id);
    void AddObjects(EntityCoordinator* coordinator);
    void Update(EntityCoordinator *coordinator);
    void DestoryObject(EntityID id);
private:
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 6;
    const int positionIterations = 2;

    b2Vec2* gravity;
    b2World* world;
    ContactListener* contactListener;

    void UpdatePhysicsComponent(PhysicsComponent* physComponent);
    void UpdateMovementComponent(MovementComponent* moveComponent, PhysicsComponent* physComponent);
    void UpdateTransform(Transform* transform, PhysicsComponent* physComponent);
};
