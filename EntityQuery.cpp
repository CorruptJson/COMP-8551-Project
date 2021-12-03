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

EntityQuery::EntityQuery(std::vector<ComponentType> _compTypes, std::vector<Tag> _tags)
{
    compTypes = _compTypes;
    std::sort(compTypes.begin(), compTypes.end());
    tags = _tags;
    std::sort(tags.begin(), tags.end());

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

void EntityQuery::DeleteFoundEntities()
{
    for (int i = 0; i < chunks.size(); i++)
    {
        chunks[i]->scheduleAllEntitiesToDelete();
    }
}

std::size_t EntityQuery::TagsHash(std::vector<Tag>& tags)
{
    std::size_t seed = tags.size();
    for (int i = 0; i < tags.size(); i++)
    {
        seed ^= tags[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::size_t EntityQuery::ComponentTypesHash(std::vector<ComponentType>& compTypes)
{
    std::size_t seed = compTypes.size();
    for (int i = 0; i < compTypes.size(); i++)
    {
        seed ^= compTypes[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::size_t EntityQuery::QueryParamterHash(std::vector<ComponentType>& compTypes, std::vector<Tag>& tags)
{
    std::size_t seed = compTypes.size() + tags.size();
    std::size_t hash = ComponentTypesHash(compTypes);
    seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash = TagsHash(tags);
    seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

EntityQuery::EntityQuery(std::vector<Chunk*> chosenChunks)
{
    chunks = chosenChunks;
    for (int i = 0; i < chunks.size(); i++)
    {
        entityCount += chunks[i]->getCurrEntCount();
    }
}

void EntityQuery::searchChunks(std::vector<Chunk*>& allChunks,int _chunkListVersion)
{
    chunks.clear();
    for (int i = 0; i < allChunks.size(); i++)
    {
        Chunk* chunk = allChunks[i];
        const std::vector<ComponentType>& chunkComps = chunk->getArchetype().getComponentTypeArray();
        std::vector<Tag>& chunkTags = chunk->getAllTags();
        if (chunkComps.size() < compTypes.size() || chunkTags.size() < tags.size())
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
        }
    }
    recountFoundEntities();
    chunkListVersion = _chunkListVersion;
}

//std::size_t EntityQuery::ComponentTypesHash()
//{
//    std::size_t seed = compTypes.size();
//    for (int i = 0; i < compTypes.size(); i++)
//    {
//        seed ^= compTypes[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//    }
//    return seed;
//}
//
//std::size_t EntityQuery::TagsHash()
//{
//    std::size_t seed = tags.size();
//    for (int i = 0; i < tags.size(); i++)
//    {
//        seed ^= tags[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//    }
//    return seed;
//}
//
//std::size_t EntityQuery::QueryHash()
//{
//    std::size_t seed = compTypes.size() + tags.size();
//    std::size_t hash = ComponentTypesHash();
//    seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//    hash = TagsHash();
//    seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//    return seed;
//}

void EntityQuery::recountFoundEntities()
{
    entityCount = 0;
    for (int i = 0; i < chunks.size(); i++)
    {
        entityCount += chunks[i]->getCurrEntCount();
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
    return chunkListVersion;
}

Chunk* EntityQuery::chunk(int i)
{
    return chunks[i];
}


