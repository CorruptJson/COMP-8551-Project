#pragma once
#include "ShaderFactory.h"
#include <glm/glm.hpp>

struct RenderComponent {
    // shader program that we want to use
    ShaderName shaderName;

    // store the name of the spritesheet
    const char *spriteName;

    // store the sprite position within
    // the spritesheet
    // column increases from 0 to 1 left to right
    // row increasese from 0 to 1 top to bottom
    float rowIndex;
    float colIndex;

    // whether the texture needs to be flipped
    bool flipX = false;

    //color of the panel/background of the sprite
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

    // whether to only display the color (ignore the sprite) onyl
    bool colorOnly = false;
};
