#include "EntityQuery.h"

EntityQuery::EntityQuery(std::vector<ComponentType>& _compTypes, std::vector<Chunk*>& allChunks)
{
    compTypes = _compTypes;
    std::sort(compTypes.begin(), compTypes.end());
    chunkListVersion = allChunks.size();
    searchChunks(allChunks);
}

std::vector<Chunk*> EntityQuery::foundChunks()
{
    return chunks;
}

void EntityQuery::searchChunks(std::vector<Chunk*>& allChunks)
{
    for (int i = 0; i < allChunks.size(); i++)
    {
        Archetype arch = allChunks[i]->getArchetype();
        std::vector<ComponentType> archTypes = arch.getComponentTypeArray();

        if (allChunks[i]->getCurrEntCount() == 0 || archTypes.size() < compTypes.size())
        {
            continue;
        }

        int k = 0;

        for (int j = 0; j < compTypes.size() && k < archTypes.size(); j++)
        {
            for (; k < archTypes.size(); k++)
            {
                if (compTypes[j] == archTypes[k])
                {
                    break;
                }
            }
        }

        if (k != archTypes.size())
        {
            chunks.push_back(allChunks[i]);
            entityCount += allChunks[i]->getCurrEntCount();
            //std::cout << "found chunk " << std::endl;
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

int EntityQuery::getChunkListVersion()
{
    return 0;
}

Chunk* EntityQuery::chunk(int i)
{
    return chunks[i];
}

