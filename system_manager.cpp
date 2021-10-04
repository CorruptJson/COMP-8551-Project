#include "system_manager.h"



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

void SystemManager::initializeSystemManager() {
    systemList.push_back(new TestSystem{ 1 });
}
