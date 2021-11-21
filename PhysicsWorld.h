#pragma once

#include "ContactListener.h"
#include "EntityUserData.h"
#include "IObserver.h"

// thompson testing
#include <set>

class PhysicsWorld : public IObserver {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    static PhysicsWorld& getInstance();
    void AddObject(EntityID id);
    void AddObjects(EntityCoordinator* coordinator);
    void Update(EntityCoordinator *coordinator);
    void DestoryObject(EntityID id);
    void B2DBodyDeleteGuardFunction(b2Body* body, EntityID id);
    void B2DBodyAddGuardFunction(b2Body* body, EntityID id);
    ContactListener* GetContactListener();

private:
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 6;
    const int positionIterations = 2;

    b2Vec2* gravity;
    b2World* world;
    ContactListener* contactListener;

    // thompson testing
    std::map<b2Body*,EntityID> destroyedPointers;
    std::set<b2Body*> activeBodies;
    std::set<b2Body*> deactivatedBodies;

    void UpdatePhysicsComponent(PhysicsComponent* physComponent);
    void UpdateMovementComponent(MovementComponent* moveComponent, PhysicsComponent* physComponent);
    void UpdateTransform(Transform* transform, PhysicsComponent* physComponent);

    void Receive(Event e, void* args) override;
};
