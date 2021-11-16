#include "TimerSystem.h"

void TimerSystem::update() {
    counter++;
    if (counter > 5000) {
        Notify(Event::SPAWN_STAR, nullptr);
        counter = 0;
    }
}

void TimerSystem::Notify(Event e, void* args)
{
    for (IObserver* o : observerList) {
        o->Receive(e, nullptr);
    }
}
