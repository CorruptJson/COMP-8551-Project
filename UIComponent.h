#pragma once
#include <glm/glm.hpp>
struct UIComponent {
    //sprite for the UI
    const char* spriteName;
    int row, col;
    //color of the panel/background of the sprite
    glm::vec4 color;
};
