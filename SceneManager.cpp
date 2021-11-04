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

    for (auto& entity : jsonArray.items()) {

        std::vector<uint8_t> components;

        std::vector<Tag> tags;
        const char* spriteName = "";

        // Makes archetype
        // TODO: clean this up and don't loop through everything twice
        for (auto& component : entity.value().items()) {
            //loop through json

            std::cout << component.key() << std::endl;
            switch (keyMap[component.key()]) {

                case TAG:
                    //TODO: allow for multiple tags
                    tags.push_back(tagMap[component.value()]);
                    break;

                case TRANSFORM : 
                    components.push_back(coordinator->GetComponentType<Transform>());
                    
                    break;

                case RENDER :
                    components.push_back(coordinator->GetComponentType<RenderComponent>());
                    if (component.value().contains("sprite")) {
                        spriteName = spriteMap[component.value()["sprite"].get<std::string>()];
                    }
                    break;

                case PHYSICS:
                    components.push_back(coordinator->GetComponentType<PhysicsComponent>());
                    components.push_back(coordinator->GetComponentType<MovementComponent>());
                    components.push_back(coordinator->GetComponentType<StateComponent>());
                    break;

                case ANIMATION:
                    components.push_back(coordinator->GetComponentType<AnimationComponent>());
                    
                        

                    break;

            }

        }


        Archetype arch = coordinator->GetArchetype(components);
        std::cout << spriteName << std::endl;
        EntityID ent = coordinator->CreateEntity(arch, spriteName, tags);


        // loop through again to set their values
        for (auto& component : entity.value().items()) {
            std::cout << component.key() << std::endl;
            float xPos;
            float yPos;
            float xScale;
            float yScale;
            float rotation;
            bool hasAnimation;
            float friction;
            float density;
            b2BodyType bodyType;

            switch (keyMap[component.key()]) {

            case TRANSFORM:


                // if component json contains postition and position contains x, use x, else 0
                xPos = (component.value().contains("position") && component.value().find("position").value().contains("x"))
                    ? component.value()["position"]["x"].get<float>() : 0.0f;

                // if component json contains postition and position contains y, use y, else 0
                yPos = (component.value().contains("position") && component.value().find("position").value().contains("y"))
                    ? component.value()["position"]["y"].get<float>() : 0.0f;

                // if component json contains scale and scale contains x, use x, else 0
                xScale = (component.value().contains("scale") && component.value().find("scale").value().contains("x"))
                    ? component.value()["scale"]["x"].get<float>() : 1.0f;

                // if component json contains scale and scale contains y, use y, else 0
                yScale = (component.value().contains("scale") && component.value().find("scale").value().contains("y"))
                    ? component.value()["scale"]["y"].get<float>() : 1.0f;

                // if component contains rotation, use rotation, else 0;
                rotation = (component.value().contains("rotation"))
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
                hasAnimation = component.value().contains("hasAnim") ? component.value()["hasAnim"].get<bool>() : false;
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

                // TODO: use Transform values instead
                // 
                // if component json contains postition and position contains x, use x, else 0
                xPos = (component.value().contains("position") && component.value().find("position").value().contains("x"))
                    ? component.value()["position"]["x"].get<float>() : 0.0f;
                
                // if component json contains postition and position contains y, use y, else 0
                yPos = (component.value().contains("position") && component.value().find("position").value().contains("y"))
                    ? component.value()["position"]["y"].get<float>() : 0.0f;

                // if component json contains scale and scale contains x, use x, else 0
                xScale = (component.value().contains("scale") && component.value().find("scale").value().contains("x"))
                    ? component.value()["scale"]["x"].get<float>() : 1.0f;

                // if component json contains scale and scale contains y, use y, else 0
                yScale = (component.value().contains("scale") && component.value().find("scale").value().contains("y"))
                    ? component.value()["scale"]["y"].get<float>() : 1.0f;




                friction = component.value().contains("friction") ? component.value()["friction"].get<float>() : 0;
                density = component.value().contains("density") ? component.value()["density"].get<float>() : 1;

                // TODO: do more than just check for one string
                bodyType = component.value().contains("b2bodytype") && component.value()["b2bodytype"].get<string>() == "b2_dynamicBody" ? b2_dynamicBody : b2_staticBody;

                coordinator->GetComponent<PhysicsComponent>(ent) = {
                bodyType,
                0.5f * yScale,
                0.5f * xScale,
                xPos,
                yPos,
                density,
                friction
                };



                break;

            case ANIMATION:

                break;

            }
        }
        entities.push_back(ent);

    }

}


