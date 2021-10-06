#pragma once
#include "chunkManager.h"
#include <vector>
#include <iostream>


class ProtoChunkManager : public ChunkManager
{
    int currEntities = 0;
    std::vector<bool> active = {};
    std::vector<TempPosition> positions = {};

public:

    //ChunkAddress allocateNewEntity(SigArch sa) override;
    //void deallocateEntity(ChunkAddress id) override;
    //TempPosition getEntityPosition(ChunkAddress id) override;
    //virtual void setEntityPosition(ChunkAddress id, TempPosition pos) override;

    // finds an index for a new entity, creates a new EntityID, and sets the position
    //ChunkAddress allocateNewEntity(SigArch sa, TempPosition pos);
};
