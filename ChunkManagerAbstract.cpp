#include "EntityID.h";
#include "SigArch.h"

class ChunkManagerAbstract
{
    public:
        virtual EntityID allocateNewEntity(SigArch sa) = 0;
        virtual void deallocateEntity(EntityID id) = 0;
};
