#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
//#include <map>
//#include <string>

class InputTracker {
public:
    //std::map<std::string, int> keys = {
    //    // All letter keys
    //    {"A", GLFW_KEY_A},
    //    {"B", GLFW_KEY_B},
    //    {"C", GLFW_KEY_C},
    //    {"D", GLFW_KEY_D},
    //    {"E", GLFW_KEY_E},
    //    {"F", GLFW_KEY_F},
    //    {"G", GLFW_KEY_G},
    //    {"H", GLFW_KEY_H},
    //    {"I", GLFW_KEY_I},
    //    {"J", GLFW_KEY_J},
    //    {"K", GLFW_KEY_K},
    //    {"L", GLFW_KEY_L},
    //    {"M", GLFW_KEY_M},
    //    {"N", GLFW_KEY_N},
    //    {"O", GLFW_KEY_O},
    //    {"P", GLFW_KEY_P},
    //    {"Q", GLFW_KEY_Q},
    //    {"R", GLFW_KEY_R},
    //    {"S", GLFW_KEY_S},
    //    {"T", GLFW_KEY_T},
    //    {"U", GLFW_KEY_U},
    //    {"V", GLFW_KEY_V},
    //    {"W", GLFW_KEY_W},
    //    {"X", GLFW_KEY_X},
    //    {"Y", GLFW_KEY_Y},
    //    {"Z", GLFW_KEY_Z},

    //    // All number keys
    //    {"ZERO", GLFW_KEY_0},
    //    {"ONE", GLFW_KEY_1},
    //    {"TWO", GLFW_KEY_2},
    //    {"THREE", GLFW_KEY_3},
    //    {"FOUR", GLFW_KEY_4},
    //    {"FIVE", GLFW_KEY_5},
    //    {"SIX", GLFW_KEY_6},
    //    {"SEVEN", GLFW_KEY_7},
    //    {"EIGHT", GLFW_KEY_8},
    //    {"NINE", GLFW_KEY_9},

    //    // All movement keys
    //    {"SPACE", GLFW_KEY_SPACE},
    //    {"RIGHT", GLFW_KEY_RIGHT},
    //    {"LEFT", GLFW_KEY_LEFT},
    //    {"UP", GLFW_KEY_UP},
    //    {"DOWN", GLFW_KEY_DOWN},

    //};

    const int numberStartIndex = 1;
    const int letterStartIndex = 1 + (NINE - ZERO + 1);
    const int arrowStartIndex = 1 + (NINE - ZERO + 1) + (Z - A + 1);

    enum Key {

        // Space key
        SPACE = GLFW_KEY_SPACE, //returns 32

        // All number keys
        ZERO = GLFW_KEY_0, //returns 48
        ONE = GLFW_KEY_1,
        TWO = GLFW_KEY_2,
        THREE = GLFW_KEY_3,
        FOUR = GLFW_KEY_4,
        FIVE = GLFW_KEY_5,
        SIX = GLFW_KEY_6,
        SEVEN = GLFW_KEY_7,
        EIGHT = GLFW_KEY_8,
        NINE = GLFW_KEY_9, //returns 57
        

        // All letter keys
        A = GLFW_KEY_A, //returns 65
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z, //returns 90

        // Arrow keys
        ARROW_RIGHT = GLFW_KEY_RIGHT, //returns 262
        ARROW_LEFT = GLFW_KEY_LEFT,
        ARROW_DOWN = GLFW_KEY_DOWN,
        ARROW_UP = GLFW_KEY_UP, //returns 265

        // Total number of keys
        NUM_KEYS = 1 + (NINE - ZERO + 1 ) + (Z - A + 1) + (ARROW_UP - ARROW_RIGHT + 1)
    };

    enum Mouse {
        MouseLeft = GLFW_MOUSE_BUTTON_1,
        MouseMiddle = GLFW_MOUSE_BUTTON_2,
        MouseRight = GLFW_MOUSE_BUTTON_3,
        ScrollUp = GLFW_MOUSE_BUTTON_4,
        ScrollDown = GLFW_MOUSE_BUTTON_5,

        // Total number of buttons
        NUM_MOUSE_BUTTONS = (ScrollDown - MouseLeft + 1)
    };
    enum ButtonState {
        RELEASED,
        JUST_RELEASED,
        DOWN,
        JUST_DOWN
    };

    bool isKeyDown(Key aKey);
    bool isKeyJustDown(Key aKey);
    bool isKeyReleased(Key aKey);
    bool isKeyJustReleased(Key aKey);

    bool isMouseDown(Mouse aMouseButton);
    bool isMouseJustDown(Mouse aMouseButton);
    bool isMouseReleased(Mouse aMouseButton);
    bool isMouseJustReleased(Mouse aMouseButton);

    void perFrameUpdate(GLFWwindow* window);

    static InputTracker& getInstance();

private:

    //std::vector<ButtonState> mKeyState;
    ButtonState mKeyState[NUM_KEYS] = { };
    //std::vector<ButtonState> mMouseState;
    ButtonState mMouseState[NUM_MOUSE_BUTTONS] = { };
    InputTracker();

};
