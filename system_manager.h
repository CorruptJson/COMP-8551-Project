#pragma once

#include <vector>
#include "system.h"
#include "test_system.h"

using namespace std;


class SystemManager
{
public:
    vector<System*> systemList;
    void preUpdate();
    void update();
    void postUpdate();
    void initializeSystemManager();
};

