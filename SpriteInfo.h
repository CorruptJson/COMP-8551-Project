#pragma once
#include <stb/stb_image.h>

// store information on a spritesheet
struct SpriteInfo {
    // pixel height and width
    int height;
    int width;

    // info on the grid sizes
    int rows;
    int columns;

    // opengl texture buffer id
    GLuint id;
};
