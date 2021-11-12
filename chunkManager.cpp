#include "chunkManager.h"

Chunk* ChunkManager::createChunk(Archetype arch, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap)
{
    Chunk* newChunk = new Chunk(arch, currChunks++, spriteSheet, tags, sizemap);
    allChunks.push_back(newChunk);

    ArchetypeType at = arch.getType();
    auto findArch = chunksByArch.find(at);
    if (findArch != chunksByArch.end())
    {
        findArch->second.push_back(newChunk);        
    }
    else
    {
        chunksByArch.emplace(at ,std::vector<Chunk*>());
        chunksByArch[at].push_back(newChunk);
    }

    auto findSprite = chunksBySpritesheet.find(spriteSheet);
    if (findSprite != chunksBySpritesheet.end())
    {
        findSprite->second.push_back(newChunk);
    }
    else
    {
        chunksBySpritesheet.emplace(spriteSheet, std::vector<Chunk*>());
        chunksBySpritesheet[spriteSheet].push_back(newChunk);
    }

    return newChunk;
}

EntityID ChunkManager::assignNewEntity(Archetype arch, std::string sprite, std::vector<Tag> tags, ComponentSizeMap& sizemap)
{
    std::sort(tags.begin(),tags.end());

    // are there any chunks with this archetype and this sprite sheet
    auto find = chunksByArch.find(arch.getType());
    if (find != chunksByArch.end())
    {
        // yes
        // now are there any such chunks with room for a new entity?
        std::vector<Chunk*> chunks = find->second;

        for (int i = 0; i < chunks.size(); i++)
        {
            Chunk* chunk = chunks[i];
            if (chunk->GetSpritesheet() == sprite && chunk->getAllTags() == tags && chunk->getCurrEntCount() < ENTITIES_PER_CHUNK)
            {
                // yes, we found a valid chunk
                //chunk->assignNewEntity
                EntityID ca = chunk->assignNewEntity();
                return ca;
            }
        }
    }

    // there are no non-empty chunks that match this archetype and sprite
    // create chunk

    Chunk* newChunk = createChunk(arch, sprite, tags, sizemap);
    EntityID ca = newChunk->assignNewEntity();
    return ca;
}

void ChunkManager::scheduleToDelete(EntityID id)
{
    entitiesToDelete.push_back(id);    
};

int ChunkManager::GetEntityCount()
{
    int total = 0;
    for (int i = 0; i < allChunks.size(); i++) {
        total += allChunks[i]->getCurrEntCount();
    }
    return total;
}

bool ChunkManager::entityHasComponent(ComponentType type, EntityID id)
{
    Chunk* chunk = allChunks[id.chunkID];
    return chunk->hasComponentType(type);
}

bool ChunkManager::entityHasTag(Tag tag, EntityID id)
{
    Chunk* chunk = allChunks[id.chunkID];
    return chunk->hasTag(tag);
}

std::vector<Tag> ChunkManager::getTagsForEntity(EntityID id)
{
    Chunk* chunk = allChunks[id.chunkID];
    return chunk->getAllTags();
}

ChunkManager::~ChunkManager()
{
    for (int i = 0; i < allChunks.size(); i++)
    {
        delete allChunks[i];
    }
}

int ChunkManager::getChunkCount()
{
    return currChunks;
}

void ChunkManager::deleteScheduledEntities() {
    for (int i = 0; i < entitiesToDelete.size(); i++)
    {
        EntityID id = entitiesToDelete[i];
        allChunks[id.chunkID]->releaseEntity(id);
    }
    entitiesToDelete.clear();
}

bool ChunkManager::doesEntityExist(EntityID id)
{
    return allChunks[id.chunkID]->doesEntityExist(id);
}



std::shared_ptr<EntityQuery> ChunkManager::entitiesWithSpriteSheet(std::string spritesheet)
{
    auto find = chunksBySpritesheet.find(spritesheet);
    if (find != chunksBySpritesheet.end())
    {
        return std::make_shared<EntityQuery>(find->second);
    }
    else
    {
        return std::make_shared<EntityQuery>();
    }    
}

