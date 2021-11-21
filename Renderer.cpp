#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

//Fragment Shader source code
// Vertices data: coordinates, colors and texture coords
GLfloat positionsData[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f, 
    -0.5f,  0.5f, 0.0f,
};
const int POSITION_ATTRIBUTE = 0;

GLfloat texCoordsData[] = {
     1.0f, 1.0f,   // top right
     1.0f, 0.0f,   // bottom right
     0.0f, 0.0f,   // bottom left
     0.0f, 1.0f    // top left 
};
const int TEX_COORD_ATTRIBUTE = 1;

// for the element buffer object (EBO)
GLuint indices[] = {
    0, 1, 3, //indices to create the first triangle
    1, 2, 3 //indices to create the second triangle
};


//text stuff
GLuint vao;
GLuint vbo;

// a pointer to the context
GLFWwindow* window;

/// <summary>
/// 
/// </summary>
/// <param name="viewWidth">The width of the camera view in OpenGL coordinates</param>
/// <param name="viewHeight">The height of the camera view in OpenGL coordinates</param>
/// <returns></returns>
int Renderer::init(int viewWidth, int viewHeight, glm::vec4 newBackgroundColor, WindowSize windowSize) {
    backgroundColor = newBackgroundColor;
    window = Renderer::setupGLFW(&windowWidth, &windowHeight, windowSize);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    // glad handle the opengl code
    // init it
    gladLoadGL();

    // tell opengl the size of the viewport (window)
    // we are drawing on
    // arguments are (bottom-left-x, bottom-left-y, top-right-x, top-right-y)
    glViewport(0, 0, windowWidth, windowHeight);

    // enable transparency for the images
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // init other OpenGL stuff
    loadImages();
    prepareGLBuffers();

    // init helper classes
    loadTextLibrary();
    camera.setViewSize(viewWidth, viewHeight);
    shaderFactory.createAllShaderPrograms();

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
            "platform.png",
            6,
            3,
            {}
        },
        {
            "bullet.png",
            1,
            1,
            {}
        },
        {
            "Edgar.png",
            1,
            11,
            {
                Animator::createAnimation("hurt",6,6,0,true,250.0f),
                Animator::createAnimation("idle",7,8,0,true,500.0f),
                Animator::createAnimation("falling",9,10,0,true,500.0f),
                Animator::createAnimation("running",0,5,0,true,150.0f)
            }
        },
        {
            "Giant_Roach.png",
            1,
            3,
            {
                Animator::createAnimation("hurt", 0, 0, 0, true, 250.0f),
                Animator::createAnimation("run",1,2, 0, true, 500.0f),
            }
        },
        {
            "star.png",
            1,
            13,
            {
                Animator::createAnimation("flicker", 0, 12, 0, true, 100.0f)
            }
        },
        {
            "fire.png",
            1,
            4,
            {
                Animator::createAnimation("burn", 0, 3, 0, true, 250.0f)
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

GLFWwindow* Renderer::setupGLFW(int *width, int *height, WindowSize windowSize) {
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
    GLFWmonitor* fullScreenMonitor;

    // use this to make the window match screen size but still in windowed mode
    if (windowSize == WindowSize::FULL_WINDOWED) {
        *width = monitorInfo->width;
        *height = monitorInfo->height;
        fullScreenMonitor = NULL;
    }
    else if (windowSize == WindowSize::FULLSCREEN) {
        *width = monitorInfo->width;
        *height = monitorInfo->height;
        fullScreenMonitor = monitor;
    }
    else {
        // also for WINDOWED mode
        *width = 1800;
        *height = 1200;
        fullScreenMonitor = NULL;
    }

    // Make a window with size 800x800 with name of "Edgar the Exterminator"
    // pass in monitor for the 4rd param if we want it to be full screen
    GLFWwindow* window = glfwCreateWindow(*width, *height, "Edgar the Exterminator", fullScreenMonitor, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return NULL;
    }

    if (windowSize == WindowSize::WINDOWED) {
        // set window in the center of the monitor
        // origin is top left corner
        glfwSetWindowPos(window, (monitorInfo->width - *width) / 2, (monitorInfo->height - *height) / 2);
    }

    // set the window's icon
    GLFWimage icon;
    int colChannel;
    icon.pixels = FileManager::readImageFile("logo.png", &icon.width, &icon.height, &colChannel);
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(icon.pixels); // free memory

    // tell glfw that the window we just create will
    // be used to draw on
    glfwMakeContextCurrent(window);

    delete monitor, monitorInfo;
    return window;
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
    if (FT_New_Face(ft, "Pixeboy.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, windowWidth / 20);

    // load the characters into the characters
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(width, height),
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

// prepare the VertexArrays, positions, tex coords,
// and indices data.
void Renderer::prepareGLBuffers() {
    // tell OpenGL to use this VAO (set it as active)
    // need to do this before put data into the VAO
	glGenVertexArrays(1, &vertexAttribs);
    glBindVertexArray(vertexAttribs);

    // create all the OpenGL buffers at the start so we can
    // reuse them
	// first param is how many 3D object we have (we have 1)
	// second param is a reference to the VBO id/ref
	// This is similar to GLuint VBO = glGenBuffers(1);
	// except we use refs
	glGenBuffers(1, &positionBuffer); // gen == generate
	// now we need to bind the VBO so OpenGL knows to draw it
	// How "binding" works is it sets an object to the current object.
	// OpenGL can only draw one thing at a time (state machine), so
	// we have to tell it to draw the current object. Binding sets
	// an object (specified via the ref created by GLuint) to
	// be the current object
	// GL_ARRAY_BUFFER is the type of buffer that we want to bind
	// VBO is the ID/ref of the buffer we are binding (the actual
	// value is use here rather than a ref to it)
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionsData), positionsData, GL_STATIC_DRAW);

    // create vertex attrib pointers
    // has to do this after loading data into buffers
    // tell OpenGL how to intepret the vertex data that 
    // we pass in. In memory, it just know we give it a chunk
    // of bytes => tell it how many bytes make a vertex
    // it helps to see the image at "Applying Texture"
    // here: https://learnopengl.com/Getting-started/Textures
    // first param is the input index of the vertex shader (see the first few lines).
    // `aPos` is located at location 0 so we want to set the pointer for this
    glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE);

    // do the same thing for the texture
	glGenBuffers(1, &texCoordBuffer); 
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    // use DYNAMIC DRAW cause we'll be changing it often
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordsData), texCoordsData, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(TEX_COORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(TEX_COORD_ATTRIBUTE);

    //binds the EBO similar to the VBO, EBO is of the type GL_ELEMENT_ARRAY_BUFFER
    //this tells OpenGL to make use of the EBO when making a draw call
    //if the EBO is missing and the drawelements is called nothing will be drawn
    glGenBuffers(1, &indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

    //passes the indices to the EBO, with the size of the indices array, passes the indices, and GL_STATIC_DRAW
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Reset VAO
    glBindVertexArray(0);
}

// update the tex coord vertex data so it draws 
// specific section of a spritesheet
void Renderer::updateTexCoord(RenderComponent comp, SpriteInfo& info) {
    // set the texcoords by specifying its x and y
    float leftX = info.cellWidth * comp.colIndex;
    float rightX = leftX + info.cellWidth; 
    float topY = 1 - info.cellHeight * comp.rowIndex;
    float bottomY = topY - info.cellHeight; 

    // coordinates of the texture coords in the vertices array
    if (comp.flipX) {
        texCoordsData[0] = leftX; // top right x
        texCoordsData[2] = leftX; // bottom right x
        texCoordsData[4] = rightX; // bottom left x
        texCoordsData[6] = rightX; // top left x
    }
    else {
        // normal coords
        texCoordsData[0] = rightX; // top right x
        texCoordsData[2] = rightX; // bottom right x
        texCoordsData[4] = leftX; // bottom left x
        texCoordsData[6] = leftX; // top left x
    }

    // y values of the tex coords
    texCoordsData[1] = topY; // top right y
    texCoordsData[3] = bottomY; // bottom right y
    texCoordsData[5] = bottomY; // bottom left y
    texCoordsData[7] = topY;  // top left y

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordsData), texCoordsData, GL_DYNAMIC_DRAW);
}

void Renderer::updateTexCoord(UIComponent comp, SpriteInfo& info) {
    // set the texcoords by specifying its x and y
    float leftX = info.cellWidth * comp.col;
    float rightX = leftX + info.cellWidth; 
    float topY = 1 - info.cellHeight * comp.row;
    float bottomY = topY - info.cellHeight; 

    bool flipX = false;

    // coordinates of the texture coords in the vertices array
    if (flipX) {
        texCoordsData[0] = leftX; // top right x
        texCoordsData[2] = leftX; // bottom right x
        texCoordsData[4] = rightX; // bottom left x
        texCoordsData[6] = rightX; // top left x
    }
    else {
        // normal coords
        texCoordsData[0] = rightX; // top right x
        texCoordsData[2] = rightX; // bottom right x
        texCoordsData[4] = leftX; // bottom left x
        texCoordsData[6] = leftX; // top left x
    }

    // y values of the tex coords
    texCoordsData[1] = topY; // top right y
    texCoordsData[3] = bottomY; // bottom right y
    texCoordsData[5] = bottomY; // bottom left y
    texCoordsData[7] = topY;  // top left y

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordsData), texCoordsData, GL_DYNAMIC_DRAW);
}

