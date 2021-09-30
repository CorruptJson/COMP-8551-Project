#include <iostream>
#include "RenderTutorial.h"

int initialize()
{
    // when the engine starts

    return 0;
}

int runEngine()
{
    // check input
    // run physics
    // run ECS
    // render

    return 0;
}

int teardown()
{
    // when the engine closes

    return 0;
}

int main() {

    initialize();

    RenderTutorial();

    // should try to get the code below working to make a more general game loop?

    //// keep the window open if it's not supposed to close
    //while (!glfwWindowShouldClose(window))
    //{
    //    // tell glfw to keep track of window resize 
    //    // and input events
    //    glfwPollEvents();
    //    runEngine();
    //}    

    teardown();

    return 0;
}
