#include <iostream>
#include "chunk.h"


Chunk::Chunk(Signature sig, int chunkID, Spritesheet spriteSheet)
{
    this->sig = sig;
    this->chunkID = chunkID;
    this->spritesheet = spriteSheet;
}

Chunk::Chunk(Archetype archetype, int chunkID, Spritesheet spriteSheet, ComponentSizeMap& sizemap)
{
    this->arch = archetype;
    this->chunkID = chunkID;
    this->spritesheet = spriteSheet;
    addComponentArrays(archetype, sizemap);
    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        entToDat[i] = -1;
    }
}

void Chunk::addComponentArrays(Archetype t, ComponentSizeMap& sizemap)
{
    std::vector<ComponentType> archComponents = t.getComponentTypesArrayCopy();
    for (int i = 0; i < archComponents.size(); i++)
    {
        ComponentType ct = archComponents[i];
        ComponentSize cs = sizemap[ct];
        Byte* newArray = new Byte[ENTITIES_PER_CHUNK * int(cs)];
        componentArrays.emplace(ct, newArray);
    }
}



Spritesheet Chunk::GetSpritesheet()
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
                entToDat[i] = currEnts;
                datToEnt[currEnts] = i;

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
    int releasedEntDataIndex = entToDat[id.index];
    int lastIndex = currEnts - 1;
    if (releasedEntDataIndex != lastIndex)
    {
        // move component data with index == lastIndex into dataIndex
        // must swap data!
        for (int i = 0; i < componentArrays.size(); i++)
        {
            //ComponentType type = arch.
            //ComponentSize size = sizeMap[]
            throw "release entity function not completed";
        }

        // then swap indexes
        int entWithLastData = datToEnt[lastIndex];
        datToEnt[releasedEntDataIndex] = entWithLastData;
        entToDat[entWithLastData] = releasedEntDataIndex;
    }

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
