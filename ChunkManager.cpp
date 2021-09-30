#include "chunkManagerAbstract.h"
#include <exception>

// dont use this yet, kay?
class ChunkManager: public ChunkManagerAbstract
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
