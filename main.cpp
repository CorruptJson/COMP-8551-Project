#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include <thread>
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
#include "DeleteTimerSystem.h"
#include "SceneManager.h"
#include "GameEntityCreator.h"
#include "Components.h"
#include "Tags.h"
#include "PlayerControlSystem.h"
#include "GameManager.h"
#include "Sound.h"

#include "FPSCounter.h"


EntityCoordinator* coordinator;
Sound& se = Sound::getInstance();

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
PlayerControlSystem* playerControl;
InputSystem inputSystem;
Animator animator;

GameManager* gameManager;
FPSCounter fpsCounter = FPSCounter();

Archetype standardArch;

// special entities
EntityID timer;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

const int VIEW_WIDTH = 14;
const int VIEW_HEIGHT = 10;

void initComponents()
{
    coordinator->RegisterComponent<Transform>();
    coordinator->RegisterComponent<RenderComponent>();
    coordinator->RegisterComponent<PhysicsComponent>();
    coordinator->RegisterComponent<AnimationComponent>();
    coordinator->RegisterComponent<TimerComponent>();
    coordinator->RegisterComponent<StateComponent>();
    coordinator->RegisterComponent<MovementComponent>();
    coordinator->RegisterComponent<TextComponent>();    
}

void initSystems()
{
    coordinator->addSystem<DeleteTimerSystem>();
    inputSystem = InputSystem();

    //Subscribe playercontrol to recieve inputSystem events
    inputSystem.Attach(playerControl);
    inputSystem.Attach(renderer);
    inputSystem.Attach(gameManager);

    shared_ptr<SpawnSystem> spawnSys = coordinator->addSystem<SpawnSystem>();
    coordinator->addSystem<TimerSystem>()->Attach(spawnSys.get());

    //Subscribe playercontrol to recieve collision events
    physicsWorld->GetContactListener()->Attach(playerControl);
    physicsWorld->GetContactListener()->Attach(spawnSys.get());

    shared_ptr<ScoreSystem> scoreSys = coordinator->addSystem<ScoreSystem>();
    physicsWorld->GetContactListener()->Attach(scoreSys.get());

    scoreSys->UpdateScore();
    playerControl->Attach(gameManager);
    gameManager->Attach(playerControl);
}


// gets called once when engine starts
// put initilization code here
int initialize()
{
    // when the engine starts
    glm::fvec4 backgroundColor(81.f / 255, 50.f / 255, 37.f / 255, 1);
    renderer->init(VIEW_WIDTH, VIEW_HEIGHT, backgroundColor, WindowSize::MAXIMIZED_WINDOWED);
    animator = Animator();

    coordinator = &(EntityCoordinator::getInstance());
    physicsWorld = &(PhysicsWorld::getInstance());
    coordinator->chunkManager->Attach(physicsWorld);

    playerControl = new PlayerControlSystem();
    // game manager can only be init after components are init
    initComponents();
    gameManager = &GameManager::getInstance();
    gameManager->loadScene(GameManager::menuScene);
    initSystems();

    //sound test
    std::vector<std::string> music;
    music.push_back("fighting_BGM.wav");
    
    std::vector<std::string> sfx;
    std::vector<std::string> sfxV;
    sfxV.push_back("shoot.wav");
    sfxV.push_back("jump.wav");
    sfxV.push_back("enemyDeath2.wav");
    sfxV.push_back("enemyDeath.wav"); // playerDeath
    sfxV.push_back("flameDeath.wav");
    se.loadSfx(sfxV);
    se.loadSfx(sfx);
    se.loadMusic(music);

    prevTime = Clock::now();

    return 0;
}

// this update runs 60 times a second
void fixedFrameUpdate()
{
    InputTracker::getInstance().perFrameUpdate(window);
    inputSystem.update();

    if (!gameManager->GameIsPaused())
    {
        // run physics
        physicsWorld->Update(coordinator);
        // run ECS systems
        coordinator->runSystemUpdates();
        playerControl->processPlayer();

        coordinator->endOfUpdate();
    }
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
        gameManager->countGameFrame();
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

    delete playerControl;

    return 0;
}

int main() {
    initialize();       
    
    //se.playMusic("brionac.wav"); // Play background music on loop
    se.playMusic(0);
    //se.playSound(0);
    //se.playSound("bullet.wav"); // Play sound effects once

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
