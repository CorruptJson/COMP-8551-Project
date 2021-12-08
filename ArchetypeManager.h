#pragma once
#include <map>
#include <vector>
#include "Types.h"
#include "Archetype.h"

// this class is responsible associating archetypes with archetype IDs
// archetypes with the same combination of components have the same ID

// yes, ArchetypeTYPE is an unfortunate choice of name

class ArchetypeManager
{
private:
    ArchetypeType typeCounter = 0;
    ArchetypeType getArchetypeType(std::vector<ComponentType>& compTypes);
    std::map<std::vector<ComponentType>, ArchetypeType> archetypeTypes;

    
public:

    Archetype getArchetype(std::vector<ComponentType>& compTypes);
};
