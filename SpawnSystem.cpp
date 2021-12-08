#include "SpawnSystem.h"
#include "Sound.h"

SpawnSystem::SpawnSystem() {
    ec = &EntityCoordinator::getInstance();
    gameEntCreator = &GameEntityCreator::getInstance();
}

//Receives events to spawn various entities
void SpawnSystem::Receive(Event e, void* args)
{
    GameManager& gm = GameManager::getInstance();
    switch (e) {
    case Event::SPAWN_STAR:
        Sound::getInstance().playSound(STARSHINING);
        if(!gm.hasActiveStar)
            SpawnStar();
        break;
    case Event::STAR_PICKED_UP:
        gm.hasActiveStar = false;
        Sound::getInstance().playSound(STARGRABBED);
        break;
    case Event::SPAWN_ENEMY:
        Sound::getInstance().playSound(COCKROACHHIT);
        SpawnEnemy();
        break;
    case Event::ENEMY_BULLET:
        Sound::getInstance().playSound(DEATHORHIT);
        break;
    case Event::ENEMY_DEATH:
        Sound::getInstance().playSound(ENEMYDEATH);
        break;
    }
}
//Spawns a star at one of the spawn locations specified in the scene json
void SpawnSystem::SpawnStar()
{
    std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
        ec->GetComponentType<Transform>()
        }, {
            Tag::SPAWNPOINT
        });

    //Stop if no spawn points are found
    if (eq->totalEntitiesFound() < 1)
        return;

    ComponentIterator<Transform> tI = ComponentIterator<Transform>(eq);

    srand(time(NULL));
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
    EntityID star = GameEntityCreator::getInstance().CreateStar(xPos, yPos, 1, 1, "star.png", { Tag::STAR });

    PhysicsWorld::getInstance().AddObject(star);
    GameManager::getInstance().hasActiveStar = true;
}

//Spawns an enemy at the enemy spawner entity
void SpawnSystem::SpawnEnemy()
{
    std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
        ec->GetComponentType<Transform>()
        }, {
            Tag::ENEMYSPAWNER
        });

    //Stop if no spawn points are found
    if (eq->totalEntitiesFound() < 1)
        return;

    ComponentIterator<Transform> tI = ComponentIterator<Transform>(eq);
    Transform* t = tI.nextComponent();

    float xPos = t->getPosition().x;
    float yPos = t->getPosition().y;

    srand(time(NULL));
    //Choosing a random spawn location
    bool facingRight = rand() % 2;
    bool isRoach = rand() % 2;
    if (isRoach == 0) {
        gameEntCreator->CreateRoach(xPos, yPos, facingRight);
    }
    else {
        gameEntCreator->CreateSmallRoach(xPos, yPos, facingRight);
    }
}
