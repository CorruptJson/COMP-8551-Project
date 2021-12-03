#pragma once
#include "system.h"
#include "Event.h"
#include "TextComponent.h"

class ScoreSystem: public System, public IObserver
{
public:
    static int score;
    ScoreSystem() {};
    const char* scoreText = "Score: ";
    void UpdateScore();
    void Receive(Event e, void* args) override;
};

