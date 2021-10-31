#pragma once
#include <iostream>

using namespace std;
class EntityCoordinator;
class System {
public:
    //EntityCoordinator* coordinator;
    System() {};
    //System(EntityCoordinator* coord) :coordinator{ coord }{};
    virtual void preUpdate() {};
    virtual void update() {};
    virtual void postUpdate() {};
};
