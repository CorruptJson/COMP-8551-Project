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

    if (isWallCollision(id)) {
        switchDirection(id);
    }
}

void AIControlSystem::Update(EntityCoordinator* coordinator) {
    std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<MovementComponent>(),
    });

    int entitiesFound = entityQuery->totalEntitiesFound();
    std::vector<MovementComponent*> moveComponents = entityQuery->getComponentArray<MovementComponent>();

    for (int i = 0; i < entitiesFound; i++) {
    }
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
                return true;
            }
        }

        contactList = contactList->next;
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
