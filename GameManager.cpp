#include "GameManager.h"

GameManager::GameManager()
{

}

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}


EntityID GameManager::PlayerRespawnerID()
{
    return playerRespawner;
}

void GameManager::SetPlayerRespawnerID(EntityID id)
{
    playerRespawner = id;
}

void GameManager::countGameFrame()
{
    gameFrameCount++;
}

int GameManager::getCurrGameFrame()
{
    return gameFrameCount;
}
