#include "chunkManager.h"
#include <exception>

// dont use this yet, kay?
class ChunkManagerStandard : public ChunkManager
{
private:
    int currEntities = 0;


public:

    EntityID allocateNewEntity(SigArch sa) override
    {
        throw std::exception("not implemented");
    };

    void deallocateEntity(EntityID id) override
    {
        throw std::exception("not implemented");
    };

    TempPosition getEntityPosition(EntityID id) override
    {
        throw std::exception("not implemented");
    };
};
