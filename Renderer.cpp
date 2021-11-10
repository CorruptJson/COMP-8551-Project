#include "Renderer.h"

std::string Renderer::DEFAULT_VERT_SHADER_NAME = "DefaultVertShader.vs";
std::string Renderer::DEFAULT_FRAG_SHADER_NAME = "DefaultFragShader.fs";
std::string Renderer::TEXT_VERT_SHADER_NAME = "TextVertShader.vs";
std::string Renderer::TEXT_FRAG_SHADER_NAME = "TextFragShader.fs";

//Fragment Shader source code
// Vertices data: coordinates, colors and texture coords
GLfloat vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
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
    PROJECTION_MATRIX_LOCATION,
    NUM_OF_UNIFORMS
};

// store the locations of the shaders uniforms
GLuint uniformsLocation[NUM_OF_UNIFORMS];

//text stuff
GLuint textUniformsLocation[NUM_OF_UNIFORMS];
GLuint vao;
GLuint vbo;

// a pointer to the context
GLFWwindow* window;

/// <summary>
/// Initialize the Render tutorial.
/// </summary>
/// <returns></returns>
int Renderer::init(int viewWidth, int viewHeight) {
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
    float CENTER_X_COORD = 0;
    float CENTER_Y_COORD = 0;
    float LEFT_X_COORD = CENTER_X_COORD - viewWidth / 2;
    float RIGHT_X_COORD = CENTER_X_COORD + viewWidth / 2;
    float BOTTOM_Y_COORD = CENTER_Y_COORD - viewHeight / 2;
    float TOP_Y_COORD = CENTER_Y_COORD + viewHeight / 2;
    float EYE_NEAR = 0.f;
    float EYE_FAR = -1.f;
    projectionMatrix = glm::ortho(LEFT_X_COORD, RIGHT_X_COORD, BOTTOM_Y_COORD, TOP_Y_COORD, EYE_NEAR, EYE_FAR);
    Camera camera(0.0, 0.0, 0.0, 0.0);

    defaultShaderProgram = createDefaultShaderProgram();
    textShaderProgram = createTextShaderProgram();

    //sets the text shader to be used currently
    glUseProgram(textShaderProgram);

    loadImages();

    // create all the OpenGL buffers at the start so we can
    // reuse them
	// first param is how many 3D object we have (we have 1)
	// second param is a reference to the VBO id/ref
	// This is similar to GLuint VBO = glGenBuffers(1);
	// except we use refs
	glGenBuffers(1, &vertexBuffer); // gen == generate
    glGenBuffers(1, &indicesBuffer);
	glGenVertexArrays(1, &vertexAttribs);

    // tell opengl the size of the viewport (window)
    // we are drawing on
    // arguments are (bottom-left-x, bottom-left-y, top-right-x, top-right-y)
    glViewport(0, 0, width, height);

    // enable transparency for the images
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //loads the text library for printing characters
    loadTextLibrary();

    return 0;
}

// Load all images that we need into the sprites map
// This allows us to specify their row and column number
// Also accomodate future sprite loading.
void Renderer::loadImages() {
    // read config data
    struct ImgConfig {
        string name;
        int rows;
        int columns;
        Animation anims[5];
    };

    ImgConfig configs[]{
        {
            "background.png",
            1,
            1,
            {}
        },
        {
            "wall.jpg",
            1,
            1,
            {}
        },
        {
            "Edgar.png",
            1,
            11,
            {Animator::createAnimation("hurt",6,6,0,true,250.0f),
            Animator::createAnimation("idle",7,8,0,true,500.0f),
            Animator::createAnimation("falling",9,10,0,true,500.0f),
            Animator::createAnimation("running",0,5,0,true,150.0f)
            }
        },
        {
            "Giant_Roach.png",
            1,
            3,
            {Animator::createAnimation("hurt",0,0,0,true,250.0f),
            Animator::createAnimation("run",1,2,0,true,500.0f),
            }
        }
    };


    // function for using stbi
    // this is because OpenGL's y-axis starts from bottoms up
    // however, images have y-axis starts top down
    stbi_set_flip_vertically_on_load(true);
    // opengl clamp tex coord in range [0. 1];
    const float SPRITESHEET_HEIGHT = 1.f;
    const float SPRITESHEET_WIDTH = 1.f;

    for (ImgConfig config : configs) {
        // read the image from the file and store it
        SpriteInfo info;
        for (Animation var : config.anims)
        {
            info.animations.insert(std::pair<std::string, Animation>(var.animationName, var));
        }
        

        int colChannels;
        stbi_uc* imgData = FileManager::readImageFile(config.name, &info.width, &info.height, &colChannels);
        if (!imgData) {
            std::cout << "Failed to load texture: " << config.name << std::endl;
            return;
        }

        // create the texture buffer and store its id in info
        info.id = createTexBuffer(info.height, info.width, imgData);
        info.rows = config.rows;
        info.columns = config.columns;

        // calculate the cell size
        info.cellHeight = SPRITESHEET_HEIGHT / info.rows;
        info.cellWidth = SPRITESHEET_WIDTH / info.columns;

        // store the new sprite
        sprites[config.name] = info;

        // delete the data
        stbi_image_free(imgData); 
    }
}

