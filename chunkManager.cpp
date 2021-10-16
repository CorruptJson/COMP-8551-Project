#include "chunkManager.h"

Chunk* ChunkManager::createChunk(Archetype arch, Spritesheet spriteSheet, ComponentSizeMap& sizemap)
{
    Chunk* newChunk = new Chunk(arch, currChunks++, spriteSheet, sizemap);
    allChunks.push_back(newChunk);
    chunksBySpritesheet.emplace(spriteSheet, newChunk);
    return newChunk;
}

ChunkAddress ChunkManager::assignNewEntity(Archetype arch, Spritesheet sprite, ComponentSizeMap& sizemap)
{
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
            if (chunk->GetSpritesheet() == sprite && chunk->getCurrEntCount() < ENTITIES_PER_CHUNK)
            {
                // yes, we found a valid chunk
                //chunk->assignNewEntity
                ChunkAddress ca = chunk->assignNewEntity();
                return ca;
            }
        }
    }

    // there are no non-empty chunks that match this archetype and sprite
    // create chunk

    Chunk* newChunk = createChunk(arch,sprite,sizemap);
    ChunkAddress ca = newChunk->assignNewEntity();
    return ca;
}

void ChunkManager::releaseEntity(ChunkAddress id)
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

ChunkManager::~ChunkManager()
{
    for (int i = 0; i < allChunks.size(); i++)
    {
        delete allChunks[i];
    }
}
