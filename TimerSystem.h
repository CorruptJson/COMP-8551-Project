#pragma once
#include "system.h"
#include "ISubject.h"
class TimerSystem: public System, public ISubject
{
    int counter;
    int enemyTimer = 120;
    int starTimer = 120;
    int renderTimer = 10;
    void update();
    void Notify(Event e, void* args) override;
};

