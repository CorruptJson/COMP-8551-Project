#include "PhysicsWorld.h"

enum collisionCategory {
    C_NONE = 0x0000,
    C_PLAYER = 0x0001,
    C_ENEMY = 0x0002,
    C_PLATFORM = 0x0004,
    C_BULLET = 0x0008,
    C_FIRE = 0x0016,
    C_STAR = 0x0032,
    C_ENEMYSPAWNER = 0x0064,
    C_PLAYERSPAWNER = 0x0128

};

PhysicsWorld::PhysicsWorld() {

    // Initialization
    gravity = new b2Vec2(0.0f, -9.8f);
    world = new b2World(*gravity);
    world->SetAllowSleeping(false);

    contactListener = new ContactListener();
    world->SetContactListener(contactListener);
}

PhysicsWorld& PhysicsWorld::getInstance()
{
    static PhysicsWorld physicsWorld;
    return physicsWorld;
}

// Adds an entity to the physics world by it's ID
void PhysicsWorld::AddObject(EntityID id) {

    //std::cout << "adding to phys: " << id << std::endl;

    EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    Transform* transformComponent = &coordinator.GetComponent<Transform>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);

    moveComponent->physComponent = physComponent;

    //EntityUserData* entityUserData = new EntityUserData;
    //entityUserData->id = id;

    //b2BodyDef bodyDef = b2BodyDef();
    b2BodyDef bodyDef = b2BodyDef();
    bodyDef.type = physComponent->bodyType;
    bodyDef.position.Set(physComponent->x, physComponent->y);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(physComponent);

    bodyDef.bullet = coordinator.entityHasTag(BULLET, id);
    b2World* world = PhysicsWorld::getInstance().world;
    if (world == nullptr)
    {
        std::cout << "b2 world is nullptr" << std::endl;
    }
    physComponent->box2dBody = world->CreateBody(&bodyDef);
    B2DBodyAddGuardFunction(physComponent->box2dBody,id);
    physComponent->entityID = id;
    physComponent->box2dBody->SetFixedRotation(true);
    if (physComponent->box2dBody) {
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(physComponent->halfWidth, physComponent->halfHeight);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = physComponent->density;
        fixtureDef.friction = physComponent->friction;
        fixtureDef.restitution = 0;

        // set collision filter base on tag
        if (coordinator.entityHasTag(PLAYER, id)) {
            physComponent->box2dBody->SetGravityScale(1.5);
            fixtureDef.filter.categoryBits = C_PLAYER;
            fixtureDef.filter.maskBits = C_PLATFORM | C_ENEMY;
        }
        else if (coordinator.entityHasTag(ENEMY, id)) {
            physComponent->box2dBody->SetGravityScale(1.5);
            moveComponent->setVelocity(2.0, 0); // THIS IS TEMPORARY, 

            fixtureDef.filter.categoryBits = C_ENEMY;
            fixtureDef.filter.maskBits = C_PLAYER | C_PLATFORM | C_BULLET;
        }
        else if (coordinator.entityHasTag(PLATFORM, id)) {
            fixtureDef.filter.categoryBits = C_PLATFORM;
            fixtureDef.filter.maskBits = C_PLAYER | C_ENEMY | C_BULLET;
        }
        else if (coordinator.entityHasTag(BULLET, id)) {
            fixtureDef.filter.categoryBits = C_BULLET;
            fixtureDef.filter.maskBits = C_PLATFORM | C_ENEMY;
        }
        else if (coordinator.entityHasTag(FIRE, id)) {
            fixtureDef.filter.categoryBits = C_FIRE;
            fixtureDef.filter.maskBits = C_PLAYER | C_ENEMY;
        }
        else if (coordinator.entityHasTag(STAR, id)) {
            fixtureDef.filter.categoryBits = C_STAR;
            fixtureDef.filter.maskBits = C_PLAYER;
        }
        else if (coordinator.entityHasTag(ENEMYSPAWNER, id)) {
            fixtureDef.filter.categoryBits = C_ENEMYSPAWNER;
            fixtureDef.filter.maskBits = C_PLAYER;
        }
        else if (coordinator.entityHasTag(PLAYERSPAWNER, id)) {
            fixtureDef.filter.categoryBits = C_PLAYERSPAWNER;
            fixtureDef.filter.maskBits = C_NONE;
        }
        physComponent->box2dBody->CreateFixture(&fixtureDef);
    }
}

