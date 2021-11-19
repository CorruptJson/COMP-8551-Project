#include <string>
#pragma once

class TextComponent
{
public:
    //string to be rendered
    std::string value; //text value
    float x, y; //x and y position on the screen. (0,0) is at bottom left and goes right and up
    float size; //scale of the text
    float R = 1.0f; //red
    float G = 0.0f; //blue
    float B = 0.0f; //green
    void setColor(float r, float g, float b) { R = r; G = g; B = b; }
private:
};
