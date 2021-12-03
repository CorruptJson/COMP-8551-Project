#pragma once
#include "Types.h"
#include "Renderer.h"
#include "GameEntityCreator.h"
#include "IObserver.h"

class GameManager : public IObserver
{
private:
    GameManager();
    EntityID player;
    EntityID playerRespawner;
    int gameFrameCount = 0;

    void createGameOverOverlay(int playerScore, vector<string> dates, vector<string> scores);
    void removeGameOverOverlay();
public:
    static GameManager& getInstance();
    EntityID PlayerID();
    void SetPlayerID(EntityID);
    EntityID PlayerRespawnerID();
    void SetPlayerRespawnerID(EntityID);
    void countGameFrame();
    int getCurrGameFrame();
    void handleGameOver();
    void Receive(Event e, void* args) override;
};
