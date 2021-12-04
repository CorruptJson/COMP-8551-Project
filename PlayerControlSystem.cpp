#pragma once

#include "PlayerControlSystem.h"
#include "InputComponent.h"
#include "Animation.h"
#include "Renderer.h"
#include "Sound.h"
#include <thread>


PlayerControlSystem::PlayerControlSystem()
{
    invincibleTimer = new b2Timer();
    invincibleTimer->Reset();
    isInvincible = false;
    isInContactWithEnemy = false;
    isRespawning = false;
    health = maxHealth;
    isDead = false;
}

PlayerControlSystem::~PlayerControlSystem()
{
    delete invincibleTimer;
}

void PlayerControlSystem::processPlayer() {
    if (isDead) return;
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    Renderer* renderer = Renderer::getInstance();
    InputTracker& input = InputTracker::getInstance();
    GameEntityCreator& creator = GameEntityCreator::getInstance();
    PhysicsWorld& physWorld = PhysicsWorld::getInstance();


    //init components
    std::shared_ptr<EntityQuery> eq = coordinator.GetEntityQuery(
        {
            coordinator.GetComponentType<PhysicsComponent>(),
            coordinator.GetComponentType<Transform>(),
            coordinator.GetComponentType<RenderComponent>(),
            coordinator.GetComponentType<MovementComponent>(),
            coordinator.GetComponentType<StateComponent>(),
            coordinator.GetComponentType<AnimationComponent>(),

        }, { Tag::PLAYER }
    );

    if (eq->totalEntitiesFound() < 1) {
        return;
    }

    PhysicsComponent* physComponent = eq->getComponentArray<PhysicsComponent>()[0];
    Transform* transformComponent = eq->getComponentArray<Transform>()[0];
    RenderComponent* renderComponent = eq->getComponentArray<RenderComponent>()[0];
    MovementComponent* moveComponent = eq->getComponentArray<MovementComponent>()[0];
    StateComponent* stateComponent = eq->getComponentArray<StateComponent>()[0];
    AnimationComponent* animationComponent = eq->getComponentArray<AnimationComponent>()[0];

    //Sound se;

    // Setting animations 
    Animation* animRunning = renderer->getAnimation("running", renderComponent->spriteName);
    Animation* animHurting = renderer->getAnimation("hurt", renderComponent->spriteName);
    Animation* animIdle = renderer->getAnimation("idle", renderComponent->spriteName);

    // Character control
    float xVelocity = moveComponent->getVelocity().x;
    float yVelocity = moveComponent->getVelocity().y;
    // state
    int currState = stateComponent->state;

    float speed = 5.0f;
    int jumpCount = 0;
    int jumpLimit = 1;
    bool isReset = false;
    // Colliding with Platform count as ground check 
    //TODO: Improve it with a sensor detector at the bottom of the player to detect if colliding with ground.
    bool isCollided = physComponent->box2dBody->GetContactList();
    //float force = coordinator->GetComponent<PhysicsComponent>(mike).box2dBody->GetMass() * 10 / (1 / 60.0);
    //force /= 3;
    if (yVelocity == 0) {
        // Change to normal state only if previous state was falling(no mid air jump)
        if (currState == stateComponent->state == STATE_JUMPING) {
            stateComponent->state = STATE_NORMAL;
        }
        // Set to moving state if not falling and moving on x axis
        if (xVelocity != 0) {
            stateComponent->state = STATE_MOVING;
        }
    } 


    if (!input.isKeyDown(InputTracker::A) && !input.isKeyDown(InputTracker::D)) {
        moveComponent->setVelocity(0, yVelocity);

        if (!(animationComponent->currAnim == animIdle)) {
            animationComponent->currFrame = animIdle->startFrame;
            animationComponent->currAnim = animIdle;
        }
    }

    // Animation, flip, and velocity
    // Play cannot move when respawning
    if (!isRespawning) {
        if (input.isKeyDown(InputTracker::A)) {
            renderComponent->flipX = true;
            if (!(animationComponent->currAnim == animRunning)) {
                animationComponent->currAnim = animRunning;
                animationComponent->currFrame = animRunning->startFrame;
            }
            moveComponent->setVelocity(-speed, yVelocity);
            stateComponent->faceRight = false;
        }
        if (input.isKeyDown(InputTracker::D)) {
            renderComponent->flipX = false;
            if (!(animationComponent->currAnim == animRunning)) {
                animationComponent->currAnim = animRunning;
                animationComponent->currFrame = animRunning->startFrame;
            }
            moveComponent->setVelocity(speed, yVelocity);
            stateComponent->faceRight = true;
        }
    }
    else
    {
        moveComponent->setVelocity(0, 0);
    }

    // Update isInvincible boolean and play animation
    if (isInvincible) {
        if (!(animationComponent->currAnim == animHurting)) {
            animationComponent->currAnim = animHurting;
            animationComponent->currFrame = animHurting->startFrame;
        }
        isInvincible = invincibleTimer->GetMilliseconds() < invincibleLength;

        renderComponent->shaderName = ShaderName::DOODLE;

        if (!isInvincible) {
            if (!(animationComponent->currAnim == animIdle)) {
                animationComponent->currAnim = animIdle;
                animationComponent->currFrame = animIdle->startFrame;
            }
            renderComponent->shaderName = ShaderName::DEFAULT;
            if (isInContactWithEnemy) damaged();
        }
    }

    // respawn player
    if (health == 0) {
        isDead = true;
        Notify(Event::PLAYER_DIES, {});
        //respawn();
    }
}

