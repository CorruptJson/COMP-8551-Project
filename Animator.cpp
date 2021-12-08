#include "Animator.h"

void Animator::updateAnim(EntityCoordinator* coordinator)
{

    std::shared_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<RenderComponent>(),
        coordinator->GetComponentType<AnimationComponent>()
        }, {});

    int entitiesFound = entityQuery->totalEntitiesFound();
    ComponentIterator<RenderComponent> renderComponents = ComponentIterator<RenderComponent>(entityQuery);
    ComponentIterator<AnimationComponent> animComps = ComponentIterator<AnimationComponent>(entityQuery);
    
    for (int i = 0; i < entitiesFound; i++) {

        AnimationComponent* animationComponent = animComps.nextComponent();
        RenderComponent* renderCompnent = renderComponents.nextComponent();

        //skips the iteration if rendercomponent doesn't have an animation component
        if (animationComponent->isPlaying) {
            animationComponent->currTimeStamp = std::clock();

            if (renderCompnent->rowIndex != animationComponent->currAnim->row)
                renderCompnent->rowIndex = animationComponent->currAnim->row;

            //checks frame timing for switching frames
            if ((animationComponent->currTimeStamp - animationComponent->lastTimeStamp) >= animationComponent->currAnim->speed) {
                animationComponent->lastTimeStamp = std::clock();

                animationComponent->currFrame++;

                //check if it reached the end
                if (animationComponent->currFrame >= animationComponent->currAnim->endFrame + 1) {
                    animationComponent->currFrame = animationComponent->currAnim->startFrame;

                    //add finite state machine behaviour here *TBD
                }

                renderCompnent->colIndex = animationComponent->currFrame;
            }
        }
    }
}

AnimationComponent Animator::createAnimationComponent(Animation* anim, bool isPlaying)
{
    
    AnimationComponent component{
        anim,
        0.0f, //starts off at zero for currTimeStamp
        0.0f, //starts off at zero for lastTimeStamp
        anim->startFrame,
        isPlaying
    };

    return component;
}

Animation Animator::createAnimation(std::string name, int startFrame, int endFrame, int row, bool looping, float speed)
{
    //creates the animation struct
    Animation anim{
        name,
        startFrame, endFrame,
        row,
        looping,
        speed
    };
    return anim;
}
