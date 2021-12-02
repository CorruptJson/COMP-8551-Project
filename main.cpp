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
    shared_ptr<InputSystem> inputSys = coordinator->addSystem<InputSystem>();

    //Subscribe playercontrol to recieve inputSystem events
    inputSys->Attach(playerControl);
    inputSys->Attach(renderer);

    shared_ptr<SpawnSystem> spawnSys = coordinator->addSystem<SpawnSystem>();
    coordinator->addSystem<TimerSystem>()->Attach(spawnSys.get());

    //Subscribe playercontrol to recieve collision events
    physicsWorld->GetContactListener()->Attach(playerControl);
    physicsWorld->GetContactListener()->Attach(spawnSys.get());

    shared_ptr<ScoreSystem> scoreSys = coordinator->addSystem<ScoreSystem>();
    physicsWorld->GetContactListener()->Attach(scoreSys.get());

    scoreSys->UpdateScore();
}

void identifyPlayerAndPlayerSpawner()
{
    for (auto const& e : sceneManager->entities)
    {
        if (coordinator->entityHasTag(Tag::PLAYER, e))
        {
            mike = e;
            gameManager.SetPlayerID(mike);
        }
        else if (coordinator->entityHasTag(Tag::PLAYERSPAWNER, e))
        {
            mikeRespawner = e;
            gameManager.SetPlayerRespawnerID(mikeRespawner);
        }
    }
}

/// <summary>
/// Create the game over overlay on top of the current scene.
/// Note that we are only displaying the top 5 highest scores.
/// </summary>
/// <param name="dates">The dates the scores were gotten.</param>
/// <param name="scores">The scores that were gotten.</param>
void createGameOverOverlay(int playerScore, vector<string> dates, vector<string> scores) {
    auto& creator = GameEntityCreator::getInstance();
    float viewHeight = Renderer::getInstance()->getCamera()->getViewHeight();
    float viewWidth = Renderer::getInstance()->getCamera()->getViewWidth();
    float yPos = viewHeight / 2 - 2; // use this so we can change the starting yPos easily

    // create the panel
    creator.CreateUI(0, 0, viewHeight * 0.8, viewWidth * 0.6, 0, 0, 0, {Tag::GAME_OVER_UI});

    // create the title and user score
    creator.CreateText("GAME OVER", 0, yPos, 1, 0, 0, 1.5, { Tag::GAME_OVER_UI });
    yPos -= 0.75;
    creator.CreateText("SCORE: " + to_string(playerScore), 0, yPos, 1, 1, 1, 1, { Tag::GAME_OVER_UI });
    yPos -= 1;

    // create the highscore section
    creator.CreateText("HIGHSCORES", 0, yPos, 1, 1, 1, 1, { Tag::GAME_OVER_UI });
    yPos -= 1;
    int highscoresCount = 5;
    for (int i = 0; i < highscoresCount; i++) {
        string date;
        try {
            date = dates.at(i);
        }
        catch (out_of_range) {
            date = "-----";
        }

        string score;
        try {
            score = scores.at(i);
        }
        catch (out_of_range) {
            score = "0";
        }

        creator.CreateText(date, -3, yPos, 1, 1, 1, 1, TextAlign::LEFT, { Tag::GAME_OVER_UI });
        creator.CreateText(score, 3, yPos, 1, 1, 1, 1, TextAlign::RIGHT, { Tag::GAME_OVER_UI });
        yPos -= 0.75;
    }

    // replay instruction
    yPos -= 0.25;
    creator.CreateText("J to replay. Q to quit", 0, yPos, 1, 0, 0, 1, {Tag::GAME_OVER_UI});
}

void removeGameOverOverlay() {
    std::shared_ptr<EntityQuery> query = coordinator->GetEntityQuery(
        { }, { Tag::GAME_OVER_UI });


    int uiFound = query->totalEntitiesFound();
    for (int i = 0; i < uiFound; i++) {

    }
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

    initComponents();

    sceneManager = new SceneManager();
    sceneManager->CreateEntities();

    initSystems();

    identifyPlayerAndPlayerSpawner();      

    //sound test
    std::vector<std::string> music;
    music.push_back("fighting_BGM.wav");
    
    std::vector<std::string> sfx;
    //sfx.push_back("bullet.wav");
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

    // code to make the game over overlay
    //vector<string> dates = {
    //    "2020/11/30 11:30",
    //    "2020/11/30 11:30",
    //    "2020/11/30 11:30",
    //    "2020/11/30 11:30",
    //    "2020/11/30 11:30",
    //};

    //vector<string> scores = {
    //    "15",
    //    "5",
    //    "15",
    //    "15",
    //    "5",
    //};
    //createGameOverOverlay(10, dates, scores);

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
        gameManager.countGameFrame();
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

    delete sceneManager;

    delete playerControl;

    return 0;
}

int main() {
    initialize();       

    for (auto const& e : sceneManager->entities) {
        if (coordinator->entityHasComponent<PhysicsComponent>(e)) {
            
            physicsWorld->AddObject(e);
        }

    }

    
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
