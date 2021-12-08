#pragma once

#include <vector>
#include "system.h"

//using namespace std;
//class EntityCoordinator;
//EntityCoordinator& getInstance();

class SystemManager
{
public:

    template<typename T>
    shared_ptr<T> addSystem() {
        auto system = std::make_shared<T>();
        systemList.push_back(system);
        return system;
    }

    vector<std::shared_ptr<System>> getSystems();

    void runUpdates();
    void preUpdate();
    void update();
    void postUpdate();
private:
    vector<std::shared_ptr<System>> systemList;
    int curSystemID = 0;
};

