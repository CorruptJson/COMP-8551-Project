#include <iostream>
#include <vector>
#include "Renderer.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "Transform.h"
#include "Types.h"

//ChunkManager* chunkManager;
EntityCoordinator coordinator;

Renderer renderer;

// test entities
Entity entity1;
Entity entity2;

// gets called once when engine starts
// put initilization code here
int initialize()
{
    // when the engine starts
    renderer.init();
    coordinator.Init();
    Signature signature;


    coordinator.RegisterComponent<Transform>();
    signature.set(coordinator.GetComponentType<Transform>());

    entity1 = coordinator.CreateEntity();

    //coordinator.AddComponent(entity1, renderComp1);

    //chunkManager = new ProtoChunkManager();

    return 0;
}





// Use for now to make entities with components
Entity CreateStandardEntity() {
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
    renderer.update();

    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderer.teardown();

    //delete chunkManager;

    return 0;
}

int main() {
    initialize();


    //entity test

    entity1 = CreateStandardEntity();
    entity2 = CreateStandardEntity();

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