void PlayerControlSystem::jump()
{
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    //get player components
    std::shared_ptr<EntityQuery> eq = coordinator.GetEntityQuery(
        {
            coordinator.GetComponentType<StateComponent>(),
            coordinator.GetComponentType<MovementComponent>()

        }, { Tag::PLAYER }
    );
    if (eq->totalEntitiesFound() < 1) {
        return;
    }

    StateComponent& stateComponent = *(eq->getComponentArray<StateComponent>()[0]);

    MovementComponent& moveComponent = *(eq->getComponentArray<MovementComponent>()[0]);

    float jumpForce = 1000.0f;

    if (isGrounded() && !isRespawning) {
        moveComponent.addForce(0, jumpForce);
        stateComponent.state = STATE_JUMPING;
        Sound::getInstance().playSound(JUMP);
    }
}

void PlayerControlSystem::shoot()
{
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    GameEntityCreator& creator = GameEntityCreator::getInstance();
    PhysicsWorld& physWorld = PhysicsWorld::getInstance();

    //get player components
    std::shared_ptr<EntityQuery> eq = coordinator.GetEntityQuery(
        {
            coordinator.GetComponentType<Transform>(),
            coordinator.GetComponentType<StateComponent>()

        }, { Tag::PLAYER }
    );
    if (eq->totalEntitiesFound() < 1) {
        return;
    }
    StateComponent& stateComponent = *(eq->getComponentArray<StateComponent>()[0]);
    Transform& transformComponent = *(eq->getComponentArray<Transform>()[0]);

    // create a new entity for bullet
    float bulletScaleX = transformComponent.getScale().x * 0.75;
    float bulletScaleY = transformComponent.getScale().y * 0.75;
    float xPos = (stateComponent.faceRight) ? transformComponent.getPosition().x + bulletScaleX : transformComponent.getPosition().x - bulletScaleX;
    float yPos = transformComponent.getPosition().y;
    EntityID bullet = creator.CreateActor(xPos, yPos, bulletScaleX, bulletScaleY, "bullet.png", { Tag::BULLET }, false, 0);

    RenderComponent* bulletrenderComp = &coordinator.GetComponent<RenderComponent>(bullet);
    bulletrenderComp->flipX = !stateComponent.faceRight;

    physWorld.AddObject(bullet);

    // set velocity to the bullet entity
    PhysicsComponent* bulletPhysComp = &coordinator.GetComponent<PhysicsComponent>(bullet);
    b2Vec2 bulletVelocity = (stateComponent.faceRight) ? b2Vec2(8, 0) : b2Vec2(-8, 0);
    bulletPhysComp->box2dBody->SetLinearVelocity(bulletVelocity);
}

