#pragma once

#include "Types.h"
#include "Transform.h"
#include "PhysicsComponent.h"
#include "ContactListener.h"
#include "EntityCoordinator.h"

struct EntityUserData {
    EntityID id;
    vector<Tag> tags;
    PhysicsComponent physComponent;
    Transform transformComponent;
};

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void AddObject(EntityID id);
    void AddObjects(EntityCoordinator* coordinator);
    void Update(EntityCoordinator* coordinator);
private:
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 10;
    const int positionIterations = 3;

    b2Vec2* gravity;
    b2World* world;
    ContactListener* contactListener;

    void UpdatePhysicsComponent(PhysicsComponent* physComponent);
    void UpdateTransform(Transform* transform, PhysicsComponent* physComponent);
};
