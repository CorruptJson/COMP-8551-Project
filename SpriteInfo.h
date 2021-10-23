#pragma once
#include <stb/stb_image.h>
#include <map>
#include "AnimationComponent.h"
#include <string>
// store information on a spritesheet
struct SpriteInfo {
    // pixel height and width
    int height;
    int width;

    // info on the grid sizes
    int rows;
    int columns;
    std::map<const char *, Animation> spriteAnims;
    // opengl texture buffer id
    GLuint id;
};
