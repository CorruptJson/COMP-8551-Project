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
    //Choosing a random spawn location
    int position = rand() % eq->totalEntitiesFound();
    int current = 0;

    //Find the correct position
    Transform* t = tI.nextComponent();
    while (current < position) {
        t = tI.nextComponent();
        current++;
    }
    
    //Create star at location
    float xPos = t->getPosition().x;
    float yPos = t->getPosition().y;
    EntityID star = GameEntityCreator::getInstance().CreateStar(xPos, yPos, 1, 1, "star.png", { Tag::STAR }, true);

    PhysicsWorld::getInstance().AddObject(star);
    hasActiveStar = true;
}

void SpawnSystem::SpawnEnemy()
{

}
