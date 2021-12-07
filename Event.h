#pragma once
#include "Types.h"

enum class Event {
    //Input events
    INPUT_JUMP,
    INPUT_SHOOT,
    INPUT_QUIT,
    INPUT_LEFT,
    INPUT_RIGHT,

    //Spawner Events
    SPAWN_STAR,
    SPAWN_ENEMY,
    STAR_PICKED_UP,

    //Enemy events
    ENEMY_WALL_BUMP,
    ENEMY_DESTROYED,

    //Collision events
    C_START_PLAYER_ENEMY,
    C_END_PLAYER_ENEMY,
    C_PLAYER_FIRE,

    //Entity events
    B2BODY_TO_DELETE,
    B2BODY_ADD,

    // player events
    PLAYER_DIES,
    PLAYER_REPLAY,

    // enemy events with bullets
    ENEMY_BULLET,
    ENEMY_DEATH
};

struct B2BodyDeleteEventArgs
{
    EntityID id;
    b2Body* body;
};
