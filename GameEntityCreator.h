#pragma once
#include "EntityCoordinator.h"
#include "Components.h"
#include "Renderer.h"
#include "GameManager.h"
#include "IObserver.h"
#include "Transform.h"
#include "ISubject.h"

// this class is used as a centralized place to write specific code to initialize specific types of entities in specific ways
// "creating" an entity only assigns an index in memory to it
// this class is used for common data value initialization

// this class also listens for events and can create entities via events

enum Enemies {
    ROACH,
    SMALL_ROACH,
    NUM_OF_ENEMIES
};

struct TransformArg
{
    float xPos;
    float yPos;
    float xScale;
    float yScale;
};

class GameEntityCreator : public IObserver , public ISubject
{
private:
    // entity creation requires an archetype
    Archetype actorArchetype;
    Archetype platformArchetype;
    Archetype testArchetype;
    Archetype textArchetype;
    Archetype uiArchetype;
    Archetype starArchetype;
    Archetype physParticleArchetype;
    Archetype particleArchetype;
    Archetype particleAnimated;

    StateComponent enemiesInitialStates[NUM_OF_ENEMIES];

    GameEntityCreator();
    RenderComponent standardRenderComponent(const char* spriteName, bool flipX);
public:
    static GameEntityCreator& getInstance();
    EntityID CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool flipX, int state);
    EntityID CreateRoach(float xPos, float yPos, bool facingRight);
    EntityID CreateSmallRoach(float xPos, float yPos, bool facingRight);
    EntityID CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, int state);
    EntityID CreateTimer(const char* spriteName, std::vector<Tag> tags);
    EntityID CreateText(std::string text, float x, float y, float r, float g, float b, float size, TextAlign align, std::vector<Tag> tags);
    EntityID CreateText(std::string text, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags);
    EntityID CreateUI(float x, float y, float height, float width, float r, float g, float b, std::vector<Tag> tags);
    EntityID CreateStar(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
    EntityID CreatePhysParticle(TransformArg t, int frameLife, const char* spriteName);
    EntityID CreateParticle(TransformArg t, int frameLife, const char* spriteName, float col, float row, ParticleMove move);
    EntityID CreateSparkle(TransformArg t, int frameLife, int currFrame, ParticleMove move);
    void Receive(Event e, void* args);
};
