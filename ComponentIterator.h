#pragma once
#include <vector>
#include <iostream>
#include "EntityQuery.h"
#include "chunk.h"

// in order to access the component data of entities found by an Entity query, a componentIterator is used
// it is not an actual official-style c++ iterator
// 
// it hides the actual implementation of object pool chunks away from other developers
// other developers may treat collections of chunks like a single array, thanks to the componentIterator

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
            // skip past empty chunks
            for (int i = 0; i < chunks.size(); i++)
            {
                if (chunks[i]->getCurrEntCount() != 0)
                {
                    break;
                }
            }

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
            //std::cout << "no entities for comp iterator" << std::endl;
        }
    }

    T* nextComponent() {
        if (currEnt >= chunks[currChunk]->getCurrEntCount())
        {
            currChunk++;
            // skip past empty chunks
            while(currChunk < chunks.size())
            {
                if (chunks[currChunk]->getCurrEntCount() != 0)
                {
                    break;
                }
                else
                {
                    currChunk++;
                }
            }            
            currEnt = 0;
            currIndex = 0;
            componentArray = chunks[currChunk]->getComponentArray<T>();
        }
        int whileCounter = 0;
        while (!chunks[currChunk]->isDataIndexActive(currIndex))
        {
            currIndex++;
            whileCounter++;
            if (whileCounter > ENTITIES_PER_CHUNK)
            {
                throw "loop limit reached";
            }
        }
        T* comp = &(componentArray[currIndex]);
        currEnt++;
        currIndex++;
        return comp;
    }

    EntityID getCurrEnt()
    {
        return chunks[currChunk]->entityAtComponentIndex(currIndex - 1);
    }
};
