#pragma once
#include <stb/stb_image.h>

struct SpriteInfo {
    int height;
    int width;
    int colorChannelsAmount;
    stbi_uc *data;
};
