#include "chunkManager.h"
#include <exception>

// dont use this yet, kay?
class ChunkManagerStandard: public ChunkManager
{
private:
    int currEntities = 0;


public:

    ChunkAddress allocateNewEntity(SigArch sa) override
    {
        throw std::exception("not implemented");
    };

    void deallocateEntity(ChunkAddress id) override
    {
        throw std::exception("not implemented");
    };

    TempPosition getEntityPosition(ChunkAddress id) override
    {
        throw std::exception("not implemented");
    };
};
