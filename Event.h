#pragma once
enum class Event {
    //Input events
    INPUT_JUMP,
    INPUT_SHOOT,
    //Spawner Events
    SPAWN_STAR,
    SPAWN_ENEMY,
    STAR_PICKED_UP,
    //Enemy events
    ENEMY_WALL_BUMP,
    //Collision events
    C_START_PLAYER_ENEMY,
    C_END_PLAYER_ENEMY,
    C_PLAYER_FIRE,
    //Entity events
    B2BODY_TO_DELETE
};

struct B2BodyDeleteEventArgs
{
    EntityID id;
    b2Body* body;
};
