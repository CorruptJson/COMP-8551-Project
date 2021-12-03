#include "inputSystem.h"

InputSystem::InputSystem()
{

}

//InputSystem::InputSystem(EntityCoordinator* coord) : System{ coord } {}

void InputSystem::preUpdate()
{
    int currFrame = GameManager::getInstance().getCurrGameFrame();

    InputTracker tracker = InputTracker::getInstance();
    if (tracker.isKeyJustDown(InputTracker::W))
    {
        Notify(Event::INPUT_JUMP, nullptr);
    }

    if (tracker.isKeyJustDown(InputTracker::J))
    {
        Notify(Event::INPUT_SHOOT, nullptr);
    }

    if (tracker.isKeyJustDown(InputTracker::Q))
    {
        Notify(Event::INPUT_QUIT, nullptr);
    }
}

void InputSystem::update()
{

}

void InputSystem::Notify(Event e, void* args)
{
    for (IObserver* o : observerList) {
        o->Receive(e, args);
    }
}
