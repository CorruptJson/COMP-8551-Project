#pragma once

#include "ContactListener.h"
#include "EntityUserData.h"

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void AddObject(EntityID id);
    void AddObjects(EntityCoordinator* coordinator);
    void Update(EntityCoordinator *coordinator);
private:
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 10;
    const int positionIterations = 3;

    b2Vec2* gravity;
    b2World* world;
    ContactListener* contactListener;

    void UpdatePhysicsComponent(PhysicsComponent* physComponent);
    //void UpdateMovementComponent(MovementComponent* moveComponent);
    void UpdateTransform(Transform* transform, PhysicsComponent* physComponent);
};
