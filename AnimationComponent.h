#pragma once
#include <string>
#include <vector>
#include <list>

struct Animation {
    const char *animationName;
    int startFrame, endFrame; // the columns for the start and end
    int row; //the row that the animation is within the spritesheet
    bool loop;//controls if the current animation should be looped
    float speed;//controls if the current animation should be looped
    //Animation* nextAnim; //this is how were planning on doing the finite state machine
};

struct AnimationComponent {
    Animation currAnim;

    float currTimeStamp;//current frame of the animation
    float lastTimeStamp;//current frame of the animation

    int currFrame; //the col that the animation is in

    bool isPlaying; //determines if the current animation should be updated
};
