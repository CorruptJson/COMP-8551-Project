#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "SpriteInfo.h"

namespace FileManager
{
    SpriteInfo* readImageFile(const char* fileName);
    std::string readTextFile(const char* fileName);
    std::string readShaderFile(const char* fileName);
}
