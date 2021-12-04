#include "inputSystem.h"

InputSystem::InputSystem()
{

}

//InputSystem::InputSystem(EntityCoordinator* coord) : System{ coord } {}

void InputSystem::preUpdate()
{

}

void InputSystem::update()
{
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

    //if (tracker.isKeyDown(InputTracker::A))
    //{
    //    Notify(Event::INPUT_LEFT, nullptr);
    //}

    //if (tracker.isKeyDown(InputTracker::D))
    //{
    //    Notify(Event::INPUT_RIGHT, nullptr);
    //}
}

void InputSystem::Notify(Event e, void* args)
{
    for (IObserver* o : observerList) {
        o->Receive(e, args);
    }
}
