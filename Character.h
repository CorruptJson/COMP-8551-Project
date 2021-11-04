#pragma once
#include <glad/glad.h>

// store info about a text character texture.
struct Character {
    GLuint TextureID;  // ID handle of the glyph texture
    int height; // the height of the glyph
    int width; // the width of the glyph
    int bearingX; // the horizontal bearing e.g. the horizontal position (in pixels) of the bitmap relative to the origin accessed via
    int bearingY; // the vertical bearing e.g. the horizontal position (in pixels) of the bitmap relative to the origin accessed via
    unsigned int Advance;    // Offset to advance to next glyph;
};
