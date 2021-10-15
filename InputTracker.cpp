#include "InputTracker.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
//#include <string>

bool InputTracker::isKeyDown(Key aKey) { return mKeyState[aKey - 'A'] == DOWN || mKeyState[aKey - 'A'] == JUST_DOWN; }
bool InputTracker::isKeyJustDown(Key aKey) { return mKeyState[aKey - 'A'] == JUST_DOWN; }
bool InputTracker::isKeyReleased(Key aKey) { return mKeyState[aKey - 'A'] == RELEASED || mKeyState[aKey - 'A'] == JUST_RELEASED; }
bool InputTracker::isKeyJustReleased(Key aKey) { return mKeyState[aKey - 'A'] == JUST_RELEASED; }

bool InputTracker::isMouseDown(InputTracker::Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == DOWN || mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_DOWN; }
bool InputTracker::isMouseJustDown(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_DOWN; }
bool InputTracker::isMouseReleased(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == RELEASED || mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_RELEASED; }
bool InputTracker::isMouseJustReleased(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_RELEASED; }

void InputTracker::perFrameUpdate(GLFWwindow* window)
{
    for (char i = Key::A; i <= Key::Z; i++) // if i == 39 change it to 65 OR change the header file to contain an enum of vectors
    {
        int vectorIndex = i - Key::A; // have to change this too
        bool isKeyDown = glfwGetKey(window, i) == GLFW_PRESS;

        ButtonState lastKeyState = mKeyState[vectorIndex];

        if (isKeyDown && lastKeyState == ButtonState::JUST_DOWN)
            mKeyState[vectorIndex] = ButtonState::DOWN;
        else if (isKeyDown && (lastKeyState == ButtonState::RELEASED || lastKeyState == ButtonState::JUST_RELEASED))
            mKeyState[vectorIndex] = ButtonState::JUST_DOWN;

        else if (!isKeyDown && (lastKeyState == ButtonState::RELEASED || lastKeyState == ButtonState::JUST_RELEASED))
            mKeyState[vectorIndex] = ButtonState::RELEASED;
        else if (!isKeyDown && (lastKeyState == ButtonState::DOWN || lastKeyState == ButtonState::JUST_DOWN))
            mKeyState[vectorIndex] = ButtonState::JUST_RELEASED;

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
