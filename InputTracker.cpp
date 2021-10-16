#include "InputTracker.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

bool InputTracker::isKeyDown(Key aKey)
{
    if (aKey == Key::SPACE) { return mKeyState[0] == DOWN || mKeyState[0] == JUST_DOWN; }
    else if (Key::ZERO <= aKey <= Key::NINE) { return mKeyState[aKey - Key::ZERO + distSpaToNum] == DOWN || mKeyState[aKey - Key::ZERO + distSpaToNum] == JUST_DOWN; }
    else if (Key::A <= aKey <= Key::Z) { return mKeyState[aKey - Key::A + distNumToLett] == DOWN || mKeyState[aKey - Key::A + distNumToLett] == JUST_DOWN; }
    else if (Key::ARROW_RIGHT <= aKey <= ARROW_UP) { return mKeyState[aKey - Key::ARROW_RIGHT + distLettToArr] == DOWN || mKeyState[aKey - Key::ARROW_RIGHT + distLettToArr] == JUST_DOWN; }
}
bool InputTracker::isKeyJustDown(Key aKey) 
{
    if (aKey == Key::SPACE) { return mKeyState[0] == JUST_DOWN; }
    else if (Key::ZERO <= aKey <= Key::NINE) { return mKeyState[aKey - Key::ZERO + distSpaToNum] == JUST_DOWN; }
    else if (Key::A <= aKey <= Key::Z) { return mKeyState[aKey - Key::A + distNumToLett] == JUST_DOWN; }
    else if (Key::ARROW_RIGHT <= aKey <= ARROW_UP) { return mKeyState[aKey - Key::ARROW_RIGHT + distLettToArr] == JUST_DOWN; }
}

bool InputTracker::isKeyReleased(Key aKey) 
{
    if (aKey == Key::SPACE) { return mKeyState[0] == RELEASED || mKeyState[0] == JUST_RELEASED; }
    else if (Key::ZERO <= aKey <= Key::NINE) { return mKeyState[aKey - Key::ZERO + distSpaToNum] == RELEASED || mKeyState[aKey - Key::ZERO + distSpaToNum] == JUST_RELEASED; }
    else if (Key::A <= aKey <= Key::Z) { return mKeyState[aKey - Key::A + distNumToLett] == RELEASED || mKeyState[aKey - Key::A + distNumToLett] == JUST_RELEASED; }
    else if (Key::ARROW_RIGHT <= aKey <= ARROW_UP) { return mKeyState[aKey - Key::ARROW_RIGHT + distLettToArr] == RELEASED || mKeyState[aKey - Key::ARROW_RIGHT + distLettToArr] == JUST_RELEASED; }
}

bool InputTracker::isKeyJustReleased(Key aKey) 
{
    if (aKey == Key::SPACE) { return mKeyState[0] == JUST_RELEASED; }
    else if (Key::ZERO <= aKey <= Key::NINE) { return mKeyState[aKey - Key::ZERO + distSpaToNum] == JUST_RELEASED; }
    else if (Key::A <= aKey <= Key::Z) { return mKeyState[aKey - Key::A + distNumToLett] == JUST_RELEASED; }
    else if (Key::ARROW_RIGHT <= aKey <= ARROW_UP) { return mKeyState[aKey - Key::ARROW_RIGHT + distLettToArr] == JUST_RELEASED; }
}

bool InputTracker::isMouseDown(InputTracker::Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == DOWN || mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_DOWN; }
bool InputTracker::isMouseJustDown(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_DOWN; }
bool InputTracker::isMouseReleased(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == RELEASED || mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_RELEASED; }
bool InputTracker::isMouseJustReleased(Mouse aMouseButton) { return mMouseState[aMouseButton - Mouse::MouseLeft] == JUST_RELEASED; }

void InputTracker::perFrameUpdate(GLFWwindow* window)
{
    int amountSubForPos = Key::SPACE;
    for (char i = Key::SPACE; i <= Key::ARROW_UP; i++)
    {
        int vectorIndex = i - amountSubForPos;
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

        if (i == Key::SPACE) {
            i = Key::ZERO - 1;
            amountSubForPos = Key::ZERO - mKeyState.size();
        }
        else if (i == Key::NINE) {
            i = Key::A - 1;
            amountSubForPos = Key::A - mKeyState.size();
        }
        else if (i == Key::Z) {
            i = Key::ARROW_RIGHT - 1;
            amountSubForPos = Key::ARROW_RIGHT - mKeyState.size();
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
