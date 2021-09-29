
#include "RenderTutorial.h"

// shader code => tutorial provide
// inline code. In reality, we should parse them
// from a file
// 
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

GLFWwindow* setupGLFW() {
    // glfw helps with creating windows, contexts
    // and receiving inputs and events
    // init this to call its function
    // see https://www.khronos.org/opengl/wiki/Getting_Started#Initialization
    // for more info
    glfwInit();

    // tell glfw we are using version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell glfw the opengl functions that we want to use 
    // core profile is a preset list of functions that opengl can init
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make a window with size 800x800 with name of "OpenGLTutorial"
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGLTutorial", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return NULL;
    }

    // tell glfw that the window we just create will
    // be used to draw on
    glfwMakeContextCurrent(window);
    return window;
}

// called in main()
int RenderTutorial() {
    GLFWwindow* window = setupGLFW();
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    // glad handle the opengl code
    // init it
    gladLoadGL();

    // tell opengl the size of the viewport (window)
    // we are drawing on
    // arguments are (bottom-left-x, bottom-left-y, top-right-x, top-right-y)
    glViewport(0, 0, 800, 800);

    // set background color (gray)
    glClearColor(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, 1.0f);
    // recall that OpenGL works using buffers
    // so first, set up the buffers by cleaning them out
    // this is for the foreground color.
    // Recall MS Paint having a foreground and background color => same thing
    glClear(GL_COLOR_BUFFER_BIT);

    // foreground is currently cleared (default to white)
    // we want to display the gray, which is the background color
    // => swap them
    glfwSwapBuffers(window);


    // keep the window open if it's not supposed to close
    while (!glfwWindowShouldClose(window)) {
        // tell glfw to keep track of window resize 
        // and input events
        glfwPollEvents();
    }

    // cleanup
    glfwDestroyWindow(window);
    // call this to destroy glfw
    glfwTerminate();
    return 0;
}
