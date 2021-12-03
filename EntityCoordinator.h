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
#include "ComponentIterator.h"

using EntityQueryCache = std::unordered_map<size_t, shared_ptr<EntityQuery> >;

class EntityCoordinator
{
private:

    EntityQueryCache queryCache;
    
    //std::unordered_map< std::vector<ComponentType>, EntityQuery> queryCache;
    //int queryCacheVersion = 0;

    EntityCoordinator();

public:
    std::shared_ptr<ComponentManager> componentManager;
    std::shared_ptr<ChunkManager> chunkManager;
    std::shared_ptr<ArchetypeManager> archetypeManager;
    std::shared_ptr<SystemManager> systemManager;

    static EntityCoordinator& getInstance();

    // Chunk manager searches for space in a chunk to assign an entity ID to, and returns it
    // creates a new chunk if no matching chunk is found
    // all entities in the chunk must share the same spritshseet
    EntityID CreateEntity(Archetype arch, std::string sprite, std::vector<Tag> tags);

    // Creates entity with no sprite sheet
    EntityID CreateEntity(Archetype arch, std::vector<Tag> tags);

    // get a validated archetype object from a list of component types
    // an archetype defines which components an entity has
    // an archetype is simply a definition of a type, the same archetype object can be used to create an number of entities
    Archetype GetArchetype(std::vector<ComponentType> compTypes);

    // schedules entity to be deleted at the end of the current fixed update
    void scheduleEntityToDelete(EntityID entity);

    // Register component with the component manager
    // all components must be registered before use
    template<typename T>
    void RegisterComponent()
    {
        componentManager->RegisterComponent<T>();
    }

    // gets the int id associated with a component type
    template<typename T>
    ComponentType GetComponentType()
    {
        return componentManager->GetComponentType<T>();
    }

    // gets a reference to an entity's component
    template<typename T>
    T& GetComponent(EntityID entity)
    {
        return chunkManager->getComponentRef<T>(entity);
    }
    
    // returns an entity query, an object which contains the search results upon creation
    // the entity query searches for all entities that contain these components
    // searches without checking for tags
    //std::shared_ptr<EntityQuery> GetEntityQuery(std::vector<ComponentType> compTypes);

    // returns an entity query, an object which contains the search results upon creation
    // the entity query searches for all entities that contain these components and tags
    shared_ptr<EntityQuery> GetEntityQuery(std::vector<ComponentType> compTypes, std::vector<Tag> tags);

    uint32_t GetEntityCount();

    void runSystemUpdates();

    template<typename T>
    shared_ptr<T> addSystem(std::shared_ptr<T> system) {
        return systemManager->addSystem(std::static_pointer_cast<System>(system));
    };

    template<typename T>
    shared_ptr<T> addSystem() {
        return systemManager->addSystem<T>();
    };

    template<typename T>
    bool entityHasComponent(EntityID id)
    {
        return chunkManager->entityHasComponent(GetComponentType<T>(),id);
    }

    bool entityHasTag(Tag tag, EntityID id);
    std::vector<Tag> getTagsForEntity(EntityID id);

    void endOfUpdate();

    bool doesEntityExist(EntityID id);

    shared_ptr<EntityQuery> entitiesWithSpriteSheet(std::string spritesheet);

    void deactivateAllEntitiesAndPhysicsBodies();

    std::shared_ptr<ChunkManager> GetChunkManager();
};
