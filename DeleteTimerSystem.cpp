#include "DeleteTimerSystem.h"

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
            // delete entity
            EntityID id = iterator.getCurrEnt();
            EntityCoordinator::getInstance().scheduleEntityToDelete(id);            
        }
    }
}
