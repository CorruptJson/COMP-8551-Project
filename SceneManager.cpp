#include "SceneManager.h"
#include "file_manager.h"

// Component Keys
enum eKeys
{
    TRANSFORM,
};

// convert strings to enums here
std::map<std::string, eKeys> keyMap = {
    {"transform", TRANSFORM},
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

        for (auto& component : entity.value().items()) {

            // setup json formatting here
            switch (keyMap[component.key()]) {


                case TRANSFORM : 
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

                    break;









            }
        }

    }

}

