#pragma once
#include "system.h"
#include "Event.h"
#include "TextComponent.h"

class ScoreSystem: public System, public IObserver
{
public:
    ScoreSystem() : score{ 0 } {};
    int score = 0;
    const char* scoreText = "Score: ";
    void UpdateScore();
    void Receive(Event e, void* args) override;
};

