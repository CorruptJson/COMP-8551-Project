#pragma once
#include <iterator>
#include <cstddef>
#include <vector>
#include <algorithm>
#include "chunk.h"
#include "Types.h"
#include "ComponentManager.h"

// this class stores the result of a search
// it searches for which entities contain the specified components
// it can be used to access those components of those entities
class EntityQuery
{
    friend class EntityCoordinator;

private:
    // entity count
    int entityCount = 0;
    int chunkListVersion = 0;

    // chunks
    std::vector<Chunk*> chunks;

    // the components the query is searching for 
    std::vector<ComponentType> compTypes;

    void searchChunks(std::vector<Chunk*>& allChunks);

    EntityQuery();



public:

    int totalEntitiesFound();
    int chunkCount();
    int getChunkListVersion();
    Chunk* chunk(int i);

    // entity queries perform their search when they are created
    EntityQuery(std::vector<ComponentType>& _compTypes, std::vector<Chunk*>& allChunks);

    // get a vector of pointers for the components of the specified type
    // the components belonging to the entities found in the query
    template<typename T>
    std::vector<T*> getComponentArray()
    {
        ComponentType type = ComponentManager::GetComponentType<T>();
        if (std::find(compTypes.begin(),compTypes.end(),type) == compTypes.end())
        {
            throw "cannot get component array from query: query does not contain this type";
        }

        std::vector<T*> list;

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
