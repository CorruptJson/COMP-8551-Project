#include "SceneManager.h"
#include "file_manager.h"
#include "PhysicsComponent.h"
#include "Tags.h"
#include "MovementComponent.h"
#include "StateComponent.h"

// Component Keys
enum eKeys
{
    TAG,
    TRANSFORM,
    RENDER,
    PHYSICS,
    ANIMATION
};

// convert strings to enums here
std::map<std::string, eKeys> keyMap = {
    {"tag", TAG},
    {"transform", TRANSFORM},
    {"render", RENDER},
    {"physics", PHYSICS},
    {"animation", ANIMATION},

};


std::map<std::string, Tag> tagMap = {
    {"enemy", ENEMY},
    {"platform", PLATFORM},
    {"player", PLAYER}
};




// spritesheet map for converting from string to char
std::map < std::string, const char*> spriteMap = {
    {"wall.jpg", "wall.jpg"},
    {"Giant_Roach.png", "Giant_Roach.png"},
    {"Edgar.png", "Edgar.png"}
};



SceneManager::SceneManager() {
    coordinator = &(EntityCoordinator::getInstance());
    this->LoadJson("scene.json");
}







void SceneManager::LoadJson(const char* filename) {
    jsonArray = json::parse(FileManager::readTextFile(filename));

}

void SceneManager::CreateEntities() {

    // loop through entities
    for (auto& entity : jsonArray.items()) {

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

        // Physics
        float density = 1.0f;
        float friction = 0.0f;
        b2BodyType bodyType = b2_staticBody;

        //loop through components in the entity
        for (auto& component : entity.value().items()) {

            // Set component booleans and set their values in this switch statement
            switch (keyMap[component.key()]) {
                case TAG:
                    //TODO: allow for multiple tags
                    tags.push_back(tagMap[component.value()]);
                    break;

                case TRANSFORM:
                    transformComponent = true; // add transform to component

                    // Values
                    xPos = (component.value().contains("xPos")) // If component Json contains xPos key
                        ? component.value()["xPos"].get<float>() : xPos; // set the xPos to it's value, else keep it the same

                    yPos = (component.value().contains("yPos"))
                        ? component.value()["yPos"].get<float>() : yPos;

                    xScale = (component.value().contains("xScale"))
                        ? component.value()["xScale"].get<float>() : xScale;

                    yScale = (component.value().contains("yScale"))
                        ? component.value()["yScale"].get<float>() : yScale;

                    rotation = (component.value().contains("rotation"))
                        ? component.value()["rotation"].get<float>() : rotation;

                    break;
                
                case RENDER:
                    // Entities with Render always have transform
                    renderComponent = true;
                    transformComponent = true;

                    // Values

                    // Todo: Stop using a map to convert from string to string
                    spriteName = (component.value().contains("sprite"))
                        ? spriteMap[component.value()["sprite"].get<std::string>()] : spriteName; 

                    hasAnimation = component.value().contains("hasAnim")
                        ? component.value()["hasAnim"].get<bool>() : false;

                    break;

                case PHYSICS:
                    // Entties with Physics always have these components
                    physicsComponent = true;
                    transformComponent = true;
                    movementComponent = true;
                    stateComponent = true;

                    // TODO: do more than just check for one string
                    bodyType = component.value().contains("b2bodytype") && component.value()["b2bodytype"].get<string>() == "b2_dynamicBody" 
                        ? b2_dynamicBody : bodyType;

                    friction = component.value().contains("friction") 
                        ? component.value()["friction"].get<float>() : 0;

                    density = component.value().contains("density") 
                        ? component.value()["density"].get<float>() : 1;

                    break;

                case ANIMATION:
                    animationComponent = true;
                    break;

            }
        }

        // add components to archetype if they're enabled
        if (transformComponent) components.push_back(coordinator->GetComponentType<Transform>());
        if (renderComponent) components.push_back(coordinator->GetComponentType<RenderComponent>());
        if (physicsComponent) components.push_back(coordinator->GetComponentType<PhysicsComponent>());
        if (animationComponent) components.push_back(coordinator->GetComponentType<AnimationComponent>());
        if (movementComponent) components.push_back(coordinator->GetComponentType<MovementComponent>());
        if (stateComponent) components.push_back(coordinator->GetComponentType<StateComponent>());

        Archetype arch = coordinator->GetArchetype(components);
        std::cout << spriteName << std::endl;
        EntityID ent = coordinator->CreateEntity(arch, spriteName, tags);

        entities.push_back(ent);

        // Set component values from before here
        if (transformComponent) {
            coordinator->GetComponent<Transform>(ent) = {
                    xPos,
                    yPos,
                    rotation,
                    xScale,
                    yScale
            };
        }

        if (renderComponent) {
            coordinator->GetComponent<RenderComponent>(ent) = {
                    "defaultVertShader.vs",
                    "defaultFragShader.fs",
                    spriteName,
                    0,
                    0,
                    hasAnimation,
                    true
            };
        }

        if (physicsComponent) {
            coordinator->GetComponent<PhysicsComponent>(ent) = {
                bodyType,
                0.5f * yScale,
                0.5f * xScale,
                xPos,
                yPos,
                density,
                friction
            };

        }
        
    }

}


