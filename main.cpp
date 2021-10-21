#include <iostream>
//#include "RenderTutorial.h"
#include <vector>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include "Renderer.h"
#include "PhysicsWorld.h"
//#include "protoChunkManager.h"
#include "EntityCoordinator.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "TimerComponent.h"
#include "AnimationComponent.h"
#include "Types.h"
#include "Animator.h"
#include "InputTracker.h"
#include "InputComponent.h"
#include "inputSystem.h";

//ChunkManager* chunkManager;
EntityCoordinator coordinator;

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
Animator animator;
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
    renderer->init();
    coordinator.Init();

    physicsWorld = new PhysicsWorld();

    prevTime = Clock::now();

    return 0;
}

int test(){

    coordinator.RegisterComponent<Transform>();
    coordinator.RegisterComponent<RenderComponent>();
    coordinator.RegisterComponent<PhysicsComponent>();
    coordinator.RegisterComponent<AnimationComponent>();
    coordinator.RegisterComponent<TimerComponent>();

    standardArch = coordinator.GetArchetype({
        coordinator.GetComponentType<Transform>(),
        coordinator.GetComponentType<RenderComponent>(),
        coordinator.GetComponentType<PhysicsComponent>(),
        coordinator.GetComponentType<AnimationComponent>()
        });

    coordinator.addSystem<InputSystem>(&coordinator);    

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
        InputTracker::getInstance().perFrameUpdate(window);

        // run physics
        physicsWorld->Update(&coordinator);
        // run ECS systems
        coordinator.runSystemUpdates();

        catchupTime -= MS_PER_FRAME;
    }

    //animation
    animator.updateAnim(&coordinator);

    // render
    renderer->update(&coordinator);

    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderer->teardown();

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
    coordinator.GetComponent<Transform>(turtle).setScale(0.4, 0.4);
    coordinator.GetComponent<Transform>(turtle).setPosition(0.5, 3);    

    coordinator.GetComponent<RenderComponent>(turtle) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "turtles.png",
        0,
        0,
        false
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
        0,
        false
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

    animator = Animator();

    coordinator.GetComponent<RenderComponent>(dude) = {
        "defaultVertShader.vs",
        "defaultFragShader.fs",
        "game_sprites.png",
        0,
        0,
        true
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
        
    Transform t = coordinator.GetComponent<Transform>(turtle);

    //this is where we create the animations for a given entity (dude)
    /*Animation anim1 = Animator::createAnimation("wLeft", 0,3,3,true);
    Animation anim2 = Animator::createAnimation("wRight", 0,3,2,true);

    Animation anims[] = {anim1, anim2};*/

    Animation anim = renderer->getAnimation("wLeft", coordinator.GetComponent<RenderComponent>(dude).spriteName);
    coordinator.GetComponent<AnimationComponent>(dude) = animator.createAnimationComponent(anim, true);

    std::cout << "turtle " << t << std::endl;
    std::cout << "wall " << coordinator.GetComponent<Transform>(wall) << std::endl;
    std::cout << "Dude " << coordinator.GetComponent<Transform>(dude) << std::endl;
        
    std::cout << "From Component array: x: " << coordinator.GetComponent<Transform>(turtle).getPosition().x << std::endl;
    std::cout << "Number of Entities: " << coordinator.GetEntityCount() << std::endl;    

    physicsWorld->AddObjects(&coordinator);


    std::cout << "turtle " << t << std::endl;

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
