#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <glad/glad.h>
#include "SpriteInfo.h"

namespace FileManager
{
    stbi_uc* readImageFile(const char* fileName, int* width, int* height, int* colorChannels);
    std::string readTextFile(const char* fileName);
    std::string readShaderFile(const char* fileName);
}
