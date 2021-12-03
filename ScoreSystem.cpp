#include "ScoreSystem.h"
#include "EntityCoordinator.h"
#include <string>
using namespace std;
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
    string* s = new string("000");
    s->append(to_string(score));

    //Set score text
    tci.nextComponent()->setText(*s);
}

void ScoreSystem::Receive(Event e, void* args)
{
    switch (e) {
    case Event::STAR_PICKED_UP:
        score++;
        UpdateScore();
        break;
    }
}
