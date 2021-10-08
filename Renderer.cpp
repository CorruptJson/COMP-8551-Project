#include "Renderer.h"
#include <string>

const char *Renderer::DEFAULT_VERT_SHADER_NAME = "DefaultVertShader.vs";
const char *Renderer::DEFAULT_FRAG_SHADER_NAME = "DefaultFragShader.fs";

//Fragment Shader source code
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


// a pointer to the context
GLFWwindow* window;

/// <summary>
/// Initialize the Render tutorial.
/// </summary>
/// <returns></returns>
int Renderer::init() {
    int width, height;
    window = Renderer::setupGLFW(&width, &height);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    // glad handle the opengl code
    // init it
    gladLoadGL();

    // init other OpenGL stuff
    Camera camera(0.0, 0.0, 0.0, 0.0);
    defaultShaderProgram = createDefaultShaderProgram();

    // create all the OpenGL buffers at the start so we can
    // reuse them
	// first param is how many 3D object we have (we have 1)
	// second param is a reference to the VBO id/ref
	// This is similar to GLuint VBO = glGenBuffers(1);
	// except we use refs
	glGenBuffers(1, &vertexBuffer); // gen == generate
    glGenBuffers(1, &indicesBuffer);
    glGenTextures(1, &textureBuffer);
	glGenVertexArrays(1, &vertexAttribs);

    // tell opengl the size of the viewport (window)
    // we are drawing on
    // arguments are (bottom-left-x, bottom-left-y, top-right-x, top-right-y)
    glViewport(0, 0, width, height);

    // enable transparency for the images
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // function for using stbi
    // this is because OpenGL's y-axis starts from bottoms up
    // however, images have y-axis starts top down
    stbi_set_flip_vertically_on_load(true);

    return 0;
}

GLFWwindow* Renderer::setupGLFW(int *width, int *height) {
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

    // Find the main monitor and its screen size
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* monitorInfo = glfwGetVideoMode(monitor);

    // use this to make the window match screen size but still in windowed mode
    //*width = monitorInfo->width;
    //*height = monitorInfo->height;

    // make a small width and a height for ease of testing
    *width = 1000;
    *height = 1000;

    // Make a window with size 800x800 with name of "Chunky Soup"
    // pass in monitor for the 3rd param if we want it to be full screen
    GLFWwindow* window = glfwCreateWindow(*width, *height, "Chunky Soup", NULL, NULL);
    //glfwSetWindowPos(window, 0, 0);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return NULL;
    }

    // tell glfw that the window we just create will
    // be used to draw on
    glfwMakeContextCurrent(window);

    delete monitor, monitorInfo;
    return window;
}

// Create the shader program by loading 
// the shaders
GLuint Renderer::createDefaultShaderProgram() {
	// shaders are OpenGL objects => we need to init them
	// and store a reference to them so we can use them later

	// init an empty shader and store the ref OpenGL returns
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderSource = FileManager::readShaderFile(Renderer::DEFAULT_VERT_SHADER_NAME);
    const char* vertCStr = vertShaderSource.c_str();

	// first param is the pointer/ID that we will use the as ref
	// to the shader (the one we create above), 1 is the number of strings
	// we are storing the shader in, &vertexShaderSource is a pointer
	// to the shader code string, and NULL is the length that we will
    // read from the vertCStr => NULL means we keep reading until we see
    // a NUL EOF char.
	glShaderSource(vertexShader, 1, &vertCStr, NULL);
	glCompileShader(vertexShader);

    // check for success
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Shader Compilation Error: " << infoLog << std::endl;
    }

	// do the same thing for the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragShaderSource = FileManager::readShaderFile(Renderer::DEFAULT_FRAG_SHADER_NAME);
    const char* fragCStr = fragShaderSource.c_str();

	// first param is the pointer/ID that we will use the as ref
	// to the shader, 1 is the number of strings
	// we are storing the shader in, &fragmentShaderSource is a pointer
	// to the shader code string, and NULL is unimportant
	glShaderSource(fragmentShader, 1, &fragCStr, NULL);
	glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Shader Compilation Error: " << infoLog << std::endl;
    }

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
void Renderer::loadVertexData() {
	// now we need to bind the VBO so OpenGL knows to draw it
	// How "binding" works is it sets an object to the current object.
	// OpenGL can only draw one thing at a time (state machine), so
	// we have to tell it to draw the current object. Binding sets
	// an object (specified via the ref created by GLuint) to
	// be the current object
	// GL_ARRAY_BUFFER is the type of buffer that we want to bind
	// VBO is the ID/ref of the buffer we are binding (the actual
	// value is use here rather than a ref to it)
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

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

void Renderer::loadIndicesData() {
    //binds the EBO similar to the VBO, EBO is of the type GL_ELEMENT_ARRAY_BUFFER
    //this tells OpenGL to make use of the EBO when making a draw call
    //if the EBO is missing and the drawelements is called nothing will be drawn
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

    //passes the indices to the EBO, with the size of the indices array, passes the indices, and GL_STATIC_DRAW
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void Renderer::loadTexture(const char *spriteName) {
    // bind the texture (tell OpenGL that it's the cur tex)
    //note* needs to be above any texture functions so that it is applied to the binded texture
    glBindTexture(GL_TEXTURE_2D, textureBuffer);

    const auto& result = sprites.find(spriteName);

    SpriteInfo *info;
    // if not found
    if (result == sprites.end()) {
        // read the image from the file and store it
        info = FileManager::readImageFile(spriteName);
        if (info == NULL) {
            std::cout << "Failed to load texture" << std::endl;
            return;
        }

        sprites[spriteName] = *info;

        // delete the data if needed
        //stbi_image_free(data); 
    }
    else {
        // take the sprite info from the sprites map
        info = &(sprites[spriteName]);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info->width, info->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::loadUniforms(mat4 modelMatrix) {
    // pass in the uniforms value
    glUniformMatrix4fv(uniformsLocation[0], 1, 0, value_ptr(modelMatrix));
    glUniformMatrix4fv(uniformsLocation[1], 1, 0, value_ptr(camera.getViewMatrix()));
}

// called in main()
int Renderer::update(EntityCoordinator* coordinator) {
    // calculate the modelViewMatrix
    //camera.moveCamera(0.01, 0.0);

    // set background color (gray)
    glClearColor(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, 1.0f);

    // recall that OpenGL works using buffers
    // this is for the foreground color.
    // Recall MS Paint having a foreground and background color => same thing
    glClear(GL_COLOR_BUFFER_BIT);

    std::array<RenderComponent, MAX_ENTITIES> renderComps = coordinator->GetComponentArray<RenderComponent>();
    std::array<Transform, MAX_ENTITIES> transforms = coordinator->GetComponentArray<Transform>();
    for (int i = 0; i < coordinator->GetEntityCount(); i++) {
        RenderComponent component = renderComps[i];
        mat4 modelMatrix = transforms[i].getModelMatrix();

        transforms[i].update();

        // tell OpenGL to use this VAO (set it as active)
        // need to do this before put data into the VAO
        glBindVertexArray(vertexAttribs);

        // load the data
        loadVertexData();
        loadIndicesData();
        loadTexture(component.spriteName);

        // create vertex attrib pointers
        // has to do this after loading data into buffers
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

        glUseProgram(defaultShaderProgram);
        loadUniforms(modelMatrix);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

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

