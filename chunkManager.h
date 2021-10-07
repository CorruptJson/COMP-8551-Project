#pragma once

#include "Types.h"
#include "SigArch.h"
#include "tempPosition.h"

// abstract class interface
class ChunkManager
{
public:
    virtual ChunkAddress allocateNewEntity(SigArch sa) = 0;
    virtual void deallocateEntity(ChunkAddress id) = 0;
    virtual TempPosition getEntityPosition(ChunkAddress id) = 0;
    virtual void setEntityPosition(ChunkAddress id, TempPosition pos) = 0;
};
