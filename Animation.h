#pragma once
#include <string>

struct Animation {
    std::string animationName;
    int startFrame, endFrame; // the columns for the start and end
    int row; //the row that the animation is within the spritesheet
    bool loop;//controls if the current animation should be looped
    float speed;//controls if the current animation should be looped
    //Animation* nextAnim; //this is how were planning on doing the finite state machine
};
