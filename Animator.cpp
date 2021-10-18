#include "Animator.h"

#pragma region TBD
//waiting for new ecs system integration
void Animator::startNewAnim()
{
    //sets the old anim back to to the beginning
    //entity.animationcomponent.currentAnim.currFrame = 0;

    //Animation newAnim = entity.animationComponent.animList.find(AnimName);
    //entity.animationcomponent.currentAnim = newAnim;
    //entity.renderComponent.rowIndex = newAnim.row;
    //entity.renderComponent.colIndex = newAnim.currFrame;
}

//plays the currently set animation
void Animator::playAnim() {
    //entity.animationComponent.isPlaying = true;
}

//waiting for new ecs system integration
void Animator::pauseAnim()
{
    //entity.animationComponent.isPlaying = true;
}

void Animator::setSpeed(Entity* x)
{
    //entity.animationComponent.speed = 500.0f;
}

#pragma endregion TBD

void Animator::updateAnim(EntityCoordinator* coordinator)
{
    std::array<RenderComponent, MAX_ENTITIES>& renderComps = coordinator->GetComponentArray<RenderComponent>();
    std::array<AnimationComponent, MAX_ENTITIES>& animComps = coordinator->GetComponentArray<AnimationComponent>();
    
    for (int i = 0; i < coordinator->GetEntityCount(); i++) {

        AnimationComponent* animationComponent = &(animComps[i]);
        RenderComponent* renderCompnent = &(renderComps[i]);

        //skips the iteration if rendercomponent doesn't have an animation component
        if (renderCompnent->hasAnimation) {

            if (animationComponent->isPlaying) {
                animationComponent->currAnim.currTimeStamp = std::clock();
                //checks frame timing for switching frames
                if ((animationComponent->currAnim.currTimeStamp - animationComponent->currAnim.lastTimeStamp) >= animationComponent->speed) {
                    animationComponent->currAnim.lastTimeStamp = std::clock();

                    animationComponent->currAnim.currFrame++;

                    //check if it reached the end
                    if (animationComponent->currAnim.currFrame >= animationComponent->currAnim.endFrame + 1) {
                        animationComponent->currAnim.currFrame = animationComponent->currAnim.startFrame;

                        //add finite state machine behaviour here *TBD
                    }

                    renderCompnent->colIndex = animationComponent->currAnim.currFrame;
                }
            }
        }
    }
}


AnimationComponent Animator::createAnimationComponent(Animation current, float speed, bool isPlaying)
{
    //empty animation vector
    vector<Animation> animVector;
    
    AnimationComponent component{
        current,
        //anim,
        speed,
        isPlaying
    };

    return component;
}

Animation Animator::createAnimation(const char* name, int startFrame, int endFrame, int row, bool looping)
{
    //creates the animation struct
    Animation anim{
        name,
        0.0f, //starts off at zero for currTimeStamp
        0.0f, //starts off at zero for lastTimeStamp
        startFrame, endFrame,
        startFrame,
        row,
        looping
    };
    return anim;
}
