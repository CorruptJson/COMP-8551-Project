#pragma once
#include <unordered_map>
#include "chunkManager.h"


class ProtoChunkManager : public ChunkManager
{

private:
    std::unordered_map<char const*, int> typeNum;
public:

    EntityID allocateNewEntity(SigArch sa) override;
    void deallocateEntity(EntityID id) override;
    TempPosition getEntityPosition(EntityID id) override;
    virtual void setEntityPosition(EntityID id, TempPosition pos) override;

    // finds an index for a new entity, creates a new EntityID, and sets the position
    EntityID allocateNewEntity(SigArch sa, TempPosition pos);

    template<typename T, typename... Args>
    EntityID assignNewEntity()
    {
        SigArch sigarch;
    }



};
