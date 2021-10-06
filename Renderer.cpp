#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera camera;

// shader code => tutorial provide
// inline code. In reality, we should parse them
// from a file
// 
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 viewMatrix;\n"
"void main()\n"
"{\n"
"   ourColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"// Set gl_Position with transformed vertex position\n"
"   gl_Position = viewMatrix * modelMatrix * vec4(aPos, 1);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, TexCoord);\n"
"}\n\0";

// Vertices data: coordinates, colors and texture coords
const GLfloat vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

// for the element buffer object (EBO)
GLuint indices[] = {
    0, 1, 3, //indices to create the first triangle
    1, 2, 3 //indices to create the second triangle
};

// the object's matrix
mat4 modelMatrix = mat4(1.0);

// the shaders uniforms we are using
// uniforms are extra data that we pass in
// manually to the shaders. They stay 
// the same in both vertex and fragment shaders.
enum UNIFORMS {
    MODEL_MATRIX_LOCATION,
    VIEW_MATRIX_LOCATION,
    NUM_OF_UNIFORMS
};

// store the locations of the shaders uniforms
GLuint uniformsLocation[NUM_OF_UNIFORMS];

// make the shader program
// see function for more details
GLuint shaderProgram;

// a pointer to the context
GLFWwindow* window;

/// <summary>
/// Initialize the Render tutorial.
/// </summary>
/// <returns></returns>
int Renderer::init() {
    window = Renderer::setupGLFW();
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    // glad handle the opengl code
    // init it
    gladLoadGL();

    Camera camera(0.0, 0.0, 0.0, 0.0);
    shaderProgram = createShaderProgram();

    // enable transparency for the images
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    return 0;
}

GLFWwindow* Renderer::setupGLFW() {
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

// Create the shader program by loading 
// the shaders
GLuint Renderer::createShaderProgram() {
	// shaders are OpenGL objects => we need to init them
	// and store a reference to them so we can use them later

	// init an empty shader and store the ref OpenGL returns
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// first param is the pointer/ID that we will use the as ref
	// to the shader (the one we create above), 1 is the number of strings
	// we are storing the shader in, &vertexShaderSource is a pointer
	// to the shader code string, and NULL is unimportant
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// do the same thing for the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// first param is the pointer/ID that we will use the as ref
	// to the shader, 1 is the number of strings
	// we are storing the shader in, &fragmentShaderSource is a pointer
	// to the shader code string, and NULL is unimportant
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// now that we have the shaders, we have to create and 
	// a "shader program" and attach them so it can work
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// link the program to the rendering pipeline
	glLinkProgram(shaderProgram);

    // clean up and delete the shader refs
	// we already compile and link them to the program
	// so we don't need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    // load the uniforms
    // see the uniforms defined in the vertex shader 
    // we get their locations here
    uniformsLocation[MODEL_MATRIX_LOCATION] = glGetUniformLocation(shaderProgram, "modelMatrix");
    uniformsLocation[VIEW_MATRIX_LOCATION] = glGetUniformLocation(shaderProgram, "viewMatrix");

	return shaderProgram;
}

// load the vertex data for the object
void loadVertexData() {
	// create a vertex buffer object
	// which is a buffer object for containing vertices
	GLuint VBO;

	// first param is how many 3D object we have (we have 1)
	// second param is a reference to the VBO id/ref
	// This is similar to GLuint VBO = glGenBuffers(1);
	// except we use refs
	glGenBuffers(1, &VBO); // gen == generate

	// now we need to bind the VBO so OpenGL knows to draw it
	// How "binding" works is it sets an object to the current object.
	// OpenGL can only draw one thing at a time (state machine), so
	// we have to tell it to draw the current object. Binding sets
	// an object (specified via the ref created by GLuint) to
	// be the current object
	// GL_ARRAY_BUFFER is the type of buffer that we want to bind
	// VBO is the ID/ref of the buffer we are binding (the actual
	// value is use here rather than a ref to it)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// now that we tell OpenGL what the current buffer is,
	// we have to put data into it. Think of it as giving
	// someone a bucket without anything inside it. They
	// know that they need to draw using that specific bucket
	// but they can't do it without anything inside the bucket (no data).

	// First is the type of buffer we are using
	// second is the size of the input
	// third is the vertices which is the actual input
	// finally is how we want the buffer to be used. This will help OpenGL
	// optimize the usage. STATIC belongs to three types: STREAM, STATIC, and
	// DYNAMIC. 
	// STREAM means the vertices will be modified once and will be used a few times
	// STATIC means the vertices will be modified once and will be used many many times
	// DYNAMIC means the vertices will be modified many times and will be used many many times
	// 
	// DRAW belongs to 3 types: READ, DRAW, and COPY
	// DRAW means the vertices will be modified and used to be draw an image on the screen
	// the other ones are not stated but can be inferred by the name.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void loadVertexIndices() {
    //create element buffer object
    //a buffer that stores indices that OpenGl uses to decide the vertices to draw
    GLuint EBO;
    glGenBuffers(1, &EBO);

    //binds the EBO similar to the VBO, EBO is of the type GL_ELEMENT_ARRAY_BUFFER
    //this tells OpenGL to make use of the EBO when making a draw call
    //if the EBO is missing and the drawelements is called nothing will be drawn
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //passes the indices to the EBO, with the size of the indices array, passes the indices, and GL_STATIC_DRAW
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void loadTexture() {
    // make the textures, set up is similar to buffers
    GLuint texture;
    glGenTextures(1, &texture);

    // bind the texture (tell OpenGL that it's the cur tex)
    //note* needs to be above any texture functions so that it is applied to the binded texture
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, amountOfColorChannels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load("resources/turtles.png", &width, &height, &amountOfColorChannels, STBI_rgb_alpha);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data); // delete the data

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadUniforms() {
    // pass in the uniforms value
    glUniformMatrix4fv(uniformsLocation[0], 1, 0, value_ptr(modelMatrix));
    glUniformMatrix4fv(uniformsLocation[1], 1, 0, value_ptr(camera.getViewMatrix()));
}

// called in main()
int Renderer::update() {
    // calculate the modelViewMatrix
    camera.moveCamera(0.01, 0.0);

	// create a vertex array
    // setting up 
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
    // tell OpenGL to use this VAO (set it as active)
    // need to do this before put data into the VAO
    glBindVertexArray(VAO);

    // load the data
    loadVertexData();
    loadVertexIndices();
    loadTexture();

    // create vertex attrib pointer
    // tell OpenGL how to intepret the vertex data that 
    // we pass in. In memory, it just know we give it a chunk
    // of bytes => tell it how many bytes make a vertex
    // it helps to see the image at "Applying Texture"
    // here: https://learnopengl.com/Getting-started/Textures
    // first param is the input index of the vertex shader (see the first few lines).
    // `aPos` is located at location 0 so we want to set the pointer for this
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // do the same thing for the color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // do the same thing for the texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

    glUseProgram(shaderProgram);
    loadUniforms();

    //we bind the ebo before the draw call to indicate to OpenGL that we want to use it
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // foreground is currently cleared (default to white)
    // we want to display the gray, which is the background color
    // => swap them
    glfwSwapBuffers(window);

    return 0;
}

int Renderer::teardown() {
    // cleanup
    glfwDestroyWindow(window);
    // call this to destroy glfw
    glfwTerminate();
    return 0;
};

