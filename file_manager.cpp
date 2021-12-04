#include "file_manager.h"
// has to import stb_image and the implementation definition
// in an implementation file
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

stbi_uc* FileManager::readImageFile(std::string fileName, int* width, int* height, int* colorChannels) {
    return stbi_load(fileName.c_str(), width, height, colorChannels, STBI_rgb_alpha);
}

std::string FileManager::readTextFile(std::string fileName) {
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

std::string FileManager::readShaderFile(std::string fileName) {
    return readTextFile(fileName);
}

void FileManager::writeTextFile(std::string fileName, std::string text) {
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(fileName);
        file << text;
        file.close();
    }
    catch (std::ifstream::failure e) {
        std::string fileNameStr(fileName);
        throw std::invalid_argument("Couldn't read file: " + fileNameStr);
    }
}
