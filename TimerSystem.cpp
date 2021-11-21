#include "TimerSystem.h"

void TimerSystem::update() {
    counter++;
    if (counter % 200 == 0) {
        Notify(Event::SPAWN_STAR, nullptr);
    }
    if (counter % 300 == 0) {
        Notify(Event::SPAWN_ENEMY, nullptr);
    }
    if (counter > 3000) {
        counter = 0;
    }
}

void TimerSystem::Notify(Event e, void* args)
{
    for (IObserver* o : observerList) {
        o->Receive(e, nullptr);
    }
}
