#include <iostream>
#include "RenderTutorial.h"
#include "Box2DTutorial.h"

Box2DTutorial* box2DTutorial;

// gets called once when engine starts
// put initilization code here
int initialize()
{
    // when the engine starts
    renderTutorialInit();
    box2DTutorial = new Box2DTutorial();
    box2DTutorial->GravityExample();

    return 0;
}

// the main update function
// game loop will be put here eventually
int runEngine()
{
    // check input
    // run physics
    box2DTutorial->Update();
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

    delete box2DTutorial;

    return 0;
}

int main() {

    initialize();

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
