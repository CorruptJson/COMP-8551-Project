#include "inputSystem.h"

InputSystem::InputSystem()
{

}

//InputSystem::InputSystem(EntityCoordinator* coord) : System{ coord } {}

void InputSystem::preUpdate()
{
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::W))
    {
        std::cout << "pressed the W key!" << std::endl;
    }
    //if (InputTracker::getInstance().isKeyDown(InputTracker::A))
    //{
    //    std::cout << "hold the W key!" << std::endl;
    //}
    //if (InputTracker::getInstance().isKeyJustReleased(InputTracker::W))
    //{
    //    std::cout << "jsut release the W key!" << std::endl;
    //}
    //if (InputTracker::getInstance().isKeyReleased(InputTracker::W))
    //{
    //    std::cout << "release the W key!" << std::endl;
    //}
}

void InputSystem::update()
{

}
