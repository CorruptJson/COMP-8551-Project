#pragma once
#include "Types.h"
#include "Renderer.h"
#include "GameEntityCreator.h"
#include "IObserver.h"
#include "SceneManager.h"
#include "EntityCoordinator.h"

class GameManager : public IObserver
{
private:
    GameManager();
    EntityID player;
    EntityID playerRespawner;
    int gameFrameCount = 0;
    std::string curScene;
    SceneManager* sceneManager;

    void createGameOverOverlay(int playerScore, vector<string> dates, vector<string> scores);
    void removeGameOverOverlay();
public:
    // config files
    static std::string menuScene;
    static std::string gameScene;
    static std::string prefabs;

    static GameManager& getInstance();
    void identifyPlayerAndPlayerSpawner();
    EntityID PlayerRespawnerID();
    void countGameFrame();
    int getCurrGameFrame();

    // game states
    std::string getCurScene();
    void loadScene(std::string scene);
    void handleGameOver();

    void Receive(Event e, void* args) override;
};
