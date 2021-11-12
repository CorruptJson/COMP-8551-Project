#include "inputSystem.h"

InputSystem::InputSystem()
{

}

//InputSystem::InputSystem(EntityCoordinator* coord) : System{ coord } {}

void InputSystem::preUpdate()
{
    int currFrame = GameManager::getInstance().getCurrFrame();

    // jump test
    //if (InputTracker::getInstance().isKeyJustDown(InputTracker::W))
    //{
    //    std::cout << "pressed the W key on frame " << currFrame << std::endl;
    //}


    /*
    if (InputTracker::getInstance().isKeyDown(InputTracker::W))
    {
        std::cout << "W key down!" << std::endl;
    }

    if (InputTracker::getInstance().isKeyJustReleased(InputTracker::W))
    {
        std::cout << "W key just released!" << std::endl;
    }*/
}

void InputSystem::update()
{

}
