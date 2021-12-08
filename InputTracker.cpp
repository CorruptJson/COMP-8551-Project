#include "InputTracker.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

bool InputTracker::isKeyDown(Key aKey)
{
    if (aKey == Key::SPACE) { 
        return mKeyState[0] == DOWN || mKeyState[0] == JUST_DOWN; 
    }
    else if (Key::ZERO <= aKey && aKey <= Key::NINE) {
        return mKeyState[aKey - Key::ZERO + numberStartIndex] == DOWN || mKeyState[aKey - Key::ZERO + numberStartIndex] == JUST_DOWN; 
    }
    else if (Key::A <= aKey && aKey <= Key::Z) {
        return mKeyState[aKey - Key::A + letterStartIndex] == DOWN || mKeyState[aKey - Key::A + letterStartIndex] == JUST_DOWN;
    }
    else if (Key::ARROW_RIGHT <= aKey && aKey <= ARROW_UP) {
        return mKeyState[aKey - Key::ARROW_RIGHT + arrowStartIndex] == DOWN || mKeyState[aKey - Key::ARROW_RIGHT + arrowStartIndex] == JUST_DOWN;
    }
    else
    {
        throw "key not present in dictionary?";
    }
}
bool InputTracker::isKeyJustDown(Key aKey) 
{

    if (aKey == Key::SPACE) { 
        return mKeyState[0] == JUST_DOWN; 
    }
    else if (Key::ZERO <= aKey && aKey <= Key::NINE) {
        return mKeyState[aKey - Key::ZERO + numberStartIndex] == JUST_DOWN; 
    }
    else if (Key::A <= aKey && aKey <= Key::Z) {
        int index = aKey - Key::A + letterStartIndex;
        return mKeyState[index] == JUST_DOWN;
    }
    else if (Key::ARROW_RIGHT <= aKey && aKey <= ARROW_UP) {
        return mKeyState[aKey - Key::ARROW_RIGHT + arrowStartIndex] == JUST_DOWN; 
    }
    else
    {
        throw "key not present in dictionary?";
    }
}

bool InputTracker::isKeyReleased(Key aKey) 
{
    if (aKey == Key::SPACE) { 
        return mKeyState[0] == RELEASED || mKeyState[0] == JUST_RELEASED; 
    }
    else if (Key::ZERO <= aKey && aKey <= Key::NINE) {
        return mKeyState[aKey - Key::ZERO + numberStartIndex] == RELEASED || mKeyState[aKey - Key::ZERO + numberStartIndex] == JUST_RELEASED; 
    }
    else if (Key::A <= aKey && aKey <= Key::Z) {
        return mKeyState[aKey - Key::A + letterStartIndex] == RELEASED || mKeyState[aKey - Key::A + letterStartIndex] == JUST_RELEASED; 
    }
    else if (Key::ARROW_RIGHT <= aKey && aKey <= ARROW_UP) {
        return mKeyState[aKey - Key::ARROW_RIGHT + arrowStartIndex] == RELEASED || mKeyState[aKey - Key::ARROW_RIGHT + arrowStartIndex] == JUST_RELEASED; 
    }
    else
    {
        throw "key not present in dictionary?";
    }
}

bool InputTracker::isKeyJustReleased(Key aKey) 
{
    if (aKey == Key::SPACE) {
        return mKeyState[0] == JUST_RELEASED; 
    }
    else if (Key::ZERO <= aKey && aKey <= Key::NINE) {
        return mKeyState[aKey - Key::ZERO + numberStartIndex] == JUST_RELEASED; 
    }
    else if (Key::A <= aKey && aKey <= Key::Z) {
        return mKeyState[aKey - Key::A + letterStartIndex] == JUST_RELEASED;
    }
    else if (Key::ARROW_RIGHT <= aKey && aKey <= ARROW_UP) {
        return mKeyState[aKey - Key::ARROW_RIGHT + arrowStartIndex] == JUST_RELEASED; 
    }
    else
    {
        throw "key not present in dictionary?";
    }
}

bool InputTracker::isMouseDown(InputTracker::Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == DOWN || mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_DOWN; }
bool InputTracker::isMouseJustDown(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_DOWN; }
bool InputTracker::isMouseReleased(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == RELEASED || mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_RELEASED; }
bool InputTracker::isMouseJustReleased(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_RELEASED; }

void InputTracker::perFrameUpdate(GLFWwindow* window)
{
    //std::cout << "vector size: " << mKeyState.size() << std::endl;

    int amountSubForPos = Key::SPACE;
    for (int i = Key::SPACE, index = 0; i <= Key::ARROW_UP; i++, index++)
    {
        //int vectorIndex = i - amountSubForPos;
        bool isKeyDown = glfwGetKey(window, i) == GLFW_PRESS;
        ButtonState lastKeyState = mKeyState[index];

        if (isKeyDown && lastKeyState == ButtonState::JUST_DOWN)
            mKeyState[index] = ButtonState::DOWN;
        else if (isKeyDown && (lastKeyState == ButtonState::RELEASED || lastKeyState == ButtonState::JUST_RELEASED))
            mKeyState[index] = ButtonState::JUST_DOWN;

        else if (!isKeyDown && (lastKeyState == ButtonState::RELEASED || lastKeyState == ButtonState::JUST_RELEASED))
            mKeyState[index] = ButtonState::RELEASED;
        else if (!isKeyDown && (lastKeyState == ButtonState::DOWN || lastKeyState == ButtonState::JUST_DOWN))
            mKeyState[index] = ButtonState::JUST_RELEASED;

        if (i == Key::SPACE) {
            i = Key::ZERO - 1;
        }
        else if (i == Key::NINE) {
            i = Key::A - 1;
        }
        else if (i == Key::Z) {
            i = Key::ARROW_RIGHT - 1;
        }
    }

    for (char i = Mouse::MouseLeft; i <= Mouse::ScrollDown; i++)
    {
        int vectorIndex = i - Mouse::MouseLeft;
        bool isMouseDown = glfwGetKey(window, i) == GLFW_PRESS;

        ButtonState lastMouseState = mMouseState[vectorIndex];

        if (isMouseDown && lastMouseState == ButtonState::JUST_DOWN)
            mMouseState[vectorIndex] = ButtonState::DOWN;
        else if (isMouseDown && (lastMouseState == ButtonState::RELEASED || lastMouseState == ButtonState::JUST_RELEASED))
            mMouseState[vectorIndex] = ButtonState::JUST_DOWN;

        else if (!isMouseDown && (lastMouseState == ButtonState::RELEASED || lastMouseState == ButtonState::JUST_RELEASED))
            mMouseState[vectorIndex] = ButtonState::RELEASED;
        else if (!isMouseDown && (lastMouseState == ButtonState::DOWN || lastMouseState == ButtonState::JUST_DOWN))
            mMouseState[vectorIndex] = ButtonState::JUST_RELEASED;
    }


}

InputTracker& InputTracker::getInstance()
{
    static InputTracker instance;
    return instance;
}

InputTracker::InputTracker()
{
}
