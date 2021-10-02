#include "Box2DTutorial.h"

Box2DTutorial::Box2DTutorial() {
    // Initalization
    gravity = new b2Vec2(0.0f, -9.8f);
    world = new b2World(*gravity);
    world->SetAllowSleeping(false);

    contactListener = new ContactListener();
    world->SetContactListener(contactListener);
}

void Box2DTutorial::GravityExample() {
    // Box2D Hello World example with gravity
    // Creating a ground box
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f); // SetAsBox takes the half-width and half-height, so in this case, the box is 100 units wide, 20 units tall

    float density = 0.0f;
    groundBody->CreateFixture(&groundBox, density);

    // Creating a dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // a body must be dynamic if you want it to move in response to forces
    bodyDef.position.Set(0.0f, 20.0f);
    body = world->CreateBody(&bodyDef); // creating this body within the world

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox; // making our body fixture dynamic
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    body->CreateFixture(&fixtureDef); // give our body this fixture
}

void Box2DTutorial::Update() {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        printf("X-Pos: %0.2f Y-Pos %0.2f\n", position.x, position.y);
    }
}

Box2DTutorial::~Box2DTutorial() {
    if (gravity) delete gravity;
    if (world) delete world;
    if (contactListener) delete contactListener;
}
