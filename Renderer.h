#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Camera.h"
#include "SpriteInfo.h"
#include "EntityCoordinator.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "file_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

extern GLFWwindow* window;

class Renderer
{
public:
    static const char *DEFAULT_VERT_SHADER_NAME;
    static const char *DEFAULT_FRAG_SHADER_NAME;
    static GLFWwindow* setupGLFW(int *width, int *height);
    int init();
    int update(EntityCoordinator* coordinator);
    int teardown();
private:
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


    glm::mat4 projectionMatrix;
    Camera camera;

    // store the sprites that have been read
    // from the image files
    std::map<const char*, SpriteInfo> sprites;

    GLuint createDefaultShaderProgram();
    void loadVertexData();
    void loadIndicesData();
    GLuint createTexBuffer(SpriteInfo info, stbi_uc* imgData);
    void loadTexture(const char* spriteName);
    void loadUniforms(mat4 modelMatrix);
    void loadImages();
    void updateTexCoord(RenderComponent comp, const char* spriteName);
};

