#include "EntityID.h";
#include "SigArch.h"

class ChunkManagerAbstract
{
    public:
        virtual EntityID AllocateNewEntity(SigArch sa) = 0;
        virtual void DeallocateEntity(EntityID id) = 0;
};
