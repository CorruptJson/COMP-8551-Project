#include "EntityQuery.h"

EntityQuery::EntityQuery(std::vector<ComponentType>& _compTypes, std::vector<Chunk*>& allChunks)
{
    compTypes = _compTypes;
    std::sort(compTypes.begin(), compTypes.end());
}

void EntityQuery::searchChunks(std::vector<Chunk*>& allChunks)
{
    for (int i = 0; i < allChunks.size(); i++)
    {
        Archetype arch = allChunks[i]->getArchetype();
        std::vector<ComponentType> archTypes = arch.getComponentTypesArrayCopy();
        int k = 0;
        for (int j = 0; j < compTypes.size(); j++)
        {
            if (compTypes[j] != archTypes[k])
            {
                k++;
                if (k == archTypes.size())
                {
                    continue;
                }
            }
        }

        if (k != archTypes.size())
        {
            chunks.push_back(allChunks[i]);
            entityCount += allChunks[i]->getCurrEntCount();
        }
    }
}

int EntityQuery::totalEntitiesFound()
{
    return entityCount;
}

int EntityQuery::chunkCount()
{
    return chunks.size();
}

Chunk* EntityQuery::chunk(int i)
{
    return chunks[i];
}
