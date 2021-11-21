#include "TimerSystem.h"

void TimerSystem::update() {
    counter++;
    if (counter % 200 == 0) {
        Notify(Event::SPAWN_STAR, nullptr);
    }
    if (counter % enemyTimer == 0) {
        Notify(Event::SPAWN_ENEMY, nullptr);
        if (enemyTimer > 150)
            enemyTimer -= 5;
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
