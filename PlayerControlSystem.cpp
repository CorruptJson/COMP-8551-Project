#pragma once

#include "PlayerControlSystem.h"
#include "InputComponent.h"
#include "Animation.h"
#include "Renderer.h"

void PlayerControlSystem::processEntity(EntityID id) {

    // Getting Components needed for the player
    Renderer* renderer = Renderer::getInstance();
    InputTracker& input = InputTracker::getInstance();
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    Transform* transformComponent = &coordinator.GetComponent<Transform>(id);
    RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);
    StateComponent* stateComponent = &coordinator.GetComponent<StateComponent>(id);
    AnimationComponent* animationComponent = &coordinator.GetComponent<AnimationComponent>(id);

    // Setting animations 
    Animation* animRunning = renderer->getAnimation("running", renderComponent->spriteName);
    Animation* animHurting = renderer->getAnimation("hurt", renderComponent->spriteName);
    


    // Character control
    float xVelocity = moveComponent->xVelocity;
    float yVelocity = moveComponent->yVelocity;

    float speed = 2.0f;
    float jumpForce = 160.0f;
    int jumpCount = 0;
    int jumpLimit = 1;
    bool isReset = false;
    // Colliding with Platform count as ground check 
    //TODO: Improve it with a sensor detector at the bottom of the player to detect if colliding with ground.
    bool isCollided = physComponent->box2dBody->GetContactList();
    //float force = coordinator->GetComponent<PhysicsComponent>(mike).box2dBody->GetMass() * 10 / (1 / 60.0);
    //force /= 3;

    // Animation, flip, and velocity
    if (input.isKeyDown(InputTracker::A)) {
        renderComponent->flipX = false;
        animationComponent->currAnim = animRunning;
        moveComponent->setVelocity(-speed, yVelocity);
    }
    if (input.isKeyDown(InputTracker::D)) {
        renderComponent->flipX = true;
        animationComponent->currAnim = animRunning;
        moveComponent->setVelocity(speed, yVelocity);
    }
    if (input.isKeyDown(InputTracker::S)) {
        animationComponent->currAnim = animHurting;
        moveComponent->setVelocity(0, 0);
    }
    if (isCollided) {
        isReset = true;
        jumpCount = 0;
    }
    else {
        isReset = false;
    }
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::W)) {
        if (isReset) {
            if (jumpCount < jumpLimit) {
                moveComponent->addForce(0, jumpForce);
                jumpCount++;
            }
        }
    }
    if (InputTracker::getInstance().isKeyJustReleased(InputTracker::A) || InputTracker::getInstance().isKeyJustReleased(InputTracker::W)) {
        moveComponent->setVelocity(0, yVelocity);
    }
    if (InputTracker::getInstance().isKeyJustReleased(InputTracker::D) || InputTracker::getInstance().isKeyJustReleased(InputTracker::W)) {
        moveComponent->setVelocity(0, yVelocity);
    }
    if (InputTracker::getInstance().isKeyJustReleased(InputTracker::S)) {
        moveComponent->setVelocity(xVelocity, 0);
    }
    if (InputTracker::getInstance().isKeyJustReleased(InputTracker::W)) {
        if (yVelocity > 0) {
            moveComponent->setVelocity(xVelocity, 0);
        }
    }
    // Testing output
    //std::cout << "xVelocity: " << xVelocity << std::endl;
    //std::cout << "yVelocity: " << yVelocity << std::endl;
}
