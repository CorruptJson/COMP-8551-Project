#pragma once
struct FloatPair
{
    float x;
    float y;

    void add(FloatPair other)
    {
        x += other.x;
        y += other.y;
    }
};

struct ParticleMove
{
    FloatPair velocity;
    FloatPair change;
};
