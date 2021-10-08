#include <iostream>
#include <vector>
#include "Renderer.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "Types.h"

//ChunkManager* chunkManager;
EntityCoordinator coordinator;
ChunkManager* chunkManager;

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

    chunkManager = new ChunkManager();

    Signature signature;

    coordinator.RegisterComponent<Transform>();
    signature.set(coordinator.GetComponentType<Transform>());

    coordinator.RegisterComponent<RenderComponent>();
    signature.set(coordinator.GetComponentType<RenderComponent>());

    return 0;
}

// Use for now to make entities with components
Entity CreateStandardEntity() {
    Entity e = coordinator.CreateEntity();

    coordinator.AddComponent<Transform>(e, Transform{});
    coordinator.AddComponent<RenderComponent>(e, RenderComponent{});

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
    renderer.update(&coordinator);

    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderer.teardown();

    delete chunkManager;

    return 0;
}

int main() {
    initialize();


    //entity test

    entity1 = CreateStandardEntity();
    entity2 = CreateStandardEntity();

    coordinator.GetComponent<Transform>(entity1).Position = { 0, 0 };
    coordinator.GetComponent<RenderComponent>(entity1) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "turtles.png",
        0,
        0,
        1,
        1
    };

    coordinator.GetComponent<Transform>(entity2).Position = { 0, 0 };
    coordinator.GetComponent<RenderComponent>(entity2) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "wall.jpg",
        0,
        0,
        1,
        1
    };

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
