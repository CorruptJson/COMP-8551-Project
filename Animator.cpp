#include "Animator.h"

Animator::Animator(int spriteCount, int r, int c)
{
    isPlaying = false;
    rowSize = r;
    columnSize = c;
    currRow = 0;
    currCol = 0;
    speed = 1;
    currFrame = 0;
    lastFrame = 0;
}

Animator::Animator(int spriteCount, int r, int c,float spd)
{
    isPlaying = false;
    rowSize = r;
    columnSize = c;
    currRow = 0;
    currCol = 0;
    speed = spd;
    currFrame = 0;
    lastFrame = 0;
}

void Animator::startAnim()
{
    isPlaying = true;
    lastFrame = std::clock(); //sets the initial time stamp of the last frame
}

void Animator::stopAnim()
{
    isPlaying = false;
}

void Animator::updateAnim(RenderComponent* comp)
{
    currFrame = std::clock();
    /*cout << "CurrFrame = " + std::to_string(currFrame) << endl;
    cout << "lastFrame = " + std::to_string(lastFrame) << endl;
    cout << "speed = " + std::to_string(speed) << endl;*/
    if ((currFrame - lastFrame) >= speed) {
        lastFrame = std::clock();

        (*comp).colIndex++;
       /* cout << "col = " + std::to_string(comp.colIndex) << endl;*/
       // cout << "col Anim = " + std::to_string(currCol) << endl;
    }

}

void Animator::setSpeed(float x)
{
    speed = x;
}

void Animator::setIndex(int r, int c)
{
    currRow = r;
    currCol = c;
}

float Animator::getCurrRow()
{
    return currRow;
}

float Animator::getCurrColumn()
{
    return currCol;
}