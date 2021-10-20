#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include "renderComponent.h"
#include "AnimationComponent.h"
#include "EntityCoordinator.h"

using namespace std;

class Animator
{
public:
    Animator() {}
    void startNewAnim();
    void playAnim();
    void pauseAnim();
    void updateAnim(EntityCoordinator* coordinator);
    void setSpeed();
    AnimationComponent createAnimationComponent(Animation current, float speed, bool isPlaying);
    Animation createAnimation(const char* name, int startFrame, int endFrame, int row, bool looping);
private:
};

