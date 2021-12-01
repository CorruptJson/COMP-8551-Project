#pragma once
#include "Types.h"

class GameManager
{
private:
    GameManager();
    EntityID player;
    EntityID playerRespawner;
    int gameFrameCount = 0;
public:
    static GameManager& getInstance();
    EntityID PlayerID();
    void SetPlayerID(EntityID);
    EntityID PlayerRespawnerID();
    void SetPlayerRespawnerID(EntityID);
    void countGameFrame();
    int getCurrGameFrame();
};
