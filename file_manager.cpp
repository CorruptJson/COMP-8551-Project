#include "file_manager.h"
// has to import stb_image and the implementation definition
// in an implementation file
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

stbi_uc* FileManager::readImageFile(const char* fileName, int* width, int* height, int* colorChannels) {
    return stbi_load(fileName, width, height, colorChannels, STBI_rgb_alpha);
}

std::string FileManager::readTextFile(const char* fileName) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(fileName);
        std::stringstream strStream;

        // read the file into the stream
        strStream << file.rdbuf();
        file.close();

        // convert the stream into a string
        return strStream.str();
    }
    catch (std::ifstream::failure e) {
        std::string fileNameStr(fileName);
        throw std::invalid_argument("Couldn't read file: " + fileNameStr);
    }
}

std::string FileManager::readShaderFile(const char* fileName) {
    return readTextFile(fileName);
}
