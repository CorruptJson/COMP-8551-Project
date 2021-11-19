#include "GameManager.h"

GameManager::GameManager()
{

}

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

EntityID GameManager::PlayerID()
{
    return player;
}

void GameManager::SetPlayerID(EntityID id)
{
    player = id;
}

EntityID GameManager::PlayerRespawnerID()
{
    return playerRespawner;
}

void GameManager::SetPlayerRespawnerID(EntityID id)
{
    playerRespawner = id;
}

void GameManager::countFrame()
{
    frameCount++;
}

int GameManager::getCurrFrame()
{
    return frameCount;
}
