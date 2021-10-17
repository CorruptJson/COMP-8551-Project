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

void Animator::updateAnim(EntityCoordinator* coordinator)
{
    std::array<RenderComponent, MAX_ENTITIES>& renderComps = coordinator->GetComponentArray<RenderComponent>();
    std::array<AnimationComponent, MAX_ENTITIES>& animComps = coordinator->GetComponentArray<AnimationComponent>();
    
    for (int i = 0; i < coordinator->GetEntityCount(); i++) {

        AnimationComponent* animationComponent = &(animComps[i]);
        RenderComponent* renderCompnent = &(renderComps[i]);

        if (renderCompnent->hasAnimation) {
            //skips the iteration
            if (animationComponent->isPlaying) {
                animationComponent->currAnim.currTimeStamp = std::clock();
                /*cout << "CurrFrame = " + std::to_string(currFrame) << endl;
                cout << "lastFrame = " + std::to_string(lastFrame) << endl;
                cout << "speed = " + std::to_string(speed) << endl;*/
                if ((animationComponent->currAnim.currTimeStamp - animationComponent->currAnim.lastTimeStamp) >= animationComponent->speed) {
                    animationComponent->currAnim.lastTimeStamp = std::clock();

                    cout << "col = " + std::to_string(renderCompnent->colIndex) << endl;
                    cout << "col Anim = " + std::to_string(animationComponent->currAnim.currFrame) << endl;

                    animationComponent->currAnim.currFrame++;

                    renderCompnent->colIndex = animationComponent->currAnim.currFrame;
                }
            }
        }
    }
}

void Animator::setSpeed(Entity x)
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
