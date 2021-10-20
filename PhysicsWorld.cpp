#include "PhysicsWorld.h"

b2Body* test;

PhysicsWorld::PhysicsWorld() {

    // Initialization
    gravity = new b2Vec2(0.0f, -9.8f);
    world = new b2World(*gravity);
    world->SetAllowSleeping(false);

    contactListener = new ContactListener();
    world->SetContactListener(contactListener);
}

void PhysicsWorld::AddObjects(EntityCoordinator* coordinator) {
        
    std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
            coordinator->GetComponentType<PhysicsComponent>(),
            coordinator->GetComponentType<Transform>()
        });

    int entitiesFound = entityQuery->totalEntitiesFound();
    std::vector<PhysicsComponent*> physComponents = entityQuery->getComponentArray<PhysicsComponent>();
    std::vector<Transform*> transformComponents = entityQuery->getComponentArray<Transform>();  

    for (int i = 0; i < entitiesFound; i++) {
        b2BodyType type = physComponents[i]->bodyType;

        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position.Set(physComponents[i]->x, physComponents[i]->y);

        physComponents[i]->b2Body = world->CreateBody(&bodyDef);

        printf("Initial pos X-Pos: %0.2f Y-Pos %0.2f\n", physComponents[i]->b2Body->GetPosition().x, physComponents[i]->b2Body->GetPosition().y);

        if (physComponents[i]->b2Body) {

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(physComponents[i]->halfWidth, physComponents[i]->halfHeight);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = physComponents[i]->density;
            fixtureDef.friction = physComponents[i]->friction;
            fixtureDef.restitution = 0;

            physComponents[i]->b2Body->CreateFixture(&fixtureDef);

            // need some way to pass the body to the entity so it can update in rendering

            transformComponents[i]->setPhysicsBody(physComponents[i]->b2Body);

        }
    
    }
    
}

void PhysicsWorld::Update(EntityCoordinator* coordinator) {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);

        std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<PhysicsComponent>(),
        coordinator->GetComponentType<Transform>()
            });

        int entitiesFound = entityQuery->totalEntitiesFound();
        std::vector<PhysicsComponent*> physComponents = entityQuery->getComponentArray<PhysicsComponent>();
        std::vector<Transform*> transformComponents = entityQuery->getComponentArray<Transform>();

        for (int i = 0; i < entitiesFound; i++) {
            UpdatePhysicsComponent(physComponents[i]);
            UpdateTransform(transformComponents[i], physComponents[i]);
        }
    }
}

void PhysicsWorld::UpdatePhysicsComponent(PhysicsComponent* physComponent) {
    physComponent->x = physComponent->b2Body->GetTransform().p.x;
    physComponent->y = physComponent->b2Body->GetTransform().p.y;
}

void PhysicsWorld::UpdateTransform(Transform* transform, PhysicsComponent* physComponent) {
    transform->setPosition(physComponent->x, physComponent->y);
}

PhysicsWorld::~PhysicsWorld() {
    if (gravity) delete gravity;
    if (world) delete world;
    if (contactListener) delete contactListener;
}
