#include "SpawnSystem.h"

SpawnSystem::SpawnSystem() {
    ec = &EntityCoordinator::getInstance();
}

void SpawnSystem::Receive(Event e, void* args)
{
    switch (e) {
    case Event::SPAWN_STAR:
        if(!hasActiveStar)
            SpawnStar();
        break;
    case Event::STAR_PICKED_UP:
        hasActiveStar = false;
        break;
    }
}

void SpawnSystem::SpawnStar()
{
    std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
        ec->GetComponentType<Transform>()
        }, {
            Tag::SPAWNPOINT
        });

    ComponentIterator<Transform> tI = ComponentIterator<Transform>(eq);
    int position = rand() % eq->totalEntitiesFound();
    int current = 0;

    std::cout << "Total spawnpoints found: " << eq->totalEntitiesFound() << std::endl;

    std::cout << "Rand: " << position << std::endl;
    Transform* t = tI.nextComponent();
    while (current < position) {
        t = tI.nextComponent();
        current++;
    }
    
    float xPos = t->getPosition().x;
    float yPos = t->getPosition().y;
    EntityID star = GameEntityCreator::getInstance().CreateStar(xPos, yPos, 1, 1, "star.png", { Tag::STAR }, true);

    PhysicsWorld::getInstance().AddObject(star);
    hasActiveStar = true;
}

void SpawnSystem::SpawnEnemy()
{

}
