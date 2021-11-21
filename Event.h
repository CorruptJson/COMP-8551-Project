#pragma once
enum class Event {
    //Input events
    INPUT_JUMP,
    INPUT_SHOOT,
    //Enemy events (unused)
    ENEMY_WALL_BUMP,
    //Collision events
    C_START_PLAYER_ENEMY,
    C_END_PLAYER_ENEMY,
    //Entity events
    B2BODY_TO_DELETE
};

struct B2BodyDeleteEventArgs
{
    EntityID id;
    b2Body* body;
};
