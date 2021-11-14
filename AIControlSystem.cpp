#include "AIControlSystem.h"

AIControlSystem::~AIControlSystem() 
{
}

void AIControlSystem::processEntity(EntityID id) {
    // Components for enemy
    //EntityCoordinator& coordinator = EntityCoordinator::getInstance();

    //RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    //MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);

    // velocity components
    //float xVelocity = moveComponent->getVelocity().x;
    //float yVelocity = moveComponent->getVelocity().y;

    //if (isWallCollision(id)) {
        //switchDirection(id);
    //}

    handleWallCollision(id);
}

void AIControlSystem::handleWallCollision(EntityID id) {
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    PhysicsComponent* physComponent = &coordinator.GetComponent<PhysicsComponent>(id);
    RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);
    b2ContactEdge* contactList = physComponent->box2dBody->GetContactList();

    float yVelocity = moveComponent->getVelocity().y;

    while (contactList != nullptr) {
        // 0.5 from left, -0.5 from right
        PhysicsComponent* physComponentB = reinterpret_cast<PhysicsComponent*>(contactList->other->GetUserData().pointer);

        if (coordinator.entityHasTag(PLATFORM, physComponentB->entityID)) {
            //cout << "X point: " << contactList->contact->GetManifold()->localPoint.x << endl;

            if (contactList->contact->GetManifold()->localPoint.x == 0.5) {
                renderComponent->flipX = !renderComponent->flipX;
                moveComponent->setVelocity(2.0f, yVelocity);
                //return true;
            }
            else if (contactList->contact->GetManifold()->localPoint.x == 0.5) {
                moveComponent->setVelocity(-2.0f, yVelocity);
                //return true;
            }

        }

        contactList = contactList->next;
    }
    //return false;
}

void AIControlSystem::switchDirection(EntityID id) {
    //EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    //RenderComponent* renderComponent = &coordinator.GetComponent<RenderComponent>(id);
    //MovementComponent* moveComponent = &coordinator.GetComponent<MovementComponent>(id);

    //float xVelocity = moveComponent->getVelocity().x;
    //float yVelocity = moveComponent->getVelocity().y;

    //renderComponent->flipX = !renderComponent->flipX;
    //moveComponent->setVelocity(-xVelocity, yVelocity);
}

/*void AIControlSystem::Receive(Event e, void* args) {
    switch (e) {
    case Event::ENEMY_WALL_BUMP:
        switchDirection();
        break;
    }
}*/
