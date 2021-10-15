#pragma once
#include "system.h"
#include "TimerComponent.h"
#include <iostream>

using namespace std;
class TestSystem : public System
{
public:
    TestSystem() {};
    TestSystem(EntityCoordinator* coord) : System{ coord } { }
    void preUpdate() override;
    void update() override;
};
