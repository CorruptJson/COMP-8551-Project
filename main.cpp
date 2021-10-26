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
//#include "GameEntityCreator.h"
#include "Components.h"
#include "Tags.h"
#include "SceneManager.h"


//ChunkManager* chunkManager;
EntityCoordinator* coordinator;

SceneManager* sceneManager;

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
Animator animator;
Archetype standardArch;

// test entities
//EntityID roach;
//EntityID wall;
EntityID mike;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

double countDown = 2000.0;
bool trigger = false;
// gets called once when engine starts
// put initilization code here
int initialize()
{  
    // when the engine starts

    renderer->init();
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
    //coordinator->CreateEntity(coordinator->GetArchetype({ coordinator->GetComponentType<Transform>() }), "Edgar.png", { ENEMY });

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
    

    if (InputTracker::getInstance().isKeyJustDown(InputTracker::A) && !trigger) {
        Animation anim = renderer->getAnimation("running", coordinator->GetComponent<RenderComponent>(mike).spriteName);
        coordinator->GetComponent<RenderComponent>(mike).facingRight = false;
        coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    }
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::D) && !trigger) {
        Animation anim = renderer->getAnimation("running", coordinator->GetComponent<RenderComponent>(mike).spriteName);
        coordinator->GetComponent<RenderComponent>(mike).facingRight = true;
        coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    }
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::S) && !trigger) {
        Animation anim = renderer->getAnimation("hurt", coordinator->GetComponent<RenderComponent>(mike).spriteName);
        coordinator->GetComponent<RenderComponent>(mike).facingRight = true;
        coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    }

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



    coordinator->GetComponent<AnimationComponent>(mike) = Animator::createAnimationComponent(renderer->getAnimation("idle", "Edgar.png"),true);

    animator = Animator();
        
    std::cout << "Number of Entities: " << coordinator->GetEntityCount() << std::endl;

    bool isdudeplayer = coordinator->entityHasTag(Tag::PLAYER,mike);
    std::cout << "Is dude the player? " << isdudeplayer << std::endl;


    physicsWorld->AddObjects(coordinator);

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
