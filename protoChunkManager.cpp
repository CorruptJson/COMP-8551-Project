#include "protoChunkManager.h"


// finds an index for a new entity, creates a new EntityID
EntityID protoChunkManager::allocateNewEntity(SigArch sa)
{
    for (int i = 0; i < active.size(); i++)
    {
        if (active[i] == false)
        {
            active[i] = true;
            EntityID id = { 0,i,0 };
            return id;
        }
    }
    active.push_back(true);
    EntityID id = { 0,active.size(),0 };
    return id;
};

// finds an index for a new entity, creates a new EntityID, and sets the position
EntityID protoChunkManager::allocateNewEntity(SigArch sa, TempPosition pos)
{
    EntityID id = allocateNewEntity(sa);
    setPosition(id, pos);
    return id;
}

// sets entity index as inactive
void protoChunkManager::deallocateEntity(EntityID id) 
{
    if (id.index >= active.size())
    {
        std::cout << "tried to deallocate entity that does not exist, index: " << id.index << std::endl;
        return;
    }
    active[id.index] = false;
};

// set the position of the entity
void protoChunkManager::setPosition(EntityID id, TempPosition pos)
{
    if (id.index >= active.size())
    {
        std::cout << "tried to set possition for entity that does not exist, index: " << id.index << std::endl;
        return;
    }
    positions[id.index] = pos;
}

// get the position of the entity
TempPosition protoChunkManager::getEntityPosition(EntityID id) 
{
    if (id.index >= active.size())
    {
        std::cout << "tried to access possition for entity that does not exist, index: " << id.index << std::endl;
        TempPosition p = { -1,-1 };
        return p;
    }
    return positions[id.index];
};
