#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <map>
#include "SpriteInfo.h"

extern GLFWwindow* window;

class Renderer
{
public:
    static GLFWwindow* setupGLFW(int *width, int *height);
    int init();
    int update();
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

    // the texture contains the texture buffer
    GLuint textureBuffer;

    // the default shader program
    // stored here since we most likely will use it often
    GLuint defaultShaderProgram;

    Camera camera;

    // store the sprites that have been read
    // from the image files
    std::map<const char*, SpriteInfo> sprites;

    GLuint createDefaultShaderProgram();
    void loadVertexData();
    void loadIndicesData();
    void loadTexture(const char *spriteName);
    void loadUniforms(mat4 modelMatrix);
};

