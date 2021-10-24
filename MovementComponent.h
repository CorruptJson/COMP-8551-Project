#pragma once

#include "Libraries/include/box2d/box2d.h"

struct MovementComponent {

    b2Vec2 velocityVector;
    bool isGrounded;
    float jumpHeight;
    bool wake;

    b2Body* box2dBody;
};
