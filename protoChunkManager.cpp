#include "protoChunkManager.h"


// finds an index for a new entity, creates a new ChunkAddress
ChunkAddress ProtoChunkManager::allocateNewEntity(SigArch sa)
{
    for (int i = 0; i < active.size(); i++)
    {
        if (active[i] == false)
        {
            active[i] = true;
            ChunkAddress id = { 0,i,0 };
            return id;
        }
    }
    ChunkAddress id = { 0,active.size(),0 };
    active.push_back(true);
    positions.push_back(TempPosition());
    return id;
};

// finds an index for a new entity, creates a new ChunkAddress, and sets the position
ChunkAddress ProtoChunkManager::allocateNewEntity(SigArch sa, TempPosition pos)
{
    ChunkAddress id = allocateNewEntity(sa);
    setEntityPosition(id, pos);
    return id;
}

// sets entity index as inactive
void ProtoChunkManager::deallocateEntity(ChunkAddress id)
{
    if (id.index >= active.size() || active[id.index] == false)
    {
        std::cout << "tried to deallocate entity that does not exist, index: " << id.index << std::endl;
        return;
    }
    active[id.index] = false;
};

// set the position of the entity
void ProtoChunkManager::setEntityPosition(ChunkAddress id, TempPosition pos)
{
    if (id.index >= active.size() || active[id.index] == false)
    {
        std::cout << "tried to set possition for entity that does not exist, index: " << id.index << std::endl;
        return;
    }
    positions[id.index] = pos;
}

// get the position of the entity
TempPosition ProtoChunkManager::getEntityPosition(ChunkAddress id)
{
    if (id.index >= active.size() || active[id.index] == false)
    {
        std::cout << "tried to access possition for entity that does not exist, index: " << id.index << std::endl;
        TempPosition p = { -1,-1 };
        return p;
    }
    return positions[id.index];
};
