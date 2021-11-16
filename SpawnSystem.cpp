#include "SpawnSystem.h"

SpawnSystem::SpawnSystem() {
    ec = &EntityCoordinator::getInstance();
}

void SpawnSystem::Receive(Event e, void* args)
{
    switch (e) {
    case Event::SPAWN_STAR:
        SpawnStar();
        break;
    }
}

void SpawnSystem::SpawnStar()
{
    GameManager gm = GameManager::getInstance();
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    GameEntityCreator& creator = GameEntityCreator::getInstance();
    PhysicsWorld& physWorld = PhysicsWorld::getInstance();

    // create a new entity for bullet
    float xPos = transformComponent.getPosition().x + transformComponent.getScale().x / 2;
    float yPos = transformComponent.getPosition().y;
    EntityID star = creator.CreateStar(xPos, yPos, transformComponent.getScale().x / 2, transformComponent.getScale().y / 2, "star.png", { Tag::STAR }, false);

    std::unique_ptr<EntityQuery> eq = ec->GetEntityQuery({
        ec->GetComponentType<Transform>()
        });

    int entityCount = eq->totalEntitiesFound();

    std::vector<Transform*> spawnLocations;
    
    ComponentIterator<Transform> transformComponents = ComponentIterator<Transform>(eq);

    for (int i = 0; i < entityCount; i++)
    {
        Transform* r = transformComponents.nextComponent();
    }
    for (Chunk* c : eq->foundChunks()) {
        if (c->hasTag(Tag::SPAWNPOINT)) {
            Transform* t = c->getComponentArray<Transform>();
            
        }
    }

    physWorld.AddObject(star);
}

void SpawnSystem::SpawnEnemy()
{

}
