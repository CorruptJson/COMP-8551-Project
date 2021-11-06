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
    ANIMATION,
    STATE
};

// convert strings to enums here
unordered_map<std::string, eKeys> keyMap = {
    {"tag", TAG},
    {"transform", TRANSFORM},
    {"render", RENDER},
    {"physics", PHYSICS},
    {"animation", ANIMATION},
    {"state", STATE}
};


unordered_map<std::string, Tag> tagMap = {
    {"enemy", ENEMY},
    {"platform", PLATFORM},
    {"player", PLAYER}
};




// spritesheet map for converting from string to char
unordered_map <std::string, const char*> spriteMap = {
    {"wall.jpg", "wall.jpg"},
    {"Giant_Roach.png", "Giant_Roach.png"},
    {"Edgar.png", "Edgar.png"}
};







SceneManager::SceneManager() {
    coordinator = &(EntityCoordinator::getInstance());
    renderer = Renderer::getInstance();
    this->LoadScene("scene.json");
    this->LoadPrefabs("prefab.json");
}


void SceneManager::LoadScene(std::string filename) {
    sceneJsonArray = json::parse(FileManager::readTextFile(filename));
}

void SceneManager::LoadPrefabs(std::string filename) {
    prefabJsonArray = json::parse(FileManager::readTextFile(filename));
    for (auto& prefab : prefabJsonArray.items()) {
        if (prefab.value().contains("setPrefab")) {
            prefabMap.emplace(
                prefab.value()["setPrefab"].get<std::string>(),
                prefab.value()["prefabValues"].get<json>()
                
            );
        }
    }
}

void SceneManager::CreateEntities() {

    // loop through entities

    for (auto& entity : sceneJsonArray.items()) {

        // For setting all the entity and component values
        // See EntityValue class in the SceneManager.h to add more
        EntityValues ev;
        this->ParseEntityValues(ev, entity.value()); // parses json and updates entityvalue object

        // add components to archetype if they're enabled
        if (ev.transformComponent) ev.components.push_back(coordinator->GetComponentType<Transform>());
        if (ev.renderComponent) ev.components.push_back(coordinator->GetComponentType<RenderComponent>());
        if (ev.physicsComponent) ev.components.push_back(coordinator->GetComponentType<PhysicsComponent>());
        if (ev.animationComponent) ev.components.push_back(coordinator->GetComponentType<AnimationComponent>());
        if (ev.movementComponent) ev.components.push_back(coordinator->GetComponentType<MovementComponent>());
        if (ev.stateComponent) ev.components.push_back(coordinator->GetComponentType<StateComponent>());

        Archetype arch = coordinator->GetArchetype(ev.components);
        std::cout << ev.spriteName << std::endl;
        EntityID ent = coordinator->CreateEntity(arch, ev.spriteName, ev.tags);

        entities.push_back(ent);

        // Set component values from before here
        if (ev.transformComponent) {
            coordinator->GetComponent<Transform>(ent) = {
                    ev.xPos,
                    ev.yPos,
                    ev.rotation,
                    ev.xScale,
                    ev.yScale
            };
        }

        if (ev.renderComponent) {
            coordinator->GetComponent<RenderComponent>(ent) = {
                    "doodleVertShader.vs",
                    "doodleFragShader.fs",
                    ev.spriteName,
                    0,
                    0,
                    ev.hasAnimation,
                    true
            };
        }

        if (ev.physicsComponent) {
            coordinator->GetComponent<PhysicsComponent>(ent) = {
                ev.bodyType,
                0.5f * ev.yScale,
                0.5f * ev.xScale,
                ev.xPos,
                ev.yPos,
                ev.density,
                ev.friction
            };

        }

        if (ev.animationComponent) {
            coordinator->GetComponent<AnimationComponent>(ent) =
                Animator::createAnimationComponent(renderer->getAnimation(ev.animName, ev.spriteName), true);
        }

        if (ev.movementComponent) {
            coordinator->GetComponent<MovementComponent>(ent) = {
            0,
            0
            };
        }
        if (ev.stateComponent) {
            coordinator->GetComponent<StateComponent>(ent) = {
            0
            };
        }

    }
}


void SceneManager::ParseEntityValues(EntityValues& ev, const json& jsonObject) {
    // check for prefabs first
    if (jsonObject.contains("usePrefab")) {
        json e = prefabMap[jsonObject["usePrefab"].get<std::string>()];
        ParseEntityValues(ev, e);
    }

    //loop through components in the entity
    for (auto& component : jsonObject.items()) {

        // Set component booleans and set their values in this switch statement
        if (keyMap.find(component.key()) != keyMap.end()) {
            switch (keyMap[component.key()]) {
            case TAG:
                //TODO: allow for multiple tags
                ev.tags = { tagMap[component.value()] };
                break;

            case TRANSFORM:
                ev.transformComponent = true; // add transform to component

                // Values
                ev.xPos = (component.value().contains("xPos")) // If component Json contains xPos key
                    ? component.value()["xPos"].get<float>() : ev.xPos; // set the xPos to it's value, else keep it the same

                ev.yPos = (component.value().contains("yPos"))
                    ? component.value()["yPos"].get<float>() : ev.yPos;

                ev.xScale = (component.value().contains("xScale"))
                    ? component.value()["xScale"].get<float>() : ev.xScale;

                ev.yScale = (component.value().contains("yScale"))
                    ? component.value()["yScale"].get<float>() : ev.yScale;

                ev.rotation = (component.value().contains("rotation"))
                    ? component.value()["rotation"].get<float>() : ev.rotation;

                break;

            case RENDER:
                // Entities with Render always have transform
                ev.renderComponent = true;
                ev.transformComponent = true;

                // Values

                // Todo: Stop using a map to convert from string to string
                ev.spriteName = (component.value().contains("sprite"))
                    ? spriteMap[component.value()["sprite"].get<std::string>()] : ev.spriteName;

                ev.hasAnimation = component.value().contains("hasAnim")
                    ? component.value()["hasAnim"].get<bool>() : ev.hasAnimation;

                break;

            case PHYSICS:
                // Entties with Physics always have these components
                ev.physicsComponent = true;
                ev.transformComponent = true;
                ev.movementComponent = true;
                

                // TODO: do more than just check for one string
                ev.bodyType = component.value().contains("b2bodytype") && component.value()["b2bodytype"].get<string>() == "b2_dynamicBody"
                    ? b2_dynamicBody : ev.bodyType;

                ev.friction = component.value().contains("friction")
                    ? component.value()["friction"].get<float>() : ev.friction;

                ev.density = component.value().contains("density")
                    ? component.value()["density"].get<float>() : ev.density;

                break;

            case ANIMATION:
                ev.animationComponent = true;
                ev.renderComponent = true;

                ev.animIsPlaying = component.value().contains("isPlaying")
                    ? component.value()["isPlaying"].get<bool>() : ev.animIsPlaying;

                ev.animName = component.value().contains("animName")
                    ? component.value()["animName"].get<std::string>() : ev.animName;

                break;

            case STATE:
                ev.stateComponent = true;

                break;
            }
        }
    }

};