void PlayerControlSystem::damaged()
{
    if (!isInvincible)
    {
        Sound::getInstance().playSound(PLAYERDEATH);

        invincibleTimer->Reset();
        isInvincible = true;

        //logic for decreasing health
        EntityCoordinator* ec = &EntityCoordinator::getInstance();
        std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
            ec->GetComponentType<TextComponent>()
            }, { Tag::HEALTH_NUM });

        ComponentIterator<TextComponent> tci(eq);
        health--;
        std::string healthTxt = "X ";
        tci.nextComponent()->setText(healthTxt + std::to_string(health));

        /*eq = ec->GetEntityQuery({
            ec->GetComponentType<RenderComponent>()
            }, { Tag::PLAYER});

        ComponentIterator<RenderComponent> rci(eq);

        rci.nextComponent()->shaderName = ShaderName::DEFAULT;*/
    }
    else
    {
        //EntityCoordinator* ec = &EntityCoordinator::getInstance();
        //// Player is invincible
        //std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
        //    ec->GetComponentType<RenderComponent>()
        //    }, { Tag::PLAYER });

        //ComponentIterator<RenderComponent> rci(eq);

        //rci.nextComponent()->shaderName = ShaderName::DOODLE;
    }
}

bool PlayerControlSystem::isGrounded()
{
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    //get player components
    std::shared_ptr<EntityQuery> eq = coordinator.GetEntityQuery(
        {
            coordinator.GetComponentType<PhysicsComponent>()

        }, { Tag::PLAYER }
    );
    if (eq->totalEntitiesFound() < 1) {
        return false;
    }

    PhysicsComponent* physComponentA = eq->getComponentArray<PhysicsComponent>()[0];
    b2ContactEdge* contactList = physComponentA->box2dBody->GetContactList();

    while (contactList != nullptr) {
        PhysicsComponent* physComponetB = reinterpret_cast<PhysicsComponent*>(contactList->other->GetUserData().pointer);

        if (coordinator.entityHasTag(PLATFORM, physComponetB->entityID) && (contactList->contact->GetManifold()->localPoint.y == -0.5 || contactList->contact->GetManifold()->points[0].localPoint.y == -0.5)) {
            return true;
        }

        contactList = contactList->next;
    }

    return false;
}


void PlayerControlSystem::Receive(Event e, void* args)
{
    if (e == Event::C_END_PLAYER_ENEMY)
    {
        isInContactWithEnemy = false;
    }

    if (GameManager::getInstance().GameIsPaused())
    {
        return;
    }

    Sound& se = Sound::getInstance();
    std::shared_ptr<EntityQuery> eq = EntityCoordinator::getInstance().GetEntityQuery({}, { Tag::PLAYER });
    switch (e) {
    case(Event::INPUT_JUMP):
        if (eq->totalEntitiesFound() > 0) {
            jump();            
        }
        break;
    case(Event::INPUT_SHOOT):
        if (eq->totalEntitiesFound() > 0) {
            shoot();
            se.playSound(SHOOT);
        }
        break;
    case(Event::C_START_PLAYER_ENEMY):
        isInContactWithEnemy = true;
        damaged();
        break;
    case(Event::C_PLAYER_FIRE):
        invincibleTimer->Reset();
        health = 0;
        se.playSound(FLAMEDEATH);
        break;
    case(Event::PLAYER_REPLAY):
        health = maxHealth;
        isDead = false;
        break;
    //case(Event::INPUT_LEFT):
    //    break;
    //case(Event::INPUT_RIGHT):
    //    break;
    }
}

void PlayerControlSystem::respawn()
{
    isRespawning = true;
    isInvincible = true;

    GameManager gm = GameManager::getInstance();
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    Transform& spawnerTransformComponent = coordinator.GetComponent<Transform>(gm.PlayerRespawnerID());
    float resPosX = spawnerTransformComponent.getPosition().x;
    float resPosY = spawnerTransformComponent.getPosition().y;
    //get player components
    std::shared_ptr<EntityQuery> eq = coordinator.GetEntityQuery(
        {
            coordinator.GetComponentType<PhysicsComponent>()

        }, { Tag::PLAYER }
    );
    if (eq->totalEntitiesFound() < 1) {
        return;
    }
    PhysicsComponent* physComponentA = eq->getComponentArray<PhysicsComponent>()[0];

    if (invincibleTimer->GetMilliseconds() > respawningTime)
    {
        physComponentA->box2dBody->SetTransform(b2Vec2(resPosX, resPosY), 0);
        isRespawning = false;
        isInvincible = true;
        invincibleTimer->Reset();
        isInContactWithEnemy = false;
        health = maxHealth;

        EntityCoordinator* ec = &EntityCoordinator::getInstance();
        std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
            ec->GetComponentType<TextComponent>()
            }, { Tag::HEALTH_NUM });

        ComponentIterator<TextComponent> tci(eq);
        std::string healthTxt = "X ";
        tci.nextComponent()->setText(healthTxt + std::to_string(health));
    }
}
