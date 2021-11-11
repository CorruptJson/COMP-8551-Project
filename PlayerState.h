#pragma once

#include <iostream>

class PlayerState {
public:
    void setState(int newState);
    int getState();
    const int STATE_NORMAL = 0;
    const int STATE_JUMPING = 1;
    const int STATE_FALLING = 2;
    const int STATE_MOVING = 3;
    const int STATE_HIT = 4;
    const int STATE_SHOOTING = 5;
    bool faceRight = true;

private:
    int currState = 0;
};
