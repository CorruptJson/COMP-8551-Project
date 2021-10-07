#pragma once
#include <fstream>
#include "SpriteInfo.h"
#include <iostream>
#include <string>

namespace FileManager
{
    SpriteInfo* readImageFile(const char* fileName);
    const char* readTextFile(const char* fileName);
    const char* readShaderFile(const char* fileName);
}
