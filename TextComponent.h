#include <string>
#pragma once
struct TextComponent {
    //string to be rendered
    char* value; //text value
    float x, y; //x and y position on the screen. (0,0) is at bottom left and goes right and up
    float R = 1.0f; //red
    float G = 0.0f; //blue
    float B = 0.0f; //green
    float size; //scale of the text
    void setColor(float r, float g, float b) { R = r; G = g; B = b; }
};
