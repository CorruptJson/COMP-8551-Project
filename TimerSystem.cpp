#include "TimerSystem.h"

// this is in fixed update, which runs 60 times a second
void TimerSystem::update() {
    counter++;
    if (counter % starTimer == 0) {
        Notify(Event::SPAWN_STAR, nullptr);
    }
    if (counter % enemyTimer == 0) {
        Notify(Event::SPAWN_ENEMY, nullptr);
        // speed up the monster spawner as time goes on
        if (enemyTimer > 80)
            enemyTimer -= 4;
    }
    if (counter % renderTimer == 0) //timer for blinking the doodle shader
    {
        Notify(Event::UPDATE_RENDER_TIMER, nullptr);
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
