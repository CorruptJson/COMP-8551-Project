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

// not yet fully implemented
void EntityCoordinator::scheduleEntityToDelete(EntityID entity)
{
    mChunkManager->scheduleToDelete(entity);
}

// returns an entity query, an object which contains the search results upon creation
// the entity query searches for all entities that contain these components
std::unique_ptr<EntityQuery> EntityCoordinator::GetEntityQuery(std::vector<ComponentType> compTypes)
{

    return std::make_unique<EntityQuery>(compTypes, mChunkManager->allChunks);
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

