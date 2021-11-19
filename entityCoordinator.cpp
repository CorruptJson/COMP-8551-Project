#include "EntityCoordinator.h"

EntityCoordinator::EntityCoordinator()
{
    // Create pointers to each manager
    mComponentManager = std::make_unique<ComponentManager>();
    //mEntityManager = std::make_unique<EntityManager>();
    mChunkManager = std::make_unique<ChunkManager>();
    mArchetypeManager = std::make_unique<ArchetypeManager>();
    //mChunkManager = std::make_unique<ProtoChunkManager>();
    mSystemManager = std::make_unique<SystemManager>();

    //initializeSystemManager();
}

EntityCoordinator& EntityCoordinator::getInstance()
{
    static EntityCoordinator entityCoordinator;
    return entityCoordinator;
}

// Chunk manager searches for space in a chunk to assign an entity ID to, and returns it
// creates a new chunk if no matching chunk is found
// all entities in the chunk must share the same spritshseet
EntityID EntityCoordinator::CreateEntity(Archetype arch, std::string sprite, std::vector<Tag> tags)
{
    return mChunkManager->assignNewEntity(arch, sprite, tags, mComponentManager->mComponentSizes);
}

// Creates entity with no sprite sheet
EntityID EntityCoordinator::CreateEntity(Archetype arch, std::vector<Tag> tags)
{
    return mChunkManager->assignNewEntity(arch, noSprite, tags, mComponentManager->mComponentSizes);
}

// get a validated archetype object from a list of component types
// an archetype defines which components an entity has
// an archetype is simply a definition of a type, the same archetype object can be used to create an number of entities
Archetype EntityCoordinator::GetArchetype(std::vector<ComponentType> compTypes)
{
    return mArchetypeManager->getArchetype(compTypes);
}

// schedules entity to be deleted at the end of the current fixed update
void EntityCoordinator::scheduleEntityToDelete(EntityID entity)
{
    mChunkManager->scheduleToDelete(entity);
}

// returns an entity query, an object which contains the search results upon creation
// the entity query searches for all entities that contain these components
// searches without checking for tags
//std::shared_ptr<EntityQuery> EntityCoordinator::GetEntityQuery(std::vector<ComponentType> compTypes)
//{
//    return std::make_shared<EntityQuery>(compTypes, mChunkManager->allChunks, queryCache);
//}

// returns an entity query, an object which contains the search results upon creation
// the entity query searches for all entities that contain these components and tags
std::shared_ptr<EntityQuery> EntityCoordinator::GetEntityQuery(std::vector<ComponentType> compTypes, std::vector<Tag> tags)
{
    int chunkManagerVersion = mChunkManager->getChunkManagerVersion();
    //std::shared_ptr<EntityQuery> query = std::make_shared<EntityQuery>(compTypes, tags, chunkManagerVersion);
    std::shared_ptr<EntityQuery> query = std::make_shared<EntityQuery>(compTypes, tags, chunkManagerVersion);
    //std::shared_ptr<EntityQuery> query = std::make_shared<EntityQuery>();

    size_t hash = query->QueryHash();
    auto find = queryCache.find(hash);
    if (find != queryCache.end() && find->second->getChunkListVersion() == chunkManagerVersion)
    {
        if (find->second->getChunkListVersion() == chunkManagerVersion)
        {
            find->second->countFoundEntities();
            return find->second;
        }
        else
        {
            query->searchChunks(mChunkManager->allChunks);
            find->second = query;
            return query;
        }        
    }
    else
    {
        query->searchChunks(mChunkManager->allChunks);
        queryCache.emplace(hash,query);
        return query;
    }
}

uint32_t EntityCoordinator::GetEntityCount()
{
    return mChunkManager->GetEntityCount();
}

void EntityCoordinator::runSystemUpdates()
{
    mSystemManager->runUpdates();
}

bool EntityCoordinator::entityHasTag(Tag tag, EntityID id)
{
    return mChunkManager->entityHasTag(tag, id);
}

std::vector<Tag> EntityCoordinator::getTagsForEntity(EntityID id)
{
    return mChunkManager->getTagsForEntity(id);
}

void EntityCoordinator::endOfUpdate()
{
    mChunkManager->deleteScheduledEntities();
}

bool EntityCoordinator::doesEntityExist(EntityID id)
{
    return mChunkManager->doesEntityExist(id);
}

std::shared_ptr<EntityQuery> EntityCoordinator::entitiesWithSpriteSheet(std::string spritesheet)
{
    return mChunkManager->entitiesWithSpriteSheet(spritesheet);
}
