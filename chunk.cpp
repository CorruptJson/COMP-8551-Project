#include "chunk.h"

Chunk::Chunk(Archetype archetype, int chunkID, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap)
{
    this->arch = archetype;
    this->chunkID = chunkID;
    this->spritesheet = spriteSheet;
    this->tags = tags;
    addComponentArrays(archetype, sizemap);
    std::fill_n(versions, ENTITIES_PER_CHUNK, 0);
    std::fill_n(entToDat, ENTITIES_PER_CHUNK, -1);
    std::fill_n(deleteEnt, ENTITIES_PER_CHUNK, false);
    hasPhysics = archetype.hasComponentType(ComponentManager::GetComponentType<PhysicsComponent>());
}

void Chunk::addComponentArrays(Archetype t, ComponentSizeMap& sizemap)
{
    std::vector<ComponentType> archComponents = t.getComponentTypeArray();
    for (int i = 0; i < archComponents.size(); i++)
    {
        ComponentType ct = archComponents[i];
        ComponentSize cs = sizemap[ct];
        Byte* newArray = new Byte[ENTITIES_PER_CHUNK * int(cs)];
        componentArrays.emplace(ct, newArray);
    }
}

std::string Chunk::GetSpritesheet()
{
    return spritesheet;
}

int Chunk::getCurrEntCount()
{
    return currEnts;
}

EntityID Chunk::assignNewEntity()
{
    EntityID id;
    if (currEnts != ENTITIES_PER_CHUNK)
    {
        for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
        {
            if (entToDat[i] == -1)
            {
                id.chunkID = chunkID;
                id.index = i;
                id.version = versions[i];
                entToDat[i] = i;

                currEnts++;

                return id;
            }
        }

        std::cout << "chunk not full, but not slot found? " << id;
    }
    else
    {
        std::cout << "chunk is full, ";
    }

    id.chunkID = -1;
    id.index = -1;
    id.version = -1;

    std::cout << "could not assign entity to chunk" << std::endl;
    std::cout << "chunk id: " << chunkID << std::endl;
    std::cout << "chunk sprite: " << spritesheet << std::endl;
    std::cout << "chunk ent count : " << currEnts << std::endl;
    std::cout << "ent to dat values: ";
    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        std::cout << entToDat[i];
    }
    std::cout << std::endl;

    return id;

}

void Chunk::releaseEntity(EntityID id)
{
    if (versions[id.index] != id.version)
    {
        std::cout << "trying to delete entity that no longer exists?" << std::endl;
        throw "trying to delete entity that no longer exists?";
    }

    releaseEntity(id.index);
}

void Chunk::releaseEntity(int datIndex)
{
    entToDat[datIndex] = -1;
    versions[datIndex] = versions[datIndex] + 1;
    deleteEnt[datIndex] = false;
    currEnts--;
}

Archetype& Chunk::getArchetype()
{
    return arch;
}

Chunk::~Chunk()
{
    for (auto ca : componentArrays)
    {
        delete ca.second;
    }
}

bool Chunk::hasComponentType(ComponentType type)
{
    return arch.hasComponentType(type);
}

bool Chunk::hasTag(Tag tag)
{
    return std::find(tags.begin(),tags.end(),tag) != tags.end();
}

EntityID Chunk::entityAtComponentIndex(int i)
{
    //int entIndex = datToEnt[i];

    return { chunkID,i,versions[i] };
}

bool Chunk::doesEntityExist(EntityID id)
{
    return (entToDat[id.index] != -1 && id.version == versions[id.index]);
}

bool Chunk::isDataIndexActive(int i) {
    return entToDat[i] != -1;
}

std::vector<Tag>& Chunk::getAllTags()
{
    return tags;
}

void Chunk::releaseAllEntities()
{
    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        if (entToDat[i] != -1)
        {
            entToDat[i] = -1;
            versions[i] = versions[i] + 1;
            deleteEnt[i] = false;
        }
    }
    entitiesToDelete = false;
    currEnts = 0;
}

void Chunk::flagEntToDelete(EntityID id)
{
    deleteEnt[id.index] = true;
    entitiesToDelete = true;
}

void Chunk::scheduleAllEntitiesToDelete()
{
    if (getCurrEntCount() == 0)
    {
        return;
    }
    entitiesToDelete = true;
    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        if (entToDat[i] != -1)
        {
            deleteEnt[i] = true;
        }        
    }
}

void Chunk::releaseFlaggedEntities(ISubject& subject)
{
    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        if (deleteEnt[i])
        {
            if (hasPhysics)
            {
                EntityID id = { chunkID,i,versions[i] };
                B2BodyDeleteEventArgs* args = new B2BodyDeleteEventArgs{ id,getComponentReference<PhysicsComponent>(id).box2dBody };
                subject.Notify(Event::B2BODY_TO_DELETE, (void*)args);
            }
            releaseEntity(i);            
        }
    }
    entitiesToDelete = false;
}
