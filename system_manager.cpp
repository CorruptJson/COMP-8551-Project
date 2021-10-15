#include "system_manager.h"



vector<shared_ptr<System>> SystemManager::getSystems() {
    return systemList;
}


void SystemManager::runUpdates() {
    preUpdate();
    update();
    postUpdate();
}

void SystemManager::preUpdate() {
    for (int i = 0; i < systemList.size(); i++) {
        systemList[i]->preUpdate();
    }
}

void SystemManager::update() {
    for (int i = 0; i < systemList.size(); i++) {
        systemList[i]->update();
    }
}

void SystemManager::postUpdate() {
    for (int i = 0; i < systemList.size(); i++) {
        systemList[i]->postUpdate();
    }
}
