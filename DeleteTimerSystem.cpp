#include "DeleteTimerSystem.h"

// the DeleteTimer component has a timestamp for when an entity should be deleted
// this system checks if the current game frame count has reached that timestamp,
//      and calls for the entity to be deleted if it has

// this allows for us to have entities with specified lifetime

void DeleteTimerSystem::update()
{
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    std::shared_ptr<EntityQuery> query = coordinator.GetEntityQuery({
        coordinator.GetComponentType<DeleteTimer>()
        }, {});
    ComponentIterator<DeleteTimer> iterator = ComponentIterator<DeleteTimer>(query);

    int entCount = query->totalEntitiesFound();
    int currGameFrame = GameManager::getInstance().getCurrGameFrame();

    for (int i = 0; i < entCount; i++)
    {
        DeleteTimer* component = iterator.nextComponent();
        if (currGameFrame >= component->deletionFrame)
        {
            EntityID id = iterator.getCurrEnt();
            EntityCoordinator::getInstance().scheduleEntityToDelete(id);            
        }
    }
}
