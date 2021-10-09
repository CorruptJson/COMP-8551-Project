#include <algorithm>
#include "ArchetypeManager.h"

ArchetypeType ArchetypeManager::getArchetypeType(std::vector<ComponentType>& compTypes)
{
    auto find = archetypeTypes.find(compTypes);
    if (find == archetypeTypes.end())
    {
        //std::vector<ComponentType> v2(compTypes);
        ArchetypeType type = typeCounter++;
        archetypeTypes.emplace(compTypes, type);
        return type;
    }
    else
    {
        return find->second;
    }
}

Archetype ArchetypeManager::getArchetype(std::vector<ComponentType>& compTypes)
{
    std::sort(compTypes.begin(), compTypes.end());
    ArchetypeType type = getArchetypeType(compTypes);
    return Archetype(type, compTypes);
}
