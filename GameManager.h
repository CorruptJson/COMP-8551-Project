#pragma once
#include "Types.h"
#include "Renderer.h"
#include "GameEntityCreator.h"
#include "IObserver.h"
#include "SceneManager.h"
#include "EntityCoordinator.h"

// this class stores data specific to the game itself, and not as relevant to the underlying game engine

class GameManager : public IObserver, public ISubject
{
private:
    GameManager();
    EntityID player;
    EntityID playerRespawner;
    int gameFrameCount = 0;
    bool gamePaused = false;
    std::string curScene;
    SceneManager* sceneManager;
    bool isGameOver;

    void createGameOverOverlay(int playerScore, vector<string> dates, vector<string> scores);
    void replay();
public:
    bool hasActiveStar = false;
    // config files
    static std::string menuScene;
    static std::string gameScene;
    static std::string prefabs;

    static GameManager& getInstance();
    void identifyPlayerAndPlayerSpawner();
    EntityID PlayerRespawnerID();
    void countGameFrame();
    int getCurrGameFrame();
    bool GameIsPaused();
    void PauseGame();
    void UnpauseGame();
    // game states
    std::string getCurScene();
    void loadScene(std::string scene);
    void handleGameOver();

    void Receive(Event e, void* args) override;
};
