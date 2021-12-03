#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <map>
#include "Camera.h"
#include "SpriteInfo.h"
#include "EntityCoordinator.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Transform.h"
#include "file_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "Animator.h"
#include "Character.h"
#include "ShaderFactory.h"
#include "NBSemaphore.h"

extern GLFWwindow* window;

enum class WindowSize {
    WINDOWED,
    MAXIMIZED_WINDOWED,
    FULLSCREEN
};

struct ImgConfig {
    string name;
    int rows;
    int columns;
    Animation anims[5];
    int width;
    int height;
    stbi_uc* imgData;
};

class Renderer : public IObserver
{
public:

    int init(int viewWidth, int viewHeight, glm::vec4 newBackgroundColor, WindowSize windowSize);
    void setCamViewSize(int viewWidth, int viewHeight);
    int update(EntityCoordinator* coordinator);
    int teardown(bool closeWindow);
    static Renderer* getInstance();
    ImgConfig* configs[6];
    Animation* getAnimation(std::string animName, std::string spriteName);
    int getWindowWidth();
    int getWindowHeight();
    Interpolator* getTextXInterpolator();
    Interpolator* getTextYInterpolator();
    void setWindowWidth(int width);
    void setWindowHeight(int height);
    NBSemaphore* imgReaderSem;
    NBSemaphore* imgWriterSem;
    Camera* getCamera();

    // event receiver
    void Receive(Event e, void* args) override;

private:
    static Renderer* renderer;
    // the vertex array object (VAO)
    // this describes how the vertex attributes are 
    // stored in the a vertex buffer object (VBO)
    // see the glVertexAttribPointer function calls
    // to understand how it works
    GLuint vertexAttribs; 

     // the vertex buffer object (VBO) containing the 
    // vertex position data
    GLuint positionBuffer;

     // the vertex buffer object (VBO) containing the 
    // vertex tex coordinates data
    GLuint texCoordBuffer;

    // the element buffer object (EBO) contains the vertex indices
    GLuint indicesBuffer;

    float time;
    int counter;
    // store the sprites that have been read
    // from the image files
    std::map<std::string, SpriteInfo> sprites;

    // store the text characters
    std::map<unsigned char, Character> characters;
    glm::mat4 textProjectionMat;

    // the background color of the scene
    glm::vec4 backgroundColor;
    // size info of the window and camera
    int windowWidth;
    int windowHeight;

    // helper classes
    ShaderFactory shaderFactory;
    Camera camera;
    // for the texts' transforms
    // since it relies on screenspace, it fits in Renderer more
    Interpolator textPosInterpolX;
    Interpolator textPosInterpolY;

    GLFWwindow* setupGLFW(WindowSize windowSize);
    static void windowedResizedCallback(GLFWwindow* window, int width, int height);
    void resizeWindow(int width, int height);
    void updateInterpolation();

    void prepareGLBuffers();
    void resetVerticesData(bool flipUV);
    GLuint createTexBuffer(int height, int width, unsigned char* imgData);
    void loadTextLibrary();
    void loadImagesOld();
    void loadImages();
    void loadIntoImageData();
    void updateTexCoord(RenderComponent comp, SpriteInfo& info);
    void drawText(TextComponent* text, Transform* transform);
    int findTextWidth(TextComponent* text);
    void startDrawGameObjectsPhase(EntityCoordinator* coordinator);
    void startDrawUIPhase(EntityCoordinator* coordinator);
    void startDrawTextPhase(EntityCoordinator* coordinator);
};

