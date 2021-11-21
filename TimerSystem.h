#pragma once
#include "system.h"
#include "ISubject.h"
class TimerSystem: public System, public ISubject
{
    int counter;
    int enemyTimer = 300;
    void update();
    void Notify(Event e, void* args) override;
};

