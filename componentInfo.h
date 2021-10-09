#pragma once
#include "Types.h"
#include "chunk.h"

template <typename T>
class ComponentInfo
{
private:
    ComponentType type;
    Signature sig;

public:

    ComponentInfo(ComponentType type, Signature sig) : type(type), sig(sig)
    {
        
    }

    void* getComponentArray()
    {
        return new T[ENTITIES_PER_CHUNK];
    }
};
