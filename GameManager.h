#pragma once
#include "Types.h"

class GameManager
{
private:
    GameManager();
    EntityID player;
    int frameCount = 0;
public:
    static GameManager& getInstance();
    EntityID PlayerID();
    void SetPlayerID(EntityID);
    void countFrame();
    int getCurrFrame();
};
