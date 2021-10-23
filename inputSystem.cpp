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
}

void InputSystem::update()
{

}
