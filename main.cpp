#include <iostream>
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
#include "TimerSystem.h"
#include "SpawnSystem.h"
#include "ScoreSystem.h"
#include "SceneManager.h"
#include "GameEntityCreator.h"
#include "Components.h"
#include "Tags.h"
#include "PlayerControlSystem.h"
#include "GameManager.h"
#include "FPSCounter.h"


EntityCoordinator* coordinator;
SceneManager* sceneManager;

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
PlayerControlSystem* playerControl;

Animator animator;

GameManager& gameManager = GameManager::getInstance();
FPSCounter fpsCounter = FPSCounter();

Archetype standardArch;

// special entities
EntityID mike;
EntityID timer;
EntityID mikeRespawner;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

const int VIEW_WIDTH = 14;
const int VIEW_HEIGHT = 10;

// gets called once when engine starts
// put initilization code here
int initialize()
{  
    // when the engine starts
    glm::fvec4 backgroundColor(81.f / 255, 50.f / 255, 37.f / 255, 1);
    renderer->init(VIEW_WIDTH, VIEW_HEIGHT, backgroundColor, WindowSize::WINDOWED);
    coordinator = &(EntityCoordinator::getInstance());
    sceneManager = new SceneManager();

    physicsWorld = new PhysicsWorld();
    physicsWorld = &(PhysicsWorld::getInstance());
    playerControl = new PlayerControlSystem();

    coordinator->chunkManager->Attach(physicsWorld);

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

    shared_ptr<InputSystem> inputSys = coordinator->addSystem<InputSystem>();
    
    //Subscribe playercontrol to recieve inputSystem events
    inputSys->Attach(playerControl);

    shared_ptr<SpawnSystem> spawnSys = coordinator->addSystem<SpawnSystem>();
    coordinator->addSystem<TimerSystem>()->Attach(spawnSys.get());
    coordinator->addSystem<TimerSystem>();

    //Subscribe playercontrol to recieve collision events
    physicsWorld->GetContactListener()->Attach(playerControl);
    physicsWorld->GetContactListener()->Attach(spawnSys.get());

    sceneManager->CreateEntities();

    shared_ptr<ScoreSystem> scoreSys = coordinator->addSystem<ScoreSystem>();
    physicsWorld->GetContactListener()->Attach(scoreSys.get());

    scoreSys->UpdateScore();
    for (auto const& e : sceneManager->entities) {
        if (coordinator->entityHasTag(Tag::PLAYER, e)) {
            mike = e;
            gameManager.SetPlayerID(mike);
        }
        else if (coordinator->entityHasTag(Tag::PLAYERSPAWNER, e)) {
            mikeRespawner = e;
            gameManager.SetPlayerRespawnerID(mikeRespawner);
        }
    }

    return 0;
}

// this update runs 60 times a second
void fixedFrameUpdate()
{
    InputTracker::getInstance().perFrameUpdate(window);

    // delete all entities when space is pressed
    //if (InputTracker::getInstance().isKeyJustDown(InputTracker::SPACE))
    //{
    //    coordinator->deactivateAllEntitiesAndPhysicsBodies();
    //}

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
    fpsCounter.NextFrame();

    return 0;
}

// gets called once when engine ends
// put teardown code here
int teardown()
{
    std::cout << "ending programing" << std::endl;

    // when the engine closes
    renderer->teardown(false);

    delete coordinator;
    delete sceneManager;

    delete physicsWorld;
    delete playerControl;


    return 0;
}

int main() {
    initialize();
    test();

    animator = Animator();

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
