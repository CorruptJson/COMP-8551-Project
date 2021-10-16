#include <iostream>
//#include "RenderTutorial.h"
#include <vector>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include "Renderer.h"
#include "PhysicsWorld.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "TimerComponent.h"
#include "Types.h"

//ChunkManager* chunkManager;
EntityCoordinator coordinator;

Renderer renderer;
PhysicsWorld* physicsWorld;

Archetype standardArch;

// test entities
EntityID turtle;
EntityID wall;
EntityID dude;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

// gets called once when engine starts
// put initilization code here
int initialize()
{  
    // when the engine starts
    renderer.init();
    coordinator.Init();

    physicsWorld = new PhysicsWorld();

    prevTime = Clock::now();

    return 0;
}

int test(){

    coordinator.RegisterComponent<Transform>();
    coordinator.RegisterComponent<RenderComponent>();
    coordinator.RegisterComponent<PhysicsComponent>();
    coordinator.RegisterComponent<TimerComponent>();

    standardArch = coordinator.GetArchetype({
        coordinator.GetComponentType<Transform>(),
        coordinator.GetComponentType<RenderComponent>(),
        coordinator.GetComponentType<PhysicsComponent>()
        });

    return 0;
}

// Use for now to make entities with components
EntityID CreateStandardEntity(const char* spriteName) {
    EntityID e = coordinator.CreateEntity(standardArch, spriteName);

    // data must be initialized
    // if you know the data is going to be initialized later, you don't need to initialize it here
    Transform t = Transform();
    coordinator.GetComponent<Transform>(e) = t;

    return e;
}

// the main update function
// game loop will be put here eventually
int runEngine()

{
    Clock::time_point currTime = Clock::now();
    Duration delta = currTime - prevTime;
    prevTime = currTime;
    catchupTime += delta.count();

    // check input

    while (catchupTime >= MS_PER_FRAME)
    {
        // run physics
        physicsWorld->Update(&coordinator);
        // run ECS

        catchupTime -= MS_PER_FRAME;
    }

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

    delete physicsWorld;

    return 0;
}

int main() {
    initialize();
    test();

    //entity test

    turtle = CreateStandardEntity("turtles.png");
    wall = CreateStandardEntity("wall.jpg");
    dude = CreateStandardEntity("game_sprites.png");

    //Temporary until entityqueries are implemented
    //coordinator.AddComponent<TimerComponent>(turtle, TimerComponent());
    coordinator.testEntity = &turtle;

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
    //coordinator.GetComponent<Transform>(wall).translate(0, -1);
    coordinator.GetComponent<Transform>(wall).setPosition(0, -1);
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
        
    std::cout << "turtle x: " << coordinator.GetComponent<Transform>(turtle).getPosition().x << " y: " << coordinator.GetComponent<Transform>(turtle).getPosition().y << std::endl;
    std::cout << "wall x: " << coordinator.GetComponent<Transform>(wall).getPosition().x << " y: " << coordinator.GetComponent<Transform>(wall).getPosition().y << std::endl;
    std::cout << "Dude x: " << coordinator.GetComponent<Transform>(dude).getPosition().x << " y: " << coordinator.GetComponent<Transform>(dude).getPosition().y << std::endl;
        
    std::cout << "From Component array: x: " << coordinator.GetComponent<Transform>(turtle).getPosition().x << std::endl;
    std::cout << "Number of Entities: " << coordinator.GetEntityCount() << std::endl;

    physicsWorld->AddObjects(&coordinator);

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
