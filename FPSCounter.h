#pragma once
#include <chrono>
#include <iostream>

class FPSCounter
{
private:
    unsigned int framesThisSecond;
    clock_t prevTime;
    clock_t deltaTime = 0;
public:
    FPSCounter();
    void NextFrame();
};

