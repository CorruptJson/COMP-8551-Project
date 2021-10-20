#pragma once

#include "Libraries/include/box2d/box2d.h"

struct PhysicsComponent {
    b2BodyType bodyType;

    float halfHeight;
    float halfWidth;

    float x;
    float y;

    float density;
    float friction;

    b2Body* box2dBody;
};
