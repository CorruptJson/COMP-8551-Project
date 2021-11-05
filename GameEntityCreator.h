#pragma once
#include "EntityCoordinator.h"
#include "Components.h"

class GameEntityCreator
{
private:
    Archetype actorArchetype;
    Archetype platformArchetype;
    Archetype testArchetype;
    //Archetype sceneryArchetype;
    GameEntityCreator();
    RenderComponent standardRenderComponent(const char* spriteName, bool hasAnimation);
public:
    static GameEntityCreator& getInstance();
    EntityID CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool hasAnimation);
    EntityID CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
    //EntityID CreateScenery(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
};
