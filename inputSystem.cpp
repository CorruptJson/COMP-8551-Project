#include "inputSystem.h"

InputSystem::InputSystem()
{

}

//InputSystem::InputSystem(EntityCoordinator* coord) : System{ coord } {}

void InputSystem::preUpdate()
{
    int currFrame = GameManager::getInstance().getCurrFrame();

    // jump test
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::W))
    {
        std::cout << "pressed the W key on frame " << currFrame << std::endl;
        Notify(Event::INPUT_JUMP, nullptr);
    }

    if (InputTracker::getInstance().isKeyJustDown(InputTracker::J))
    {
        Notify(Event::INPUT_SHOOT, nullptr);
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
