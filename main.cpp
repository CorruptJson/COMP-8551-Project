#include <iostream>
#include <vector>
#include "Renderer.h"
#include "PhysicsWorld.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "Types.h"

//ChunkManager* chunkManager;
EntityCoordinator coordinator;
ChunkManager* chunkManager;

Renderer renderer;
PhysicsWorld* physicsWorld;

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
    physicsWorld = new PhysicsWorld();

    chunkManager = new ChunkManager();

    Signature signature;

    coordinator.RegisterComponent<Transform>();
    signature.set(coordinator.GetComponentType<Transform>());

    coordinator.RegisterComponent<RenderComponent>();
    signature.set(coordinator.GetComponentType<RenderComponent>());

    coordinator.RegisterComponent<PhysicsComponent>();
    signature.set(coordinator.GetComponentType<PhysicsComponent>());

    return 0;
}

// Use for now to make entities with components
Entity CreateStandardEntity() {
    Entity e = coordinator.CreateEntity();

    coordinator.AddComponent<Transform>(e, Transform());
    coordinator.AddComponent<RenderComponent>(e, RenderComponent{});
    coordinator.AddComponent<PhysicsComponent>(e, PhysicsComponent{});

    return e;
}


// the main update function
// game loop will be put here eventually
int runEngine()

{
    // check input
    // run physics
    physicsWorld->Update(&coordinator);
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

    delete physicsWorld;

    return 0;
}

int main() {
    initialize();


    //entity test

    entity1 = CreateStandardEntity();
    entity2 = CreateStandardEntity();

    // turtle
    coordinator.GetComponent<Transform>(entity1).setPosition(0, 1.2);
    coordinator.GetComponent<RenderComponent>(entity1) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "turtles.png",
        0,
        0,
        1,
        1
    };
    coordinator.GetComponent<PhysicsComponent>(entity1) = {
        b2_dynamicBody,
        coordinator.GetComponent<RenderComponent>(entity1).spriteHeight / 2,
        coordinator.GetComponent<RenderComponent>(entity1).spriteWidth / 2,
        coordinator.GetComponent<Transform>(entity1).getPosition().x,
        coordinator.GetComponent<Transform>(entity1).getPosition().y,
        1.0f,
        0.0f
    };


    // ground
    coordinator.GetComponent<Transform>(entity2).setPosition(0, -1);
    coordinator.GetComponent<RenderComponent>(entity2) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "wall.jpg",
        0,
        0,
        1,
        1
    };
    coordinator.GetComponent<PhysicsComponent>(entity2) = {
        b2_staticBody,
        coordinator.GetComponent<RenderComponent>(entity2).spriteHeight / 2,
        coordinator.GetComponent<RenderComponent>(entity2).spriteWidth / 2,
        coordinator.GetComponent<Transform>(entity2).getPosition().x,
        coordinator.GetComponent<Transform>(entity2).getPosition().y,
        1.0f,
        0.0f
    };
    physicsWorld->AddObjects(&coordinator);

    //coordinator.GetComponent<Transform>(entity2).translate(0, -1);

    std::cout << "entity1 x: " << coordinator.GetComponent<Transform>(entity1).getPosition().x << " y: " << coordinator.GetComponent<Transform>(entity1).getPosition().y << std::endl;
    std::cout << "entity2 x: " << coordinator.GetComponent<Transform>(entity2).getPosition().x << " y: " << coordinator.GetComponent<Transform>(entity2).getPosition().y << std::endl;

    
    std::cout << "From Component array: x: " << coordinator.GetComponentArray<Transform>()[0].getPosition().x << std::endl;
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
