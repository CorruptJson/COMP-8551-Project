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
#include "UIComponent.h"
#include "Transform.h"
#include "file_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "Animator.h"
#include "Character.h"

extern GLFWwindow* window;

class Renderer
{
public:
    static std::string DEFAULT_VERT_SHADER_NAME;
    static std::string DEFAULT_FRAG_SHADER_NAME;
    static std::string TEXT_VERT_SHADER_NAME;
    static std::string TEXT_FRAG_SHADER_NAME;
    static GLFWwindow* setupGLFW(int *width, int *height);
    int init(int viewWidth, int viewHeight, glm::vec4 newBackgroundColor);
    int update(EntityCoordinator* coordinator);
    int teardown();
    static Renderer* getInstance();
    Animation* getAnimation(std::string animName, std::string spriteName);
private:
    static Renderer* renderer;
    // the vertex array object (VAO)
    // this describes how the vertex attributes are 
    // stored in the a vertex buffer object (VBO)
    // see the glVertexAttribPointer function calls
    // to understand how it works
    GLuint vertexAttribs; 

     // the vertex buffer object (VBO) containing the 
    // vertex data
    GLuint vertexBuffer;

    // the element buffer object (EBO) contains the vertex indices
    GLuint indicesBuffer;

    // the default shader program
    // stored here since we most likely will use it often
    GLuint defaultShaderProgram;
    GLuint textShaderProgram;

    Camera camera;

    // store the sprites that have been read
    // from the image files
    std::map<std::string, SpriteInfo> sprites;

    // store the text characters
    std::map<unsigned char, Character> characters;

    // the background color of the scene
    glm::vec4 backgroundColor;

    GLuint createDefaultShaderProgram();
    GLuint createTextShaderProgram();
    void loadVertexData();
    void loadIndicesData();
    GLuint createTexBuffer(int height, int width, unsigned char* imgData);
    void loadTextLibrary();

    void loadTexture(std::string spriteName);
    void loadUniforms(glm::mat4 modelMatrix);
    void loadTextUniforms(glm::mat4 modelMatrix);
    void loadImages();
    void updateTexCoord(RenderComponent comp, std::string spriteName);
    void setTexCoordToDefault();
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color);
    void renderTextComponent(TextComponent* text);

    void drawGameObjects(EntityCoordinator* coordinator);
    void drawUI(EntityCoordinator* coordinator);
};

