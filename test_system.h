#pragma once
#include "system.h"
#include <iostream>

using namespace std;
class TestSystem : public System
{
public:
    TestSystem(int ID) : System{ ID } { }
    void preUpdate() override;
    void update() override;
};
