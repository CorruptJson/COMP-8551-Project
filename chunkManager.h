#pragma once

#include <iostream>
#include <exception>
#include <unordered_map>
#include <vector>
#include <functional>
#include "Types.h"
#include "chunk.h"
#include "EntityQuery.h"
#include "ComponentManager.h"
#include "ISubject.h"
#include "PhysicsComponent.h"

const static std::string noSprite = "NO_SPRITE";

// this class contains chunks, which contain entity component data
// this class creates new chunks when needed, which is during entity creation when no valid matching chunks are found
// 
// chunk manager version is an int used to check for when an Entity query becomes outdated. Queries are outdated when the chunk manager creates new chunks

// chunk manager is also used to interface with chunks, such as getting entity data

// the chunk manager can be interfaced with via the ECSCoordinator

class ChunkManager : public ISubject
{
    friend class EntityCoordinator;

private:
    
    std::unordered_map<ArchetypeType, std::vector<Chunk*>> chunksByArch;
    std::unordered_map<std::string, std::vector<Chunk*>> chunksBySpritesheet;
    std::vector<Chunk*> allChunks;
    std::vector<EntityID> entitiesToDelete;
    int currChunks = 0;
    int chunkManagerVersion = 0;

    Chunk* createChunk(Archetype arch, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap);

public:
      
    EntityID assignNewEntity(Archetype arch, std::string sprite, std::vector<Tag> tags, ComponentSizeMap& sizemap);

    void scheduleToDelete(EntityID id);

    template<typename T>
    T& getComponentRef(EntityID ca) {
        return allChunks[ca.chunkID]->getComponentReference<T>(ca);
    };

    template<typename T>
    T* getComponentPointer(EntityID id)
    {
        return allChunks[id.chunkID]->getComponentPointer<T>(id);
    }

    int GetEntityCount();
    int getChunkCount();

    bool entityHasComponent(ComponentType type, EntityID id);
    bool entityHasTag(Tag tag, EntityID id);
    std::vector<Tag> getTagsForEntity(EntityID id);
    void deleteScheduledEntities();
    bool doesEntityExist(EntityID id);
    int getChunkManagerVersion();

    std::shared_ptr<EntityQuery> entitiesWithSpriteSheet(std::string spritesheet);

    void deactivateAllEntitiesAndPhysicsBodies();

    void Notify(Event e, void* args) override;
    void NotifyToDestroyB2Body(EntityID id, b2Body* bod);



    ~ChunkManager();
};
