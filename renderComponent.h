#pragma once
#include "Shader.h"

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

    // whether there's an animation component attached to the entity
    bool hasAnimation;

    // whether the texture needs to be flipped
    bool flipX;
};
