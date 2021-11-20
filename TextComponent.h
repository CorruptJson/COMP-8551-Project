#include <string>
#include <vector>
#pragma once

class TextComponent
{
public:
    // store all the texts in memory
    // this assumes that we'll never delete the texts individually
    // if we ever need that, switch to hashmap
    static std::vector<std::string> texts;

    // clear the texts stored in the static variable
    static void clearTexts();

    float x, y; //x and y position on the screen. (0,0) is at bottom left and goes right and up
    float size; //scale of the text
    float R = 1.0f; //red
    float G = 0.0f; //blue
    float B = 0.0f; //green

    TextComponent(std::string text, float size, float r, float g, float b);

    // set the color of the text. It must be in the range of [0, 1].
    void setColor(float r, float g, float b) { R = r; G = g; B = b; }

    void setText(std::string text);
    std::string getText();
private:
    // the index of the text in the static texts variable.
    int textID; 
};
