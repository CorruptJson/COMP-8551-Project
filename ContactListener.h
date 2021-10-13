#pragma once

#include <iostream>
#include "Libraries/include/box2d/box2d.h"

class ContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact) {
        std::cout << "Contact made!" << std::endl;
    };
    void EndContact(b2Contact* contact) {};
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};
};
