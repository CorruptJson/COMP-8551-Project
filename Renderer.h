#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

extern GLFWwindow* window;

class Renderer
{
public:
    static GLFWwindow* setupGLFW(int *width, int *height);
    int init();
    int update();
    int teardown();
private:
    // the vertex array object
    GLuint VAO; 

     // the vertex buffer object contains the 
    // vertex data
    GLuint vertexBuffer;

    // the element buffer object contains the vertex indices
    GLuint indicesBuffer;

    // the texture contains the texture buffer
    GLuint textureBuffer;

    // the default shader program
    GLuint defaultShaderProgram;

    Camera camera;

    GLuint createDefaultShaderProgram();
    void initBuffers();
    void loadVertexData();
    void loadIndicesData();
    void loadTexture(const char *spriteSheetName);
    void loadUniforms(mat4 modelMatrix);
};

