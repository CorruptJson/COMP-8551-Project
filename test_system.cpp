#include "test_system.h"
#include "EntityCoordinator.h"

void TestSystem::preUpdate(){
    //cout << "Test preUpdate ran" << endl;
    //TimerComponent testTransform = coordinator->GetComponent<TimerComponent>(*coordinator->testEntity);
    //cout << "Timer Tick: " << testTransform.ticksPassed << endl;
    //coordinator->GetComponent<TimerComponent>(*coordinator->testEntity).ticksPassed++;
}

void TestSystem::update(){
    //cout << "Test update ran" << endl;
}
