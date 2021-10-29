#include "inputSystem.h"

InputSystem::InputSystem()
{

}

//InputSystem::InputSystem(EntityCoordinator* coord) : System{ coord } {}

void InputSystem::preUpdate()
{
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::A))
    {
        std::cout << "pressed the A key!" << std::endl;
    }

    if (InputTracker::getInstance().isKeyDown(InputTracker::A))
    {
        std::cout << "A key down!" << std::endl;
    }

    if (InputTracker::getInstance().isKeyJustReleased(InputTracker::A))
    {
        std::cout << "A key just released!" << std::endl;
    }
}

void InputSystem::update()
{

}
