#pragma once
#include <iostream>
using namespace std;
class System {
public:
    int id = 0;
    System() {};
    System(int ID) :id{ ID } {};
    virtual void preUpdate() {};
    virtual void update() {};
    virtual void postUpdate() {};
};
