#pragma once
#include <vector>
//#include <>
#include "chunk.h"
#include "EntityQuery.h"

template<typename T>
class ComponentIterator
{
private:
    std::vector<Chunk*> chunks;
    T* componentArray;
    int currChunk = 0;
    int currEnt = 0;
    ComponentIterator() {};
public:
    ComponentIterator(std::vector<Chunk*>& _chunks)
    {
        chunks = _chunks;
        if (chunks.size > 0)
        {
            componentArray = chunks[0]->getComponentArray<T>();
        }        
    }

    ComponentIterator(EntityQuery& query)
    {
        if (query.entityCount == 0)
        {
            std
        }
    }

    T& nextComponent() {
        T* comp = componentArray[currEnt];
        currEnt++;
        if (currEnt >= chunks[currChunk]->getCurrEntCount())
        {
            currChunk++;
            currEnt = 0;
            componentArray = chunks[currChunk]->getComponentArray<T>();
        }
        return comp;
    }
};
