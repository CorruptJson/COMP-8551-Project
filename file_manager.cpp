#include "file_manager.h"
// has to import stb_image and the implementation definition
// in an implementation file
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

SpriteInfo* FileManager::readImageFile(const char* fileName) {
    SpriteInfo sprite;
    sprite.data = stbi_load(fileName, &(sprite.width), &(sprite.height), &(sprite.colorChannelsAmount), STBI_rgb_alpha);
    if (!sprite.data) {
        return NULL;
    }
    return &sprite;
}

const char* FileManager::readTextFile(const char* fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);

    if (file) {
        // get file length
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg); 

        char* buffer = new char [length];
        file.read(buffer, length);
        file.close();

        return buffer;
    }
    else {
        std::string fileNameStr(fileName);
        throw std::invalid_argument("Couldn't read file: " + fileNameStr);
    }
}

const char* FileManager::readShaderFile(const char* fileName) {
    return readTextFile(fileName);
}
