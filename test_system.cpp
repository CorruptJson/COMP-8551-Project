#include "test_system.h"
#include "EntityCoordinator.h"

void TestSystem::preUpdate(){
    //TODO
}

void TestSystem::update(){
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    
    std::unique_ptr<EntityQuery> entityQuery = ec.GetEntityQuery({
        ec.GetComponentType<TimerComponent>()
        });
    int entitiesFound = entityQuery->totalEntitiesFound();
    
    if (entitiesFound > 0) {
        vector<TimerComponent*> compList = entityQuery->getComponentArray<TimerComponent>();
        TimerComponent* testTimer;
        list<int> triggeredIndexes;
        for (int i = 0; i < entityQuery->totalEntitiesFound(); i++) {
            testTimer = compList[i];
            testTimer->ticksPassed++;
            if (testTimer->ticksPassed == 200) {
                triggeredIndexes.push_back(i);
            }
        }
        if (triggeredIndexes.size() > 0)
            Notify(entityQuery.get(), triggeredIndexes);
    }
}

void TestSystem::Notify(EntityQuery* eq, list<int> indexList) {
    for (IObserver* o : observerList) {
        o->Receive(Event::TIMER_REACHED_200, eq, indexList);
    }
}
