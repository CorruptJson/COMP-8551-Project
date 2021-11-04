#pragma once
#include "Types.h"
#include "EntityCoordinator.h"
#include "Renderer.h"


class SceneManager
{


private:
    Renderer* renderer;
    EntityCoordinator* coordinator;
    json jsonArray;
    



public:
    
    SceneManager();

    void LoadJson(const char* filename);
    void CreateEntities();




    std::list<EntityID> entities;

};

