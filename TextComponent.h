#include <string>
#include <vector>
#include <glm/glm.hpp>
#pragma once

enum class TextAlign {
    LEFT,
    CENTER,
    RIGHT
};

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

    TextAlign align;

    TextComponent(std::string text, float size, float r, float g, float b, TextAlign align_);

    // set the color of the text. It must be in the range of [0, 1].
    void setColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }

    void setText(std::string text);
    std::string getText();

    void setTextWidth(float width);
    float getTextWidth();
private:
    // the index of the text in the static texts variable.
    int textID; 

    // the width of the text on the screen
    // is only set after drawn one time on the screen
    float textWidth;
};
