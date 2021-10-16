#pragma once
#include <iterator>
#include <cstddef>
#include <vector>
#include "chunk.h"
#include "Types.h"
#include "ComponentManager.h"

class EntityQuery
{
private:
    // entity count
    int entityCount = 0;

    // chunks
    std::vector<Chunk*> chunks;

    // components query IDs
    std::vector<ComponentType> compTypes;

    void searchChunks(std::vector<Chunk*>& allChunks);

public:

    EntityQuery(std::vector<ComponentType>& _compTypes,std::vector<Chunk*>& allChunks);
    int totalEntitiesFound();
    int chunkCount();
    Chunk* chunk(int i);

    template<typename T>
    std::vector<T*> getComponentArray()
    {
        std::vector<T*> list;
        //ComponentType type = ComponentManager::NEW_GetComponentType<T>();

        for (int i = 0; i < chunks.size(); i++)
        {
            int chunkEnts = chunks[i]->getCurrEntCount();
            T* compArray = chunks[i]->getComponentArray<T>();
            for (int j = 0; j < chunkEnts; j++)
            {
                list.push_back(compArray);
                compArray++;
            }
        }

        return list;
    }
};

template<typename T>
struct componentArray
{
    T* start;
    int count;
};

