#include "ScoreSystem.h"
#include "EntityCoordinator.h"
#include "Sound.h"
#include <string>
#include <algorithm>
using namespace std;

int ScoreSystem::score = 0;

void ScoreSystem::UpdateScore()
{
    EntityCoordinator* ec = &EntityCoordinator::getInstance();
    std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
        ec->GetComponentType<TextComponent>()
    }, {Tag::TXT_SCORE});
    
    if (eq->totalEntitiesFound() < 1)
        return;
    ComponentIterator<TextComponent> tci(eq);

    //Create score text
    string* s = new string(string(5 - min(5, (int)to_string(ScoreSystem::score).length()), '0') + to_string(ScoreSystem::score));

    //Set score text
    tci.nextComponent()->setText(*s);
}

void ScoreSystem::Receive(Event e, void* args)
{
    switch (e) {
    case Event::STAR_PICKED_UP:
        Sound::getInstance().playSound(STARGRABBED);
        ScoreSystem::score++;
        UpdateScore();
        break;
    }
}
