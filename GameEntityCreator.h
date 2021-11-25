#pragma once
#include "EntityCoordinator.h"
#include "Components.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "GameManager.h"

enum Enemies {
    ROACH,
    SMALL_ROACH,
    NUM_OF_ENEMIES
};

struct SimpleTransform
{
    float xPos;
    float yPos;
    float xScale;
    float yScale;
};

class GameEntityCreator
{
private:
    Archetype actorArchetype;
    Archetype platformArchetype;
    Archetype testArchetype;
    Archetype textArchetype;
    Archetype starArchetype;
    Archetype physParticleArchetype;

    StateComponent enemiesInitialStates[];

    GameEntityCreator();
    RenderComponent standardRenderComponent(const char* spriteName, bool hasAnimation);
public:
    static GameEntityCreator& getInstance();
    EntityID CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool hasAnimation, int state);
    EntityID CreateRoach(float xPos, float yPos, bool facingRight);
    EntityID CreateSmallRoach(float xPos, float yPos, bool facingRight);
    EntityID CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, int state);
    EntityID CreateTimer(const char* spriteName, std::vector<Tag> tags);
    EntityID CreateText(std::string text, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags);
    EntityID CreateStar(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool hasAnimation);
    EntityID CreatePhysParticle(float xPos, float yPos, float scaleX, float scaleY, int frameLife, const char* spriteName);
    //EntityID CreateScenery(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
};
