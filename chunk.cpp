#include <iostream>
#include "chunk.h"


Chunk::Chunk(Archetype archetype, int chunkID, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap)
{
    this->arch = archetype;
    this->chunkID = chunkID;
    this->spritesheet = spriteSheet;
    this->tags = tags;
    addComponentArrays(archetype, sizemap);
    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        entToDat[i] = -1;
        datToEnt[i] = i;
    }
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
                //entToDat[i] = currEnts;
                //datToEnt[currEnts] = i;

                currEnts++;

                return id;
            }
        }
    }
    else
    {
        std::cout << "chunk is full, ";
    }

    id.chunkID = -1;
    id.index = -1;
    id.version = -1;

    std::cout << "could not assign entity to chunk";

    return id;

}

void Chunk::releaseEntity(EntityID id)
{
    if (versions[id.index] != id.version)
    {
        std::cout << "trying to delete entity that no longer exists?" << std::endl;
        throw "trying to delete entity that no longer exists?";
    }

    //int releasedEntDataIndex = entToDat[id.index];
    //int lastIndex = currEnts - 1;
    //if (releasedEntDataIndex != lastIndex)
    //{
    //    std::vector<ComponentType> componentTypes = arch.getComponentTypeArray();              

    //    // must swap data!
    //    for (int i = 0; i < componentArrays.size(); i++)
    //    {
    //        ComponentType type = componentTypes[i];
    //        ComponentSize c_size = ComponentManager::GetComponentSize(type);
    //        // these are the starting data indexes for this specific component type
    //        int releasedCDataIndex = releasedEntDataIndex * c_size;
    //        int lastCDatIndex = lastIndex * c_size;
    //        Byte* arr = componentArrays[type];
    //        for (int j = 0; j < c_size; j++)
    //        {
    //            int indexR = releasedCDataIndex + j;
    //            int indexL = lastCDatIndex + j;
    //            arr[indexR] = arr[indexL];
    //        }
    //    }

    //    // then swap indexes
    //    int entWithLastData = datToEnt[lastIndex];
    //    datToEnt[releasedEntDataIndex] = entWithLastData;
    //    entToDat[entWithLastData] = releasedEntDataIndex;
    //}

    entToDat[id.index] = -1;
    versions[id.index] = versions[id.index] + 1;
    currEnts--;
}

Archetype Chunk::getArchetype()
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
    int entIndex = datToEnt[i];

    return { chunkID,entIndex,versions[entIndex] };
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
