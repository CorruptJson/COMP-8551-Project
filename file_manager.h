#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <glad/glad.h>
#include <string>
#include "SpriteInfo.h"

namespace FileManager
{
    stbi_uc* readImageFile(std::string fileName, int* width, int* height, int* colorChannels);
    std::string readTextFile(std::string fileName);
    std::string readShaderFile(std::string fileName);
}
