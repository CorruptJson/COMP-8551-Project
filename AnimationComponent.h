#pragma once
#include "Animation.h"

struct AnimationComponent {
    Animation* currAnim;

    float currTimeStamp;//current frame of the animation
    float lastTimeStamp;//current frame of the animation

    int currFrame; //the col that the animation is in

    bool isPlaying = true; //determines if the current animation should be updated
};
