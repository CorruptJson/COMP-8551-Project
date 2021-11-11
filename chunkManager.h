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
    
    std::unordered_map<ArchetypeType, std::vector<Chunk*>> chunksByArch;
    std::unordered_map<std::string, std::vector<Chunk*>> chunksBySpritesheet;
    std::vector<Chunk*> allChunks;
    std::vector<EntityID> entitiesToDelete;
    int currChunks = 0;

    Chunk* createChunk(Archetype arch, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap);

public:

    EntityID assignNewEntity(Archetype arch, std::string sprite, std::vector<Tag> tags, ComponentSizeMap& sizemap);

    void scheduleToDelete(EntityID id);

    template<typename T>
    T& getComponentRef(EntityID ca) {
        return allChunks[ca.chunkID]->getComponentReference<T>(ca);
    };

    int GetEntityCount();
    int getChunkCount();

    bool entityHasComponent(ComponentType type, EntityID id);
    bool entityHasTag(Tag tag, EntityID id);
    std::vector<Tag> getTagsForEntity(EntityID id);
    void deleteScheduledEntities();
    bool doesEntityExist(EntityID id);

    ~ChunkManager();
};
