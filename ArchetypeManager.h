#pragma once
#include <map>
#include <vector>
#include "Types.h"
#include "Archetype.h"

class ArchetypeManager
{
private:
    ArchetypeType typeCounter = 0;
    ArchetypeType getArchetypeType(std::vector<ComponentType>& compTypes);
    std::map<std::vector<ComponentType>, ArchetypeType> archetypeTypes;

    
public:

    Archetype getArchetype(std::vector<ComponentType>& compTypes);
};
