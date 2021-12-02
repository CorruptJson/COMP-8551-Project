#include "EntityCoordinator.h"

EntityCoordinator::EntityCoordinator()
{
    // Create pointers to each manager
    componentManager = std::make_shared<ComponentManager>();
    //mEntityManager = std::make_unique<EntityManager>();
    chunkManager = std::make_shared<ChunkManager>();
    archetypeManager = std::make_shared<ArchetypeManager>();
    //mChunkManager = std::make_unique<ProtoChunkManager>();
    systemManager = std::make_shared<SystemManager>();

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
    return chunkManager->assignNewEntity(arch, sprite, tags, componentManager->mComponentSizes);
}

// Creates entity with no sprite sheet
EntityID EntityCoordinator::CreateEntity(Archetype arch, std::vector<Tag> tags)
{
    return chunkManager->assignNewEntity(arch, noSprite, tags, componentManager->mComponentSizes);
}

// get a validated archetype object from a list of component types
// an archetype defines which components an entity has
// an archetype is simply a definition of a type, the same archetype object can be used to create an number of entities
Archetype EntityCoordinator::GetArchetype(std::vector<ComponentType> compTypes)
{
    return archetypeManager->getArchetype(compTypes);
}

// schedules entity to be deleted at the end of the current fixed update
void EntityCoordinator::scheduleEntityToDelete(EntityID entity)
{
    chunkManager->scheduleToDelete(entity);
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
EntityQuery* EntityCoordinator::GetEntityQuery(std::vector<ComponentType> compTypes, std::vector<Tag> tags)
{
    int chunkManagerVersion = chunkManager->getChunkManagerVersion();
    //std::shared_ptr<EntityQuery> query = std::make_shared<EntityQuery>(compTypes, tags, chunkManagerVersion);
    //std::shared_ptr<EntityQuery> query = std::make_shared<EntityQuery>(compTypes, tags, chunkManagerVersion);
    //std::shared_ptr<EntityQuery> query = std::make_shared<EntityQuery>();

    size_t hash = EntityQuery::QueryParamterHash(compTypes,tags);
    auto find = queryCache.find(hash);
    if (find != queryCache.end())
    {
        if (find->second->getChunkListVersion() == chunkManagerVersion)
        {
            // cached query is up to date
            find->second->recountFoundEntities();
            return find->second;
        }
        else
        {
            // cached query outdated
            EntityQuery* query = find->second;
            query->searchChunks(chunkManager->allChunks,chunkManagerVersion);
            return query;
        }        
    }
    else
    {
        // no cached query found
        EntityQuery* query = new EntityQuery(compTypes,tags);
        query->searchChunks(chunkManager->allChunks,chunkManagerVersion);
        queryCache.emplace(hash,query);
        return query;
    }
}

uint32_t EntityCoordinator::GetEntityCount()
{
    return chunkManager->GetEntityCount();
}

void EntityCoordinator::runSystemUpdates()
{
    systemManager->runUpdates();
}

bool EntityCoordinator::entityHasTag(Tag tag, EntityID id)
{
    return chunkManager->entityHasTag(tag, id);
}

std::vector<Tag> EntityCoordinator::getTagsForEntity(EntityID id)
{
    return chunkManager->getTagsForEntity(id);
}

void EntityCoordinator::endOfUpdate()
{
    chunkManager->deleteScheduledEntities();
}

bool EntityCoordinator::doesEntityExist(EntityID id)
{
    return chunkManager->doesEntityExist(id);
}

shared_ptr<EntityQuery> EntityCoordinator::entitiesWithSpriteSheet(std::string spritesheet)
{
    return chunkManager->entitiesWithSpriteSheet(spritesheet);
}

void EntityCoordinator::deactivateAllEntitiesAndPhysicsBodies()
{
    chunkManager->deactivateAllEntitiesAndPhysicsBodies();
}

std::shared_ptr<ChunkManager> EntityCoordinator::ChunkManager()
{
    return chunkManager;
}
