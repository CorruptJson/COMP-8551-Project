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

    EntityID allocateNewEntity(SigArch sa) override;
    void deallocateEntity(EntityID id) override;
    TempPosition getEntityPosition(EntityID id) override;
    virtual void setEntityPosition(EntityID id, TempPosition pos) override;

    // finds an index for a new entity, creates a new EntityID, and sets the position
    EntityID allocateNewEntity(SigArch sa, TempPosition pos);
};
