#include "PlayerState.h"

void PlayerState::setState(int newState) {
    currState = newState;
}

int PlayerState::getState() {
    return currState;
}

