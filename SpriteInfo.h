#pragma once
#include <stb/stb_image.h>
#include <map>
#include "AnimationComponent.h"
#include <string>
// store information on a spritesheet
struct SpriteInfo {
    // opengl texture buffer id
    GLuint id;

    // pixel height and width
    int height;
    int width;

    // animations stuff
    // info on the grid sizes
    int rows;
    int columns;
    float cellWidth;
    float cellHeight;
    std::map<const char *, Animation> spriteAnims;

};
