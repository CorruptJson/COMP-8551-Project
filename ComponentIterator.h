#pragma once
#include <vector>
#include <iostream>
#include "EntityQuery.h"
#include "chunk.h"

template<typename T>
class ComponentIterator
{
private:
    std::vector<Chunk*> chunks;
    T* componentArray;
    int currChunk = 0;
    int currEnt = 0;
    int currIndex = 0;
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

    ComponentIterator(std::unique_ptr<EntityQuery>& query)
    {
        if (query->totalEntitiesFound() > 0)
        {
            chunks = query->foundChunks();
            componentArray = chunks[0]->getComponentArray<T>();
        }
        else
        {
            std::cout << "no entities for comp iterator" << std::endl;
        }
    }

    ComponentIterator(std::shared_ptr<EntityQuery>& query)
    {
        if (query->totalEntitiesFound() > 0)
        {
            chunks = query->foundChunks();
            componentArray = chunks[0]->getComponentArray<T>();
        }
        else
        {
            std::cout << "no entities for comp iterator" << std::endl;
        }
    }

    T* nextComponent() {
        if (currEnt >= chunks[currChunk]->getCurrEntCount())
        {
            currChunk++;
            currEnt = 0;
            currIndex = 0;
            componentArray = chunks[currChunk]->getComponentArray<T>();
        }
        int whileCounter = 0;
        while (!chunks[currChunk]->isDataIndexActive(currIndex))
        {
            currIndex++;
            whileCounter++;
            if (whileCounter > 16)
            {
                throw "loop limit reached";
            }
        }
        T* comp = &(componentArray[currIndex]);
        currEnt++;
        currIndex++;
        return comp;
    }
};