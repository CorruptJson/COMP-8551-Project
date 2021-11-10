#include "chunkManager.h"

Chunk* ChunkManager::createChunk(Archetype arch, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap)
{
    Chunk* newChunk = new Chunk(arch, currChunks++, spriteSheet, tags, sizemap);
    allChunks.push_back(newChunk);
    chunksBySpritesheet.emplace(spriteSheet, newChunk);
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
        size_t bucketIndex = chunksByArch.bucket(arch.getType());
        auto begin = chunksByArch.begin(bucketIndex);
        auto end = chunksByArch.end(bucketIndex);

        for (auto iterator = begin; iterator != end; iterator++)
        {
            Chunk* chunk = iterator->second;
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

void ChunkManager::releaseEntity(EntityID id)
{
    allChunks[id.chunkID]->releaseEntity(id);
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

