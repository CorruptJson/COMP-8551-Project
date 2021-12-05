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
    C_PLAYERSPAWNER = 0x0128,
    C_WALL = 0x0256

};

PhysicsWorld::PhysicsWorld() {

    // Initialization
    // make it 30 so we fall faster than real life
    gravity = new b2Vec2(0.0f, -40.0f);
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

    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    // get the components of the entity
    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    Transform* transformComponent = &coordinator.GetComponent<Transform>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);

    moveComponent->physComponent = physComponent;
    // create physics body for entity
    b2BodyDef bodyDef = b2BodyDef();
    bodyDef.type = physComponent->bodyType;
    bodyDef.position.Set(physComponent->x, physComponent->y);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(physComponent);

    bodyDef.bullet = coordinator.entityHasTag(BULLET, id);
    b2World* world = PhysicsWorld::getInstance().world;

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

        // set collision filter based on tag
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
            fixtureDef.filter.maskBits = C_PLATFORM | C_ENEMY | C_WALL;
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
        else if (coordinator.entityHasTag(WALL, id)) {
            fixtureDef.filter.categoryBits = C_WALL;
            fixtureDef.filter.maskBits = C_PLAYER | C_ENEMY | C_BULLET;
        }
        physComponent->box2dBody->CreateFixture(&fixtureDef);
    }
    else
    {

    }
}

void PhysicsWorld::Update(EntityCoordinator* coordinator) {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);

        std::shared_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<PhysicsComponent>(),
        coordinator->GetComponentType<Transform>(),
        coordinator->GetComponentType<MovementComponent>()

            }, {});

        int entitiesFound = entityQuery->totalEntitiesFound();
        ComponentIterator<PhysicsComponent> physCompIterator = ComponentIterator<PhysicsComponent>(entityQuery);
        ComponentIterator<Transform> transformCompIterator = ComponentIterator<Transform>(entityQuery);
        ComponentIterator<MovementComponent> moveCompIterator = ComponentIterator<MovementComponent>(entityQuery);

        for (int i = 0; i < entitiesFound; i++) {
            PhysicsComponent* physComponent = physCompIterator.nextComponent();
            Transform* transform = transformCompIterator.nextComponent();
            MovementComponent* moveComponent = moveCompIterator.nextComponent();

            if (physComponent->isFlaggedForDelete) {
                EntityCoordinator::getInstance().scheduleEntityToDelete(physComponent->entityID);
                continue;
            }
            UpdateTransform(transform, physComponent);
            UpdateMovementComponent(moveComponent, physComponent);
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
    std::vector<Tag> tags = EntityCoordinator::getInstance().getTagsForEntity(id);

    auto activeFind = activeBodies.find(body);
    if (activeFind == activeBodies.end())
    {

    }
    else
    {
        activeBodies.erase(body);
    }

    auto deactiveFind = deactivatedBodies.find(body);
    if (deactiveFind != deactivatedBodies.end())
    {

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

    }
    else
    {
        activeBodies.emplace(body);
    }

    auto deactiveFind = deactivatedBodies.find(body);
    if (deactiveFind != deactivatedBodies.end())
    {
        deactivatedBodies.erase(body);
    }
}

ContactListener* PhysicsWorld::GetContactListener()
{
    return contactListener;
}

void PhysicsWorld::UpdateMovementComponent(MovementComponent* moveComponent, PhysicsComponent* physComponent) {
    moveComponent->xVelocity = physComponent->box2dBody->GetLinearVelocity().x;
    moveComponent->yVelocity = physComponent->box2dBody->GetLinearVelocity().y;
    moveComponent->update();
}

void PhysicsWorld::UpdateTransform(Transform* transform, PhysicsComponent* physComponent) {
    transform->setPosition(physComponent->box2dBody->GetPosition().x, physComponent->box2dBody->GetPosition().y);
}

void PhysicsWorld::Receive(Event e, void* args)
{
    if (e == Event::B2BODY_TO_DELETE)
    {
        B2BodyDeleteEventArgs* eventArgs = (B2BodyDeleteEventArgs*)args;
        B2DBodyDeleteGuardFunction(eventArgs->body, eventArgs->id);
        delete eventArgs;
    }
}

PhysicsWorld::~PhysicsWorld() {
    if (gravity) delete gravity;
    if (world) delete world;
    if (contactListener) delete contactListener;
}
