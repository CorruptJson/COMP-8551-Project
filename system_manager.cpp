#include "system_manager.h"

vector<std::shared_ptr<System>> SystemManager::getSystems() {
    return systemList;
}

//Run all updates in order of priority
void SystemManager::runUpdates() {
    preUpdate();
    update();
    postUpdate();
}

//Runs all systems' preUpdates functions
void SystemManager::preUpdate() {
    for (int i = 0; i < systemList.size(); i++) {
        systemList[i]->preUpdate();
    }
}

//Runs all systems' update functions
void SystemManager::update() {
    for (int i = 0; i < systemList.size(); i++) {
        systemList[i]->update();
    }
}

//Runs all systems' postUpdate functions
void SystemManager::postUpdate() {
    for (int i = 0; i < systemList.size(); i++) {
        systemList[i]->postUpdate();
    }
}
