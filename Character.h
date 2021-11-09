#pragma once
#include <glad/glad.h>

// store info about a text character texture.
//struct Character {
//    GLuint TextureID;  // ID handle of the glyph texture
//    unsigned int height; // the height of the glyph
//    unsigned int width; // the width of the glyph
//    unsigned int bearingX; // the horizontal bearing e.g. the horizontal position (in pixels) of the bitmap relative to the origin accessed via //left
//    unsigned int bearingY; // the vertical bearing e.g. the horizontal position (in pixels) of the bitmap relative to the origin accessed via //top
//    unsigned int Advance;    // Offset to advance to next glyph;
//};

struct Character {
    GLuint TextureID;  // ID handle of the glyph texture
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int Advance;
};
