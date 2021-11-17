#include "EntityQuery.h"

EntityQuery::EntityQuery(std::vector<ComponentType> _compTypes, std::vector<Chunk*> allChunks)
{
    compTypes = _compTypes;
    std::sort(compTypes.begin(), compTypes.end());
    tags = {};
    chunkListVersion = allChunks.size();    
    searchChunks(allChunks);
}

EntityQuery::EntityQuery(std::vector<ComponentType> _compTypes, std::vector<Tag> _tags, std::vector<Chunk*>& allChunks)
{
    compTypes = _compTypes;
    std::sort(compTypes.begin(), compTypes.end());
    tags = _tags;
    std::sort(tags.begin(), tags.end());
    chunkListVersion = allChunks.size();    
    searchChunks(allChunks);
}

std::vector<Chunk*> EntityQuery::foundChunks()
{
    return chunks;
}

EntityQuery::EntityQuery(std::vector<Chunk*>& chosenChunks)
{
    chunks = chosenChunks;
    for (int i = 0; i < chunks.size(); i++)
    {
        entityCount += chunks[i]->getCurrEntCount();
    }
}


void EntityQuery::searchChunks(std::vector<Chunk*>& allChunks)
{
    for (int i = 0; i < allChunks.size(); i++)
    {
        Chunk* chunk = allChunks[i];
        Archetype arch = chunk->getArchetype();
        std::vector<ComponentType> chunkComps = arch.getComponentTypeArray();
        std::vector<Tag>& chunkTags = chunk->getAllTags();
        if (allChunks[i]->getCurrEntCount() == 0 || chunkComps.size() < compTypes.size() || chunkTags.size() < tags.size())
        {
            continue;
        }

        int c_search = 0;

        // check that chunk contains query components
        for (int j = 0; j < compTypes.size() && c_search < chunkComps.size(); j++)
        {
            for (; c_search < chunkComps.size(); c_search++)
            {
                if (compTypes[j] == chunkComps[c_search])
                {
                    break;
                }
            }
        }
        
        int t_search = 0;
        // check that chunk contains tags, if there are no tags in the query, pass
        if (tags.size() != 0)
        {
            for (int j = 0; j < tags.size() && t_search < chunkTags.size(); j++)
            {
                for (; t_search < chunkTags.size(); t_search++)
                {
                    if (tags[j] == chunkTags[t_search])
                    {
                        break;
                    }
                }
            }
        }

        if (c_search != chunkComps.size() && (t_search != chunkTags.size() || tags.size() == 0))
        {
            chunks.push_back(allChunks[i]);
            entityCount += allChunks[i]->getCurrEntCount();
            //std::cout << "found chunk " << std::endl;
        }
    }
}

EntityQuery::EntityQuery()
{
    int entityCount = 0;
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


