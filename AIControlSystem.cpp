#include "AIControlSystem.h"

AIControlSystem::~AIControlSystem() {

}

void AIControlSystem::processEntity(EntityID id) {

    // Components for enemy
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);

    // velocity components
    float xVelocity = moveComponent->getVelocity().x;
    float yVelocity = moveComponent->getVelocity().y;

    //bool isCollided = physComponent->box2dBody->GetContactList();
    
    // TODO: check collision between faces/edges to flip enemies
    // also ignore collision on star

    // Currently testing this way because there is no state connected to entities with ENEMY tag

    float speed = 2.0f;
    xVelocity = speed;
    moveComponent->setVelocity(xVelocity, yVelocity);
    
}

bool AIControlSystem::isWallCollision(EntityID id) {
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    b2ContactEdge* contactList = physComponent->box2dBody->GetContactList();

    while (contactList != nullptr) {
        // 0.5 on right, -0.5 on left
        PhysicsComponent* physComponentB = reinterpret_cast<PhysicsComponent*>(contactList->other->GetUserData().pointer);

        if (coordinator.entityHasTag(PLATFORM, physComponentB->entityID)) {
            if (contactList->contact->GetManifold()->localPoint.x == -0.5 || contactList->contact->GetManifold()->localPoint.x == 0.5) {
                switchDirection(id);
            }
        }
        return true;
    }

    return false;
}

void AIControlSystem::switchDirection(EntityID id) {
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);

    float xVelocity = moveComponent->getVelocity().x;
    float yVelocity = moveComponent->getVelocity().y;

    renderComponent->flipX = !renderComponent->flipX;
    moveComponent->setVelocity(-xVelocity, yVelocity);
}

/*void AIControlSystem::Receive(Event e, void* args) {
    switch (e) {
    case Event::ENEMY_WALL_BUMP:
        switchDirection();
        break;
    }
}*/
