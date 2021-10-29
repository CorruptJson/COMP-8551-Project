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
    void startNewAnim(EntityCoordinator* coordinator, EntityID* ent, std::string animName);
    void playAnim();
    void pauseAnim();
    void updateAnim(EntityCoordinator* coordinator);
    void setSpeed();
    static AnimationComponent createAnimationComponent(Animation* anim, bool isPlaying);
    static Animation createAnimation(std::string name, int startFrame, int endFrame, int row, bool looping, float speed);
private:
};

