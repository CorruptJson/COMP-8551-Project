#include "EntityQuery.h"

//EntityQuery::EntityQuery(std::vector<ComponentType> _compTypes, std::vector<Chunk*> allChunks, EntityQueryCache& cache)
//{
//    compTypes = _compTypes;
//    std::sort(compTypes.begin(), compTypes.end());
//    tags = {};
//    chunkListVersion = allChunks.size();    
//
//    size_t hash = QueryHash();
//    auto find = cache.find(hash);
//    if (find != cache.end() && find->second->getChunkListVersion() == chunkListVersion)
//    {
//        chunks = find->second->foundChunks();
//        entityCount = find->second->totalEntitiesFound();
//    }
//    else
//    {
//        searchChunks(allChunks);
//        //cache.emplace(hash,)
//    }    
//}

EntityQuery::EntityQuery(std::vector<ComponentType> _compTypes, std::vector<Tag> _tags,int _chunkListVersion)
{
    compTypes = _compTypes;
    std::sort(compTypes.begin(), compTypes.end());
    tags = _tags;
    std::sort(tags.begin(), tags.end());
    chunkListVersion = _chunkListVersion;

    //size_t hash = QueryHash();
    //auto find = cache.find(hash);
    //if (find != cache.end() && find->second->getChunkListVersion() == chunkListVersion)
    //{
    //    chunks = find->second->foundChunks();
    //    entityCount = find->second->totalEntitiesFound();
    //}
    //else
    //{
    //    searchChunks(allChunks);
    //}
}

EntityQuery::EntityQuery()
{

}

std::vector<Chunk*> EntityQuery::foundChunks()
{
    return chunks;
}

EntityQuery::EntityQuery(std::vector<Chunk*> chosenChunks)
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
        const std::vector<ComponentType>& chunkComps = chunk->getArchetype().getComponentTypeArray();
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

        if ((c_search != chunkComps.size() || compTypes.size() == 0)  && (t_search != chunkTags.size() || tags.size() == 0))
        {
            chunks.push_back(allChunks[i]);
            entityCount += allChunks[i]->getCurrEntCount();
            //std::cout << "found chunk " << std::endl;
        }
    }
}

std::size_t EntityQuery::ComponentTypesHash()
{
    std::size_t seed = compTypes.size();
    for (int i = 0; i < compTypes.size(); i++)
    {
        seed ^= compTypes[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::size_t EntityQuery::TagsHash()
{
    std::size_t seed = tags.size();
    for (int i = 0; i < tags.size(); i++)
    {
        seed ^= tags[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::size_t EntityQuery::QueryHash()
{
    std::size_t seed = compTypes.size() << 8;
    seed += tags.size();
    std::size_t hash = ComponentTypesHash();
    hash ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return hash;
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


