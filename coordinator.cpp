#include "coordinator.h"

void Coordinator::initializeCoordinator() {
    systemManager.initializeSystemManager();
}

void Coordinator::runSystemUpdates(){
    systemManager.preUpdate();
    systemManager.update();
    systemManager.postUpdate();
}