GLuint Renderer::createTexBuffer(int height, int width, unsigned char* imgData) {
    // create a texture buffer
    GLuint id;
    glGenTextures(1, &id);

    // bind the texture (tell OpenGL that it's the cur tex)
    //note* needs to be above any texture functions so that it is applied to the binded texture
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return id;
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
    uniformsLocation[PROJECTION_MATRIX_LOCATION] = glGetUniformLocation(shaderProgram, "projectionMatrix");

	return shaderProgram;
}

//text shader setup, might need to create a general shader program creator
GLuint Renderer::createTextShaderProgram() {
	// shaders are OpenGL objects => we need to init them
	// and store a reference to them so we can use them later

	// init an empty shader and store the ref OpenGL returns
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderSource = FileManager::readShaderFile(Renderer::TEXT_VERT_SHADER_NAME);
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
    std::string fragShaderSource = FileManager::readShaderFile(Renderer::TEXT_FRAG_SHADER_NAME);
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
    textUniformsLocation[MODEL_MATRIX_LOCATION] = glGetUniformLocation(shaderProgram, "modelMatrix");
    textUniformsLocation[VIEW_MATRIX_LOCATION] = glGetUniformLocation(shaderProgram, "viewMatrix");
    textUniformsLocation[PROJECTION_MATRIX_LOCATION] = glGetUniformLocation(shaderProgram, "projectionMatrix");

	return shaderProgram;
}

