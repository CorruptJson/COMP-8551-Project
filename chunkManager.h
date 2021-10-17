#pragma once

#include <iostream>
#include <exception>
#include <unordered_map>
#include <vector>
#include "Types.h"
#include "chunk.h"

class ChunkManager
{
    friend class EntityCoordinator;

private:
    
    std::unordered_map<ArchetypeType, Chunk*> chunksByArch;
    std::unordered_map<Spritesheet, Chunk*> chunksBySpritesheet;

    std::unordered_map<const char*, Chunk*> chunksPerSpritesheet;
    std::vector<Chunk*> allChunks;
    int currChunks = 0;

    Chunk* createChunk(Archetype arch, Spritesheet spriteSheet, ComponentSizeMap& sizemap);

public:

    EntityID assignNewEntity(Archetype arch, Spritesheet sprite, ComponentSizeMap& sizemap);

    void releaseEntity(EntityID id);

    template<typename T>
    T& getComponentRef(EntityID ca) {
        return allChunks[ca.chunkID]->getComponentReference<T>(ca);
    };

    int GetEntityCount();

    ~ChunkManager();
};
