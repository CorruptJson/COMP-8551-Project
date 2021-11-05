#include <iostream>
//#include "RenderTutorial.h"
#include <vector>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "EntityCoordinator.h"
#include "TimerComponent.h"
#include "Types.h"
#include "Animator.h"
#include "InputTracker.h"
#include "InputComponent.h"
#include "inputSystem.h"
#include "SceneManager.h"
#include "GameEntityCreator.h"
#include "Components.h"
#include "Tags.h"
#include "PlayerControlSystem.h"


//ChunkManager* chunkManager;
EntityCoordinator* coordinator;

SceneManager* sceneManager;

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
PlayerControlSystem* playerControl;

Animator animator;
Archetype standardArch;

// test entities
EntityID mike;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

const int VIEW_WIDTH = 4;
const int VIEW_HEIGHT = 4;

// gets called once when engine starts
// put initilization code here
int initialize()
{  
    // when the engine starts
    renderer->init(VIEW_WIDTH, VIEW_HEIGHT);
    coordinator = &(EntityCoordinator::getInstance());
    sceneManager = new SceneManager();

    physicsWorld = new PhysicsWorld();

    prevTime = Clock::now();


    return 0;
}

int test(){

    coordinator->RegisterComponent<Transform>();
    coordinator->RegisterComponent<RenderComponent>();
    coordinator->RegisterComponent<PhysicsComponent>();
    coordinator->RegisterComponent<AnimationComponent>();
    coordinator->RegisterComponent<TimerComponent>();

    //coordinator->addSystem<InputSystem>(coordinator);    
    coordinator->addSystem(std::make_shared<InputSystem>());

    try {
    sceneManager->CreateEntities();
    }
    catch (const std::exception& e) { std::cout << e.what(); }

    // For testing different archetypes
    //EntityID e = coordinator->CreateEntity(coordinator->GetArchetype({ coordinator->GetComponentType<Transform>() }), "Edgar.png", { ENEMY });
    //coordinator->GetComponent<Transform>(e) = Transform(1, 1, 0, 1, 1);


    for (auto const& e : sceneManager->entities) {
        if (coordinator->entityHasTag(Tag::PLAYER, e)) {
            mike = e;
        }

    }
    return 0;
}


// the main update function
// game loop will be put here eventually
int runEngine()

{
    Clock::time_point currTime = Clock::now();
    Duration delta = currTime - prevTime;
    prevTime = currTime;
    catchupTime += delta.count();

    // check input

    while (catchupTime >= MS_PER_FRAME)
    {
        InputTracker::getInstance().perFrameUpdate(window);

        // run physics
        physicsWorld->Update(coordinator);
        // run ECS systems
        coordinator->runSystemUpdates();

        catchupTime -= MS_PER_FRAME;
    }
    
    /////////////////
    //Testing character control
    playerControl->processEntity(mike);
    //float xVelocity = coordinator->GetComponent<MovementComponent>(mike).xVelocity;
    //float yVelocity = coordinator->GetComponent<MovementComponent>(mike).yVelocity;
    //float speed = 2.0f;
    //float jumpForce = 160.0f;
    //int jumpCount = 0;
    //int jumpLimit = 1;
    //bool isReset = false;
    //// Colliding with Platform count as ground check
    //bool isCollided = coordinator->GetComponent<PhysicsComponent>(mike).box2dBody->GetContactList();
    ////float force = coordinator->GetComponent<PhysicsComponent>(mike).box2dBody->GetMass() * 10 / (1 / 60.0);
    ////force /= 3;
    //if (InputTracker::getInstance().isKeyDown(InputTracker::A)) {
    //    Animation* anim = renderer->getAnimation("running", coordinator->GetComponent<RenderComponent>(mike).spriteName);
    //    coordinator->GetComponent<RenderComponent>(mike).flipX = false;
    //    coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    //    coordinator->GetComponent<MovementComponent>(mike).setVelocity(-speed, yVelocity);
    //}
    //if (InputTracker::getInstance().isKeyDown(InputTracker::D)) {
    //    Animation* anim = renderer->getAnimation("running", coordinator->GetComponent<RenderComponent>(mike).spriteName);
    //    coordinator->GetComponent<RenderComponent>(mike).flipX = true;
    //    coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    //    coordinator->GetComponent<MovementComponent>(mike).setVelocity(speed, yVelocity);
    //}
    //if (InputTracker::getInstance().isKeyDown(InputTracker::S)) {
    //    Animation* anim = renderer->getAnimation("hurt", coordinator->GetComponent<RenderComponent>(mike).spriteName);
    //    coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    //    coordinator->GetComponent<MovementComponent>(mike).setVelocity(0, 0);

    //}
    //if (isCollided) {
    //    isReset = true;
    //    jumpCount = 0;
    //}
    //else {
    //    isReset = false;
    //}
    //if (InputTracker::getInstance().isKeyJustDown(InputTracker::W)) {
    //    if (isReset) {
    //        if (jumpCount < jumpLimit) {
    //            coordinator->GetComponent<MovementComponent>(mike).addForce(0, jumpForce);
    //            jumpCount++;
    //        }
    //    }
    //}
    //if (InputTracker::getInstance().isKeyJustReleased(InputTracker::A) || InputTracker::getInstance().isKeyJustReleased(InputTracker::W)) {
    //    coordinator->GetComponent<MovementComponent>(mike).setVelocity(0, yVelocity);
    //}
    //if (InputTracker::getInstance().isKeyJustReleased(InputTracker::D) || InputTracker::getInstance().isKeyJustReleased(InputTracker::W)) {
    //    coordinator->GetComponent<MovementComponent>(mike).setVelocity(0, yVelocity);
    //}
    //if (InputTracker::getInstance().isKeyJustReleased(InputTracker::S)) {
    //    coordinator->GetComponent<MovementComponent>(mike).setVelocity(xVelocity, 0);
    //}
    //if (InputTracker::getInstance().isKeyJustReleased(InputTracker::W)) {
    //    if (yVelocity > 0) {
    //        coordinator->GetComponent<MovementComponent>(mike).setVelocity(xVelocity, 0);
    //    }
    //}
    //std::cout << "xVelocity: " << xVelocity << std::endl;
    //std::cout << "yVelocity: " << yVelocity << std::endl;
    //animation
    animator.updateAnim(coordinator);
    
    // render
    renderer->update(coordinator);
    
    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderer->teardown();

    delete physicsWorld;

    return 0;
}

int main() {
    initialize();
    test();


    animator = Animator();

    std::cout << "Number of Entities: " << coordinator->GetEntityCount() << std::endl;

    bool isdudeplayer = coordinator->entityHasTag(Tag::PLAYER,mike);
    std::cout << "Is dude the player? " << isdudeplayer << std::endl;




    for (auto const& e : sceneManager->entities) {
        if (coordinator->entityHasComponent<PhysicsComponent>(e)) {
            
            physicsWorld->AddObject(e);
        }

    }

    

    while (!glfwWindowShouldClose(window))
    {
        // tell glfw to keep track of window resize 
        // and input events
        glfwPollEvents();        
        runEngine();        
    }    

    teardown();

    return 0;
}
