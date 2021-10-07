#pragma once



struct Position
{
    float x;
    float y;
};

struct Scale
{
    float x;
    float y;
};


using Rotation = float;

struct Transform
{
    Position Position;
    Scale Scale;
    Rotation Rotation;
};
