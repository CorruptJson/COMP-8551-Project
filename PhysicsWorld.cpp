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

void PhysicsWorld::AddObject(EntityID id) {
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    Transform* transformComponent = &coordinator.GetComponent<Transform>(id);

    EntityUserData* entityUserData = new EntityUserData;
    entityUserData->id = id;
    entityUserData->tag = coordinator.getTagsForEntity(id)[0];
    entityUserData->physComponent = *physComponent;
    entityUserData->transformComponent = *transformComponent;

    b2BodyDef bodyDef;
    bodyDef.type = physComponent->bodyType;
    bodyDef.position.Set(physComponent->x, physComponent->y);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entityUserData);

    physComponent->box2dBody = world->CreateBody(&bodyDef);

    if (physComponent->box2dBody) {
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(physComponent->halfWidth, physComponent->halfHeight);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = physComponent->density;
        fixtureDef.friction = physComponent->friction;
        fixtureDef.restitution = 0;

        physComponent->box2dBody->CreateFixture(&fixtureDef);

        transformComponent->setPhysicsBody(physComponent->box2dBody);
    }

    physComponent->box2dBody->SetLinearVelocity(b2Vec2(0.1, 5.0));

}

void PhysicsWorld::AddObjects(EntityCoordinator* coordinator) {

    std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
            coordinator->GetComponentType<PhysicsComponent>(),
            coordinator->GetComponentType<Transform>()
        });

    int entitiesFound = entityQuery->totalEntitiesFound();
    cout << "Entities found: " << entitiesFound << endl;
    std::vector<PhysicsComponent*> physComponents = entityQuery->getComponentArray<PhysicsComponent>();
    std::vector<Transform*> transformComponents = entityQuery->getComponentArray<Transform>();  

    for (int i = 0; i < entitiesFound; i++) {
        b2BodyType type = physComponents[i]->bodyType;

        EntityUserData* entityUserData = new EntityUserData;
        //entityUserData->id = id;
        //entityUserData->tag = coordinator.getTagsForEntity(id)[0];
        entityUserData->physComponent = *physComponents[i];
        entityUserData->transformComponent = *transformComponents[i];

        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position.Set(physComponents[i]->x, physComponents[i]->y);
        bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(physComponents[i]);

        physComponents[i]->box2dBody = world->CreateBody(&bodyDef);

        printf("Initial pos X-Pos: %0.2f Y-Pos %0.2f\n", physComponents[i]->box2dBody->GetPosition().x, physComponents[i]->box2dBody->GetPosition().y);

        if (physComponents[i]->box2dBody) {

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(physComponents[i]->halfWidth, physComponents[i]->halfHeight);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = physComponents[i]->density;
            fixtureDef.friction = physComponents[i]->friction;
            fixtureDef.restitution = 0;
            

            physComponents[i]->box2dBody->CreateFixture(&fixtureDef);

            // need some way to pass the body to the entity so it can update in rendering

            transformComponents[i]->setPhysicsBody(physComponents[i]->box2dBody);

        }
    }

    physComponents[2]->box2dBody->SetLinearVelocity(b2Vec2(0.1, 5.0));
    
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
    physComponent->x = physComponent->box2dBody->GetTransform().p.x;
    physComponent->y = physComponent->box2dBody->GetTransform().p.y;
}

void PhysicsWorld::UpdateTransform(Transform* transform, PhysicsComponent* physComponent) {
    transform->setPosition(physComponent->x, physComponent->y);
}

PhysicsWorld::~PhysicsWorld() {
    if (gravity) delete gravity;
    if (world) delete world;
    if (contactListener) delete contactListener;
}
