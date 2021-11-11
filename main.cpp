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
#include "TextComponent.h"
#include "inputSystem.h"
#include "SceneManager.h"
#include "GameEntityCreator.h"
#include "Components.h"
#include "Tags.h"
#include "PlayerControlSystem.h"
#include "GameManager.h"


//ChunkManager* chunkManager;
EntityCoordinator* coordinator;

SceneManager* sceneManager;

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
PlayerControlSystem* playerControl;

Animator animator;

GameManager& gameManager = GameManager::getInstance();

Archetype standardArch;

// test entities
EntityID mike;
EntityID timer;
EntityID text;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

const int VIEW_WIDTH = 15;
const int VIEW_HEIGHT = 10;

// gets called once when engine starts
// put initilization code here
int initialize()
{  
    // when the engine starts
    renderer->init(VIEW_WIDTH, VIEW_HEIGHT);
    coordinator = &(EntityCoordinator::getInstance());
    sceneManager = new SceneManager();

    //physicsWorld = new PhysicsWorld();
    physicsWorld = &(PhysicsWorld::getInstance());
    playerControl = new PlayerControlSystem();


    prevTime = Clock::now();


    return 0;
}

int test(){

    coordinator->RegisterComponent<Transform>();
    coordinator->RegisterComponent<RenderComponent>();
    coordinator->RegisterComponent<PhysicsComponent>();
    coordinator->RegisterComponent<AnimationComponent>();
    coordinator->RegisterComponent<TimerComponent>();
    coordinator->RegisterComponent<StateComponent>();
    coordinator->RegisterComponent<MovementComponent>();
    coordinator->RegisterComponent<TextComponent>();

    //coordinator->addSystem<InputSystem>(coordinator);    
    //coordinator->addSystem(std::make_shared<InputSystem>());

    shared_ptr<InputSystem> inputSys = coordinator->addSystem<InputSystem>();

    /*
    //Equivalent to attaching code below
    shared_ptr<TestSystem> testSys = coordinator->addSystem<TestSystem>();
    shared_ptr<PrinterSystem> printerSys = coordinator->addSystem<PrinterSystem>();
    testSys.get()->Attach(printerSys.get());
    */

    coordinator->addSystem<TestSystem>().get()->Attach(coordinator->addSystem<PrinterSystem>().get());


    


    // For testing different archetypes
    //EntityID e = coordinator->CreateEntity(coordinator->GetArchetype({ coordinator->GetComponentType<Transform>() }), "Edgar.png", { ENEMY });
    //coordinator->GetComponent<Transform>(e) = Transform(1, 1, 0, 1, 1);
    sceneManager->CreateEntities();

    //creating text
    //                                                                   X      Y      R     G     B     Tags
    text = GameEntityCreator::getInstance().CreateText("Text Component", 50.0f, 50.0f, 0.5f, 0.2f, 0.8f, 0.9f, {});
    


    for (auto const& e : sceneManager->entities) {
        if (coordinator->entityHasTag(Tag::PLAYER, e)) {
            mike = e;
        }

    }
    return 0;
}

// this update runs 60 times a second
void fixedFrameUpdate()
{
    InputTracker::getInstance().perFrameUpdate(window);

    // run physics
    physicsWorld->Update(coordinator);
    // run ECS systems
    coordinator->runSystemUpdates();

    playerControl->processEntity(mike);

    coordinator->endOfUpdate();
}

void graphicsUpdate()
{
    animator.updateAnim(coordinator);
    renderer->update(coordinator);
}

// the main update function
// game loop will be put here eventually
int runEngine()

{
    Clock::time_point currTime = Clock::now();
    Duration delta = currTime - prevTime;
    prevTime = currTime;
    catchupTime += delta.count();

    // Game engine loop
    // this loop runs 60 times a second
    while (catchupTime >= MS_PER_FRAME)
    {
        fixedFrameUpdate();

        catchupTime -= MS_PER_FRAME;
        gameManager.countFrame();
    }
        
    // Graphics code runs independently from the fixed-frame game update
    graphicsUpdate();
    
    return 0;
}

// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderer->teardown();

    

    return 0;
}

int main() {
    initialize();
    test();

    //entity test
    GameEntityCreator& creator = GameEntityCreator::getInstance();
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
