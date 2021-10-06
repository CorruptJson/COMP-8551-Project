#include <string>

#pragma once
struct RenderComponent {
    // store the name of the vertex and fragment shaders
    std::string vShader;
    std::string fShader;

    // store the name of the spritesheet
    std::string spriteName;

    // store the sprite information
    // such as offsets and width/height
    float offsetX;
    float offsetY;
    float spriteWidth;
    float spriteHeight;
};
