#pragma once
#include "EntityCoordinator.h"
#include "Components.h"
#include "Renderer.h"
#include "PhysicsWorld.h"

enum Enemies {
    ROACH,
    SMALL_ROACH,
    NUM_OF_ENEMIES
};

class GameEntityCreator
{
private:
    Archetype actorArchetype;
    Archetype platformArchetype;
    Archetype testArchetype;
    Archetype textArchetype;
    Archetype starArchetype;
    //Archetype sceneryArchetype;

    StateComponent enemiesInitialStates[];

    GameEntityCreator();
    RenderComponent standardRenderComponent(const char* spriteName, bool flipX);
public:
    static GameEntityCreator& getInstance();
    EntityID CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool flipX, int state);
    EntityID CreateRoach(float xPos, float yPos, bool facingRight);
    EntityID CreateSmallRoach(float xPos, float yPos, bool facingRight);
    EntityID CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, int state);
    EntityID CreateTimer(const char* spriteName, std::vector<Tag> tags);
    EntityID CreateText(std::string text, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags);
    EntityID CreateStar(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
    //EntityID CreateScenery(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
};
