#include <iostream>
//#include "RenderTutorial.h"
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
Entity turtle;
Entity wall;
Entity dude;

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

    turtle = CreateStandardEntity();
    wall = CreateStandardEntity();
    dude = CreateStandardEntity();

    //Temporary until entityqueries are implemented
    coordinator.testEntity = CreateStandardEntity();
    // turtle
    coordinator.GetComponent<Transform>(turtle).setPosition(0.5, 3);
    coordinator.GetComponent<Transform>(turtle).setScale(0.4, 0.4);

    coordinator.GetComponent<RenderComponent>(turtle) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "turtles.png",
        0,
        0
    };
    coordinator.GetComponent<PhysicsComponent>(turtle) = {
        b2_dynamicBody,
        0.5f * coordinator.GetComponent<Transform>(turtle).getScale().y,
        0.5f * coordinator.GetComponent<Transform>(turtle).getScale().x,
        coordinator.GetComponent<Transform>(turtle).getPosition().x,
        coordinator.GetComponent<Transform>(turtle).getPosition().y,
        1.0f,
        0.0f
    };


    // ground
    coordinator.GetComponent<RenderComponent>(wall) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "wall.jpg",
        0,
        0
    };
    coordinator.GetComponent<Transform>(wall).translate(0, -1);
    coordinator.GetComponent<Transform>(wall).setScale(2, 1);
 
    coordinator.GetComponent<PhysicsComponent>(wall) = {
        b2_staticBody,
        0.5f * coordinator.GetComponent<Transform>(wall).getScale().y,
        0.5f * coordinator.GetComponent<Transform>(wall).getScale().x,
        coordinator.GetComponent<Transform>(wall).getPosition().x,
        coordinator.GetComponent<Transform>(wall).getPosition().y,
        1.0f,
        0.0f
    };

    coordinator.GetComponent<RenderComponent>(dude) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "game_sprites.png",
        2,
        0
    };
    coordinator.GetComponent<Transform>(dude).translate(-0.5, 0);
    coordinator.GetComponent<PhysicsComponent>(dude) = {
       b2_dynamicBody,
       0.5f * coordinator.GetComponent<Transform>(dude).getScale().y,
       0.5f * coordinator.GetComponent<Transform>(dude).getScale().x,
       coordinator.GetComponent<Transform>(dude).getPosition().x,
       coordinator.GetComponent<Transform>(dude).getPosition().y,
       1.0f,
       0.0f
    };
    physicsWorld->AddObjects(&coordinator);


    
    std::cout << "turtle x: " << coordinator.GetComponent<Transform>(turtle).getPosition().x << " y: " << coordinator.GetComponent<Transform>(turtle).getPosition().y << std::endl;
    std::cout << "wall x: " << coordinator.GetComponent<Transform>(wall).getPosition().x << " y: " << coordinator.GetComponent<Transform>(wall).getPosition().y << std::endl;
    std::cout << "Dude x: " << coordinator.GetComponent<Transform>(dude).getPosition().x << " y: " << coordinator.GetComponent<Transform>(dude).getPosition().y << std::endl;

    
    std::cout << "From Component array: x: " << coordinator.GetComponentArray<Transform>()[0].getPosition().x << std::endl;
    std::cout << "Number of Entities: " << coordinator.GetEntityCount() << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        // tell glfw to keep track of window resize 
        // and input events
        glfwPollEvents();
        runEngine();
        //System updates
        coordinator.runSystemUpdates();
    }    

    teardown();

    return 0;
}
