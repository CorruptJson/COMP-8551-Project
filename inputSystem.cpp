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
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::A))
    {
        std::cout << "pressed the A key!" << std::endl;
    }
}
