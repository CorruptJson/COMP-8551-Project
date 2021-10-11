#pragma once

#include <vector>
#include "system.h"
#include "test_system.h"


using namespace std;
class SystemManager
{
public:
    template<typename T>
    shared_ptr<T> addSystem() {
        auto system = make_shared<T>();
        systemList.push_back(system);
        return system;
    }
    vector<shared_ptr<System>> getSystems();
private:
    vector<shared_ptr<System>> systemList;
    int curSystemID = 0;
};

