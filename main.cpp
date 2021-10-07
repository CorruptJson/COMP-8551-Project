#include <iostream>
#include <vector>
#include "./rendering/RenderTutorial.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "Transform.h"
#include "Types.h"

//ChunkManager* chunkManager;
EntityCoordinator coordinator;

Entity entity1;
Entity entity2;

// gets called once when engine starts
// put initilization code here
int initialize()
{
    // when the engine starts
    renderTutorialInit();
    
    coordinator.Init();
    Signature signature;


    coordinator.RegisterComponent<Transform>();
    signature.set(coordinator.GetComponentType<Transform>());

    //chunkManager = new ProtoChunkManager();

    return 0;
}





// Use for now to make entities with components
Entity CreateEntity() {
    Entity e = coordinator.CreateEntity();

    coordinator.AddComponent<Transform>(e, Transform{});

    return e;

}




// the main update function
// game loop will be put here eventually
int runEngine()
{
    // check input
    // run physics
    // run ECS
    // render
    renderTutorialUpdate();

    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderTutorialTeardown();

    //delete chunkManager;

    return 0;
}

int main() {
    initialize();


    //entity test

    entity1 = CreateEntity();
    entity2 = CreateEntity();


    coordinator.GetComponent<Transform>(entity1).Position = { 1, 6 };
    coordinator.GetComponent<Transform>(entity2).Position = { 3, 3 };

    std::cout << "entity1 x: " << coordinator.GetComponent<Transform>(entity1).Position.x << " y: " << coordinator.GetComponent<Transform>(entity1).Position.y << std::endl;
    std::cout << "entity2 x: " << coordinator.GetComponent<Transform>(entity2).Position.x << " y: " << coordinator.GetComponent<Transform>(entity2).Position.y << std::endl;

    
    std::cout << "From Component array: x: " << coordinator.GetComponentArray<Transform>()[0].Position.x << std::endl;
    std::cout << "Number of Entities: " << coordinator.GetEntityCount() << std::endl;



    while (!glfwWindowShouldClose(window))
    {
        // tell glfw to keep track of window resize 
        // and input events
        glfwPollEvents();
        runEngine();
    }    

    teardown();

    return 0;
}
