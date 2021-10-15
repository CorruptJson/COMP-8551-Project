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

    PhysicsComponent* physComponent;

    //for(int j = 0; j < )

    for (int i = 0; i < entityQuery->size(); i++) {
        PhysicsComponent component = components[i];
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

            transforms[i].setPhysicsBody(body);

        }
    
    }
    
}

void PhysicsWorld::Update(EntityCoordinator* coordinator) {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);

        b2Body* body = world->GetBodyList();

        // skip static bodies
        while (body->GetType() != b2_dynamicBody) {
            body = body->GetNext();
        }

        EntityQuery<PhysicsComponent,Transform> eq = coo

        //printf("In physics X-Pos: %0.2f Y-Pos %0.2f\n", body->GetPosition().x, body->GetPosition().y);
        for (int i = coordinator->GetEntityCount()-1; i >= 0; i--) {
            if (coordinator->GetComponentArray<PhysicsComponent>()[i].bodyType != b2_dynamicBody || body->GetType() != b2_dynamicBody)
            {
                body = body->GetNext();
                continue;
            }
            coordinator->GetComponentArray<Transform>()[i].setPosition(body->GetPosition().x, body->GetPosition().y);
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
