#pragma once
#include "system.h"
#include "TimerComponent.h"
#include <iostream>

using namespace std;
class TestSystem : public System, public ISubject
{
public:
    TestSystem() {};
    void preUpdate() override;
    void update() override;
    void Notify(EntityQuery* eq, list<int> indexList) override;
};
