#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Camera.h"

extern GLFWwindow* window;
GLFWwindow* setupGLFW();

int renderTutorialInit();
int renderTutorialUpdate();
int renderTutorialTeardown();

