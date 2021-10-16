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

    //std::array<PhysicsComponent, MAX_ENTITIES> components = coordinator->GetComponentArray<PhysicsComponent>();
    //std::array<Transform, MAX_ENTITIES> transforms = coordinator->GetComponentArray<Transform>();
        
    std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
            coordinator->GetComponentType<PhysicsComponent>(),
            coordinator->GetComponentType<Transform>()
        });

    int entitiesFound = entityQuery->totalEntitiesFound();
    std::vector<PhysicsComponent*> physComponents = entityQuery->getComponentArray<PhysicsComponent>();
    std::vector<Transform*> transformComponents = entityQuery->getComponentArray<Transform>();  
    
    std::cout << "addObjects query count: " << entitiesFound << std::endl;
    //
    //PhysicsComponent* physComponent;

    //for(int j = 0; j < )

    for (int i = 0; i < entitiesFound; i++) {
        PhysicsComponent component = *physComponents[i];
        b2BodyType type = component.bodyType;

        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position.Set(component.x, component.y);
      
        b2Body* body = world->CreateBody(&bodyDef);

        printf("Initial pos X-Pos: %0.2f Y-Pos %0.2f\n", body->GetPosition().x, body->GetPosition().y);

        if (body) {

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(component.halfWidth, component.halfHeight);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = component.density;
            fixtureDef.friction = component.friction;
            fixtureDef.restitution = 0;

            body->CreateFixture(&fixtureDef);

            // need some way to pass the body to the entity so it can update in rendering

            transformComponents[i]->setPhysicsBody(body);

        }
    
    }
    
}

void PhysicsWorld::Update(EntityCoordinator* coordinator) {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);

        b2Body* body = world->GetBodyList();

        if (body == NULL)
        {
            std::cout << "first body is null " << std::endl;
        }

        // skip static bodies
        while (body->GetType() != b2_dynamicBody) {
            body = body->GetNext();
        }

        std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<PhysicsComponent>(),
        coordinator->GetComponentType<Transform>()
            });

        int entitiesFound = entityQuery->totalEntitiesFound();
        std::vector<PhysicsComponent*> physComponents = entityQuery->getComponentArray<PhysicsComponent>();
        std::vector<Transform*> transformComponents = entityQuery->getComponentArray<Transform>();

        //EntityQuery<PhysicsComponent,Transform> eq = coo

        //printf("In physics X-Pos: %0.2f Y-Pos %0.2f\n", body->GetPosition().x, body->GetPosition().y);
        for (int i = entitiesFound -1; i >= 0; i--) {
            if (physComponents[i]->bodyType != b2_dynamicBody || body->GetType() != b2_dynamicBody)
            {
                body = body->GetNext();
                continue;
            }
            transformComponents[i]->setPosition(body->GetPosition().x, body->GetPosition().y);
            //printf("In physics X-Pos: %0.2f Y-Pos %0.2f\n", body->GetPosition().x, body->GetPosition().y);
            body = body->GetNext();
        }

    }
}

PhysicsWorld::~PhysicsWorld() {
    if (gravity) delete gravity;
    if (world) delete world;
    if (contactListener) delete contactListener;
}
