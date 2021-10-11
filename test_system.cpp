#include "test_system.h"

void TestSystem::preUpdate(){
    cout << "Test preUpdate ran" << endl;
    //Transform testTransform = coordinator->GetComponent<Transform>(coordinator->testEntity);
    //cout << "Transform Position: " << testTransform.getPosition().x << ", " << testTransform.getPosition().y << endl;
    //coordinator->GetComponent<Transform>(coordinator->testEntity).setPosition(testTransform.getPosition().x + 1, testTransform.getPosition().y);
}

void TestSystem::update(){
    cout << "Test update ran" << endl;
}
