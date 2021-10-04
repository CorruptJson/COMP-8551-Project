#include <iostream>
#include "./rendering/RenderTutorial.h"
#include "protoChunkManager.h"

ChunkManager* chunkManager;

// gets called once when engine starts
// put initilization code here
int initialize()
{
    // when the engine starts
    renderTutorialInit();

    chunkManager = new ProtoChunkManager();

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
    renderTutorialUpdate();

    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderTutorialTeardown();

    delete chunkManager;

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
