#pragma once

#include "Libraries/include/box2d/box2d.h"

struct PhysicsComponent {
    b2BodyType bodyType;

    b2BodyDef bodyDef;
    b2PolygonShape shape;
    b2FixtureDef fixtureDef;
};
