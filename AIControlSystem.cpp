#include "AIControlSystem.h"

AIControlSystem::~AIControlSystem() {

}

void AIControlSystem::processEntity(EntityID id) {

    // Components for enemy
    Renderer* renderer = Renderer::getInstance();
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    if (&coordinator.GetComponent<PhysicsComponent>(id)) {

    }
    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    Transform* transformComponent = &coordinator.GetComponent<Transform>(id);
    RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);
    GameEntityCreator& creator = GameEntityCreator::getInstance();
    PhysicsWorld& physWorld = PhysicsWorld::getInstance();

    // velocity components
    float xVelocity = moveComponent->getVelocity().x;
    float yVelocity = moveComponent->getVelocity().y;

    // test speed for now
    float speed = 2.5f;
    bool isReset = false;

    //bool isCollided = physComponent->box2dBody->GetContactList();
    
    // TODO: check collision between faces/edges to flip enemies
    // also ignore collision on star

    // Currently testing this way because there is no state connected to entities with ENEMY tag
    if (renderComponent->flipX) {
        renderComponent->flipX = false;
        moveComponent->setVelocity(-speed, yVelocity);
    }

}

bool AIControlSystem::enemyDeleted() {

}