void Renderer::renderTextComponent(TextComponent* text)
{
    glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);

    //no need to disable depth test, already disabled

   //sets the current shader program to the text shader program
    glm::mat4 model(1);
    shaderFactory.useTextShader(model, projection, text->color);

    //sets the current shader program to use the projection matrix.
    //need to tell opengl which sampler2d to use
    glActiveTexture(GL_TEXTURE0);

    float x = text->x;
    float y = text->y;
    std::string textValue = text->getText();

    std::string::const_iterator c;
    for (c = textValue.begin(); c != textValue.end(); c++) {
        Character ch = characters[*c];

        float xpos = x + ch.bearing.x * text->size;
        float ypos = y - (ch.size.y - ch.bearing.y) * text->size;

        float w = ch.size.x * text->size;
        float h = ch.size.y * text->size;

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
        x += (ch.Advance >> 6) * text->size;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::renderUIComponent(UIComponent* ui, Transform* transform) {

    std::string spritesheet = ui->spriteName;

    if (spritesheet != "") {
        SpriteInfo& spriteInfo = sprites[spritesheet];
        glBindTexture(GL_TEXTURE_2D, spriteInfo.id);

        updateTexCoord(*ui, spriteInfo);
    }


    //glUniform4f(glGetUniformLocation(shaders[ShaderName::UI].Program, "panelColor"), ui->color.r, ui->color.g, ui->color.b, ui->color.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D,0);
}

Animation* Renderer::getAnimation(std::string animName, std::string spriteName)
{
    return &(sprites[spriteName].animations[animName]);
}

// called in main()
int Renderer::update(EntityCoordinator* coordinator) {
    ++counter;
    if (counter >= 60) {
        counter = 0;
        time++;
    }

    // reset the background
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

    // recall that OpenGL works using buffers
    // this is for the foreground color.
    // Recall MS Paint having a foreground and background color => same thing
    glClear(GL_COLOR_BUFFER_BIT);

    drawGameObjects(coordinator);

    drawUI(coordinator);

    // foreground is currently cleared (default to white)
    // we want to display the gray, which is the background color
    // => swap them
    glfwSwapBuffers(window);

    return 0;
}

void Renderer::drawGameObjects(EntityCoordinator* coordinator) {
    // draw entities by the spritesheet they use, so only need to load each sprite sheet once
    auto mapIterator = sprites.begin();
    
    // tell OpenGL to use this VAO (set it as active)
    // always need to do both of these together
    glBindVertexArray(vertexAttribs);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

    for (; mapIterator != sprites.end(); mapIterator++)
    {
        std::string spriteSheet = mapIterator->first;
        std::shared_ptr<EntityQuery> entsWithSprite = coordinator->entitiesWithSpriteSheet(spriteSheet);
        int entitiesFound = entsWithSprite->totalEntitiesFound();

        // no entities using this sprite sheet, go to next loop
        if (entitiesFound == 0) continue;

        // prepare the spritesheet
        SpriteInfo& spriteInfo = sprites[spriteSheet];
        glBindTexture(GL_TEXTURE_2D, spriteInfo.id);

        ComponentIterator<RenderComponent> renderComponents = ComponentIterator<RenderComponent>(entsWithSprite);
        ComponentIterator<Transform> transformComponents = ComponentIterator<Transform>(entsWithSprite);        

        RenderComponent* prevRenderComp = NULL;
        for (int i = 0; i < entitiesFound; i++)
        {
            RenderComponent* renderComp = renderComponents.nextComponent();
            Transform* t = transformComponents.nextComponent();

            //calculate the tex coord from the component.index
            if (i == 0 || (prevRenderComp->colIndex != renderComp->colIndex || prevRenderComp->rowIndex != renderComp->rowIndex)) {
                updateTexCoord(*renderComp, spriteInfo);
            }

            shaderFactory.useDefaultShader(t->getModelMatrix(), camera.getViewMatrix(), camera.getProjectionMatrix(), renderComp->color);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            prevRenderComp = renderComp;
        }
    }
}

void Renderer::drawUI(EntityCoordinator* coordinator) {
    //std::shared_ptr<EntityQuery> UIQuery = coordinator->GetEntityQuery({
    //    coordinator->GetComponentType<UIComponent>(),
    //    coordinator->GetComponentType<Transform>()
    //    }, {});

    //int uiFound = UIQuery->totalEntitiesFound();
    //ComponentIterator<UIComponent> uiComps = ComponentIterator<UIComponent>(UIQuery);
    //ComponentIterator<Transform> transforms = ComponentIterator<Transform>(UIQuery);

    ////load the UI data here
    ////glUseProgram(shaders[ShaderName::UI].Program);

    //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    ////sets the current matrix to a projection matrix which would overlay over the screen
    ////glUniformMatrix4fv(glGetUniformLocation(shaders[ShaderName::UI].Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

    //glBindTexture(GL_TEXTURE_2D, 0);

    //for (int i = 0; i < uiFound; i++) {
    //    renderUIComponent(uiComps.nextComponent(),transforms.nextComponent());
    //}

    ////unbinds the current vao and vbo
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //for (int i = 0; i < uiFound; i++) {
    //}
    //text rendering begins here
    std::shared_ptr<EntityQuery> TextQuery = coordinator->GetEntityQuery({
        coordinator->GetComponentType<TextComponent>(),
        }, {});

    int textFound = TextQuery->totalEntitiesFound();
    ComponentIterator<TextComponent> textComps = ComponentIterator<TextComponent>(TextQuery);

    for (int i = 0; i < textFound; i++) {
        renderTextComponent(textComps.nextComponent());
    }
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
