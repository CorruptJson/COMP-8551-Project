#include <string>
#include <vector>
#include <glm/glm.hpp>
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

    float size; //scale of the text
    glm::vec3 color;

    TextComponent(std::string text, float size, float r, float g, float b);

    // set the color of the text. It must be in the range of [0, 1].
    void setColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }

    void setText(std::string text);
    std::string getText();
private:
    // the index of the text in the static texts variable.
    int textID; 
};
