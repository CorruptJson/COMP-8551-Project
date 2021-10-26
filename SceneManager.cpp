#include "SceneManager.h"
#include "file_manager.h"
#include "PhysicsComponent.h"
#include "Tags.h"
#include "PlayerComponent.h"

// Component Keys
enum eKeys
{
    TAG,
    TRANSFORM,
    RENDER,
    PHYSICS,
    ANIMATION,
    PLAYER_COMPONENT
};

// convert strings to enums here
std::map<std::string, eKeys> keyMap = {
    {"tag", TAG},
    {"transform", TRANSFORM},
    {"render", RENDER},
    {"physics", PHYSICS},
    {"animation", ANIMATION},
    {"player", PLAYER_COMPONENT},

};


std::map<std::string, Tag> tagMap = {
    {"enemy", ENEMY},
    {"platform", PLATFORM},
    {"player", PLAYER}
};



SceneManager::SceneManager() {
    coordinator = &(EntityCoordinator::getInstance());
    this->LoadJson("scene.json");

















}


void SceneManager::LoadJson(const char* filename) {
    jsonArray = json::parse(FileManager::readTextFile(filename));

}

void SceneManager::CreateEntities() {

    for (auto& entity : jsonArray.items()) {

        std::vector<uint8_t> components;

        std::vector<Tag> tags;
        const char* spriteName;

        // Makes archetype
        // todo: clean this up and don't loop through everything twice
        for (auto& component : entity.value().items()) {
            //loop through json
            switch (keyMap[component.key()]) {

                case TAG:
                    //todo: allow for multiple tags
                    tags.push_back(tagMap[component.value()]);
                    break;

                case TRANSFORM : 
                    components.push_back(coordinator->GetComponentType<Transform>());
                    break;

                case RENDER :
                    components.push_back(coordinator->GetComponentType<RenderComponent>());

                    break;

                case PHYSICS:
                    components.push_back(coordinator->GetComponentType<PhysicsComponent>());

                    break;

                case ANIMATION:
                    components.push_back(coordinator->GetComponentType<AnimationComponent>());

                    break;

                case PLAYER_COMPONENT:
                    components.push_back(coordinator->GetComponentType<PlayerComponent>());
                    break;

            }

        }

        Archetype arch = coordinator->GetArchetype(components);
        EntityID ent = coordinator->CreateEntity(arch, spriteName, tags);


        // loop through again to set their values
        for (auto& component : entity.value().items()) {
            switch (keyMap[component.key()]) {

            case TRANSFORM:


                // if component json contains postition and position contains x, use x, else 0
                float xPos = (component.value().contains("position") && component.value().find("position").value().contains("x"))
                    ? component.value()["position"]["x"].get<float>() : 0.0f;

                // if component json contains postition and position contains y, use y, else 0
                float yPos = (component.value().contains("position") && component.value().find("position").value().contains("y"))
                    ? component.value()["position"]["y"].get<float>() : 0.0f;

                // if component json contains scale and scale contains x, use x, else 0
                float xScale = (component.value().contains("scale") && component.value().find("scale").value().contains("x"))
                    ? component.value()["position"]["x"].get<float>() : 0.0f;

                // if component json contains scale and scale contains y, use y, else 0
                float yScale = (component.value().contains("scale") && component.value().find("scale").value().contains("y"))
                    ? component.value()["scale"]["y"].get<float>() : 0.0f;

                // if component contains rotation, use rotation, else 0;
                float rotation = (component.value().contains("rotation"))
                    ? component.value()["rotation"].get<float>() : 0.0f;

                coordinator->GetComponent<Transform>(ent) = {
                    xPos,
                    yPos,
                    rotation,
                    xScale,
                    yScale
                };


                break;

            case RENDER:
                bool hasAnimation = component.value().contains("hasAnim") ? component.value().get<bool>() : false;
                coordinator->GetComponent<RenderComponent>(ent) = {
                    "defaultVertShader.vs",
                    "defaultFragShader.fs",
                    spriteName, 
                    0,
                    0,
                    hasAnimation,
                    true
                };

                break;

            case PHYSICS:
                components.push_back(coordinator->GetComponentType<PhysicsComponent>());

                break;

            case ANIMATION:
                components.push_back(coordinator->GetComponentType<AnimationComponent>());

                break;

            }

        }

    }

}



if (component.value().contains("position")) {
    if (component.value().find("position").value().contains("x")) {
        //set x
    }
    if (component.value().find("position").value().contains("y")) {
        //set y
    }
}

if (component.value().contains("scale")) {
    //set scale
}

if (component.value().contains("scale")) {
    //set rotation
}
