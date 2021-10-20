#pragma once

#include <memory>
#include <array>
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

public:
    EntityID* testEntity;

    void Init()
    {
        // Create pointers to each manager
        mComponentManager = std::make_unique<ComponentManager>();
        //mEntityManager = std::make_unique<EntityManager>();
        mChunkManager = std::make_unique<ChunkManager>();
        mArchetypeManager = std::make_unique<ArchetypeManager>();
        //mChunkManager = std::make_unique<ProtoChunkManager>();
        mSystemManager = std::make_unique<SystemManager>();

        initializeSystemManager();
    }

    // Chunk manager searches for space in a chunk to assign an entity ID to, and returns it
    // creates a new chunk if no matching chunk is found
    // all entities in the chunk must share the same spritshseet
    EntityID CreateEntity(Archetype arch, Spritesheet sprite)
    {
        return mChunkManager->assignNewEntity(arch, sprite, mComponentManager->mComponentSizes);
    }

    // get a validated archetype object from a list of component types
    // an archetype defines which components an entity has
    // an archetype is simply a definition of a type, the same archetype object can be used to create an number of entities
    Archetype GetArchetype(std::vector<ComponentType> compTypes)
    {
        return mArchetypeManager->getArchetype(compTypes);
    }

    // not yet fully implemented
    void DestroyEntity(EntityID entity)
    {
        mChunkManager->releaseEntity(entity);
    }

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
    std::unique_ptr<EntityQuery> GetEntityQuery(std::vector<ComponentType> compTypes)
    {
        return std::make_unique<EntityQuery>(compTypes, mChunkManager->allChunks);        
    }

    uint32_t GetEntityCount()
    {
        return mChunkManager->GetEntityCount();
    }

    void runSystemUpdates() {
        mSystemManager->runUpdates();
    }

    void initializeSystemManager() {
        mSystemManager->coordinator = this;

        mSystemManager->addSystem<TestSystem>(this);
    }

    template<typename T>
    std::shared_ptr<T> addSystem(EntityCoordinator* coord)
    {
        return mSystemManager->addSystem<T>(coord);
    }
};
