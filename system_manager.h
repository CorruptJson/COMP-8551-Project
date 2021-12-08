#pragma once

#include <vector>
#include "system.h"

// this class stores and runs systems
// systems must be registered with the system manager in order to be automatically run

class SystemManager
{
public:
    template<typename T>
    shared_ptr<T> addSystem() {
        auto system = std::make_shared<T>();
        systemList.push_back(system);
        return system;
    }

    void addSystem(std::shared_ptr<System> system);

    vector<std::shared_ptr<System>> getSystems();

    void runUpdates();
    void preUpdate();
    void update();
    void postUpdate();
private:
    vector<std::shared_ptr<System>> systemList;
    int curSystemID = 0;
};

