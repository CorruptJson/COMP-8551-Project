#pragma once
#include "Types.h"
#include "EntityCoordinator.h"


class SceneManager
{


private:
    EntityCoordinator* coordinator;
    json jsonArray;




public:
    
    SceneManager();

    void LoadJson(const char* filename);
    void CreateEntities();


};

