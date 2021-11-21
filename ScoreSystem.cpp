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
    
    ComponentIterator<TextComponent> tci(eq);

    //Create score text
    string* s = new string("Score: ");
    s->append(to_string(score));
    char* output = (char*)s->c_str(); 

    //Set score text
    tci.nextComponent()->value = output;
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
