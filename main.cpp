#include <iostream>
#include "Renderer.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "tempPosition.h"

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
    coordinator.RegisterComponent<TempPosition>();

    Signature signature;
    signature.set(coordinator.GetComponentType<TempPosition>());
    coordinator.AddComponent(entity1);

    //chunkManager = new ProtoChunkManager();

    return 0;
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
    
    entity1 = coordinator.CreateEntity();
    entity2 = coordinator.CreateEntity();
    
    coordinator.AddComponent(entity1, TempPosition{ 1, 6 });
    coordinator.AddComponent(entity2, TempPosition{ 3, 3 });


    std::cout << "entity 1 x: " << coordinator.GetComponent<TempPosition>(entity1).x << "y: " << coordinator.GetComponent<TempPosition>(entity1).y << std::endl;
    std::cout << "entity 1 x: " << coordinator.GetComponent<TempPosition>(entity2).x << "y: " << coordinator.GetComponent<TempPosition>(entity2).y << std::endl;
    


    

    // keep the window open if it's not supposed to close
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
