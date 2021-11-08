#pragma once
#include "Types.h"
#include "EntityCoordinator.h"
#include "Renderer.h"


// class for storing values to pass around scene manager
struct EntityValues {
    std::vector<uint8_t> components;

    std::vector<Tag> tags;
    const char* spriteName = "";

    // - Components ------------------------
    // 
    // Use to add components to entity
    bool transformComponent = false;
    bool renderComponent = false;
    bool physicsComponent = false;
    bool animationComponent = false;
    bool movementComponent = false;
    bool stateComponent = false;

    // - Component Values -----------------------------

    // Transform
    float xPos = 0.0f;
    float yPos = 0.0f;
    float xScale = 1.0f;
    float yScale = 1.0f;
    float rotation = 0.0f;

    // RenderComponent
    bool hasAnimation = false;
    float rowIndex = 0;
    float colIndex = 0;

    // AnimationComponent
    bool animIsPlaying = false;
    std::string animName = "";

    // Physics
    float density = 1.0f;
    float friction = 0.0f;
    b2BodyType bodyType = b2_staticBody;
};

// Scene Manager class
class SceneManager
{
private:
    Renderer* renderer;
    EntityCoordinator* coordinator;
    json sceneJsonArray;
    json prefabJsonArray;
    unordered_map<std::string, json> prefabMap;

    void ParseEntityValues(EntityValues& ev, const json& jsonObject);



public:
    
    SceneManager();

    void LoadScene(std::string filename);
    void LoadPrefabs(std::string filename);
    void CreateEntities();
    
    



    std::list<EntityID> entities;

};

