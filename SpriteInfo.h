#pragma once
#include <stb/stb_image.h>

struct SpriteInfo {
    int height;
    int width;
    int row;
    int column;
    int colorChannelsAmount;
    stbi_uc *data;
};