/// <summary>
/// Load the FreeType text library and the fonts we'll use for the game.
/// </summary>
void Renderer::loadTextLibrary() {
    // init the library
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    // load the font file so we get a texture
    FT_Face face;
    if (FT_New_Face(ft, "arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    // load the characters into the characters
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    // load the characters from the font file
    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // generate texture
        //do it without create texbuffer for testing purposes
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        int height = face->glyph->bitmap.rows;
        int width = face->glyph->bitmap.width;
        unsigned char* &data = face->glyph->bitmap.buffer;
        
        //GLuint textureId = createTexBuffer(height, width, data);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            width,
            height,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            data
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            //textureId,
            texture,
            glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x,
        };
        characters.insert(std::pair<unsigned char, Character>(c, character));
    }

    // we are done with the free type
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //sets up the vao and vbo for the text
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // vertex location = inPos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //vertex location = inTexCoord
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

void Renderer::loadTexture(std::string spriteName) {
    // if not found
    try {
        SpriteInfo info = sprites[spriteName];
        glBindTexture(GL_TEXTURE_2D, info.id);
    }
    catch (int e) {
        std::cout << "Couldn't find image matching name: " << spriteName << std::endl;
        return;
    }
}

void Renderer::loadUniforms(mat4 modelMatrix) {
    // pass in the uniforms value
    glUniformMatrix4fv(uniformsLocation[MODEL_MATRIX_LOCATION], 1, 0, value_ptr(modelMatrix));
    glUniformMatrix4fv(uniformsLocation[VIEW_MATRIX_LOCATION], 1, 0, value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(uniformsLocation[PROJECTION_MATRIX_LOCATION], 1, 0, value_ptr(projectionMatrix));
}

void Renderer::loadTextUniforms(mat4 modelMatrix) {
    // pass in the uniforms value
    glUniformMatrix4fv(textUniformsLocation[MODEL_MATRIX_LOCATION], 1, 0, value_ptr(modelMatrix));
    glUniformMatrix4fv(textUniformsLocation[VIEW_MATRIX_LOCATION], 1, 0, value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(textUniformsLocation[PROJECTION_MATRIX_LOCATION], 1, 0, value_ptr(projectionMatrix));
}

// update the tex coord vertex data so it draws 
// specific section of a spritesheet
void Renderer::updateTexCoord(RenderComponent comp, std::string spriteName) {
    SpriteInfo info = sprites[spriteName];

    // set the texcoords by specifying its x and y
    float leftX = info.cellWidth * comp.colIndex;
    float rightX = leftX + info.cellWidth; 
    float topY = 1 - info.cellHeight * comp.rowIndex;
    float bottomY = topY - info.cellHeight; 

    if (comp.flipX) {
        // coordinates of the texture coords in the vertices array
        vertices[3] = rightX; // top right x
        vertices[8] = rightX; // bottom right x
        vertices[13] = leftX; // bottom left x
        vertices[18] = leftX; // top left x
    }
    else {
        vertices[3] = leftX; // top right x
        vertices[8] = leftX; // bottom right x
        vertices[13] = rightX; // bottom left x
        vertices[18] = rightX; // top left x
    }

    // y values of the tex coords
    vertices[4] = topY; // top right y
    vertices[19] = topY; // top left y
    vertices[9] = bottomY; // bottom right y
    vertices[14] = bottomY; // bottom left y
}

void Renderer::setTexCoordToDefault() {
    // coordinates of the texture coords in the vertices array
    float rightX = 1.0f;
    float leftX = 0.0f;
    float topY = 1.0f;
    float bottomY = 0.0f;

    // x values
    vertices[3] = rightX; // top right x
    vertices[8] = rightX; // bottom right x
    vertices[13] = leftX; // bottom left x
    vertices[18] = leftX; // top left x

    // y values of the tex coords
    vertices[4] = topY; // top right y
    vertices[19] = topY; // top left y
    vertices[9] = bottomY; // bottom right y
    vertices[14] = bottomY; // bottom left y
}

//function for rendering the text, later will be changed to render text components instead
void Renderer::renderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    //no need to disable depth test, already disabled

   ////sets the current shader program to the text shader program
    glUseProgram(textShaderProgram);
    //sets the current shader program to use the projection matrix.
    glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "projectionMatrix"),1, GL_FALSE, glm::value_ptr(projection));

    //
    glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), color.x, color.y, color.z);
    //uniform location for the shader text
    glUniform1i(glGetUniformLocation(textShaderProgram, "text"), 0);
    //need to tell opengl which sampler2d to use
    glActiveTexture(GL_TEXTURE0);


    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        GLfloat verts[6][4] =
        {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        
        glBindVertexArray(vao);

        //loads the characters texture
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        //sets the vbo and vao before drawing
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //bitshift by 6 to get value in pixels (2^6 = 64)
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Animation* Renderer::getAnimation(std::string animName, std::string spriteName)
{
    return &(sprites[spriteName].animations[animName]);
}

// called in main()
int Renderer::update(EntityCoordinator* coordinator) {
    // calculate the modelViewMatrix
    //camera.moveCamera(0.01, 0.0);

    // set background color (gray)
    glClearColor(125 / 255.f, 125 / 255.f, 125 / 255.f, 0);

    // recall that OpenGL works using buffers
    // this is for the foreground color.
    // Recall MS Paint having a foreground and background color => same thing
    glClear(GL_COLOR_BUFFER_BIT);

    std::unique_ptr<EntityQuery> entityQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<RenderComponent>(),
        coordinator->GetComponentType<Transform>()
        });

    int entitiesFound = entityQuery->totalEntitiesFound();
    std::vector<RenderComponent*> renderComps = entityQuery->getComponentArray<RenderComponent>();
    std::vector<Transform*> transformComps = entityQuery->getComponentArray<Transform>();

    for (int i = 0; i < entitiesFound; i++) {
        RenderComponent component = *(renderComps[i]);
        Transform t = *(transformComps[i]);
        mat4 modelMatrix = t.getModelMatrix();

        transformComps[i]->update();

        // tell OpenGL to use this VAO (set it as active)
        // need to do this before put data into the VAO
        glBindVertexArray(vertexAttribs);

        //calculate the tex coord from the component.index
        updateTexCoord(component, component.spriteName);

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // do the same thing for the texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glUseProgram(defaultShaderProgram);
        loadUniforms(modelMatrix);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    //unbinds the current vao and vbo

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //setTexCoordToDefault();

    renderText("hello", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f,0.8f,0.2f));


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
}
Renderer* Renderer::getInstance()
{
    if (renderer == nullptr)
        renderer = new Renderer();
    return renderer;
}
;

Renderer* Renderer::renderer = nullptr;