// THIS IS CURRENTLY OUTDATED AND NOT BEING USED
// Adds all objects in the world using the entity coordinator
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

            transformComponents[i]->setPhysicsBody(physComponents[i]->box2dBody);

        }
    }

    //physComponents[2]->box2dBody->SetLinearVelocity(b2Vec2(0.1, 5.0));
    
}

void PhysicsWorld::Update(EntityCoordinator* coordinator) {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);

        std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<PhysicsComponent>(),
        coordinator->GetComponentType<Transform>(),
        coordinator->GetComponentType<MovementComponent>()

            });

        int entitiesFound = entityQuery->totalEntitiesFound();
        std::vector<PhysicsComponent*> physComponents = entityQuery->getComponentArray<PhysicsComponent>();
        std::vector<Transform*> transformComponents = entityQuery->getComponentArray<Transform>();
        std::vector<MovementComponent*> moveComponents = entityQuery->getComponentArray<MovementComponent>();

        for (int i = 0; i < entitiesFound; i++) {
            if (physComponents[i]->isFlaggedForDelete) {

                B2DBodyDeleteGuardFunction(physComponents[i]->box2dBody, physComponents[i]->entityID);
                EntityCoordinator::getInstance().scheduleEntityToDelete(physComponents[i]->entityID);
                continue;
            }
            UpdateTransform(transformComponents[i], physComponents[i]);
            UpdateMovementComponent(moveComponents[i], physComponents[i]);
        }
    }
}

// Destroy an object from physics world
void PhysicsWorld::DestoryObject(EntityID id)
{
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    physComponent->isFlaggedForDelete = true;
}

void PhysicsWorld::B2DBodyDeleteGuardFunction(b2Body* body,EntityID id)
{
    auto activeFind = activeBodies.find(body);
    if (activeFind == activeBodies.end())
    {
        std::cout << "trying to delete body that is not active? Ent: " << id << std::endl;
    }
    else
    {
        activeBodies.erase(body);
    }

    auto deactiveFind = deactivatedBodies.find(body);
    if (deactiveFind != deactivatedBodies.end())
    {
        std::cout << "trying to delete an inactive body? Ent: " << id << std::endl;
    }
    else
    {
        deactivatedBodies.emplace(body);
    }

    PhysicsWorld::getInstance().world->DestroyBody(body);
}

void PhysicsWorld::B2DBodyAddGuardFunction(b2Body* body, EntityID id)
{
    auto activeFind = activeBodies.find(body);
    if (activeFind != activeBodies.end())
    {
        std::cout << "trying to add body that is already active? Ent: " << id << std::endl;
    }
    else
    {
        activeBodies.emplace(body);
    }

    auto deactiveFind = deactivatedBodies.find(body);
    if (deactiveFind != deactivatedBodies.end())
    {
        //std::cout << "resuin? Ent: " << id << std::endl;
        deactivatedBodies.erase(body);
    }
}

void PhysicsWorld::UpdateMovementComponent(MovementComponent* moveComponent, PhysicsComponent* physComponent) {
    moveComponent->xVelocity = physComponent->box2dBody->GetLinearVelocity().x;
    moveComponent->yVelocity = physComponent->box2dBody->GetLinearVelocity().y;
    moveComponent->update();
}

void PhysicsWorld::UpdateTransform(Transform* transform, PhysicsComponent* physComponent) {
    transform->setPosition(physComponent->box2dBody->GetPosition().x, physComponent->box2dBody->GetPosition().y);
}


PhysicsWorld::~PhysicsWorld() {
    if (gravity) delete gravity;
    if (world) delete world;
    if (contactListener) delete contactListener;
}
