#pragma once
#include <unordered_map>
#include "chunkManager.h"


class ProtoChunkManager : public ChunkManager
{

private:
    std::unordered_map<char const*, int> typeNum;
public:

    ChunkAddress allocateNewEntity(SigArch sa) override;
    void deallocateEntity(ChunkAddress id) override;
    TempPosition getEntityPosition(ChunkAddress id) override;
    virtual void setEntityPosition(ChunkAddress id, TempPosition pos) override;

    // finds an index for a new entity, creates a new ChunkAddress, and sets the position
    ChunkAddress allocateNewEntity(SigArch sa, TempPosition pos);

    template<typename T, typename... Args>
    ChunkAddress assignNewEntity()
    {
        SigArch sigarch;
    }



};
