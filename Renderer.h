#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

extern GLFWwindow* window;

class Renderer
{
public:
    static GLFWwindow* setupGLFW();
    int init();
    int update();
    int teardown();
private:
    GLuint createShaderProgram();
};

