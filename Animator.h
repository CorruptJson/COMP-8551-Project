#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include "renderComponent.h"

using namespace std;

class Animator
{
public:
    Animator() {}
    Animator(int spriteCount, int r, int c);
    Animator(int spriteCount, int r ,int c,float speed);
    void startAnim();
    void stopAnim();
    void updateAnim(RenderComponent* comp);
    void setSpeed(float x);
    void setIndex(int r, int c);
    float getCurrRow();
    float getCurrColumn();
private:

    int rowSize, columnSize;

    float currRow, currCol;

    //framerate in ms 
    float speed;

    bool isPlaying;

    //for timing
    float currFrame;
    float lastFrame;

    RenderComponent comp;
};

