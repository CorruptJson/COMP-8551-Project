#pragma once
#include "Types.h"

class GameManager
{
private:
    GameManager();
    EntityID player;
public:
    static GameManager& getInstance();
    EntityID PlayerID();
    void SetPlayerID(EntityID);
};
