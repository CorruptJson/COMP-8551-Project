#pragma once
#include <string>
#include <vector>
#include <list>

struct Animation {
    const char *animationName;
    float currTimeStamp;//current frame of the animation
    float lastTimeStamp;//current frame of the animation
    int startFrame, endFrame; // the columns for the start and end
    int currFrame; //the col that the animation is in
    int row; //the row that the animation is within the spritesheet
    bool loop;//controls if the current animation should be looped
    //Animation* nextAnim; //this is how were planning on doing the finite state machine
};

struct AnimationComponent {
    Animation currAnim;
    //vector<Animation> animList; //list of animations that can be switched between
    float speed;//controls the time between each frame
    bool isPlaying; //determines if the current animation should be updated
};
