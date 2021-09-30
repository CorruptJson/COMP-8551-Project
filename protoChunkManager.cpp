#include "chunkManagerAbstract.h"
#include <vector>
#include <iostream>

class protoChunkManager : public ChunkManagerAbstract
{
private:
    int currEntities = 0;
    std::vector<bool> active;
    std::vector<TempPosition> positions;

public:

    // finds an index for a new entity, creates a new EntityID
    EntityID allocateNewEntity(SigArch sa) override
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
    EntityID allocateNewEntity(SigArch sa, TempPosition pos)
    {
        EntityID id = allocateNewEntity(sa);
        setPosition(id,pos);
        return id;
    }

    // sets entity index as inactive
    void deallocateEntity(EntityID id) override
    {
        if (id.index >= active.size())
        {
            std::cout << "tried to deallocate entity that does not exist, index: " << id.index << std::endl;
            return;
        }
        active[id.index] = false;
    };

    // set the position of the entity
    void setPosition(EntityID id, TempPosition pos)
    {
        if (id.index >= active.size())
        {
            std::cout << "tried to set possition for entity that does not exist, index: " << id.index << std::endl;
            return;
        }
        positions[id.index] = pos;
    }

    // get the position of the entity
    TempPosition getEntityPosition(EntityID id) override
    {
        if (id.index >= active.size())
        {
            std::cout << "tried to access possition for entity that does not exist, index: " << id.index << std::endl;
            return;
        }
        return positions[id.index];
    };
};
