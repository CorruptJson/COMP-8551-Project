#include "FPSCounter.h"

FPSCounter::FPSCounter()
{
    prevTime = clock();
}

void FPSCounter::NextFrame()
{
    framesThisSecond++;

    clock_t currTime = clock();
    deltaTime += currTime - prevTime;
    prevTime = currTime;

    double deltaSeconds = (deltaTime / (double)CLOCKS_PER_SEC) * 1000.0;

    if (deltaSeconds > 1000.0)
    {
        std::cout << "FPS: " << framesThisSecond << std::endl;
        framesThisSecond = 0;
        deltaTime -= CLOCKS_PER_SEC;
    }    
}
