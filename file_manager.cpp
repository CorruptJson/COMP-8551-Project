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
