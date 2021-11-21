#pragma once
#include "system.h"
#include "ISubject.h"
class TimerSystem: public System, public ISubject
{
    int counter;
    void update();
    void Notify(Event e, void* args) override;
};

