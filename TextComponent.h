#include <string>
#pragma once
struct TextComponent {
    //string to be rendered
    std::string value;
    float x, y;
    float R = 1.0f;
    float G = 0.0f;
    float B = 0.0f;
    float size;
    void setColor(float r, float g, float b) { R = r; G = g; B = b; }
};
