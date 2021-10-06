#pragma once
struct RenderComponent {
    // store the name of the vertex and fragment shaders
    const char *vShader;
    const char *fShader;

    // store the name of the spritesheet
    const char *spriteName;

    // store the sprite information
    // such as offsets and width/height
    float offsetX;
    float offsetY;
    float spriteWidth;
    float spriteHeight;
};
