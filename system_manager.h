#pragma once

#include <vector>
#include "system.h"
#include "test_system.h"


using namespace std;
class EntityCoordinator;
class SystemManager
{
public:
    EntityCoordinator* coordinator;
    template<typename T>
    shared_ptr<T> addSystem(EntityCoordinator* coord) {
        auto system = make_shared<T>();
        systemList.push_back(system);
        system->coordinator = coord;
        return system;
    }
    vector<shared_ptr<System>> getSystems();

    void runUpdates();
    void preUpdate();
    void update();
    void postUpdate();
private:
    vector<shared_ptr<System>> systemList;
    int curSystemID = 0;
};

