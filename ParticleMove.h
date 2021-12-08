#pragma once

// a simple move component used by "particle" entities
// the change variable is a constant force to be applied to the component's velocity

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
