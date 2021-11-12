#pragma once

#include <memory>
#include <array>
#include <string>
#include "ComponentManager.h"
#include "chunkManager.h"
#include "ArchetypeManager.h"
#include "Types.h"
#include "EntityQuery.h"
#include "system_manager.h"

class EntityCoordinator
{
private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<ChunkManager> mChunkManager;
    std::unique_ptr<ArchetypeManager> mArchetypeManager;
    std::unique_ptr<SystemManager> mSystemManager;

    //std::unordered_map< std::vector<ComponentType>, EntityQuery> queryCache;
    //int queryCacheVersion = 0;

    EntityCoordinator();

public:

    static EntityCoordinator& getInstance();

    // Chunk manager searches for space in a chunk to assign an entity ID to, and returns it
    // creates a new chunk if no matching chunk is found
    // all entities in the chunk must share the same spritshseet
    EntityID CreateEntity(Archetype arch, Spritesheet sprite, std::vector<Tag> tags);

    // get a validated archetype object from a list of component types
    // an archetype defines which components an entity has
    // an archetype is simply a definition of a type, the same archetype object can be used to create an number of entities
    Archetype GetArchetype(std::vector<ComponentType> compTypes);

    // not yet fully implemented
    void DestroyEntity(EntityID entity);

    // Register component with the component manager
    // all components must be registered before use
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    // gets the int id associated with a component type
    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    // gets a reference to an entity's component
    template<typename T>
    T& GetComponent(EntityID entity)
    {
        return mChunkManager->getComponentRef<T>(entity);
    }
    
    // returns an entity query, an object which contains the search results upon creation
    // the entity query searches for all entities that contain these components
    std::unique_ptr<EntityQuery> GetEntityQuery(std::vector<ComponentType> compTypes);

    uint32_t GetEntityCount();

    void runSystemUpdates();

    template<typename T>
    shared_ptr<T> addSystem(std::shared_ptr<T> system) {
        return mSystemManager->addSystem(std::static_pointer_cast<System>(system));
    };

    template<typename T>
    shared_ptr<T> addSystem() {
        return mSystemManager->addSystem<T>();
    };

    template<typename T>
    bool entityHasComponent(EntityID id)
    {
        return mChunkManager->entityHasComponent(GetComponentType<T>(),id);
    }

    bool entityHasTag(Tag tag, EntityID id);
    std::vector<Tag> getTagsForEntity(EntityID id);

    //bool entityHasTag(Tag tag, EntityID id);
    //std::vector<Tag> getTagsForEntity(EntityID id);
    //RenderArrays renderArraysForSpriteSheet(const char* spriteSheet)
    //{
    //    
    //}
};
