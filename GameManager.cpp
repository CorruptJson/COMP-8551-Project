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
